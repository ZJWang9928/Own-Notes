// Identify.cpp: implementation of the CIdentify class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Insects.h"
#include "Identify.h"
#include "GeoTrans.h"     
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIdentify::CIdentify()
{
	//初始化变量
	m_pIndentify=NULL;
	m_antennaPara=0.0;
	m_bellyPara=0.0;
	m_swingPara=0;
	m_antennaFactor=0.0;
	m_bellyFactor=0.0;
}

CIdentify::CIdentify(CDibObject *pDib)
{
	//初始化变量
	m_pIndentify=(CIdentify *)pDib;
	m_antennaPara=0.0;
	m_bellyPara=0.0;
	m_swingPara=0;
	m_antennaFactor=0.0;
	m_bellyFactor=0.0;
}

CIdentify::~CIdentify()
{
	CDibObject::~CDibObject();
}

BOOL CIdentify::AntennaSep()
{
	/*======图像指针为空，无法操作返回=======*/
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("尚未有图象载入或载入错误!");
		return FALSE;
	}
	
	/*==获取图像宽度和高度（以像素为单位）===*/
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();

	/*==定义与图像数据操作有关的变量===*/
	unsigned char *pOldBuffer, *pNewBuffer, *pOldBits, *pNewBits,*pOldTemp, *pNewTemp;

	/*==BMP文件结构变量定义===*/
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;

	int nWidthBytes, nNumColors, i,pixNum,blank_pix,flag,nums=0;
	
	/*======获取原图像指针=======*/
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//原图像文件头
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pIndentify->GetNumColors();
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
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return( FALSE );
	}
	
	//新图像指针
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
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

	/************************************************/
	/*================触角分割部分==================*/
	/************************************************/

	/*===首先设置所有图象数据的象素值为255，即为白色==*/
	memset( pNewBits, 255, nWidthBytes * nHeight);
    pOldTemp=pOldBits;
	pNewTemp=pNewBits;
	
	int top_dos=0,bottom_dos=0;
	float max_width=0.0;

	for(i=nHeight;i>nHeight/2;i--)
	{
		pOldBits=pOldTemp;
		//原图像数据指针定位到图像数据每行的中心位置
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes+nWidthBytes/2-1;
		pNewBits=pNewTemp;
		//新图像数据指针定位到图像数据每行的中心位置
		pNewBits =pNewBits + (i - 1 ) * nWidthBytes+nWidthBytes/2-1;

		//复制数据
		
		pixNum=0,blank_pix=0,flag=0;
		if(*(pOldBits+1)==255) 
		{
			for(int j=1;j<=nWidthBytes/3;j++)
			{	
				if(*pOldBits<255) 
				{
					pixNum++;
					pNewBits--;
					pOldBits--;
					while(*pOldBits==255 ) 
					{
						blank_pix++;
						pNewBits--;
						pOldBits--;
						if(blank_pix>=2) 
						{
							flag=1;
							break;
						}
					}
					if(flag==1) break;
				}
				else
				{
					pNewBits--;
					pOldBits--;
				}
			}
		}
		if(pixNum<nWidthBytes/20 && flag==1)
		{
			nums++;
			memcpy(pNewBits+1, pOldBits+1,pixNum+blank_pix);
			//计算触角的最大宽度
			if((float)pixNum>max_width) max_width=(float)pixNum;
			if(nums==1) top_dos=i;
			else
				bottom_dos=i;
		}
	}

	/***************以下是获取触角的相关信息****************/
	float total_pixs=0,part_pixs=0;

	for(i=top_dos;i>(bottom_dos+top_dos)/2+1;i--)
	{	
		pNewBits=pNewTemp;
		pNewBits =pNewBits + (i-1) * nWidthBytes;
		for(int j=0;j<nWidthBytes;j++)
		{
			if(*pNewBits!=255)
				part_pixs++;
			pNewBits++;
		}
	}
	for(i=top_dos;i>bottom_dos+1;i--)
	{	
		pNewBits=pNewTemp;
		pNewBits =pNewBits + (i-1) * nWidthBytes;
		for(int j=0;j<nWidthBytes;j++)
		{
			if(*pNewBits!=255)
				total_pixs++;
			pNewBits++;
		}
	}
	m_antennaPara=part_pixs/total_pixs;
	m_antennaFactor=max_width/nWidth;

	::GlobalUnlock( m_pIndentify->GetDib() );
    ::GlobalFree( m_pIndentify->GetDib() );
	::GlobalUnlock( hNewDib );
    m_pIndentify->SetDib( hNewDib );

    return(TRUE);

}
BOOL CIdentify::BellySep()
{
	//图像指针为空，无法操作返回
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("尚未有图象载入或载入错误!");
		return FALSE;
	}
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();
	//定义与图像数据操作有关的变量
	unsigned char *pOldBuffer, *pNewBuffer, 
				  *pOldBits, *pNewBits,
				  *pOldTemp, *pNewTemp;


	//BMP文件结构变量定义
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;

	int nWidthBytes, nNumColors, i,flag=0;
	int max=0,maxLength=0,index=0;
	//获取原图像指针
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//原图像文件头
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pIndentify->GetNumColors();
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
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return( FALSE );
	}
	
	//新图像指针
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
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

	//首先设置所有图象数据为255。即为白色
	memset( pNewBits, 255, nWidthBytes * nHeight);
    pOldTemp=pOldBits;
	pNewTemp=pNewBits;


	/************************************************/
	/*================ 分割腹部部分==================*/
	/************************************************/

	int right_nums,left_nums,top_dos=0,bottom_dos=0,n1,n2,n_left,n_right,flg1,flg2;

	for(i=0;i<=nHeight;i++)
	{
		pOldBits=pOldTemp;
		//原图像数据指针定位到图像数据每行的中心位置
		pOldBits =pOldBits + i * nWidthBytes+ nWidthBytes/2-1;

		pNewBits=pNewTemp;
		//新图像数据指针定位到图像数据每行的中心位置
		pNewBits =pNewBits + i * nWidthBytes+ nWidthBytes/2-1;
		//初始化变量
		right_nums=0,left_nums=0,n1=0,n2=0,n_left=0,n_right=0,flg1=0,flg2=0;
		//如果当前像素为白色，扫描第下一行
		if(*pOldBits==255)
		{
			while(*pOldBits==255)
			{
				pOldBits++;
				pNewBits++;
				n1++;
				while(*pOldBits<255 && n1<nWidthBytes/20)
				{
					memcpy(pNewBits,pOldBits,1);
					pOldBits++;
					pNewBits++;
					if(*pOldBits==255)
					{	
						flg1=1;
						break;
					}
					if(flg1==1) break;
				}
				
			}
			pOldBits=pOldTemp;
			//原图像数据指针定位到图像数据每行的中心位置
			pOldBits =pOldBits + i * nWidthBytes+ nWidthBytes/2-1;

			pNewBits=pNewTemp;
			//新图像数据指针定位到图像数据每行的中心位置
			pNewBits =pNewBits + i * nWidthBytes+ nWidthBytes/2-1;

			while(*pOldBits==255)
			{
				pOldBits--;
				pNewBits--;
				n2++;
				while(*pOldBits<255 && n2<nWidthBytes/20)
				{
					memcpy(pNewBits,pOldBits,1);
					pOldBits--;
					pNewBits--;
					if(*pOldBits==255)
					{	
						flg2=1;
						break;
					}
					if(flg2==1) break;
				}	
			}

		}
		else
		{
			//对右边进行处理
			while(*pOldBits<255 && right_nums<nWidthBytes/2)
			{
				right_nums++;
				pNewBits++;
				pOldBits++;

				//设置相关边界
				if(right_nums>nWidthBytes/4) 
				{
					flag=1;
					break;
				}
				//碰到像素为白色地方，复制内容退出
				if(*pOldBits==255)
				{
					memcpy(pNewBits-right_nums-1,pOldBits-right_nums-1,right_nums+1);
					break;
				}
			}

			pOldBits=pOldTemp;
			pOldBits=pOldBits + i * nWidthBytes + nWidthBytes/2-1;

			pNewBits=pNewTemp;
			pNewBits=pNewBits + i * nWidthBytes + nWidthBytes/2-1;

			//对左边进行处理
			while(*pOldBits<255 && left_nums<nWidthBytes/2)
			{
				left_nums++;
				pNewBits--;
				pOldBits--;

				if(left_nums>nWidthBytes/4) 
				{
					flag=1;
					break;
				}
				if(*pOldBits==255)
				{
					memcpy(pNewBits,pOldBits,left_nums-1);
					break;
				}
			}
			if(flag==1) break;
		}
	}
	
	/***************以下是获取腹部的相关信息****************/
	float nums=0,width_pixs=0,height_pixs=0,total_pixs=0;

	for(i=1;i<=nHeight;i++)
	{	
		pNewBits=pNewTemp;
		pNewBits =pNewBits + (i-1) * nWidthBytes;
		for(int j=0;j<nWidthBytes;j++)
		{
			if(*pNewBits!=255)
			{
				nums++;
				total_pixs++;
				if(nums==1) 
				{
					bottom_dos=i;
				}
				else
				{
					top_dos=i;		
				}
			}
			pNewBits++;
		}
	}
	
	height_pixs=(float)(top_dos-bottom_dos+1);
	width_pixs=total_pixs/height_pixs;


	m_bellyFactor=m_bellyPara=width_pixs/nWidthBytes;

	::GlobalUnlock( m_pIndentify->GetDib() );
    ::GlobalFree( m_pIndentify->GetDib() );
	::GlobalUnlock( hNewDib );
    m_pIndentify->SetDib( hNewDib );
    return(TRUE);
}


