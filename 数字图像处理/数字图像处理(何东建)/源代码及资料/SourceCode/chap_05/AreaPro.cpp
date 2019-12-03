// AreaPro.cpp: implementation of the CAreaPro class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AreaPro.h"

IMPLEMENT_DYNCREATE(CAreaPro, CObject)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CAreaPro::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CAreaPro::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif

/***********************************************************************
*                                                                      *
*                               区处理类                               *
*                                                                      *
***********************************************************************/

////////////////////////////////////////////////////////////////////////
//构造函数CAreaPro()    
//----------------------------------------------------------------------
//基本功能：构造一个CAreaPro类的对象，如不传入CDibObject对象。第一次调   
//			用某一个处理函数时必须给出一个CDibObject对象指针。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////

CAreaPro::CAreaPro()
{

}

////////////////////////////////////////////////////////////////////////
//构造函数CAreaPro()    
//----------------------------------------------------------------------
//基本功能：构造一个CAreaPro类的对象并传入CDibObject对象。所有的操作都
//			针对该对象，直到另一个对象作为参数被传给图像处理函数。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CAreaPro::CAreaPro(CDibObject *pDibObject)
{
	m_pDibObject = pDibObject;
}

//析构函数
CAreaPro::~CAreaPro()
{

}

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//基本功能：本函数为CAreaPro类对象指定一个CDibObject对象指针
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：无。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CAreaPro::SetDibObjectClass( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}

////////////////////////////////////////////////////////////////////////
//int *GetHistogram()   
//----------------------------------------------------------------------
//基本功能：本函数获得传入的m_pDibObject对象中图像直方图。如果进行此调
//			整之前没有指定一个CDibObject对象指针，则必须在调整时加以指
//			定。任何未传入的坐标值或默认的-1坐标值都将被置为图像的最大值
//			或最大植。变量nX1和nY1将被置为0，nX2将被置为图像宽度减1，nY2
//			将被置为图像高度减1。想要在整个图像上进行操作时，最好的方法
//			是不传入nX1、nY1、nX2和nY2值。这样它们会被默认为整个图像。
//----------------------------------------------------------------------
//参数说明：int	nX1, 默认为-1
//			int	nY1, 默认为-1
//			int	nX2, 默认为-1
//			int	nY2, 默认为-1
//----------------------------------------------------------------------
//返    回：直方图数组指针*pBuffer其中：
//			nHistogramBuffer[]		存储亮度直方图数据
//			nHistogramBuffer[256]	存储红色直方图数据
//			nHistogramBuffer[512]	存储绿色直方图数据
//			nHistogramBuffer[768]	存储蓝直方图数据
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CAreaPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int *CAreaPro::GetHistogram( int nX1, int nY1, int nX2, int nY2)
{
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//坐标规整化
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	//创建直方图数据
	int *nHistogramBuffer = CreateHistogram(nX1, nY1, nX2, nY2, pBits,
		                                    pPalette, nWidthBytes);

	::GlobalUnlock(m_pDibObject->GetDib());

	return(nHistogramBuffer);

}

////////////////////////////////////////////////////////////////////////
//int *CreateHistogram()   
//----------------------------------------------------------------------
//基本功能：本函数创建传入m_pDibObject对象中图像的直方图。如果进行此调
//			整之前没有指定一个CDibObject对象指针，则必须在调整时加以指
//			定。任何未传入的坐标值或默认的-1坐标值都将被置为图像的最大值
//			或最大植。变量nX1和nY1将被置为0，nX2将被置为图像宽度减1，nY2
//			将被置为图像高度减1。想要在整个图像上进行操作时，最好的方法
//			是不传入nX1、nY1、nX2和nY2值。这样它们会被默认为整个图像。
//----------------------------------------------------------------------
//参数说明：int	nX1					默认为-1
//			int	nY1					默认为-1
//			int	nX2					默认为-1
//			int	nY2					默认为-1
//			unsigned char *pData	图像位图数据指针
//			RGBQUAD *pPalette		图像调色板指针
//			int nWidthBytes			图像字节宽度
//----------------------------------------------------------------------
//返    回：直方图数组指针*pBuffer其中：
//			pBuffer[]		存储亮度直方图数据
//			pBuffer[256]	存储红色直方图数据
//			pBuffer[512]	存储绿色直方图数据
//			pBuffer[768]	存储蓝直方图数据
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CAreaPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int *CAreaPro::CreateHistogram(int nX1, int nY1, int nX2, int nY2, 
							   unsigned char *pData, 
							   RGBQUAD *pPalette, 
							   int nWidthBytes)
{
	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);

	//分配直方图数据缓存区（数组）
	int *pBuffer = new int [256 * 4];
	//分配直方图数据缓存区失败
	if(pBuffer == NULL) return( NULL );

	//直方图数据缓存区清零
	memset(pBuffer, 0, (256 * 4) * sizeof(int));

	//变量定义
	DWORD dwGray;
	int x, y;
	unsigned char *pTemp, ucRed, ucGreen, ucBlue;

	//图像的高度
	int nHeight = m_pDibObject->GetHeight();

	switch(m_pDibObject->GetNumBits())
	{
	case 1:				//每像素位数为1，不处理
		break;
	case 4:				//每像素位数为4，不处理
		break;
	case 8:				//每像素位数为8		
		for( y = nY1; y <= nY2; y++ )
		{
			//数据指针定位到图像数据起始位置
			pTemp = pData;
			//数据指针定位到图像数据每行的起始零位置
			pTemp += ((nHeight - 1 - y) * nWidthBytes);
			//数据指针定位到图像数据每行的起始nX1位置
			pTemp += nX1;
			for(x = nX1; x <= nX2; x++)
			{
				//pTemp[x]为当前像素值，以此为调色板项的索引值，
				//取出调色板项的相应红绿蓝分量值。
				ucRed   = pPalette[pTemp[x]].rgbRed;
				ucGreen = pPalette[pTemp[x]].rgbGreen;
				ucBlue  = pPalette[pTemp[x]].rgbBlue;

				//按关系L=0.3R+0.59G+0.11B，得到亮度值
				dwGray  = ((DWORD) ucRed * 30 +
					        (DWORD) ucGreen * 59 +
					        (DWORD) ucBlue * 11) / 100;
				dwGray &= 0x000000ff;

				//亮度直方图数据
				pBuffer[dwGray]++;
				//红色直方图数据
				pBuffer[256 + ucRed]++;
				//绿色直方图数据
				pBuffer[512 + ucGreen]++;
				//蓝色直方图数据
				pBuffer[768 + ucBlue]++;

			}
		}
		break;
	case 16:				//每像素位数为16
		for(y = nY1; y <= nY2; y++)
		{
			//数据指针定位到图像数据起始位置
			pTemp = pData;
			//数据指针定位到图像数据每行的起始零位置
			pTemp += (( nHeight - 1 - y) * nWidthBytes);
			//数据指针定位到图像数据每行的起始nX1位置
			pTemp += (nX1 * 2);
			for(x = nX1; x <= nX2; x++)
			{
				//调用GETRGB555宏获取三原色分量
				GETRGB555(ucRed, ucGreen, ucBlue, pTemp);

				//按关系L=0.3R+0.59G+0.11B，得到亮度值
				dwGray = ((DWORD) ucRed * 30 +
					       (DWORD) ucGreen * 59 +
					       (DWORD) ucBlue * 11) / 100;
				dwGray &= 0x000000ff;

				//亮度直方图数据
				pBuffer[dwGray]++;
				//红色直方图数据
				pBuffer[256 + ucRed]++;
				//绿色直方图数据
				pBuffer[512 + ucGreen]++;
				//蓝色直方图数据
				pBuffer[768 + ucBlue]++;

				//数据指针加2
				pTemp += 2;
			}
		}
		break;
	case 24:				//每像素位数为24
		for(y = nY1; y < nY2; y++)
		{
			//数据指针定位到图像数据起始位置
			pTemp = pData;
			//数据指针定位到图像数据每行的起始零位置
			pTemp += (( nHeight - 1 - y) * nWidthBytes);
			//数据指针定位到图像数据每行的起始nX1位置
			pTemp += (nX1 * 3);

			for(x=nX1; x<=nX2; x++)
			{
				//获取像素颜色的三原色。
				ucRed   = pTemp[x * 3 + 2];
				ucGreen = pTemp[x * 3 + 1];
				ucBlue  = pTemp[x * 3];

				//按关系L=0.3R+0.59G+0.11B，得到亮度值
				dwGray  = ((DWORD) ucRed * 30 +
					        (DWORD) ucGreen * 59 +
					        (DWORD) ucBlue * 11) / 100;
				dwGray &= 0x000000ff;

				//亮度直方图数据
				pBuffer[dwGray]++;
				//红色直方图数据
				pBuffer[256 + ucRed]++;
				//绿色直方图数据
				pBuffer[512 + ucGreen]++;
				//蓝色直方图数据
				pBuffer[768 + ucBlue]++;

				//数据指针加3
				pTemp += 3;
			}
		}
		break;
	case 32:				//每像素位数为24
		for(y = nY1; y <= nY2; y++)
		{
			//数据指针定位到图像数据起始位置
			pTemp = pData;
			//数据指针定位到图像数据每行的起始零位置
			pTemp += (( nHeight - 1 - y) * nWidthBytes);
			//数据指针定位到图像数据每行的起始nX1位置
			pTemp += (nX1 * 4);

			for(x = nX1; x <= nX2; x++)
			{
				//调用GETRGB888宏获取像素颜色的三原色。
				GETRGB888(ucRed, ucGreen, ucBlue, pTemp);

				//按关系L=0.3R+0.59G+0.11B，得到亮度值
				dwGray = ((DWORD) ucRed * 30 +
					       (DWORD) ucGreen * 59 +
					       (DWORD) ucBlue * 11) / 100;

				dwGray &= 0x000000ff;

				//亮度直方图数据
				pBuffer[dwGray]++;
				//红色直方图数据
				pBuffer[256 + ucRed]++;
				//绿色直方图数据
				pBuffer[512 + ucGreen]++;
				//蓝色直方图数据
				pBuffer[768 + ucBlue]++;

				//数据指针加4
				pTemp += 4;
			}
		}
		break;
	}
	
	return( pBuffer );
}

