// ImgAnalyse.cpp: implementation of the CImgAnalyse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dip.h"
#include "ImgAnalyse.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CImgAnalyse, CObject)

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CImgAnalyse::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CImgAnalyse::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif

/***********************************************************************
*                                                                      *
*                             图像特征分析类                           *
*                                                                      *
***********************************************************************/

////////////////////////////////////////////////////////////////////////
//构造函数CImgAnalyse()    
//----------------------------------------------------------------------
//基本功能：构造一个CImgAnalyse类的对象，如不传入CDibObject对象。第   
//			一次调用某一个处理函数时必须给出一个CDibObject对象指针。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CImgAnalyse::CImgAnalyse()
{

}

////////////////////////////////////////////////////////////////////////
//构造函数CImgAnalyse()    
//----------------------------------------------------------------------
//基本功能：构造一个CImgAnalyse类的对象并传入CDibObject对象。所有的
//			操作都针对该对象，直到另一个对象作为参数被传给图像处理函数。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CImgAnalyse::CImgAnalyse( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}

CImgAnalyse::~CImgAnalyse()
{

}

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//基本功能：本函数为CImgAnalyse类对象指定一个CDibObject对象指针
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：无。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CImgAnalyse::SetDibObjectClass( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}

////////////////////////////////////////////////////////////////////////
//函数：BOOL EdgeTrace(CDibObject *pDibObject)
//功能：对灰度图像进行轮廓跟踪并生成链码,结果存储在TraceArray中
//说明：只对二值化后的灰度图像跟踪一个连通成分；
//      跟踪之前，应滤除噪声，建议使用灰值 闭运算去噪并平滑边界。
//返回：跟踪成功则返回TRUE
////////////////////////////////////////////////////////////////////////
BOOL CImgAnalyse::EdgeTrace(CDibObject *pDibObject)
{
	if(m_pDibObject==NULL) return FALSE;
	if(m_pDibObject->GetNumBits()!=8){AfxMessageBox("不是8位灰度图像");return FALSE;}
	
	unsigned char *pOld,*pBits,*pTemp,*pNew,*pNewBits,*pNewTemp;
	int nWidthBytes, nNumColors;
	DWORD offset,BufSize;
	pOld = (BYTE *) m_pDibObject->GetDIBPointer(&nWidthBytes);
	if(pOld == NULL) return FALSE;
	
	nNumColors = m_pDibObject->GetNumColors();
	offset=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD);
	pBits =(unsigned char *)&pOld[offset];
	int nHeight=m_pDibObject->GetHeight();
	int nWidth=m_pDibObject->GetWidth();
	BufSize=nWidthBytes*nHeight ;
	HGLOBAL hNewDib;
	hNewDib = ::GlobalAlloc(GHND,offset+BufSize );
	if( hNewDib == NULL ){
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return FALSE;}
	pNew = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNew == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib());
		return FALSE;
	}
	pNewBits = (unsigned char *)&pNew[offset];
	memcpy(pNew,pOld,offset);
	//设定初始值为255
	memset(pNewBits,(BYTE)255,BufSize);
	
	//是否找到起始点及回到起始点
	bool bFindStartPoint;
	//是否扫描到一个边界点
	bool bFindPoint;
	//起始边界点与当前边界点
	CPoint StartPoint,CurPoint;
	//八个方向和起始扫描方向，依次是左上方、上方、右上方、右方、右下方、下方、左下方和左方。
	int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
	int BeginDirect;
	
	//清空样板数组中的数据
	TraceArray.RemoveAll();
	//定义一个EdgePoint型结构成员变量存放边界点的信息
	EdgePoint m_EdgePoint;
	
	//先找到最左下方的边界点
	bFindStartPoint=false;
	int i,j;
	for(j=0;j<nHeight && !bFindStartPoint;j++)
	{
		for(i=0;i<nWidth && !bFindStartPoint;i++)
		{
			//指向原图倒数第j行，第i个像素的指针
			pTemp=pBits+nWidthBytes*j+i;
			//取得当前指针处的像素值
			if(*pTemp==0)
			{
				bFindStartPoint=true;
				StartPoint.x=i;
				StartPoint.y=j;
				//指向目标图像的倒数第j行，第i个像素的指针
				pNewTemp=pNewBits+nWidthBytes*j+i;
				*pNewTemp=(BYTE)0;
			}
		}
	}
	//由于起始点是在左下方，故起始扫描沿左上方向
	BeginDirect=0;
	//跟踪边界
	bFindStartPoint=false;
	//从起始点开始扫描
	CurPoint.x=StartPoint.x;
	CurPoint.y=StartPoint.y;
	while(!bFindStartPoint)
	{
		bFindPoint=false;
		while(!bFindPoint)
		{
			//要注意以下判断的顺序
			//判断当前搜索方向上的点是否超出图像左边界
			if((CurPoint.x==0) && (CurPoint.x+Direction[BeginDirect][0]<0)) BeginDirect=1;
			//判断当前搜索方向上的点是否超出图像上边界
			if((CurPoint.y==nHeight-1) && (CurPoint.y+Direction[BeginDirect][1]>nHeight-1)) BeginDirect=3;
			//判断当前搜索方向上的点是否超出图像右边界
			if((CurPoint.x==nWidth-1) && (CurPoint.x+Direction[BeginDirect][0]>nWidth-1)) BeginDirect=5;
			//判断当前搜索方向上的点是否超出图像下边界
			if((CurPoint.y==0) && (CurPoint.y+Direction[BeginDirect][1]<0)) BeginDirect=7;
			
			
			//沿扫描方向查看一个像素
			pTemp=pBits+nWidthBytes*(CurPoint.y+Direction[BeginDirect][1])+(CurPoint.x+Direction[BeginDirect][0]);
			if(*pTemp==0)
			{
				bFindPoint=true;

				/////////////////////////////////////////////////////
				//结构数组中记录的第一个点是最左下边界点,即起始点
				//方向为到下一点的矢量方向，2003年2月20日修改
				m_EdgePoint.CurPoint.x = CurPoint.x;
				m_EdgePoint.CurPoint.y = CurPoint.y;
				m_EdgePoint.nCurVerct = (BYTE)BeginDirect;
				TraceArray.Add(m_EdgePoint);
				////////////////////////////////////////////////////

				CurPoint.x=CurPoint.x+Direction[BeginDirect][0];
				CurPoint.y=CurPoint.y+Direction[BeginDirect][1];
				if(CurPoint.y==StartPoint.y && CurPoint.x==StartPoint.x) bFindStartPoint=true;
				pNewTemp = pNewBits + nWidthBytes * CurPoint.y + CurPoint.x;
				*pNewTemp= (unsigned char)0;
				
				/*//结构数组中记录的第一个点是最左下边界点的下一个邻点，方向为
				//该点的当前矢量；最后一个点为最左下点，2002年5月18日
				m_EdgePoint.CurPoint.x = CurPoint.x;
				m_EdgePoint.CurPoint.y = CurPoint.y;
				m_EdgePoint.nCurVerct = (BYTE)BeginDirect;
				TraceArray.Add(m_EdgePoint);*/

				//扫描的方向逆时针旋转两格
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
			}
			else
			{
				//扫描方向顺时针旋转一格
				BeginDirect++;
				if(BeginDirect == 8)
					BeginDirect = 0;
			}
		}
	}
	::GlobalUnlock(m_pDibObject->GetDib());
	::GlobalFree(m_pDibObject->GetDib());
	::GlobalUnlock(hNewDib );
	
	//将新图像设置为当前图像
	m_pDibObject->SetDib( hNewDib );
	return true;
}