BOOL CIdentify::SwingSep()
{
	//图像指针为空，无法操作返回
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("尚未有图象载入或载入错误!");
		return FALSE;
	}

	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();
	//定义与图像数据操作有关的变量
	unsigned char *pOldBuffer, *pNewBuffer, 
				  *pOldBits, *pNewBits,
				  *pOldTemp, *pNewTemp;


	//BMP文件结构变量定义
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;

	int nWidthBytes, nNumColors, i;
	int max=0,maxLength=0,index=0;
	//获取原图像指针
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//原图像文件头
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pIndentify->GetNumColors();
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
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return( FALSE );
	}
	
	//新图像指针
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
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

	//首先设置所有图象数据为255。即为白色
	memset( pNewBits, 255, nWidthBytes * nHeight);
    pOldTemp=pOldBits;
	pNewTemp=pNewBits;


	int nums,flag=0,depart_point,tempNums,n=0,flg=0,nn=0;
	for(i=0;i<nHeight;i++)
	{
		pOldBits=pOldTemp;
		//原图像数据指针定位到图像数据每行的中心位置
		pOldBits =pOldBits + i * nWidthBytes+ nWidthBytes/2-1;

		nums=0;
		if(*pOldBits!=255)
		{
			while(*pOldBits<255 && nums<nWidthBytes/2)
			{

				nums++;
				pOldBits--;
				if(nums>nWidthBytes/4) 
				{
					flag=1;
					depart_point=nWidthBytes/2-tempNums;
					break;

				}
				if(*pOldBits==255)
				{
					tempNums=nums;
					break;
				}
			}  
		}
		
		if(flag==1) break;
	}

	for(i=nHeight;i>0;i--)
	{
		pOldBits=pOldTemp;
		//原图像数据指针定位到图像数据每行的开始位置
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes;
		pNewBits=pNewTemp;
		//新图像数据指针定位到图像数据每行的开始位置
		pNewBits =pNewBits + (i - 1 ) * nWidthBytes;
		//复制数据
		max=0;
		for(int j=1;j<depart_point;j++)
		{
			if(*pOldBits<255) 
			{
				memcpy( pNewBits, pOldBits, 1);
				pNewBits++;
				pOldBits++;
				if(*pOldBits==255) break;
			}
			else
			{
				max++;
				pNewBits++;
				pOldBits++;
				if(max>3*depart_point/4) break;
			}

		}
	}

	for(i=depart_point/2;i>0;i--)
	{
		pNewBits=pNewTemp;
		//新图像数据指针定位到图像数据每一列的开始位置
		pNewBits =pNewBits + (nHeight-1)* nWidthBytes+i-1;
		//复制数据
		int j=0;
		while(j<nHeight)
		{
			if(*pNewBits<255) 
			{
				j++;
				pNewBits=pNewBits-nWidthBytes;
				nn=0;
				while(*pNewBits==255 && j<nHeight-1)
				{
					j++;
					nn++;
					pNewBits=pNewBits-nWidthBytes;
					if(*pNewBits!=255 && nn<=5)
					{
						flg=1;
						index=nHeight-j+nn/2+1;
						break; 
					}
				}
			}
			else
			{
				j++;
				pNewBits=pNewBits-nWidthBytes;
			}
			if(flg==1) break;
		}
	}
	if(flg==1)
	{	
		pNewBits=pNewTemp;
		pNewBits=pNewBits+(index-1)*nWidthBytes;
	    memset( pNewBits, 255, nWidthBytes);
		m_swingPara=2;
	}
	else m_swingPara=1;

	::GlobalUnlock( m_pIndentify->GetDib() );
    ::GlobalFree( m_pIndentify->GetDib() );
	::GlobalUnlock( hNewDib );
    m_pIndentify->SetDib( hNewDib );
    return(TRUE);
}
//获取触角的相关参数
float CIdentify::GetAntennaPara()
{
	return m_antennaPara;
}
//获取腹部的相关参数
float CIdentify::GetBellyPara()
{
	return m_bellyPara;
}
//获取翅膀的相关参数
int CIdentify::GetSwingPara()
{
	return m_swingPara;
}