////////////////////////////////////////////////////////////////////////
//BOOL Convolution()   
//----------------------------------------------------------------------
//基本功能：本函数对指定的两个数组进行卷积运算。
//----------------------------------------------------------------------
//参数说明：float *fpData		被执行卷积的数组
//			float *fpKernel		卷积核数组
//			float fCoef			卷积系数
//			int nSize			卷积尺寸
//			unsigned char *nResult		卷积结果
//----------------------------------------------------------------------
//返    回：无。
//----------------------------------------------------------------------
//注    意：此函数声明为私有型，只能在CAreaPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CAreaPro::Convolution(float *fpData, float *fpKernel, float fCoef, 
						   int nSize, unsigned char *nResult)
{

	int i;
	float Sum = 0, fResult;

	//计算卷积
	for(i = 0; i < nSize; i++)
	{
		Sum += fpData[i] * fpKernel[i];
	}

	//执行卷积后的结果
	fResult = Sum / fCoef;

	//求绝对值
	fResult = (float)fabs(fResult);
	
	//判断是否超过255
	if(fResult > 255.0 )
	{
		// 直接赋值为255
		fResult = 255.0;
	}
	
	//对卷积结果四舍五入，并转换成unsigned char类型作为最后返回结果
	*nResult = (unsigned char) (fResult + 0.5);			
}