////////////////////////////////////////////////////////////////////////
//函数：Calibrate()
//功能：对图像进行尺寸标定
//参数：X_Scale、Y_Scale、XY_Scale分别为水平、垂直和对角方向的标定系数；
//      realSize为实际尺寸，单位为mm,标定后的系数单位为mm/pixel。
//返回：无
////////////////////////////////////////////////////////////////////////
void CImgAnalyse::Calibrate(float *X_Scale, float *Y_Scale, float *XY_Scale, float realSize)
{
	if(m_pDibObject == NULL) return;
	if(m_pDibObject->GetNumBits()!=8)
	{
		AfxMessageBox("不是8位灰度图像");
		return;
	}
	
	unsigned char *pOld,*pBits,*pTemp;
	int nWidthBytes;
	DWORD offset;
	pOld = (BYTE *) m_pDibObject->GetDIBPointer(&nWidthBytes);
	if(pOld == NULL) return;
	
	offset=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	pBits =(unsigned char *)&pOld[offset];

	int nHeight = m_pDibObject->GetHeight();
	int nWidth = m_pDibObject->GetWidth();

	int i,j,y1,y2,x1,x2;
	//从上向下，逐行搜索像素值为0(黑)的第一个像素点，即为参考物的上切点，记其y坐标为y1。
	for(j=0;j<nHeight;j++)
	{
		pTemp = pBits+(nHeight-j-1)*nWidthBytes;
		for(i=0;i<nWidth;i++)
			if(pTemp[i] == 0)
			{
				y1 = j;
				//跳出两层循环
				j = nHeight;
				break;
			}
	}
	//从下向上，逐行搜索像素值为0(黑)的第一个像素点，即为参考物的下切点，记其y坐标为y2。
	for(j=0;j<nHeight;j++)
	{
		pTemp = pBits+j*nWidthBytes;
		for(i=0;i<nWidth;i++)
			if(pTemp[i] == 0)
			{
				y2 = nHeight-j-1;
				//跳出两层循环
				j = nHeight;
				break;
			}
	}
	//从左向右逐列找出参考物的最左边一个为0(黑)的像素,记为x1
	for(i=0;i<nWidth;i++)
	{
		for(j=0;j<nHeight;j++)
		{
			pTemp = pBits+j*nWidthBytes+i;
			if(*pTemp == 0)
			{
				x1 = i;
				//跳出两层循环
				i = nWidth;
				break;
			}
		}
	}
	//从右向左逐列找出参考物的最右边一个0(黑)的像素,记为x2
	for(i=nWidth-1;i>=0;i--)
	{
		for(j=0;j<nHeight;j++)
		{
			pTemp = pBits+j*nWidthBytes+i;
			if(*pTemp == 0)
			{
				x2 = i;
				//跳出两层循环
				i = -1;
				break;
			}
		}
	}
	//计算标定系数
	*X_Scale = realSize/(x2-x1);
	*Y_Scale = realSize/(y2-y1);
	*XY_Scale = (float)sqrt((*X_Scale)*(*X_Scale) + (*Y_Scale)*(*Y_Scale));
}

