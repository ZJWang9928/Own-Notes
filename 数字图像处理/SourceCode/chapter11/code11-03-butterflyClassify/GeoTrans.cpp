// GeoTrans.cpp: implementation of the CGeoTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GeoTrans.h"
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CGeoTrans, CObject)

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CGeoTrans::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CGeoTrans::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif


/***********************************************************************
*                                                                      *
*                               几何处理类                             *
*                                                                      *
***********************************************************************/

////////////////////////////////////////////////////////////////////////
//构造函数CGeoTrans()    
//----------------------------------------------------------------------
//基本功能：构造一个CGeoTrans类的对象，如不传入CDibObject对象。第   
//			一次调用某一个处理函数时必须给出一个CDibObject对象指针。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CGeoTrans::CGeoTrans()
{

}

////////////////////////////////////////////////////////////////////////
//构造函数CGeoTrans()    
//----------------------------------------------------------------------
//基本功能：构造一个CGeoTrans类的对象并传入CDibObject对象。所有的
//			操作都针对该对象，直到另一个对象作为参数被传给图像处理函数。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CGeoTrans::CGeoTrans( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}

CGeoTrans::~CGeoTrans()
{
}

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//基本功能：本函数为CGeoTrans类对象指定一个CDibObject对象指针
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：无。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CGeoTrans::SetDibObjectClass( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}