//获取触角的比例因子
float CIdentify::GetAntennaFactor()
{
	return m_antennaFactor;
}
//获取腹部的比例因子
float CIdentify::GetBellyFactor()
{
	return m_bellyFactor;
}
/***********获取偏心率***********/
float CIdentify::GetEccentricity()
{
	/*======图像指针为空，无法操作返回=======*/
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("尚未有图象载入或载入错误!");
		return FALSE;
	}
	
	/*==获取图像宽度和高度（以像素为单位）===*/
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();

	/*==定义与图像数据操作有关的变量===*/
	unsigned char *pOldBuffer, *pOldBits,*pOldTemp;


	int nWidthBytes, nNumColors, i,top=0,bottom=0;
	
	/*======获取原图像指针=======*/
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//原图像颜色数
	nNumColors = m_pIndentify->GetNumColors();
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	pOldTemp=pOldBits;
	for(i=nHeight;i>0;i--)
	{
		pOldBits=pOldTemp;
		//原图像数据指针定位到图像数据每行的中心位置
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes+nWidthBytes/2-1;
		if(*pOldBits!=255) {top=i;break;}
	}
	for(i=1;i<nHeight;i++)
	{
		pOldBits=pOldTemp;
		//原图像数据指针定位到图像数据每行的中心位置
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes+nWidthBytes/2-1;
		if(*pOldBits!=255) {bottom=i;break;}
	}

	return (float)nWidthBytes/(float)(top-bottom);

}