////////////////////////////////////////////////////////////////////////
//函数：GetEdgePointNum()
//功能：获取边界点数
//返回：返回边界点数
////////////////////////////////////////////////////////////////////////
int CImgAnalyse::GetEdgePointNum()
{
	return	(TraceArray.GetSize());
}

////////////////////////////////////////////////////////////////////////
//函数：AreaByChain()
//功能：由边界链码计算区域面积
//返回：BOOL	计算正确则返回TRUE
////////////////////////////////////////////////////////////////////////
BOOL CImgAnalyse::AreaByChain(float *fArea)
{
	//循环变量
	int i;
	//边界点结构体变量
	EdgePoint m_EdgePoint;
	//边界点数，即TraceArray的大小
	int nNumNode;

	//分别用于存储条件公式中X的增量和2α
	//注意：由于边界跟踪的作用，各方向在数组中的顺序依次为
	//左上方、上方、右上方、右方、右下方、下方、左下方、左方
	//另外：TraceArray中存储的坐标与常用坐标相同，其原点在左下角，Y朝上
	int DetaX[8] = {-1,0,1,1,1,0,-1,-1};
	int Alfa[8] = {1,0,-1,0,1,0,-1,0};

	float sum = 0.0f;
	nNumNode = TraceArray.GetSize();
	if(nNumNode<1)
	{
		AfxMessageBox("是否没有进行轮廓跟踪?\n请进行正确的轮廓跟踪!");
		return FALSE;
	}

	for(i=0;i<nNumNode;i++)
	{
		//获取第i个结点的数据
		m_EdgePoint = TraceArray.GetAt(i);
		sum += m_EdgePoint.CurPoint.y*DetaX[m_EdgePoint.nCurVerct]+0.5f*Alfa[m_EdgePoint.nCurVerct];
	}
	//返回由边界链码计算的面积
	*fArea = sum;

	return (TRUE);
}