////////////////////////////////////////////////////////////////////////
//BOOL TranslationPixel()  
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像进行平移操作。如果进行
//			此调整之前没有指定一个CDibObject对象指针，则必须在调整时加
//			以指定。
//----------------------------------------------------------------------
//参数说明：long  lXOffset     - X轴平移量（像素数）
//			long  lYOffset     - Y轴平移量（像素数）
//			CDibObject *pDibObject, 默认为NULL
//----------------------------------------------------------------------
//返回：BOOL：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
//注    意：该函数用来水平移动DIB图像。。
////////////////////////////////////////////////////////////////////////
BOOL CGeoTrans::TranslationPixel(long lXOffset, 
									  long lYOffset, 
									  CDibObject *pDibObject)
{
	if( pDibObject != NULL ) m_pDibObject = pDibObject;

	if( m_pDibObject == NULL ) return( FALSE );

	//获得图像宽度和高度
	int nOldWidth = m_pDibObject->GetWidth();
	int nOldHeight = m_pDibObject->GetHeight();

    //计算平移后的图像实际宽度
	int nNewWidth  = (long) nOldWidth+lXOffset;
	//计算平移后的图像高度
	int nNewHeight = (long) nOldHeight+lYOffset;	
	

	//定义变量
	unsigned char *pOldBuffer, *pNewBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;
	int nOldWidthBytes, nNewWidthBytes, nNumColors, x, y;

	//原图像指针
	pOldBuffer = (unsigned char *) 
				m_pDibObject->GetDIBPointer( &nOldWidthBytes, 
				m_pDibObject->GetNumBits(), 
				&nNewWidthBytes, nNewWidth );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) 
			&pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *)
				&pOldBuffer[sizeof(BITMAPFILEHEADER)
				+sizeof(BITMAPINFOHEADER)
				+nNumColors*sizeof(RGBQUAD)];

	DWORD dwNewSize;
	HGLOBAL hNewDib;

	//新图像大小（包括文件头、信息头、调色板和图像数据）
	dwNewSize = (DWORD) nNumColors * sizeof( RGBQUAD ) +
						sizeof( BITMAPFILEHEADER ) + 
						sizeof( BITMAPINFOHEADER ) + 
						nNewWidthBytes * nNewHeight;
	//为新图像分配内存
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
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

	//新图像文件头指针
	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	//新图像信息头指针
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	//新图像调色板指针
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
							+sizeof(BITMAPINFOHEADER)];
	//新图像数据指针
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)
									+nNumColors*sizeof(RGBQUAD)];

	//用255（白色）填充新图像数据区
	memset(pNewBits, (BYTE)255, nNewWidthBytes * nNewHeight );

	//用原图像文件头数据填充新图像文件头
	memcpy( pNewBFH, pOldBFH, sizeof(BITMAPFILEHEADER) );

	//用原图像信息头数据填充新图像信息头
	memcpy( pNewBIH, pOldBIH, sizeof(BITMAPINFOHEADER) );

	//调整文件总字节数
	pNewBFH->bfSize = dwNewSize;

	//调整新图像的宽度和高度
	pNewBIH->biWidth = nNewWidth;
	pNewBIH->biHeight = nNewHeight;

	pNewBIH->biSizeImage = nNewWidthBytes * nNewHeight;
	pNewBIH->biClrUsed = nNumColors;
	pNewBFH->bfSize = sizeof( BITMAPFILEHEADER ) 
					+ sizeof( BITMAPINFOHEADER ) 
					+ nNumColors * sizeof( RGBQUAD ) 
					+ pNewBIH->biSizeImage;
	pNewBFH->bfOffBits = sizeof( BITMAPFILEHEADER ) 
						+ sizeof( BITMAPINFOHEADER ) 
						+ nNumColors * sizeof( RGBQUAD );

	//用原图像调色板填充新图像调色板
	if(nNumColors != 0)
		memcpy( pNewPalette, pOldPalette, nNumColors*sizeof(RGBQUAD) );

	//由图像位数确定的移动字节数
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}
   //每行
	for(y = 0; y < nOldHeight; y++)
	{
		pOldTemp = pOldBits;
		//注意由于DIB中图像第一行其实保存在最后一行的位置。
		pOldTemp +=  (nOldHeight - 1 - y) * nOldWidthBytes;
		//每列
		for(x = 0; x < nOldWidth; x++)
		{
			//指向新DIB第y行，第x个像素的指针
			//计算该像素在源DIB中的坐标
			int x0 = x + lXOffset;
			int y0 = y + lYOffset;
			
			//判断是否在源图范围内
			if( (x0 >=0) && (x0 < nNewWidthBytes) 
				&& (y0 >=0) && (y0 <nNewHeight))
			{
				//指向源DIB第y0行，第x0个像素的指针
				pNewTemp = pNewBits;
				pNewTemp += (nNewHeight - 1 - y0) * nNewWidthBytes;
				pNewTemp += x0 * nMovedBits;
				//复制像素
				memcpy( pNewTemp, pOldTemp, nMovedBits );
			}
			pOldTemp += nMovedBits;
		}
	}


	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	//将新图像设置为当前图像
	m_pDibObject->SetDib( hNewDib );

	m_pDibObject->ProcessImageHeader();

	return TRUE;
}