float CIdentify::GetArea()
{
	/*======图像指针为空，无法操作返回=======*/
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("尚未有图象载入或载入错误!");
		return FALSE;
	}
	
	/*==获取图像宽度和高度（以像素为单位）===*/
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();

	/*==定义与图像数据操作有关的变量===*/
	unsigned char *pOldBuffer, *pOldBits,*pOldTemp;


	int nWidthBytes, nNumColors, i,j;
	float area=0.0;
	
	/*======获取原图像指针=======*/
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//原图像颜色数
	nNumColors = m_pIndentify->GetNumColors();
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	pOldTemp=pOldBits;
	for(i=nHeight;i>0;i--)
	{
		pOldBits=pOldTemp;
		//原图像数据指针定位到图像数据每行的开始位置
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes;
		for(j=0;j<nWidthBytes;j++)
		{
			if(*pOldBits!=255) area=area+1;
			pOldBits++;
		}
		
	}
	return area;
}

/********基本功能：本函数对首先对传入的图像进行轮廓提取****/
////////////////////////////////////////////////////////////////////////
BOOL CIdentify::ContourTrace()
{

   //图像指针为空，无法操作返回
	if(m_pIndentify == NULL) return(FALSE);
	
	//只支持8位图像
	if(m_pIndentify->GetNumBits() != 8)
	{
		// 提示用户参数设置错误
		AfxMessageBox("只支持8位图像,请重新载入！");
		
		// 返回
		return( FALSE );
	}
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();
	
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
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer(&nWidthBytes, 
																m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pIndentify->GetNumColors();
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
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return( FALSE );
	}

	//新图像指针
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
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
	::GlobalUnlock( m_pIndentify->GetDib() );
	
	return (TRUE);
}