////////////////////////////////////////////////////////////
//函数：BOOL AreaByGreen(m_fXScale,m_fYScale,float *fArea)
//功能：利用格林公式求封闭轮廓内的面积，要利用边界点坐标
//参数：m_fXScale，m_fYScale分别为X和Y方向的标定系数
///////////////////////////////////////////////////////////
BOOL CImgAnalyse::AreaByGreen(float m_fXScale, float m_fYScale, float *fArea)
{
	//循环变量
	int i;
	//边界点结构体变量
	EdgePoint m_EdgePoint;
	//边界点数，即TraceArray的大小
	int nNumNode;
	//当前点和下一点的坐标, 第一点的坐标
	int nCurX,nCurY,nNextX,nNextY,nFirstX,nFirstY;

	float sum = 0.0f;

	nNumNode = TraceArray.GetSize();

	if(nNumNode<1)
	{
		AfxMessageBox("是否没有进行轮廓跟踪?\n请进行正确的轮廓跟踪!");
		return FALSE;
	}

	//获取第0个结点的数据
	m_EdgePoint = TraceArray.GetAt(0);
	nCurX = m_EdgePoint.CurPoint.x;
	nCurY = m_EdgePoint.CurPoint.y;
	
	nFirstX = nCurX;
	nFirstY = nCurY;
	for(i=1;i<nNumNode;i++)
	{
		//获取第i个结点的数据
		m_EdgePoint = TraceArray.GetAt(i);
		nNextX = m_EdgePoint.CurPoint.x;
		nNextY = m_EdgePoint.CurPoint.y;
		//对当前点与下一点的X和Y坐标的乘积和累加
		sum += (nCurY*nNextX - nCurX*nNextY);
		nCurX = nNextX;
		nCurY = nNextY;
	}
	//加上最后一点与第0点的X和Y的乘积和
	sum += (nNextY*nFirstX - nNextX*nFirstY);
	//返回由边界坐标计算的面积
	 *fArea = (float)(0.5*m_fXScale*m_fYScale*sum);
	 return (TRUE);
}