////////////////////////////////////////////////////////////////////////
//BOOL Mirror()  
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像进行镜像操作。如果进行
//			此调整之前没有指定一个CDibObject对象指针，则必须在调整时加
//			以指定。
//----------------------------------------------------------------------
//参数说明：BOOL  bDirection	镜像的方向，TRUE表示水平镜像，FALSE表示
//								垂直镜像
//			CDibObject *pDibObject, 默认为NULL
//----------------------------------------------------------------------
//返回：BOOL：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CGeoTrans::Mirror(BOOL bDirection,
							CDibObject *pDibObject)
{
	//使用传入的CDibObject对象
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//获得图像宽度和高度
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//定义变量
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	unsigned char *pTemp1, *pTemp2;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//原图像指针
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
											+sizeof(BITMAPINFOHEADER)
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
	
	//由图像位数确定的移动字节数
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}

	//判断镜像方式
	if (bDirection)
	{
		//水平镜像
		
		//垂直中轴坐标
		int nMiddleX = nWidth / 2;
		
		//针对图像每行进行操作
		for(y = 0; y < nHeight; y++)
		{
			//指向图像的倒数第y行
			pOldTemp = pOldBits;
			pOldTemp +=  y * nWidthBytes;
			pNewTemp = pNewBits;
			pNewTemp +=  y * nWidthBytes;
			
			//针对每行图像左半部分进行操作
			for(x = 0; x <= nMiddleX; x++)
			{
				//将源图像第x个像素复制到新图像倒数第x个像素
				pTemp1 = pOldTemp + x * nMovedBits;
				pTemp2 = pNewTemp + (nWidth -1 - x) * nMovedBits;
				memcpy(pTemp2, pTemp1, nMovedBits);
				//将源图像倒数第x个像素复制到新图像第x个像素
				pTemp1 = pOldTemp + (nWidth -1 - x) * nMovedBits;
				pTemp2 = pNewTemp + x * nMovedBits;
				memcpy(pTemp2, pTemp1, nMovedBits);
			}
		
		}
	}
	else
	{
		//垂直镜像

		//水平中轴坐标
		int nMiddleY = nHeight / 2;
		//针对上半图像进行操作
		for(y = 0; y <= nMiddleY; y++)
		{
			
			//指向源图像倒数第y行像素起点的指针
			pOldTemp = pOldBits;
			pOldTemp +=  y * nWidthBytes;

			//指向新图像第y行像素起点的指针
			pNewTemp = pNewBits;
			pNewTemp +=  (nHeight - 1 - y) * nWidthBytes;
			
			//将源图像倒数第y行像素复制到新图像第y行
			memcpy(pNewTemp, pOldTemp, nWidthBytes);

			//指向源图像第y行像素起点的指针
			pOldTemp = pOldBits;
			pOldTemp +=  (nHeight - 1 - y) * nWidthBytes;

			//指向新图像倒数第y行像素起点的指针
			pNewTemp = pNewBits;
			pNewTemp +=  y * nWidthBytes;
			
			//将源图像第y行像素复制到新图像倒数第y行
			memcpy(pNewTemp, pOldTemp, nWidthBytes);
		}
	}

	//用新图像数据填充原图像数据区
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );


	return TRUE;
}