////////////////////////////////////////////////////////////////////////
//功能：对灰度图像进行轮廓跟踪并生成链码,结果存储在TraceArray中
////////////////////////////////////////////////////////////////////////
BOOL CIdentify::EdgeTrace()
{
	if(m_pIndentify==NULL) return FALSE;
	if(m_pIndentify->GetNumBits()!=8){AfxMessageBox("不是8位灰度图像");return FALSE;}
	
	unsigned char *pOld,*pBits,*pTemp,*pNew,*pNewBits,*pNewTemp;
	int nWidthBytes, nNumColors;
	DWORD offset,BufSize;
	pOld = (BYTE *) m_pIndentify->GetDIBPointer(&nWidthBytes);
	if(pOld == NULL) return FALSE;
	
	nNumColors = m_pIndentify->GetNumColors();
	offset=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD);
	pBits =(unsigned char *)&pOld[offset];
	int nHeight=m_pIndentify->GetHeight();
	int nWidth=m_pIndentify->GetWidth();
	BufSize=nWidthBytes*nHeight ;
	HGLOBAL hNewDib;
	hNewDib = ::GlobalAlloc(GHND,offset+BufSize );
	if( hNewDib == NULL ){
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return FALSE;}
	pNew = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNew == NULL )
	{
		::GlobalFree( hNewDib );
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib());
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
				//方向为到下一点的矢量方向
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
	::GlobalUnlock(m_pIndentify->GetDib());
	::GlobalFree(m_pIndentify->GetDib());
	::GlobalUnlock(hNewDib );
	
	//将新图像设置为当前图像
	m_pIndentify->SetDib( hNewDib );
	return true;
}


////////////////////////////////////////////////////////////
//功能：由边界链码计算封闭区域的周长
////////////////////////////////////////////////////////////
double CIdentify::GetPeriByChainCode()
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
	return  int(nHorNum+nVerNum+nDiaNum*1.414);
	
}