////////////////////////////////////////////////////////////
//函数：BOOL PeriByChainCode()
//功能：由边界链码计算封闭区域的周长
//参数：分别为水平、垂直和斜方向的标定系数
////////////////////////////////////////////////////////////
BOOL CImgAnalyse::PeriByChainCode(float m_fXScale, float m_fYScale, float m_fXYScale, float *fP)
{
	//水平、垂直、斜向链码数
	int nHorNum = 0,nVerNum = 0,nDiaNum = 0;
	//循环变量
	int i;
	//边界点结构体变量
	EdgePoint m_EdgePoint;
	//当前矢量方向，即链码方向
	BYTE CurVerct;
	//边界点数，即TraceArray的大小
	int nNumNode;

	nNumNode = TraceArray.GetSize();
	if(nNumNode<1)
	{
		AfxMessageBox("是否没有进行轮廓跟踪?\n请进行正确的轮廓跟踪!");
		return FALSE;
	}

	for(i=0;i<nNumNode;i++)
	{
		//获取第i个结点的数据
		m_EdgePoint = TraceArray.GetAt(i);
		CurVerct = m_EdgePoint.nCurVerct;
		//水平方向链码数
		if(CurVerct==0 || CurVerct==4)
			nHorNum++;
		//垂直方向链码数
		else if(CurVerct==2 || CurVerct==6)
			nVerNum++;
		//斜向链码数
		else
			nDiaNum++;
	}
	//返回由边界链码计算的周长
	*fP = (nHorNum*m_fXScale+nVerNum*m_fYScale+nDiaNum*m_fXYScale);
	return (TRUE);
}

//////////////////////////////////////////////////////////////////
//函数：FourierFactor(int *nDimSize)
//功能：求傅立叶变换子
//参数：nDimSize	int型指针，用于传递数组大小
//返回：double型	变换后的频域数据
double * CImgAnalyse::FourierFactor(int *nDimSize)
{
	int j;
	EdgePoint m_EdgePoint;
	int nNumNode;
	nNumNode = TraceArray.GetSize();
	if(nNumNode<1)
	{
		AfxMessageBox("是否没有进行轮廓跟踪?\n请进行正确的轮廓跟踪!");
		return NULL;
	}

	//用于传递数组大小
	*nDimSize = nNumNode;
	//初始化为一维矩阵
	mFourier = zeros(1,nNumNode);
	//转换为复数矩阵
	mFourier = mcomplex(mFourier);

	for(j=0;j<nNumNode;j++)
	{
		m_EdgePoint = TraceArray.GetAt(j);
		//把X坐标赋予实部
		mFourier.r(j+1) = m_EdgePoint.CurPoint.x;
		//把Y坐标赋予虚部
		mFourier.i(j+1) = m_EdgePoint.CurPoint.y;
	}
	//调用Matlab下的一维傅立叶变换函数
	mFourier = fft(mFourier);
	//求复数的模
	mFourier = msqrt(dot_mul(real(mFourier),real(mFourier))+dot_mul(imag(mFourier),imag(mFourier)));

	double * mdata;
	mdata = new double[nNumNode];
	memcpy(mdata, mFourier.addr(),nNumNode*sizeof(double));
	return mdata;
}