/**********************************************************************
* 函数名称：BOOL Transpose()
* 基本功能：本函数对传入的CDibObject对象中的图像进行转置操作。如果进行此
*			调整之前没有指定一个CDibObject对象指针，则必须在调整时加以指定。
* 参数说明：CDibObject *pDibObject, 默认为NULL
* 返回值: BOOL  - 转置成功返回TRUE，否则返回FALSE。
**********************************************************************/
BOOL CGeoTrans::Transpose(CDibObject *pDibObject)
{
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//获得图像宽度和高度
	int nOldWidth = m_pDibObject->GetWidth();
	int nOldHeight = m_pDibObject->GetHeight();
	//转置后图像的宽度和高度
	int nNewWidth = nOldHeight;
	int nNewHeight = nOldWidth;

	//定义变量
	unsigned char *pOldBuffer, *pNewBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;
	int nOldWidthBytes, nNewWidthBytes, nNumColors, x, y;

	//原图像指针
	pOldBuffer = (unsigned char *) 
				m_pDibObject->GetDIBPointer( &nOldWidthBytes, 
				m_pDibObject->GetNumBits(), 
				&nNewWidthBytes, nNewWidth );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) 
			&pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *)
				&pOldBuffer[sizeof(BITMAPFILEHEADER)
				+sizeof(BITMAPINFOHEADER)
				+nNumColors*sizeof(RGBQUAD)];

	DWORD dwNewSize;
	HGLOBAL hNewDib;

	//新图像大小（包括文件头、信息头、调色板和图像数据）
	dwNewSize = (DWORD) nNumColors * sizeof( RGBQUAD ) +
						sizeof( BITMAPFILEHEADER ) + 
						sizeof( BITMAPINFOHEADER ) + 
						nNewWidthBytes * nNewHeight;
	//为新图像分配内存
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
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

	//新图像文件头指针
	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	//新图像信息头指针
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	//新图像调色板指针
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
							+sizeof(BITMAPINFOHEADER)];
	//新图像数据指针
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)
									+nNumColors*sizeof(RGBQUAD)];

	//用255（白色）填充新图像数据区
	memset(pNewBits, (BYTE)255, nNewWidthBytes * nNewHeight );

	//用原图像文件头数据填充新图像文件头
	memcpy( pNewBFH, pOldBFH, sizeof(BITMAPFILEHEADER) );

	//用原图像信息头数据填充新图像信息头
	memcpy( pNewBIH, pOldBIH, sizeof(BITMAPINFOHEADER) );

	//调整文件总字节数
	pNewBFH->bfSize = dwNewSize;

	//调整新图像的宽度和高度
	pNewBIH->biWidth = nNewWidth;
	pNewBIH->biHeight = nNewHeight;

	//pNewBIH->biBitCount = nNewBitsPerPixel;
	pNewBIH->biSizeImage = nNewWidthBytes * nNewHeight;
	pNewBIH->biClrUsed = nNumColors;
	pNewBFH->bfSize = sizeof( BITMAPFILEHEADER ) 
					+ sizeof( BITMAPINFOHEADER ) 
					+ nNumColors * sizeof( RGBQUAD ) 
					+ pNewBIH->biSizeImage;
	pNewBFH->bfOffBits = sizeof( BITMAPFILEHEADER ) 
						+ sizeof( BITMAPINFOHEADER ) 
						+ nNumColors * sizeof( RGBQUAD );

	//用原图像调色板填充新图像调色板
	if(nNumColors != 0)
		memcpy( pNewPalette, pOldPalette, nNumColors*sizeof(RGBQUAD) );

	//由图像位数确定的移动字节数
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}

	// 针对图像每行进行操作
	for(y = 0; y < nOldHeight; y++)
	{
		//指向源图像第y行
		pOldTemp = pOldBits;
		pOldTemp += (nOldHeight - 1 -y) * nOldWidthBytes;
		// 针对每行图像每列进行操作
		for(x = 0; x < nOldWidth; x++)
		{
			//指向转置后的图像第x行，第y个像素的指针
			//注意此处宽度和高度应该互换
			pNewTemp = pNewBits;
			pNewTemp += (nNewHeight - 1 - x) * nNewWidthBytes;
			pNewTemp += y * nMovedBits;
			
			//复制像素
			memcpy(pNewTemp, pOldTemp, nMovedBits);
			pOldTemp += nMovedBits;			
		}
	}

	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	//将新图像设置为当前图像
	m_pDibObject->SetDib( hNewDib );

	m_pDibObject->ProcessImageHeader();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL ZoomImage()  
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像进行缩放操作。如果进行
//			此调整之前没有指定一个CDibObject对象指针，则必须在调整时加
//			以指定。
//----------------------------------------------------------------------
//参数说明：float fXZoomRatio	- X轴方向缩放比率
//			float fYZoomRatio	- Y轴方向缩放比率
//			CDibObject *pDibObject, 默认为NULL
//----------------------------------------------------------------------
//返回：BOOL：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CGeoTrans::ZoomImage(float fXZoomRatio, 
							   float fYZoomRatio,
							   CDibObject *pDibObject)
{
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//获得图像宽度和高度
	int nOldWidth = m_pDibObject->GetWidth();
	int nOldHeight = m_pDibObject->GetHeight();

	// 计算缩放后的图像实际宽度
	int nNewWidth, nNewHeight;
	// 此处直接加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
	nNewWidth = (int)(nOldWidth * fXZoomRatio + 0.5);
	
	// 计算缩放后的图像高度
	nNewHeight = (int)(nOldHeight * fYZoomRatio + 0.5);

	//定义变量
	unsigned char *pOldBuffer, *pNewBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;
	int nOldWidthBytes, nNewWidthBytes, nNumColors, x, y;

	//原图像指针
	pOldBuffer = (unsigned char *) 
				m_pDibObject->GetDIBPointer( &nOldWidthBytes, 
				m_pDibObject->GetNumBits(), 
				&nNewWidthBytes, nNewWidth );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) 
			&pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *)
				&pOldBuffer[sizeof(BITMAPFILEHEADER)
				+sizeof(BITMAPINFOHEADER)
				+nNumColors*sizeof(RGBQUAD)];

	DWORD dwNewSize;
	HGLOBAL hNewDib;

	//新图像大小（包括文件头、信息头、调色板和图像数据）
	dwNewSize = (DWORD) nNumColors * sizeof( RGBQUAD ) +
						sizeof( BITMAPFILEHEADER ) + 
						sizeof( BITMAPINFOHEADER ) + 
						nNewWidthBytes * nNewHeight;
	//为新图像分配内存
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
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

	//新图像文件头指针
	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	//新图像信息头指针
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	//新图像调色板指针
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
							+sizeof(BITMAPINFOHEADER)];
	//新图像数据指针
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)
									+nNumColors*sizeof(RGBQUAD)];

	//用255（白色）填充新图像数据区
	memset(pNewBits, (BYTE)255, nNewWidthBytes * nNewHeight );

	//用原图像文件头数据填充新图像文件头
	memcpy( pNewBFH, pOldBFH, sizeof(BITMAPFILEHEADER) );

	//用原图像信息头数据填充新图像信息头
	memcpy( pNewBIH, pOldBIH, sizeof(BITMAPINFOHEADER) );

	//调整文件总字节数
	pNewBFH->bfSize = dwNewSize;

	//调整新图像的宽度和高度
	pNewBIH->biWidth = nNewWidth;
	pNewBIH->biHeight = nNewHeight;

	//pNewBIH->biBitCount = nNewBitsPerPixel;
	pNewBIH->biSizeImage = nNewWidthBytes * nNewHeight;
	pNewBIH->biClrUsed = nNumColors;
	pNewBFH->bfSize = sizeof( BITMAPFILEHEADER ) 
					+ sizeof( BITMAPINFOHEADER ) 
					+ nNumColors * sizeof( RGBQUAD ) 
					+ pNewBIH->biSizeImage;
	pNewBFH->bfOffBits = sizeof( BITMAPFILEHEADER ) 
						+ sizeof( BITMAPINFOHEADER ) 
						+ nNumColors * sizeof( RGBQUAD );

	//用原图像调色板填充新图像调色板
	if(nNumColors != 0)
		memcpy( pNewPalette, pOldPalette, nNumColors*sizeof(RGBQUAD) );

	//由图像位数确定的移动字节数
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}

	//针对图像每行进行操作
	for(y = 0; y < nNewHeight; y++)
	{
		//指向新图像第y行
		//注意此处宽度和高度是新图像的宽度和高度
		pNewTemp = pNewBits;
		pNewTemp += (nNewHeight - 1 - y) * nNewWidthBytes;
		//针对图像每列进行操作
		for(x = 0; x < nNewWidth; x++)
		{
			//计算该像素在源图像中的坐标
			int y0 = (long) (y / fYZoomRatio + 0.5);
			int x0 = (long) (x / fXZoomRatio + 0.5);

			//判断是否在源图范围内
			if( (x0 >= 0) && (x0 < nOldWidth) && 
				(y0 >= 0) && (y0 < nOldHeight))
			{
						
					//指向源图像第y0行，第x0个像素
					//注意此处宽度和高度应该互换
					pOldTemp = pOldBits;
					pOldTemp += (nOldHeight - 1 - y0) * nOldWidthBytes;
					pOldTemp += x0 * nMovedBits;
					
					//复制像素
					memcpy(pNewTemp, pOldTemp, nMovedBits);
			}
			pNewTemp += nMovedBits;

		}
		
	}

	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	//将新图像设置为当前图像
	m_pDibObject->SetDib( hNewDib );

	m_pDibObject->ProcessImageHeader();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL Rotate()  
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像进行旋转操作。如果进行
//			此调整之前没有指定一个CDibObject对象指针，则必须在调整时加
//			以指定。
//----------------------------------------------------------------------
//参数说明：int iRotateAngle	- 旋转的角度（0-360度）
//			CDibObject *pDibObject, 默认为NULL
//----------------------------------------------------------------------
//返回：BOOL：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
//注	意：该函数用来以图像中心为中心旋转图像
////////////////////////////////////////////////////////////////////////
BOOL CGeoTrans::Rotate(int nRotateAngle,
							CDibObject *pDibObject)
{
	if( pDibObject != NULL ) m_pDibObject = pDibObject;

	if( m_pDibObject == NULL ) return( FALSE );

	//获得图像宽度和高度
	int nOldWidth = m_pDibObject->GetWidth();
	int nOldHeight = m_pDibObject->GetHeight();

	//旋转角度（弧度）
	float	fRotateAngle;
	
	//旋转角度的正弦和余弦值
	float	fSina, fCosa;
	
	//源图四个角的坐标（以图像中心为坐标系原点）
	float	fSrcX1, fSrcY1;
	float	fSrcX2, fSrcY2;
	float	fSrcX3, fSrcY3;
	float	fSrcX4, fSrcY4;
	
	//旋转后四个角的坐标（以图像中心为坐标系原点）
	float	fDstX1, fDstY1;
	float	fDstX2, fDstY2;
	float	fDstX3, fDstY3;
	float	fDstX4, fDstY4;
	
	//两个中间常量
	float	f1,f2;

	//将旋转角度从度转换到弧度
	fRotateAngle = (float) RADIAN(nRotateAngle);

	//计算旋转角度的正弦
	fSina = (float) sin((double)fRotateAngle);
	
	//计算旋转角度的余弦
	fCosa = (float) cos((double)fRotateAngle);
	
	//计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (float) (- (nOldWidth  - 1) / 2);
	fSrcY1 = (float) (  (nOldHeight - 1) / 2);
	fSrcX2 = (float) (  (nOldWidth  - 1) / 2);
	fSrcY2 = (float) (  (nOldHeight - 1) / 2);
	fSrcX3 = (float) (- (nOldWidth  - 1) / 2);
	fSrcY3 = (float) (- (nOldHeight - 1) / 2);
	fSrcX4 = (float) (  (nOldWidth  - 1) / 2);
	fSrcY4 = (float) (- (nOldHeight - 1) / 2);
	
	//计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	//计算旋转后的图像实际宽度
	int nNewWidth  = (long) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	
	//计算旋转后的图像高度
	int nNewHeight = (long) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);
	
	//两个常数
	f1 = (float) (-0.5 * (nNewWidth - 1) * fCosa - 0.5 * (nNewHeight - 1) * fSina
		+ 0.5 * (nOldWidth  - 1));
	f2 = (float) ( 0.5 * (nNewWidth - 1) * fSina - 0.5 * (nNewHeight - 1) * fCosa
		+ 0.5 * (nOldHeight - 1));

	//定义变量
	unsigned char *pOldBuffer, *pNewBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;
	int nOldWidthBytes, nNewWidthBytes, nNumColors, x, y;

	//原图像指针
	pOldBuffer = (unsigned char *) 
				m_pDibObject->GetDIBPointer( &nOldWidthBytes, 
				m_pDibObject->GetNumBits(), 
				&nNewWidthBytes, nNewWidth );
	if( pOldBuffer == NULL ) return( FALSE );

	//原图像文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//原图像信息头指针
	pOldBIH = (BITMAPINFOHEADER *) 
			&pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//原图像调色板指针
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//原图像数据指针
	pOldBits = (unsigned char *)
				&pOldBuffer[sizeof(BITMAPFILEHEADER)
				+sizeof(BITMAPINFOHEADER)
				+nNumColors*sizeof(RGBQUAD)];

	DWORD dwNewSize;
	HGLOBAL hNewDib;

	//新图像大小（包括文件头、信息头、调色板和图像数据）
	dwNewSize = (DWORD) nNumColors * sizeof( RGBQUAD ) +
						sizeof( BITMAPFILEHEADER ) + 
						sizeof( BITMAPINFOHEADER ) + 
						nNewWidthBytes * nNewHeight;
	//为新图像分配内存
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
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

	//新图像文件头指针
	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	//新图像信息头指针
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	//新图像调色板指针
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
							+sizeof(BITMAPINFOHEADER)];
	//新图像数据指针
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)
									+nNumColors*sizeof(RGBQUAD)];

	//用255（白色）填充新图像数据区
	memset(pNewBits, (BYTE)255, nNewWidthBytes * nNewHeight );

	//用原图像文件头数据填充新图像文件头
	memcpy( pNewBFH, pOldBFH, sizeof(BITMAPFILEHEADER) );

	//用原图像信息头数据填充新图像信息头
	memcpy( pNewBIH, pOldBIH, sizeof(BITMAPINFOHEADER) );

	//调整文件总字节数
	pNewBFH->bfSize = dwNewSize;

	//调整新图像的宽度和高度
	pNewBIH->biWidth = nNewWidth;
	pNewBIH->biHeight = nNewHeight;

	pNewBIH->biSizeImage = nNewWidthBytes * nNewHeight;
	pNewBIH->biClrUsed = nNumColors;
	pNewBFH->bfSize = sizeof( BITMAPFILEHEADER ) 
					+ sizeof( BITMAPINFOHEADER ) 
					+ nNumColors * sizeof( RGBQUAD ) 
					+ pNewBIH->biSizeImage;
	pNewBFH->bfOffBits = sizeof( BITMAPFILEHEADER ) 
						+ sizeof( BITMAPINFOHEADER ) 
						+ nNumColors * sizeof( RGBQUAD );

	//用原图像调色板填充新图像调色板
	if(nNumColors != 0)
		memcpy( pNewPalette, pOldPalette, nNumColors*sizeof(RGBQUAD) );

	//由图像位数确定的移动字节数
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}

	//针对图像每行进行操作
	for(y = 0; y < nNewHeight; y++)
	{
		//指向新图像第y行
		//注意此处宽度和高度是新图像的宽度和高度
		pNewTemp = pNewBits;
		pNewTemp += (nNewHeight - 1 - y) * nNewWidthBytes;
		//针对图像每列进行操作
		for(x = 0; x < nNewWidth; x++)
		{
			//计算该像素在源图像中的坐标
			int y0 = (long) (-((float) x) * fSina + ((float) y)
					* fCosa + f2 + 0.5);
			int x0 = (long) ( ((float) x) * fCosa + ((float) y)
					* fSina + f1 + 0.5);

			//判断是否在源图范围内
			if( (x0 >= 0) && (x0 < nOldWidth) && 
				(y0 >= 0) && (y0 < nOldHeight))
			{			
					//指向源图像第y0行，第x0个像素
					//注意此处宽度和高度应该互换
					pOldTemp = pOldBits;
					pOldTemp += (nOldHeight - 1 - y0) * nOldWidthBytes;
					pOldTemp += x0 * nMovedBits;
					
					//复制像素
					memcpy(pNewTemp, pOldTemp, nMovedBits);
			}
			pNewTemp += nMovedBits;
		}
		
	}

	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	//将新图像设置为当前图像
	m_pDibObject->SetDib( hNewDib );

	m_pDibObject->ProcessImageHeader();

	return TRUE;
}