////////////////////////////////////////////////////////////////////////
//void Smoothing()   
//----------------------------------------------------------------------
//基本功能：该函数用指定的模板（任意大小）来对指针m_pDibObject中的图像
//			对象进行平滑操作，参数nTempH指定模板的高度，参数nTempW指定模
//			板的宽度，参数nTempXc和nTempYc指定模板的中心元素坐标，参数
//			fpArray为指定模板元素数组的指针，fCoef指定系数。任何未指
//			定的或默认为-1的坐标将被置为图像的边缘值。比如：nX1和nY1会被
//			置为0；nX2和nY2会被置为图像的宽和高。对整个图像进行低通滤波
//			的最好方法是不传递任何参数。函数使用原先传入的CDibObject对
//			象指针。
//----------------------------------------------------------------------
//参数说明：float *fpArray	指向模板数组的指针
//			float fCoef		模板系数
//			int   nTempW	模板的宽度
//			int   nTempH	模板的高度
//			int   nTempXc	模板的中心元素X坐标 ( <= nTempW - 1)
//			int   nTempYc	模板的中心元素Y坐标 ( <= nTempH - 1)
//			int	  nX1		默认为-1
//			int   nY1		默认为-1
//			int	  nX2		默认为-1
//			int	  nY2		默认为-1
//----------------------------------------------------------------------
//返    回：无。
//----------------------------------------------------------------------
//注    意：此函数调用区处理类的TemplateOperation()模板操作函数对图像进行
//			平滑操作，默认的模板是3×3的Box平滑模板。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::Smoothing(float *fpArray, float fCoef, int nTempW, 
						int nTempH, int nTempXc, int nTempYc,
						int nX1, int nY1, int nX2, int nY2)
{
	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);

	//只处理8位图像
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("目前只支持8位灰度图像的处理！");
		return( FALSE );
	}

	//坐标规整化
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//对边界像素不作处理
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	DWORD dwNewSize;

	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits(),
											   &nNewWidthBytes, 8);
	if( pBuffer == NULL ) return( NULL );

	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	dwNewSize = nNewWidthBytes * nHeight;

	//定义用于存储色相值的临时数组
	double *pHue = new double [dwNewSize];
	if(pHue == NULL) return(FALSE);
	memset(pHue, 0, dwNewSize * sizeof(double));

	//定义用于存储饱和度值的临时数组
	double *pSaturation = new double [dwNewSize];
	if(pSaturation == NULL) return(FALSE);
	memset(pSaturation, 0, dwNewSize * sizeof(double));

	//定义用于存储亮度值的临时数组
	unsigned char *pLight = new unsigned char [dwNewSize];
	if(pLight == NULL) return(FALSE);
	memset(pLight, 0, dwNewSize * sizeof(unsigned char));

	float *fTempArray;

	//默认为3×3的高通滤波器1模板
	static float fpDefaultArray[] = {1.0, 1.0, 1.0, 
							         1.0, 1.0, 1.0,
							         1.0, 1.0, 1.0};

	//没有传入模板，用默认模板
	if( fpArray == NULL ) 
	{
		fTempArray = fpDefaultArray;
		fCoef = 9;
	}
	//采用传入的模板
	else fTempArray = fpArray;

	//调用Template操作函数
	if(!TemplateOperation(fTempArray, fCoef, nTempW, nTempH, nTempXc, nTempYc,
						pBits, nWidthBytes, nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//内存解锁
	::GlobalUnlock(m_pDibObject->GetDib());

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//void Sharping()   
//----------------------------------------------------------------------
//基本功能：该函数用指定的模板（任意大小）来对指针m_pDibObject中的图像
//			对象进行锐化操作，参数nTempH指定模板的高度，参数nTempW指定模
//			板的宽度，参数nTempXc和nTempYc指定模板的中心元素坐标，参数
//			fpArray为指定模板元素数组的指针，fCoef指定系数。任何未指
//			定的或默认为-1的坐标将被置为图像的边缘值。比如：nX1和nY1会被
//			置为0；nX2和nY2会被置为图像的宽和高。对整个图像进行低通滤波
//			的最好方法是不传递任何参数。函数使用原先传入的CDibObject对
//			象指针。
//----------------------------------------------------------------------
//参数说明：float *fpArray	指向模板数组的指针
//			int nTempNum	模板个数
//			int   nTempW	模板的宽度
//			int   nTempH	模板的高度
//			int   nTempXc	模板的中心元素X坐标 ( <= nTempW - 1)
//			int   nTempYc	模板的中心元素Y坐标 ( <= nTempH - 1)
//			int	  nX1		默认为-1
//			int   nY1		默认为-1
//			int	  nX2		默认为-1
//			int	  nY2		默认为-1
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::Sharping(float *fpArray, int *nDirection, int nTempW, 
						int nTempH, int nTempXc, int nTempYc,
						int nX1, int nY1, int nX2, int nY2)
{
	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);

	//只处理8位图像
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("目前只支持8位灰度图像的处理！");
		return( FALSE );
	}

	//坐标规整化
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//对边界像素不作处理
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//定义变量
	unsigned char *pBuffer, *pBits, *pNewBits1, *pNewBits2;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;
	DWORD dwNewSize;

	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	HGLOBAL hNewDib1, hNewDib2;

	//新图像文件大小（以字节为单位）
	dwNewSize = nWidthBytes * nHeight;
	//为新图像分配内存
	hNewDib1 = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//内存分配失败
	if( hNewDib1 == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像指针
	pNewBits1 = (unsigned char *) ::GlobalLock( hNewDib1 );
	if( pNewBits1 == NULL )
	{
		::GlobalFree( hNewDib1 );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//复制图像数据
	memcpy(pNewBits1, pBits, nWidthBytes * nHeight);

	//为新图像分配内存
	hNewDib2 = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//内存分配失败
	if( hNewDib2 == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像指针
	pNewBits2 = (unsigned char *) ::GlobalLock( hNewDib2 );
	if( pNewBits2 == NULL )
	{
		::GlobalFree( hNewDib2 );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//复制图像数据
	memcpy(pNewBits2, pBits, nWidthBytes * nHeight);

	float *fTempArray;

	//默认为3×3的Kirsch梯度模板
	static float fpDefaultArray[] = {5.0, 5.0, 5.0, 
							         -3.0, 0.0, -3.0,
							         -3.0, -3.0, -3.0};

	//没有传入模板，用默认模板
	if( fpArray == NULL ) fTempArray = fpDefaultArray;
	//采用传入的模板
	else fTempArray = fpArray;

	int *nTempDirection;

	//默认为8个方向
	static int nDefaultDirection[] = {1, 1, 1, 1, 1, 1, 1, 1};

	//没有传入方向，用默认方向
	if( nDirection == NULL ) nTempDirection = nDefaultDirection;
	//采用传入的方向
	else nTempDirection = nDirection;
	
	//起始方向
	int nStartD = 0;
	
	int i = 0;
	//查找起始方向
	while(nTempDirection[i] == 0 && i < 8)
	{
		//方向旋转45°		
		i++;

		//给起始方向赋值
		nStartD = i;

		//对模板进行旋转
		TemplateRotating(fTempArray);

	}

	//没指定起始方向，返回
	if(i == 8) return(FALSE);

	//调用Template操作函数
	if(!TemplateOperation(fTempArray, 1.0, nTempW, nTempH, nTempXc, nTempYc,
		pNewBits1, nWidthBytes, 
		nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	
	//定义与图像数据操作有关的变量
	unsigned char *pNewTemp1, *pNewTemp2;
	int x, y;

	for(i = nStartD + 1; i < 8; i++)
	{
		
		//对模板进行旋转
		TemplateRotating(fTempArray);

		if(nTempDirection[i] == 1)
		{
			//调用Template操作函数
			if(!TemplateOperation(fTempArray, 1.0, nTempW, nTempH, nTempXc, nTempYc,
				pNewBits2, nWidthBytes, 
				nX1, nY1, nX2, nY2))
			{
				return(FALSE);
			}
			
			switch(m_pDibObject->GetNumBits())
			{
			case 8:			//8位图像
				//行位置
				for(y = nY1; y <= nY2; y++ )
				{
					//新图像数据指针定位到起始位置
					pNewTemp1 = pNewBits1;
					//新图像数据指针定位到图像数据每行的起始零位置
					pNewTemp1 += y * nWidthBytes;
					//新图像数据指针定位到图像数据每行的起始nX1位置
					pNewTemp1 += nX1;
					
					//新图像数据指针定位到起始位置
					pNewTemp2 = pNewBits2;
					//新图像数据指针定位到图像数据每行的起始零位置
					pNewTemp2 += y * nWidthBytes;
					//新图像数据指针定位到图像数据每行的起始nX1位置
					pNewTemp2 += nX1;
					
					//列位置
					for(x = nX1; x <= nX2; x++)
					{
						if(pNewTemp2[x] > pNewTemp1[x])
							pNewTemp1[x] = pNewTemp2[x];
					}
				}
				break;
			}
			//复制图像数据
			memcpy(pNewBits2, pBits, nWidthBytes * nHeight);
		}
		
	}

	//复制图像数据
	memcpy(pBits, pNewBits1, nWidthBytes * nHeight);

	//内存解锁
	::GlobalUnlock(m_pDibObject->GetDib());

	//释放不再使用的内存
	::GlobalUnlock( hNewDib1 );
	::GlobalFree( hNewDib1 );
	::GlobalUnlock( hNewDib2 );
	::GlobalFree( hNewDib2 );

	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL LapSharping()   
//----------------------------------------------------------------------
//基本功能：该函数用指定的模板（任意大小）来对指针m_pDibObject中的图像
//			对象进行Laplacian锐化操作，参数nTempH指定模板的高度，参数
//			nTempW指定模板的宽度，参数nTempXc和nTempYc指定模板的中心元素
//			坐标，参数fpArray为指定模板元素数组的指针，fCoef指定系数。任
//			何未指定的或默认为-1的坐标将被置为图像的边缘值。比如：nX1和
//			nY1会被置为0；nX2和nY2会被置为图像的宽和高。对整个图像进行低
//			通滤波的最好方法是不传递任何参数。函数使用原先传入的
//			CDibObject对象指针。
//----------------------------------------------------------------------
//参数说明：float *fpArray	指向模板数组的指针
//			float fCoef		模板系数
//			int   nTempW	模板的宽度
//			int   nTempH	模板的高度
//			int   nTempXc	模板的中心元素X坐标 ( <= nTempW - 1)
//			int   nTempYc	模板的中心元素Y坐标 ( <= nTempH - 1)
//			int	  nX1		默认为-1
//			int   nY1		默认为-1
//			int	  nX2		默认为-1
//			int	  nY2		默认为-1
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::LapSharping(float *fpArray, float fCoef, int nTempW, 
						int nTempH, int nTempXc, int nTempYc,
						int nX1, int nY1, int nX2, int nY2)
{
	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);

	//只处理8位图像
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("目前只支持8位灰度图像的处理！");
		return( FALSE );
	}

	//坐标规整化
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//对边界像素不作处理
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	DWORD dwNewSize;

	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits(),
											   &nNewWidthBytes, 8);
	if( pBuffer == NULL ) return( NULL );

	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	dwNewSize = nNewWidthBytes * nHeight;

	//定义用于存储色相值的临时数组
	double *pHue = new double [dwNewSize];
	if(pHue == NULL) return(FALSE);
	memset(pHue, 0, dwNewSize * sizeof(double));

	//定义用于存储饱和度值的临时数组
	double *pSaturation = new double [dwNewSize];
	if(pSaturation == NULL) return(FALSE);
	memset(pSaturation, 0, dwNewSize * sizeof(double));

	//定义用于存储亮度值的临时数组
	unsigned char *pLight = new unsigned char [dwNewSize];
	if(pLight == NULL) return(FALSE);
	memset(pLight, 0, dwNewSize * sizeof(unsigned char));

	float *fTempArray;

	//默认为3×3的高通滤波器1模板
	static float fpDefaultArray[] = {-1.0, -1.0, -1.0, 
							         -1.0,  9.0, -1.0,
							         -1.0, -1.0, -1.0};

	//没有传入模板，用默认模板
	if( fpArray == NULL ) fTempArray = fpDefaultArray;
	//采用传入的模板
	else fTempArray = fpArray;

	//调用Template操作函数
	if(!TemplateOperation(fTempArray, fCoef, nTempW, nTempH, nTempXc, nTempYc,
						pBits, nWidthBytes, nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//内存解锁
	::GlobalUnlock(m_pDibObject->GetDib());

	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL MedianFilter()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject图像对象进行中值滤波。任何未指定的或
//			默认为-1的坐标将被置为图像的边缘值。比如：nX1和nY1会被置为0；
//			nX2和nY2会被置为图像的宽和高。对整个图像进行中值滤波的最好方
//			法是不传递任何参数。如果不指定一个CDibObject对象指针，函数将
//			使用原先传入的CDibObject对象指针。
//----------------------------------------------------------------------
//参数说明：int	  nType		获取中值的方式, 默认为1
//								0——冒泡排序
//								1——数组排序
//			int   nTempW	窗口的宽度
//			int   nTempH	窗口的高度
//			int   nTempXc	窗口的中心元素X坐标
//			int   nTempYc	窗口的中心元素Y坐标
//			int	  nX1		默认为-1
//			int   nY1		默认为-1
//			int	  nX2		默认为-1
//			int	  nY2		默认为-1
//					
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::MedianFilter(int nType, int nTempW, int nTempH, int nTempXc, 
							int nTempYc, int nX1, int nY1, int nX2, int nY2)
{
   //图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//对1位及4位图像不作任何操作直接返回
	if(m_pDibObject->GetNumBits() != 8)
	{
		// 提示用户参数设置错误
		AfxMessageBox("只支持8位图像,请重新载入！");
		
		// 返回
		return( FALSE );
	}
	
	//坐标规整化
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//对边界像素不作处理
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 >= nWidth - nTempW + nTempXc) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 >= nHeight - nTempH + nTempYc) nY2 = nHeight - nTempH + nTempYc + 1;
	
	//定义变量
	unsigned char Data;
	//定义与图像数据操作有关的变量
	unsigned char *pOldBuffer, *pNewBuffer, 
				  *pOldBits, *pNewBits, 
				  *pOldTemp, *pNewTemp,
				  *pNeighborTemp;

	BITMAPFILEHEADER *pOldBFH, *pNewBFH;

	BITMAPINFOHEADER *pOldBIH, *pNewBIH;

	RGBQUAD *pOldPalette, *pNewPalette;

	int nWidthBytes, nNumColors, x, y, i, j;
	
	//获取原图像指针
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//原图像文件头
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	
	DWORD dwNewSize;
	HGLOBAL hNewDib;
	
	//新图像文件大小（以字节为单位）
	dwNewSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + 
				nNumColors * sizeof( RGBQUAD ) + nWidthBytes * nHeight;

	//为新图像分配内存
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );

	//内存分配失败
	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}
	
	//新图像指针
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}
	
	//新图像文件头
	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	//新图像信息头
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	//新图像调色板指针
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//新图像数据指针
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors*sizeof(RGBQUAD)];
	
	//复制原图像数据到新图像

	//复制文件头
	memcpy(pNewBFH, pOldBFH, sizeof(BITMAPFILEHEADER));
	
	//复制信息头
	memcpy(pNewBIH, pOldBIH, sizeof(BITMAPINFOHEADER));
	
	//复制调色板
	for(i = 0; i < nNumColors; i++ ) pNewPalette[i] = pOldPalette[i];
	
	//复制图像数据
	memcpy( pNewBits, pOldBits, nWidthBytes * nHeight );

	//计算模板的尺寸
	int nSize = nTempW * nTempH;
	
	//定义查找中值用的临时数组
	unsigned char *pGray = new unsigned char [nTempW * nTempH];
	if(pGray == NULL) return( NULL );
	memset(pGray, 0, (nSize) * sizeof(unsigned char));

	unsigned char nData;

	switch(m_pDibObject->GetNumBits())
	{
	case 8:			//8位图像
		//行位置
		for(y = nY1; y < nY2; y++ )
		{
			//原图像数据指针定位到起始位置
			pOldTemp = pOldBits;
			//原图像数据指针定位到图像数据每行的起始零位置
			pOldTemp += (nHeight - 1 - y) * nWidthBytes;
			//原图像数据指针定位到图像数据每行的起始nX1位置
			pOldTemp += nX1;
			
			//新图像数据指针定位到起始位置
			pNewTemp = pNewBits;
			//新图像数据指针定位到图像数据每行的起始零位置
			pNewTemp += (nHeight - 1 - y) * nWidthBytes;
			//新图像数据指针定位到图像数据每行的起始nX1位置
			pNewTemp += nX1;
			
			//列位置
			for(x = nX1; x < nX2; x++)
			{
				//取出原图像nTempW×nTempH邻域内的像素值
				for (i = 0; i < nTempH; i++)
				{
					for (j = 0; j < nTempW; j++)
					{
						//数据指针指向当前像素
						pNeighborTemp = pOldTemp;
						//数据指针指向原图像第y - nTempYc + i行。
						pNeighborTemp += nWidthBytes * (nTempYc - i);
						//第x - nTempXc + j列像素的指针
						pNeighborTemp += (j - nTempXc);
						
						//获取像素值
						Data = *pNeighborTemp;

						pGray[i*nTempW+j] = ( pOldPalette[Data].rgbRed * 30 +
										pOldPalette[Data].rgbGreen * 59 +
										pOldPalette[Data].rgbBlue * 11 ) / 100;
					}
				}
				
				nData = GetMedian(pGray, nSize, nType);
				//将计算结果赋于新图像
				*pNewTemp = (unsigned char) m_pDibObject->GetNearestIndex( 
					pNewPalette[nData].rgbRed, pNewPalette[nData].rgbGreen, 
					pNewPalette[nData].rgbBlue,	pNewPalette, nNumColors );

				//新旧图像数据指针加1
				pOldTemp++;
				pNewTemp++;
				
			}
		}
		break;
	}
	
	//释放内存
	delete [] pGray;

	::GlobalUnlock( m_pDibObject->GetDib() );
    ::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
    m_pDibObject->SetDib( hNewDib );
    return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL TemplateOperation()   
//----------------------------------------------------------------------
//基本功能：该函数用指定的模板（任意大小）来对图像数据区的数据进行模板操
//			作，参数nTempH指定模板的高度，参数nTempW指定模板的宽度，参数
//			nTempXc和nTempYc指定模板的中心元素坐标，参数fpArray为指定模
//			板元素数组的指针，fCoef指定模板系数。
//----------------------------------------------------------------------
//参数说明：float *fpArray			指向模板数组的指针
//			float fCoef				模板系数
//			int   nTempW			模板的宽度
//			int   nTempH			模板的高度
//			int   nTempXc			模板的中心元素X坐标 ( <= nTempW - 1)
//			int   nTempYc			模板的中心元素Y坐标 ( <= nTempH - 1)
//			unsigned char *pData	图像数据指针
//			int nWidthBytes			图像字节宽度
//			int	  nX1				处理区域左边界		
//			int   nY1				处理区域上边界
//			int	  nX2				处理区域右边界
//			int	  nY2				处理区域下边界
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CAreaPro类中使用。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::TemplateOperation(float *fpArray, float fCoef, 
						int nTempW, int nTempH, int nTempXc, int nTempYc,
						unsigned char *pData, int nWidthBytes, 
						int nX1, int nY1, int nX2, int nY2)
{
	//获取图像高度（以像素为单位）
	int nHeight = m_pDibObject->GetHeight();

	//定义变量
	unsigned char Data;

	//定义与图像数据操作有关的变量
	unsigned char *pOldBits, *pNewBits, 
				  *pOldTemp, *pNewTemp,
				  *pNeighborTemp;
	int nNumColors, x, y, i, j;
	DWORD dwNewSize;

	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();

	//原图像数据指针
	pOldBits = pData;

	HGLOBAL hNewDib;

	//新图像文件大小（以字节为单位）
	dwNewSize = nWidthBytes * nHeight;
	//为新图像分配内存
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//内存分配失败
	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像指针
	pNewBits = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//复制图像数据
	memcpy( pNewBits, pOldBits, nWidthBytes * nHeight );

	//定义卷积运算中用的临时数组
	float *pGray = new float [nTempW * nTempH];
	if(pGray == NULL) return( NULL );
	memset(pGray, 0, (nTempW * nTempH) * sizeof(float));
	
	//行位置
	for(y = nY1; y < nY2; y++ )
	{
		//原图像数据指针定位到起始位置
		pOldTemp = pOldBits;
		//原图像数据指针定位到图像数据每行的起始零位置
		pOldTemp += (nHeight -1 - y) * nWidthBytes;
		//原图像数据指针定位到图像数据每行的起始nX1-1位置
		pOldTemp += nX1;
		
		//新图像数据指针定位到起始位置
		pNewTemp = pNewBits;
		//新图像数据指针定位到图像数据每行的起始零位置
		pNewTemp += (nHeight -1 - y) * nWidthBytes;
		//新图像数据指针定位到图像数据每行的起始nX1位置
		pNewTemp += nX1;
		
		//列位置
		for(x = nX1; x < nX2; x++)
		{
			//取出原图像nTempW×nTempH邻域内的像素值
			for (i = 0; i < nTempH; i++)
			{
				for (j = 0; j < nTempW; j++)
				{
					//数据指针指向当前像素
					pNeighborTemp = pOldTemp;
					//数据指针指向原图像第y - nTempYc + i行。
					pNeighborTemp += nWidthBytes * (nTempYc - i);
					//第x - nTempXc + j列像素的指针
					pNeighborTemp += (j - nTempXc);
					
					//获取像素值
					Data = *pNeighborTemp;

					pGray[i * nTempW + j] = (float)Data;
				}
			}

			//卷积核尺寸
			int nSize = nTempW * nTempH;
			
			//对红色分量作卷积
			Convolution(pGray, fpArray, fCoef, nSize, &Data);
			
			//将计算结果赋于新图像
			*pNewTemp = Data;
			
			//新旧图像数据指针加1
			pOldTemp++;
			pNewTemp++;
			
		}
	}
	
	//释放内存
	delete [] pGray;

	//复制图像数据
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	
	return( TRUE );
}

BOOL CAreaPro::GradSharping(int Threshold, int nX1, int nY1, int nX2, int nY2,  
							CDibObject *pDibObject)
{
	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);

	//只处理8位图像
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("目前只支持8位灰度图像的处理！");
		return( FALSE );
	}

	//坐标规整化
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	int nTempXc = 0;
	int nTempYc = 0;
	int nTempW  = 2;
	int nTempH  = 2;

	//对边界像素不作处理
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//定义变量
	unsigned char *pBuffer, *pBits, *pNewBits1, *pNewBits2;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;
	DWORD dwNewSize;

	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	HGLOBAL hNewDib1, hNewDib2;

	//新图像文件大小（以字节为单位）
	dwNewSize = nWidthBytes * nHeight;
	//为新图像分配内存
	hNewDib1 = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//内存分配失败
	if( hNewDib1 == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像指针
	pNewBits1 = (unsigned char *) ::GlobalLock( hNewDib1 );
	if( pNewBits1 == NULL )
	{
		::GlobalFree( hNewDib1 );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//复制图像数据
	memcpy(pNewBits1, pBits, nWidthBytes * nHeight);

	//为新图像分配内存
	hNewDib2 = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//内存分配失败
	if( hNewDib2 == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像指针
	pNewBits2 = (unsigned char *) ::GlobalLock( hNewDib2 );
	if( pNewBits2 == NULL )
	{
		::GlobalFree( hNewDib2 );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//复制图像数据
	memcpy(pNewBits2, pBits, nWidthBytes * nHeight);

	float aTemplate[4];

	//Roberts H1模板
	aTemplate[0] =  1.0;
	aTemplate[1] =  0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] =  0.0;

	//调用Template操作函数
	if(!TemplateOperation(aTemplate, 1.0, nTempW, nTempH, nTempXc, nTempYc,
						pNewBits1, nWidthBytes, 
						nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//Roberts H2模板
	aTemplate[0] =  0.0;
	aTemplate[1] =  1.0;
	aTemplate[2] =  0.0;
	aTemplate[3] = -1.0;

	//调用Template操作函数
	if(!TemplateOperation(aTemplate, 1.0, nTempW, nTempH, nTempXc, nTempYc,
						pNewBits2, nWidthBytes, 
						nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//定义与图像数据操作有关的变量
	unsigned char *pOldTemp, *pNewTemp1, *pNewTemp2;
	int x, y;

	DWORD dwTemp;

	switch(m_pDibObject->GetNumBits())
	{
	case 8:			//8位图像
		//行位置
		for(y = nY1; y <= nY2; y++ )
		{
			//原图像数据指针定位到起始位置
			pOldTemp = pBits;
			//原图像数据指针定位到图像数据每行的起始零位置
			pOldTemp += y * nWidthBytes;
			//原图像数据指针定位到图像数据每行的起始nX1-1位置
			pOldTemp += nX1;

			//新图像数据指针定位到起始位置
			pNewTemp1 = pNewBits1;
			//新图像数据指针定位到图像数据每行的起始零位置
			pNewTemp1 += y * nWidthBytes;
			//新图像数据指针定位到图像数据每行的起始nX1位置
			pNewTemp1 += nX1;

			//新图像数据指针定位到起始位置
			pNewTemp2 = pNewBits2;
			//新图像数据指针定位到图像数据每行的起始零位置
			pNewTemp2 += y * nWidthBytes;
			//新图像数据指针定位到图像数据每行的起始nX1位置
			pNewTemp2 += nX1;

			//列位置
			for(x = nX1; x <= nX2; x++)
			{
				//计算目标图像中当前点的最终像素值
				dwTemp = (DWORD)(pNewTemp1[x] + pNewTemp2[x]);
				//dwTemp = (DWORD)(sqrt(pNewTemp1[x] * pNewTemp1[x] 
				//					+ pNewTemp2[x] * pNewTemp2[x]));
				//判断是否小于阈值
				if(dwTemp < 255)
				{
					//判断是否大于阈值，对于小于的情况，灰度值不变
					if((int)dwTemp >= Threshold)
					{
						pOldTemp[x] = (unsigned char)dwTemp;
					}
				}
				else
				{
					//直接赋值为255
					pOldTemp[x] = 255;
				}
			}
		}
		break;
	}

	//内存解锁
	::GlobalUnlock(m_pDibObject->GetDib());

	//释放不再使用的内存
	::GlobalUnlock( hNewDib1 );
	::GlobalFree( hNewDib1 );
	::GlobalUnlock( hNewDib2 );
	::GlobalFree( hNewDib2 );

	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL TemplateRotating()   
//----------------------------------------------------------------------
//基本功能：该函数用于对传入的模板进行旋转操作，以获取不同方向上的模板。
//----------------------------------------------------------------------
//参数说明：float *fpTemplate		指向模板数组的指针
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
//注    意：此函数为保护型，只能在CAreaPro类中使用，并只能用于3×3的模板。
//			此函数每次旋转45°
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CAreaPro::TemplateRotating(float *fpTemplate)
{
	float fTemp[8];
	float fDataTemp;

	//将模板周围值按顺时针方向赋于一临时数组
	fTemp[0] = fpTemplate[0];
	fTemp[1] = fpTemplate[1];
	fTemp[2] = fpTemplate[2];
	fTemp[3] = fpTemplate[5];
	fTemp[4] = fpTemplate[8];
	fTemp[5] = fpTemplate[7];
	fTemp[6] = fpTemplate[6];
	fTemp[7] = fpTemplate[3];

	//f进行数据旋转
	fDataTemp = fTemp[7];
	
	for(int i = 7; i > 0; i--)
	{
		fTemp[i] = fTemp[i - 1];
	}
	
	fTemp[0] = fDataTemp;

	//求得结果
	fpTemplate[0] = fTemp[0];
	fpTemplate[1] = fTemp[1];
	fpTemplate[2] = fTemp[2];
	fpTemplate[5] = fTemp[3];
	fpTemplate[8] = fTemp[4];
	fpTemplate[7] = fTemp[5];
	fpTemplate[6] = fTemp[6];
	fpTemplate[3] = fTemp[7];
}

////////////////////////////////////////////////////////////////////////
//BOOL GetMedian()   
//----------------------------------------------------------------------
//基本功能：本函数对指定的一个数组进行排序，并得到其中值。
//----------------------------------------------------------------------
//参数说明：int nType					排序方式，其取值如下：
//											0——冒泡排序
//											1——数组排序
//			unsigned char *pData		数组
//			int nSize					数组大小
//----------------------------------------------------------------------
//返    回：无。
//----------------------------------------------------------------------
//注    意：此函数声明为私有型，只能在CAreaPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char CAreaPro::GetMedian(unsigned char *pData, int nSize, int nType)
{
	int nMedian;

	//指定的排序算法不可识别，用冒泡排序算法。
	if(nType < 0 || nType > 1) nType = 1;

	switch( nType )
	{
	case 0: //冒泡排序
		{
			int nTemp;
			int nLast = nSize - 1;
			bool bSorted = true;
			
			do
			{
				bSorted = true;
				for (int i = 0; i < nLast; i++)
				{
					if (pData[i] > pData[i + 1])
					{	
						nTemp = pData[i];
						pData[i] = pData[i + 1];
						pData[i + 1] = nTemp;
						bSorted = false;
					}
					
				}
				nLast--;
			} while (!bSorted);
			
		}
		nMedian = pData[nSize / 2];
		break;
	case 1:
		{
			int i;
			int nHisto[256];
			for( i = 0; i < 256; i++) nHisto[ i ] = 0;
			//统计相同灰度值出现的次数				
			for( i = 0; i < nSize; i++)
			{    
				nHisto[(int)pData[i]]++;
			}

			//取直方图的中值
			int nCount = 0;
			for( i = 0; i < 256; i++)
			{ 
				//存放直方图中相同灰度值出现次数的累加和
				nCount += nHisto[i];
				if(nCount >= (int)(nSize / 2 + 1))
				{  
					nMedian = i;
					break;
				}
			}			
		}
		break;
		
	}
	return nMedian;
}

////////////////////////////////////////////////////////////////////////
//BOOL EdgeTracing()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject图像对象进行轮廓跟踪。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	图像对象指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::EdgeTracing(CDibObject *pDibObject)
{
   //图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//只支持8位图像
	if(m_pDibObject->GetNumBits() != 8)
	{
		// 提示用户参数设置错误
		AfxMessageBox("只支持8位图像,请重新载入！");
		
		// 返回
		return( FALSE );
	}
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//定义变量
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//是否找到起始点及回到起始点
	bool bFindStartPoint;

	//是否扫描到一个边界点
	bool bFindPoint;

	//起始边界点与当前边界点
	CPoint StartPoint,CurrentPoint;

	//八个方向和起始扫描方向
	int Direction[8][2]={
							{-1, 1},
							{ 0, 1},
							{ 1, 1},
							{ 1, 0},
							{ 1,-1},
							{ 0,-1},
							{-1,-1},
							{-1, 0}
						};
	int BeginDirect;

	//原图像指针
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//为新图像数据区分配内存
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//新图像数据区大小
	dwNewSize = nWidthBytes * nHeight;
	//为新图像分配内存
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像指针
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//用255（白色）填充新图像数据区
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );
	//用原图像数据填充新图像数据区
	//memcpy( pNewBits, pOldBits, nWidthBytes * nHeight );

	//查找最左下方的边界点
	bFindStartPoint = false;

	for (y = 0; y < nHeight && !bFindStartPoint; y++)
	{
		pOldTemp = pOldBits;
		pOldTemp +=  y * nWidthBytes;
		pNewTemp = pNewBits;
		pNewTemp +=  y * nWidthBytes;
		for(x = 0; x < nWidth && !bFindStartPoint; x++)
		{
			//找到最左上方的边界点
			if(pOldTemp[x] == 0)
			{
				bFindStartPoint = true;
				StartPoint.y = y;
				StartPoint.x = x;
				//新图像数据区相应位置赋0（黑色）			
				pNewTemp[x] = (unsigned char)0;
			}		
		}
	}

	//由于起始点是在左下方，故起始扫描沿左上方向
	BeginDirect = 0;

	//是否跟踪到起始点
	bFindStartPoint = false;

	//从初始点开始扫描
	CurrentPoint = StartPoint;

	//边界跟踪
	while(!bFindStartPoint)
	{
		bFindPoint = false;
		while(!bFindPoint)
		{
			//沿扫描方向查看一个像素
			pOldTemp = pOldBits + nWidthBytes * 
				( CurrentPoint.y + Direction[BeginDirect][1])
				+ (CurrentPoint.x + Direction[BeginDirect][0]);
			if(*pOldTemp == 0)
			{
				bFindPoint = true;
				CurrentPoint.y = CurrentPoint.y + 
								Direction[BeginDirect][1];
				CurrentPoint.x = CurrentPoint.x + 
								Direction[BeginDirect][0];
				if(CurrentPoint == StartPoint)
				{
					bFindStartPoint = true;
				}
				pNewTemp = pNewBits + nWidthBytes * 
									CurrentPoint.y + 
									CurrentPoint.x;
				*pNewTemp = (unsigned char)0;

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

	//用新图像数据填充原图像数据区
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL ContourPickUp()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject图像对象进行轮廓提取。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	图像对象指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::ContourPickUp(CDibObject *pDibObject)
{
   //图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//只支持8位图像
	if(m_pDibObject->GetNumBits() != 8)
	{
		// 提示用户参数设置错误
		AfxMessageBox("只支持8位图像,请重新载入！");
		
		// 返回
		return( FALSE );
	}
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//定义变量
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//八个方向
	unsigned char n,e,s,w,ne,se,nw,sw;

	//临时存放像素值
	unsigned char pixel;

	//原图像指针
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//为新图像数据区分配内存
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//新图像数据区大小
	dwNewSize = nWidthBytes * nHeight;
	//为新图像分配内存
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像指针
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//用255（白色）填充新图像数据区
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );

	for(y = 1; y < nHeight - 1; y++)
	{
		//指向源图像倒数第y行
		pOldTemp = pOldBits;
		pOldTemp +=  y * nWidthBytes;
		//指向目标图像倒数第y行			
		pNewTemp = pNewBits;
		pNewTemp +=  y * nWidthBytes;
		for(x = 1; x < nWidth - 1; x++)
		{
			//取得当前指针处的像素值
			pixel = *pOldTemp;

			if(pixel == 0)
			{
				*pNewTemp = 0;
				nw = *(pOldTemp + nWidthBytes - 1);
				n  = *(pOldTemp + nWidthBytes);
				ne = *(pOldTemp + nWidthBytes + 1);
				w = *(pOldTemp - 1);
				e = *(pOldTemp + 1);
				sw = *(pOldTemp - nWidthBytes - 1);
				s  = *(pOldTemp - nWidthBytes );
				se = *(pOldTemp - nWidthBytes + 1);
				//如果相邻的八个点都是黑点
				if(nw + n + ne + w + e + sw + s + se == 0)
				{
					*pNewTemp = 255;
				}
			}
			pOldTemp++;			
			pNewTemp++;
		}
	}

	//用新图像数据填充原图像数据区
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL Feeding1()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject图像对象进行种子填充。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	图像对象指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::Feeding1(CDibObject *pDibObject)
{
   //图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//只支持8位图像
	if(m_pDibObject->GetNumBits() != 8)
	{
		// 提示用户参数设置错误
		AfxMessageBox("只支持8位图像,请重新载入！");
		
		// 返回
		return( FALSE );
	}
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//定义变量
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits;
	unsigned char *pOldTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors;

	//种子堆栈及指针
	Seed *Seeds;
	int StackPoint;

	//当前像素位置
	int iCurrentPixelx,iCurrentPixely;

	//临时存放像素值
	unsigned char pixel;

	//原图像指针
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//初始化种子
	Seeds = new Seed[nWidthBytes * nHeight];
	Seeds[1].Height = nHeight / 2;
	Seeds[1].Width = nWidthBytes / 2;
	StackPoint = 1;

	while( StackPoint != 0)
	{
		//取出种子
		iCurrentPixelx = Seeds[StackPoint].Width;
		iCurrentPixely = Seeds[StackPoint].Height;
		StackPoint--;

		pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + iCurrentPixelx;
		//取得当前指针处的像素值
		pixel = *pOldTemp;

		//将当前点涂黑
		*pOldTemp = 0;

		//判断左面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixelx > 0)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + iCurrentPixelx - 1;
			//取得当前指针处的像素值
			pixel = *pOldTemp;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = iCurrentPixelx - 1;
			}
		}

		//判断上面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixely < nHeight - 1)
		{
			pOldTemp = pOldBits + nWidthBytes * (iCurrentPixely + 1) + iCurrentPixelx;
			//取得当前指针处的像素值
			pixel = *pOldTemp;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely + 1;
				Seeds[StackPoint].Width = iCurrentPixelx;
			}
		}

		//判断右面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixelx < nWidth - 1)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + iCurrentPixelx + 1;
			//取得当前指针处的像素值
			pixel = *pOldTemp;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = iCurrentPixelx + 1;
			}
		}

		//判断下面的点，如果为白，则压入堆栈
		//注意防止越界
		if(iCurrentPixely > 0)
		{
			pOldTemp = pOldBits + nWidthBytes * (iCurrentPixely - 1) + iCurrentPixelx;
			//取得当前指针处的像素值
			pixel = *pOldTemp;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely - 1;
				Seeds[StackPoint].Width = iCurrentPixelx;
			}
		}
	}

	//释放堆栈
	delete Seeds;

	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL Feeding2()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject图像对象进行种子填充。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	图像对象指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::Feeding2(CDibObject *pDibObject)
{
   //图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//只支持8位图像
	if(m_pDibObject->GetNumBits() != 8)
	{
		// 提示用户参数设置错误
		AfxMessageBox("只支持8位图像,请重新载入！");
		
		// 返回
		return( FALSE );
	}
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//定义变量
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits;
	unsigned char *pOldTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors;

	//左右边界像素位置
	int xl, xr, i;

	//是否已填充至边界
	BOOL bFilll, bFillr;

	//种子堆栈及指针
	Seed Seeds[10];
	int StackPoint;

	//当前像素位置
	int iCurrentPixelx, iCurrentPixely;
	int iBufferPixelx, iBufferPixely;

	//临时存放像素值
	unsigned char pixel;

	//原图像指针
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//初始化种子
	Seeds[1].Height = nHeight / 2;
	Seeds[1].Width = nWidthBytes / 2;
	StackPoint = 1;

	while( StackPoint != 0)
	{
		//取出种子
		iCurrentPixelx = Seeds[StackPoint].Width;
		iCurrentPixely = Seeds[StackPoint].Height;
		StackPoint--;

		bFilll = true;
		bFillr = true;

		//填充种子所在的行
		//保存种子像素的位置
		iBufferPixelx = iCurrentPixelx;
		iBufferPixely = iCurrentPixely;
		//先向左填充
		while(bFilll)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + iCurrentPixelx;
			//取得当前指针处的像素值
			pixel = *pOldTemp;

			//遇到边界
			if(pixel == 0)
			{
				bFilll = false;
				xl=iCurrentPixelx+1;
			}
			else
			{
				*pOldTemp = 0;
				iCurrentPixelx--;
				//防止越界
				if(iCurrentPixelx < 0)
				{	
					bFilll = false;
					iCurrentPixelx = 0;
					xl = 0; 
				}
			}		
		}
		//再向右填充
		//取回种子像素的位置
		iCurrentPixelx = iBufferPixelx+1;
		if(iCurrentPixelx > nWidth)
		{	
			bFillr = false;
			iCurrentPixelx = nWidth;
			xr = nWidth; 
		}
		iCurrentPixely = iBufferPixely;
		while(bFillr)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + iCurrentPixelx;
			//取得当前指针处的像素值
			pixel = *pOldTemp;

			//遇到边界
			if(pixel == 0)
			{
				bFillr = false;
				xr=iCurrentPixelx-1;
			}
			else
			{
				*pOldTemp = 0;
				iCurrentPixelx++;
				//防止越界
				if(iCurrentPixelx > nWidth)
				{	
					bFillr = false;
					iCurrentPixelx = nWidth;
					xr = nWidth; 
				}
			}		
		}
		//上、下两条扫描线是否全为边界象素或已填充过
		//先看上面的扫描线
		iCurrentPixely--;
		if(iCurrentPixely < 0)
		{
			iCurrentPixely = 0;
		}
		for (i = xr; i >= xl;i--)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + i;
			//取得当前指针处的像素值
			pixel = *pOldTemp;

			//有未填充的像素，将新的种子压入堆栈
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = i;
				break;
			}				
		}
		//再看下面的扫描线
		iCurrentPixely += 2;
		if(iCurrentPixely > nHeight)
		{
			iCurrentPixely = nHeight;
		}
		for (i = xr; i >= xl; i--)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + i;
			//取得当前指针处的像素值
			pixel = *pOldTemp;

			//有未填充的像素，将新的种子压入堆栈
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = i;
				break;
			}				
		}


	}

	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//void AddBuffer()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的两幅图像数据进行加操作
//----------------------------------------------------------------------
//参数说明：unsigned char *pOut		指向输出图像数据的指针
//			unsigned char *pIn		指向输入图像数据的指针
//			LONG lSize				图像数据的大小（nWidthBytes*nHeight）
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CAreaPro类中使用。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CAreaPro::AddBuffer(unsigned char *pOut, unsigned char *pIn,
						   LONG lSize)
{
	LONG i;
	BYTE Buffer;
	for(i = 0; i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//执行与操作
		*(pOut + i) = *(pOut + i) + Buffer > 255 ? 255 : *(pOut + i) + Buffer;
	}
}

////////////////////////////////////////////////////////////////////////
//void AddBuffer()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的两幅图像数据进行加操作
//----------------------------------------------------------------------
//参数说明：unsigned char *pOut		指向输出图像数据的指针
//			unsigned char *pIn		指向输入图像数据的指针
//			LONG lSize				图像数据的大小（nWidthBytes*nHeight）
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CAreaPro类中使用。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CAreaPro::MinusBuffer(unsigned char *pOut, unsigned char *pIn,
						   LONG lSize)
{
	LONG i;
	BYTE Buffer;
	for(i = 0; i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//执行与操作
		*(pOut + i) = *(pOut + i) - Buffer < 0 ? 0 : *(pOut + i) - Buffer;
	}
}

////////////////////////////////////////////////////////////////////////
//BOOL ProjectH()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject图像对象进行水平投影。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	图像对象指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::ProjectH(CDibObject *pDibObject)
{
   //图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//只支持8位图像
	if(m_pDibObject->GetNumBits() != 8)
	{
		// 提示用户参数设置错误
		AfxMessageBox("只支持8位图像,请重新载入！");
		
		// 返回
		return( FALSE );
	}
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//定义变量
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//图像中每行内的黑点个数
	long lBlackNumbers;

	//临时存放像素值
	unsigned char pixel;

	//原图像指针
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//为新图像数据区分配内存
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//新图像数据区大小
	dwNewSize = nWidthBytes * nHeight;
	//为新图像分配内存
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像指针
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//用255（白色）填充新图像数据区
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );

	for (y = 0;y < nHeight ;y++)
	{
		lBlackNumbers = 0;
		for(x = 0;x < nWidth ;x++)
		{
			//指向源图像倒数第y行，第x个象素的指针			
			pOldTemp = pOldBits + nWidthBytes * y + x;
	
			pixel = *pOldTemp;

			if (pixel != 255 && pixel != 0)
			{
				return false;
			}
			if(pixel == 0)
			{
				lBlackNumbers++;
			}
		}
		for(x = 0;x < lBlackNumbers ;x++)
		{	
			//指向目标图像倒数第y行，第x个象素的指针			
			pNewTemp = pNewBits + nWidthBytes * y + x;

			*pNewTemp = 0;
		}		
	}

	//用新图像数据填充原图像数据区
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL ProjectV()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject图像对象进行垂直投影。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	图像对象指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::ProjectV(CDibObject *pDibObject)
{
   //图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//只支持8位图像
	if(m_pDibObject->GetNumBits() != 8)
	{
		// 提示用户参数设置错误
		AfxMessageBox("只支持8位图像,请重新载入！");
		
		// 返回
		return( FALSE );
	}
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//定义变量
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//图像中每行内的黑点个数
	long lBlackNumbers;

	//临时存放像素值
	unsigned char pixel;

	//原图像指针
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//为新图像数据区分配内存
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//新图像数据区大小
	dwNewSize = nWidthBytes * nHeight;
	//为新图像分配内存
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像指针
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//用255（白色）填充新图像数据区
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );

	for (x = 0; x < nWidth ;x++)
	{
		lBlackNumbers = 0;
		for(y = 0; y < nHeight ;y++)
		{
			//指向源图像倒数第y行，第x个象素的指针			
			pOldTemp = pOldBits + nWidthBytes * y + x;
	
			pixel = *pOldTemp;

			if(pixel == 0)
			{
				lBlackNumbers++;
			}
		}
		for(y = 0; y < lBlackNumbers; y++)
		{	
			// 指向目标图像倒数第y行，第x个象素的指针			
			pNewTemp = pNewBits + nWidthBytes * y + x;

			*pNewTemp = 0;
		}		
	}

	//用新图像数据填充原图像数据区
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}