//////////////////////////////////////////////////////////////
//函数：LabelArea(CDibObject *pDibObject)
//功能：对不同的连通成分作不同的标记,同时计算出区域面积和中心坐标
//说明：只对8位二值灰度图像(0和255两种值)有效，需经灰值化。
//参数：若pDibObject为空，则使用当前的图像对象。
//////////////////////////////////////////////////////////////
BOOL CImgAnalyse::LabelArea(CDibObject *pDibObject)
{
	if(pDibObject!=NULL) m_pDibObject=pDibObject;
	if(m_pDibObject==NULL) return(FALSE);
	int nWidth,nHeight;
	nWidth=m_pDibObject->GetWidth();
	nHeight=m_pDibObject->GetHeight();
	//当前像素的邻域方向数组
	int direct[8][2]={{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
	int lx,ly;//当前点的邻点坐标
	unsigned char *pOldBuffer;
	unsigned char *pNewBuffer,*pNewBits,*pNewTemp;
	int nWidthBytes, nNumColors, x, y, i, j, k;
	HGLOBAL hNewDib;
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes,m_pDibObject->GetNumBits());
	if(pOldBuffer == NULL) return(FALSE);
	nNumColors = m_pDibObject->GetNumColors();
	//位图数据偏移量
	DWORD offset=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD);
	//位图缓冲区大小
	DWORD dwNewSize=offset+nHeight*nWidthBytes;
	hNewDib=::GlobalAlloc(GHND,dwNewSize);
	if(hNewDib==NULL){AfxMessageBox("Alloc memory error!");return FALSE;}
	pNewBuffer=(unsigned char *)::GlobalLock(hNewDib);
	if(pNewBuffer==NULL)
	{
		::GlobalFree(hNewDib);
		AfxMessageBox("Lock memory error!");
		return FALSE;
	}
	pNewBits=(unsigned char *)&pNewBuffer[offset];
	memcpy(pNewBuffer,pOldBuffer,dwNewSize);

	LabelArray.RemoveAll();
	int label=100;//起始标记号
	LabelResult m_LabelResult;//存放标记结果的结构体
	int NumPixels;//区域面积
	int xStart,yStart;//区域起始点
	long cx,cy;//区域中心坐标
	switch(m_pDibObject->GetNumBits())
	{
	case 8:
		for(y=0;y<nHeight;y++)
		{
			pNewTemp=pNewBits;
			pNewTemp+=y*nWidthBytes;
			for(x=0;x<nWidth; x++)
			{
				if(pNewTemp[x]==255)
				{
					if(label>255){AfxMessageBox("标记太多!");return (FALSE);}
					pNewTemp[x]=label;//赋予当前元素以标记号
					int cnt=1;
					xStart=x;/////////////
					yStart=nHeight-1-y;/////////////
					cx=xStart,cy=yStart;
					NumPixels=1;
					while(cnt)
					{
						cnt=0;//循环标记
						for(j=0;j<nHeight;j++)
						{
							pNewTemp=pNewBits;
							for(k=0;k<nWidth;k++)
							{
								if(pNewTemp[k+j*nWidthBytes]==label)//如果找到一个相同的记号
								{
									for(i=0;i<8;i++)
									{
										lx=k+direct[i][0];
										ly=j+direct[i][1];
										//判断是否超出图像边界
										if(lx>=0 && lx<nWidth && ly>=0 && ly<nHeight)
											//循环标记八邻域
											if(*(pNewTemp+lx+ly*nWidthBytes)==255){
												*(pNewTemp+lx+ly*nWidthBytes)=label;
												NumPixels++;
												cx+=lx;
												cy+=nHeight-1-ly;
												cnt++;}
									}
								}
							}
						}
					}
					if(NumPixels==0){cx=0;cy=0;}
					else
					{
						cx=cx/NumPixels;cy=cy/NumPixels;
					}
					m_LabelResult.nLabelNo=label;
					m_LabelResult.StartPoint.x=xStart;
					m_LabelResult.StartPoint.y=yStart;
					m_LabelResult.CenterPoint.x=cx;
					m_LabelResult.CenterPoint.y=cy;
					m_LabelResult.LabelArea=NumPixels;
					LabelArray.Add(m_LabelResult);
					label=label+1;
				}
			}
		}
		break;
	}
	if(label-100==0){::GlobalUnlock(hNewDib);::GlobalFree(hNewDib);return (FALSE);}
	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib());
	::GlobalFree(m_pDibObject->GetDib());
	::GlobalUnlock(hNewDib);
	m_pDibObject->SetDib(hNewDib);
	return( TRUE );
}

////////////////////////////////////////////////////////
//函数名称：GetCentroid()
//功    能：返回区域标记LabelArea()中计算的质心坐标
//参    数：无
//返    回：POINT
////////////////////////////////////////////////////////
POINT CImgAnalyse::GetCentroid()
{
	//定义一个LabelResult类型的变量
	LabelResult m_LabelResult;
	
	POINT m_Center;
	if(LabelArray.GetSize() > 0)
	{
		//把模板数组LabelArray的第一个元素的数据赋予变量m_LabelResult
		m_LabelResult = LabelArray.GetAt(0);
		m_Center = m_LabelResult.CenterPoint;
	}
		return (m_Center);
}