POINT CIdentify::GetCentroid()
{
	/*======图像指针为空，无法操作返回=======*/
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("尚未有图象载入或载入错误!");
	}
	
	/*==获取图像宽度和高度（以像素为单位）===*/
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();

	/*==定义与图像数据操作有关的变量===*/
	unsigned char *pOldBuffer,  *pOldBits,*pOldTemp;

	int nWidthBytes, nNumColors,i,j;
	
	/*======获取原图像指针=======*/
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	//原图像颜色数
	nNumColors = m_pIndentify->GetNumColors();
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	POINT point;
	long total_x=0,total_y=0,total_pix=0;

	pOldTemp=pOldBits;
	for(i=0;i<nHeight;i++)
	{
		pOldBits=pOldTemp;
		pOldBits=pOldBits+nWidthBytes*i;
		for(j=0;j<nWidthBytes;j++)
		{
			if(*pOldBits==0)
			{
				total_x+=i;
				total_y+=j;
				total_pix++;
			}
			pOldBits++;
		}
	}
    //获取质心
	point.x=total_x/total_pix;
	point.y=total_y/total_pix;

	return point;
}

float CIdentify::GetSquare()
{
	/*======图像指针为空，无法操作返回=======*/
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("尚未有图象载入或载入错误!");
	}
	
	/*==获取图像宽度和高度（以像素为单位）===*/
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();

	/*==定义与图像数据操作有关的变量===*/
	unsigned char *pOldBuffer,  *pOldBits,*pOldTemp;

	int nWidthBytes, nNumColors,i,j;
	
	/*======获取原图像指针=======*/
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	//原图像颜色数
	nNumColors = m_pIndentify->GetNumColors();
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	POINT point;
	long total_x=0,total_y=0,total_pix=0,square_11=0,square_02=0,square_20=0,square=0;
	double angle=0;
	double radian=0.0;

	pOldTemp=pOldBits;
	for(i=0;i<nHeight;i++)
	{
		pOldBits=pOldTemp;
		pOldBits=pOldBits+nWidthBytes*i;
		for(j=0;j<nWidthBytes;j++)
		{
			if(*pOldBits==0)
			{
				total_x+=i;
				total_y+=j;
				total_pix++;
			}
			pOldBits++;
		}
	}

    //获取质心
	point.x=total_x/total_pix;
	point.y=total_y/total_pix;

	//求中心矩
	for(i=0;i<nHeight;i++)
	{
		pOldBits=pOldTemp;
		pOldBits=pOldBits+nWidthBytes*i;
		for(j=0;j<nWidthBytes;j++)
		{
			if(*pOldBits==0)
			{
				square_11=square_11+(i-point.x)*(j-point.y);
				square_20=square_20+(i-point.x)*(i-point.x);
				square_02=square_02+(j-point.y)*(j-point.y);
			}
			pOldBits++;
		}
	}

	radian=atan(2*square_11/(double)(square_20-square_02));
	angle=(radian/3.1416)*360;

	CGeoTrans GeoTrans(m_pIndentify);
	GeoTrans.Rotate((int)(angle+0.5));

	nWidth=m_pIndentify->GetWidth();
	nHeight=m_pIndentify->GetHeight();

	/*==定义与图像数据操作有关的变量===*/
	unsigned char *pOldBuffer1,  *pOldBits1,*pOldTemp1;

	int nWidthBytes1, nNumColors1;
	
	/*======获取原图像指针=======*/
	pOldBuffer1 = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes1, 
												m_pIndentify->GetNumBits() );
	//原图像颜色数
	nNumColors1 = m_pIndentify->GetNumColors();
	//原图像数据指针
	pOldBits1 = (unsigned char *) &pOldBuffer1[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors1 * sizeof(RGBQUAD)];
	total_pix=0; 
	pOldTemp1=pOldBits1;
	for(i=0;i<nHeight;i++)
	{
		pOldBits1=pOldTemp1;
		pOldBits1=pOldBits1+nWidthBytes1*i;
		for(j=0;j<nWidth;j++)
		{
			if(*pOldBits1==0)
			{
				square=square+(i-point.x)*(j-point.y);
				total_pix++;
			}
			pOldBits1++;
		}
	}
	//返回归一化后的二阶中心矩
	return (float)square/(float)(total_pix*total_pix);
} 