////////////////////////////////////////////////////////////////////////
//unsigned char *GetBitsPointer()   
//----------------------------------------------------------------------
//基本功能：本函数获取传入的CDibObject图像对象数据区的指针。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	图像对象指针
//----------------------------------------------------------------------
//返    回：unsigned char*
//			成功时返回图像数据区的指针，失败时返回NULL。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char *CAreaPro::GetBitsPointer(CDibObject *pDibObject)
{
   //图像指针为空，无法操作返回
	if(pDibObject == NULL) return(GetBitsPointer());
	
	//只支持8位图像
	if(pDibObject->GetNumBits() != 8)
	{
		// 提示用户参数设置错误
		AfxMessageBox("只支持8位图像,请重新载入！");
		
		// 返回
		return( NULL );
	}
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = pDibObject->GetWidth();
	int nHeight = pDibObject->GetHeight();
	
	//定义变量
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors;

	//原图像指针
	pOldBuffer = (unsigned char *) pDibObject->GetDIBPointer(&nWidthBytes, 
																pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( NULL );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	::GlobalUnlock( pDibObject->GetDib() );

	return(pOldBits);
}

////////////////////////////////////////////////////////////////////////
//unsigned char *GetBitsPointer()   
//----------------------------------------------------------------------
//基本功能：本函数获取传入的CDibObject图像对象数据区的指针。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：unsigned char*
//			成功时返回图像数据区的指针，失败时返回NULL。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char *CAreaPro::GetBitsPointer()
{
   //图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(NULL);
	
	//只支持8位图像
	if(m_pDibObject->GetNumBits() != 8)
	{
		// 提示用户参数设置错误
		AfxMessageBox("只支持8位图像,请重新载入！");
		
		// 返回
		return( NULL );
	}
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//定义变量
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors;

	//原图像指针
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( NULL );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	::GlobalUnlock( m_pDibObject->GetDib() );

	return(pOldBits);
}

////////////////////////////////////////////////////////////////////////
//BOOL Minusing()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject图像对象进行差影运算。
//----------------------------------------------------------------------
//参数说明：unsigned char *pTemplate	模板图像数据指针
//			CDibObject *pDibObject		图像对象指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::Minusing(unsigned char *pTemplate, CDibObject *pDibObject)
{
   //图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//获取图像高度（以像素为单位）
	int nHeight = m_pDibObject->GetHeight();
	
	//定义变量
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits;
	int nWidthBytes;

	//原图像指针
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );

	pOldBits = GetBitsPointer();

	MinusBuffer(pOldBits, pTemplate, nWidthBytes * nHeight);

	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL TemplateMatch()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject图像对象进行图像匹配运算。
//----------------------------------------------------------------------
//参数说明：CDibObject *pTemplate	模板图像数据指针
//			CDibObject *pDibObject		图像对象指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::TemplateMatch(CDibObject *pTemplate, CDibObject *pDibObject)
{
   //图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	//每像素位数
	int nNumBits = m_pDibObject->GetNumBits();

	//获取图像宽度和高度（以像素为单位）
	int nTempWidth = pTemplate->GetWidth();
	int nTempHeight = pTemplate->GetHeight();
	//每像素位数
	int nTempNumBits = pTemplate->GetNumBits();
	
	//定义变量
	unsigned char *pOldBits, *pTempBits, *pNewBits;
	int nWidthBytes, nTempWidthBytes;

	//获得图像的字节宽度
	nWidthBytes = m_pDibObject->WidthBytes(nNumBits, nWidth);
	//获得模板图像的字节宽度
	nTempWidthBytes = pTemplate->WidthBytes(nTempNumBits, nTempWidth);

	//获得图像的数据区指针
	pOldBits = GetBitsPointer();
	//获得模板图像的数据区指针
	pTempBits = GetBitsPointer(pTemplate);

	//创建一个新图像它是源图像的复制结果
	CDibObject *pDibNew = new CDibObject(m_pDibObject);
	//获得新图像的数据区指针
	pNewBits = GetBitsPointer(pDibNew);

	//用255（白色）填充新图像数据区
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );

	//循环变量
	int i, j, m, n;

	//中间结果
	double dSigmaST;
	double dSigmaS;
	double dSigmaT;

	//相似性测度
	double R;

	//最大相似性测度
	double MaxR;

	//最大相似性出现位置
	int nMaxWidth, nMaxHeight;

	//像素值
	unsigned char pixel, templatepixel;

	unsigned char *pOldTemp, *pTemplateTemp, *pNewTemp;

	//计算dSigmaT
	dSigmaT = 0;

	for (n = 0; n < nTempHeight; n++)
	{
		for(m = 0; m < nTempWidth; m++)
		{
			// 指向模板图像倒数第j行，第i个象素的指针			
			pTemplateTemp = pTempBits + nTempWidthBytes * n + m;
			templatepixel = *pTemplateTemp;
			dSigmaT += (double)templatepixel * templatepixel;
		}
	}

	//找到图像中最大相似性的出现位置
	MaxR = 0.0;
	for (j = 0; j < nHeight - nTempHeight + 1; j++)
	{
		for(i = 0; i < nWidth - nTempWidth + 1; i++)
		{
			dSigmaST = 0;
			dSigmaS = 0;
	
			for (n = 0; n < nTempHeight; n++)
			{
				for(m = 0; m < nTempWidth; m++)
				{
					//指向源图像倒数第j+n行，第i+m个象素的指针			
					pOldTemp  = pOldBits + nWidthBytes * (j + n) + (i + m);
			
					//指向模板图像倒数第n行，第m个象素的指针			
					pTemplateTemp  = pTempBits + nTempWidthBytes * n + m;

					pixel = *pOldTemp;
					templatepixel = *pTemplateTemp;

					dSigmaS += (double)pixel * pixel;
					dSigmaST += (double)pixel * templatepixel;
				}
			}
			//计算相似性
			R = dSigmaST / ( sqrt(dSigmaS) * sqrt(dSigmaT));
			//与最大相似性比较
			if (R > MaxR)
			{
				MaxR = R;
				nMaxWidth = i;
				nMaxHeight = j;
			}
		}
	}

	//将最大相似性出现区域部分复制到目标图像
	for (n = 0; n < nTempHeight; n++)
	{
		for(m = 0; m < nTempWidth; m++)
		{
			pTemplateTemp = pTempBits + nTempWidthBytes * n + m;
			pNewTemp = pNewBits + nWidthBytes * (n+nMaxHeight) + (m+nMaxWidth);
			*pNewTemp = *pTemplateTemp;
		}
	}

	//用新图像数据填充原图像数据区
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	delete pDibNew;

	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}
