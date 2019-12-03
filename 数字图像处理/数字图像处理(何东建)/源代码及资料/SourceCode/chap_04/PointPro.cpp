// PointPro.cpp: implementation of the CPointPro class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PointPro.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPointPro, CObject)

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CPointPro::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CPointPro::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif

/***********************************************************************
*                                                                      *
*                               点处理                                 *
*                                                                      *
***********************************************************************/

////////////////////////////////////////////////////////////////////////
//CPointPro()   
//----------------------------------------------------------------------
//基本功能：构造CPointPro类对象。由于此构造函数没有
//			CDibObject类，所以必须为其他所用到的CPointPro函
//			数提供一个CDibObject构造器。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CPointPro::CPointPro()
{
}

////////////////////////////////////////////////////////////////////////
//CPointPro(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//基本功能：构造CPointPro类对象。由于此构造函数有一个
//			CDibObject类，所以无须为其他所用到的CPointPro函
//			数提供一个CDibObject构造器
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CPointPro::CPointPro( CDibObject *pDibObject )
{

	m_pDibObject = pDibObject;

}

////////////////////////////////////////////////////////////////////////
//BOOL MakeGray256(BYTE mGrayType, CDibObject *pDibObject)   
//----------------------------------------------------------------------
//基本功能：本函数将传入的CDibObject对象中的图像从彩色转换为灰度图像。
//			如果进行此调整之前没有指定一个CDibObject对象指针，则必须在
//			调整时加以指定。
//----------------------------------------------------------------------
//参数说明：BYTE mGrayType  0：Y=0.3R+0.59G+0.11B 
//							1: Y=R 
//							2: Y=G
//							3: Y=B					
//			CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::MakeGray256(BYTE mGrayType, 
									CDibObject *pDibObject )
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	//低于8位的图像不进行处理
	if( m_pDibObject->GetNumBits() < 8 ) return( FALSE );


	//获取原图像字节宽度和转换后的8位256色灰度图像的字节宽度
	int nOldWidthBytes, nNewWidthBytes;
	char *pBuffer = (char *) m_pDibObject->GetDIBPointer( &nOldWidthBytes, 8, &nNewWidthBytes );
	if( pBuffer == NULL ) return( FALSE );

	//定义变量
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	RGBQUAD *pOldRGBPalette, *pNewRGBPalette;
	unsigned char *pOldBits, *pNewBits, *pTemp, *pNewTemp;
	int nNumColors, nNumNewColors;

	//获取文件头指针
	pOldBFH = (BITMAPFILEHEADER *) pBuffer;
	//获取信息头指针
	pOldBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//获取调色板指针
	pOldRGBPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//新图像颜色数
	nNumNewColors = 256;
	//获取原图像数据指针
	pOldBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	//为新图像分配内存
	HGLOBAL hGlobal;
	//新图像总字节数
	DWORD dwSize;
	dwSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) +
			256 * sizeof( RGBQUAD ) + 
			m_pDibObject->GetHeight() * nNewWidthBytes;
	hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
	if( hGlobal == NULL )
	{
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	pBuffer = (char *) ::GlobalLock( hGlobal );
	if( pBuffer == NULL )
	{
		::GlobalFree( hGlobal );
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//获得新图像的文件头指针
	pNewBFH = (BITMAPFILEHEADER *) pBuffer;
	//获得新图像的信息头指针
	pNewBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//获得新图像的调色板指针
	pNewRGBPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)
						+sizeof(BITMAPINFOHEADER)];
	//复制原图像文件头数据到新图像文件头
	*pNewBFH = *pOldBFH;
	//复制原图像信息头数据到新图像信息头
	*pNewBIH = *pOldBIH;

	//循环变量定义
	int i, j = 256, x, y;

	pNewBIH->biBitCount = 8;
	pNewBIH->biSizeImage = nNewWidthBytes * m_pDibObject->GetHeight();
	pNewBIH->biClrUsed = 256;
	pNewBFH->bfSize =	sizeof( BITMAPFILEHEADER ) + 
						sizeof( BITMAPINFOHEADER ) + 
						256 * sizeof( RGBQUAD ) + 
						pNewBIH->biSizeImage;
	pNewBFH->bfOffBits =	sizeof( BITMAPFILEHEADER ) + 
							sizeof( BITMAPINFOHEADER ) + 
							nNumNewColors * sizeof( RGBQUAD );
	pNewBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
	m_pDibObject->SetPaletteBytes( 256 * sizeof( RGBQUAD ));
	
	//创建256色灰度调色板
	for( i = 0; i < j; i++ )
	{
		pNewRGBPalette[i].rgbRed = i;
		pNewRGBPalette[i].rgbGreen = i;
		pNewRGBPalette[i].rgbBlue = i;
	}


	unsigned char *pLookup;	//调色板查找表
	DWORD dwGray;	//灰度级别

	switch( m_pDibObject->GetNumBits() )
	{
	case 8:		//256色图像
		pLookup = new unsigned char [256];
		if( pLookup == NULL ) break;
		memset( pLookup, 0, 256 );	//调色板查找表清0（256项）
		switch( mGrayType)
		{
		case 0:	//按亮度Y=0.3R+0.59G+0.11B将彩色图像转换为灰度图像
			for( i=0; i<256; i++ )
			{
				dwGray = ( (DWORD) pOldRGBPalette[i].rgbRed * 30 +
					(DWORD) pOldRGBPalette[i].rgbGreen * 59 +
					(DWORD) pOldRGBPalette[i].rgbBlue * 11 ) / 100;
				pLookup[i] = (unsigned char) dwGray;
			}
			break;
		case 1:	//按亮度Y=R将彩色图像转换为灰度图像
			for( i=0; i<256; i++ )
			{
				dwGray =  (DWORD) pOldRGBPalette[i].rgbRed;
				pLookup[i] = (unsigned char) dwGray;
			}
			break;
		case 2:	//按亮度Y=G将彩色图像转换为灰度图像
			for( i=0; i<256; i++ )
			{
				dwGray = (DWORD) pOldRGBPalette[i].rgbGreen;
				pLookup[i] = (unsigned char) dwGray;
			}
			break;
		case 3:	//按亮度Y=B将彩色图像转换为灰度图像
			for( i=0; i<256; i++ )
			{
				dwGray =(DWORD) pOldRGBPalette[i].rgbBlue;
				pLookup[i] = (unsigned char) dwGray;
			}
			break;
		}
		for( y = 0; y < pOldBIH->biHeight; y++ )
		{
			pTemp = pOldBits;	//位图数据起始指针
			pTemp += y * nOldWidthBytes;	//位图数据下一行起始指针
			//转换成灰度索引
			for( x = 0; x < pOldBIH->biWidth; x++ ) pTemp[x] = pLookup[pTemp[x]];
		}
		delete [] pLookup;	//释放pLookup查找表所占内存
		
		memcpy( pNewBits, pOldBits, nNewWidthBytes * m_pDibObject->GetHeight());
		
		break;
	case 16:	//16位色真彩色图像
		unsigned char ucRed, ucGreen, ucBlue;
		for( y=0; y<pOldBIH->biHeight; y++ )
		{
			//位图数据起始指针
			pTemp = pOldBits;	
			pNewTemp = pNewBits;
			//位图数据下一行起始指针
			pTemp += y * nOldWidthBytes;	
			pNewTemp += y * nNewWidthBytes;
			switch( mGrayType )
			{
			case 0:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					GETRGB555( ucRed, ucGreen, ucBlue, &pTemp[x*2] );
					//按亮度Y=0.3R+0.59G+0.11B将彩色图像转换为灰度图像
					dwGray = (ucRed * 30 + ucGreen * 59 +ucBlue * 11) / 100;
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 1:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					GETRGB555( ucRed, ucGreen, ucBlue, &pTemp[x*2] );
					//按亮度Y=R将彩色图像转换为灰度图像
					dwGray = (ucRed * 30 + ucGreen * 59 +ucBlue * 11) / 100;
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 2:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					GETRGB555( ucRed, ucGreen, ucBlue, &pTemp[x*2] );
					//按亮度Y=G将彩色图像转换为灰度图像
					dwGray = (ucRed * 30 + ucGreen * 59 +ucBlue * 11) / 100;
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 3:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					GETRGB555( ucRed, ucGreen, ucBlue, &pTemp[x*2] );
					//按亮度Y=B将彩色图像转换为灰度图像
					dwGray = (ucRed * 30 + ucGreen * 59 +ucBlue * 11) / 100;
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			}
		}
		break;
	case 24:	//24位真彩色图像
		for( y=0; y<pOldBIH->biHeight; y++ )
		{
			//位图数据起始指针
			pTemp = pOldBits;	
			pNewTemp = pNewBits;
			//位图数据下一行起始指针
			pTemp += y * nOldWidthBytes;	
			pNewTemp += y * nNewWidthBytes;
			switch( mGrayType )
			{
			case 0:	//按亮度Y=0.3R+0.59G+0.11B将彩色图像转换为灰度图像
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					dwGray = ( (DWORD) pTemp[x*3+2] * 30	//红色
						+(DWORD) pTemp[x*3+1] * 59		//绿色
						+(DWORD) pTemp[x*3] * 11		//兰色
						) / 100;
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 1:	//按亮度Y=R将彩色图像转换为灰度图像
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					dwGray =  (DWORD) pTemp[x*3+2]; //红色
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 2:	//按亮度Y=G将彩色图像转换为灰度图像
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					dwGray = (DWORD) pTemp[x*3+1] ;	//绿色
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 3:	//按亮度Y=B将彩色图像转换为灰度图像
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					dwGray =(DWORD) pTemp[x*3];	//兰色
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			}
		}
		break;
	case 32:	//32位真彩色图像
		for( y=0; y<pOldBIH->biHeight; y++ )
		{
			//位图数据起始指针
			pTemp = pOldBits;	
			pNewTemp = pNewBits;
			//位图数据下一行起始指针
			pTemp += y * nOldWidthBytes;	
			pNewTemp += y * nNewWidthBytes;
			switch( mGrayType )
			{
			case 0:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					//按亮度Y=0.3R+0.59G+0.11B将彩色图像转换为灰度图像
					dwGray = ( (DWORD) pTemp[x*4] * 30 //红色
						+(DWORD) pTemp[x*4+1] * 59	//绿色
						+(DWORD) pTemp[x*4+2] * 11 //兰色
						) / 100;
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 1:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					//按亮度Y=R将彩色图像转换为灰度图像
					dwGray =  (DWORD) pTemp[x*4];//红色
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 2:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					//按亮度Y=G将彩色图像转换为灰度图像
					dwGray = (DWORD) pTemp[x*4+1] ;	//绿色
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 3:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					//按亮度Y=B将彩色图像转换为灰度图像
					dwGray =(DWORD) pTemp[x*4+2] ; //兰色
					//给新图像数据赋值
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			}
		}
		break;
	}
			
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hGlobal );
	m_pDibObject->SetDib( hGlobal );

	m_pDibObject->ProcessImageHeader();

	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//基本功能：本函数为CImagePointProcess类对象指定一个CDibObject对象指针
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：无。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CPointPro::SetDibObjectClass( CDibObject *pDibObject )
{

	m_pDibObject = pDibObject;

}

////////////////////////////////////////////////////////////////////////
//int *CreateHistogram()   
//----------------------------------------------------------------------
//基本功能：本函数创建传入的CDibObject对象中图像的直方图。如果进行此调
//			整之前没有指定一个CDibObject对象指针，则必须在调整时加以指
//			定。任何未传入的坐标值或默认的-1坐标值都将被置为图像的最大值
//			或最大植。变量nX1和nY1将被置为0，nX2将被置为图像宽度减1，nY2
//			将被置为图像高度减1。想要在整个图像上进行操作时，最好的方法
//			是不传入nX1、nY1、nX2和nY2值。这样它们会被默认为整个图像。
//----------------------------------------------------------------------
//参数说明：int	nX1						默认为-1
//			int	nY1						默认为-1
//			int	nX2						默认为-1
//			int	nY2						默认为-1
//			unsigned char *pData		图像位图数据指针
//			RGBQUAD *pPalette			图像调色板指针
//			int nWidthBytes				图像字节宽度
//			CDibObject *pDibObject	默认为NULL。
//----------------------------------------------------------------------
//返    回：直方图数组指针*pBuffer其中：
//			pBuffer[]		存储亮度直方图数据
//			pBuffer[256]	存储红色直方图数据
//			pBuffer[512]	存储绿色直方图数据
//			pBuffer[768]	存储蓝直方图数据
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int *CPointPro::CreateHistogram( int nX1, int nY1, int nX2, int nY2, 
									   unsigned char *pData, RGBQUAD *pPalette, 
									   int nWidthBytes, CDibObject *pDibObject )
{

	//图像指针为空，无法操作返回
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	if( m_pDibObject == NULL ) return( FALSE );

	//分配直方图数据缓存区（数组）
	int *pBuffer = new int [256 * 4];
	//分配直方图数据缓存区失败
	if( pBuffer == NULL ) return( NULL );
	//直方图数据缓存区清零
	memset( pBuffer, 0, ( 256 * 4) * sizeof( int ) );

	//变量定义
	DWORD dwGray;
	int x, y;
	unsigned char *pTemp, ucRed, ucGreen, ucBlue;

	//图像的高度
	int nHeight = m_pDibObject->GetHeight();

	switch( m_pDibObject->GetNumBits() )
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
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//数据指针定位到图像数据每行的起始nX1位置
			pTemp += nX1;
			for( x = nX1; x <= nX2; x++ )
			{
				//pTemp[x]为当前像素值，它为调色板项的索引值，
				//以此为索引，取出调色板项的相应红绿蓝分量值。
				ucRed   = pPalette[pTemp[x]].rgbRed;
				ucGreen = pPalette[pTemp[x]].rgbGreen;
				ucBlue  = pPalette[pTemp[x]].rgbBlue;

				//按关系L=0.3R+0.59G+0.11B，得到亮度值
				dwGray  = ( (DWORD) ucRed * 30 +
					        (DWORD) ucGreen * 59 +
					        (DWORD) ucBlue * 11 ) / 100;
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
		for( y = nY1; y <= nY2; y++ )
		{
			//数据指针定位到图像数据起始位置
			pTemp = pData;
			//数据指针定位到图像数据每行的起始零位置
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//数据指针定位到图像数据每行的起始nX1位置
			pTemp += ( nX1 * 2 );
			for( x = nX1; x <= nX2; x++ )
			{
				//获取三原色分量
				GETRGB555( ucRed, ucGreen, ucBlue, pTemp );

				//按关系L=0.3R+0.59G+0.11B，得到亮度值
				dwGray = ( (DWORD) ucRed * 30 +
					       (DWORD) ucGreen * 59 +
					       (DWORD) ucBlue * 11 ) / 100;
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
		for( y = nY1; y < nY2; y++ )
		{
			//数据指针定位到图像数据起始位置
			pTemp = pData;
			//数据指针定位到图像数据每行的起始零位置
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//数据指针定位到图像数据每行的起始nX1位置
			pTemp += ( nX1 * 3 );

			for( x=nX1; x<=nX2; x++ )
			{
				//获取像素颜色的三原色。
				ucRed   = pTemp[x * 3 + 2];
				ucGreen = pTemp[x * 3 + 1];
				ucBlue  = pTemp[x * 3];

				//按关系L=0.3R+0.59G+0.11B，得到亮度值
				dwGray  = ( (DWORD) ucRed * 30 +
					        (DWORD) ucGreen * 59 +
					        (DWORD) ucBlue * 11 ) / 100;
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
		for( y = nY1; y <= nY2; y++ )
		{
			//数据指针定位到图像数据起始位置
			pTemp = pData;
			//数据指针定位到图像数据每行的起始零位置
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//数据指针定位到图像数据每行的起始nX1位置
			pTemp += ( nX1 * 4 );

			for( x = nX1; x <= nX2; x++ )
			{
				//获取像素颜色的三原色。
				GETRGB888( ucRed, ucGreen, ucBlue, pTemp );

				//按关系L=0.3R+0.59G+0.11B，得到亮度值
				dwGray = ( (DWORD) ucRed * 30 +
					       (DWORD) ucGreen * 59 +
					       (DWORD) ucBlue * 11 ) / 100;

				dwGray &= 0x000000ff;

				//亮度直方图数据
				pBuffer[dwGray]++;
				//红色直方图数据
				pBuffer[256 + ucRed]++;
				//绿色直方图数据
				pBuffer[512 + ucGreen]++;
				//蓝色直方图数据
				pBuffer[768 + ucBlue]++;

				pTemp += 4;
			}
		}
		break;
	}
	
	return( pBuffer );

}

////////////////////////////////////////////////////////////////////////
//int FindThresh_Auto( CDibObject *pDibObject )   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像用用自调整阈值法确定阈
//			值(只对8位灰度图像有效）
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：int 阈值
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CPointPro::FindThresh_Auto( CDibObject *pDibObject )
{
	if( pDibObject != NULL ) m_pDibObject = pDibObject;	//CDibObject对象指针
	if( m_pDibObject == NULL ) return( -1 );	//若未指定 CDibObject 对象指针返回FALSE

	unsigned char *pBuffer, *pBits;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, i;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH：获得位图信息头地址
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors：获得调色板中的颜色数。图像为16位色或更高时为0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette：获得调色板数据地址
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits：获得位图数据地址
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];
	if (pBIH->biBitCount != 8) return (-1);
	//获取灰度图像的宽度和高度（以像素表示）
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;
	
	int *pHistogram = GetHistogram();
	
	long MaxPiex1 = 0;
	long MaxPiex2 = 0;
	unsigned char MaxGray1, MaxGray2;

	for(i = 0; i < 256; i++)
	{
		if ( pHistogram[i] > MaxPiex1)
		{
			MaxPiex1 = pHistogram[i];
			MaxGray1 = i;
		}
	}

	for(i = 0; i < 256; i++)
	{
		if ( pHistogram[i] > MaxPiex2 && pHistogram[i] < MaxPiex1)
		{
			MaxPiex2 = pHistogram[i];
			MaxGray2 = i;
		}
	}

	unsigned char Temp;
	
	if(MaxGray1 > MaxGray2)
	{
		Temp = MaxGray1;
		MaxGray1 = MaxGray2;
		MaxGray2 = MaxGray1;
	}

	int Thresh;

	long MinPiex = nWidth * nHeight;

	for(i = MaxGray1; i <= MaxGray2; i++)
	{
		if(pHistogram[i] < MinPiex) Thresh = i;
	}

	delete [] pHistogram;

	return(Thresh);
}

////////////////////////////////////////////////////////////////////////
//int FindThresh( CDibObject *pDibObject )   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像用判别分析法确定阈值，
//			本函数中对于像素总数大于等于255的图像采用其直方图数据作为最
//			佳阈值的数据源，对于像素总数小于255的图像直接采用图像数据作
//			为最佳阈值的数据源，这样可大大提高计算速度。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：int 阈值
//----------------------------------------------------------------------
//注：		只对8位灰度图像有效
////////////////////////////////////////////////////////////////////////
int CPointPro::FindThresh_Discriminant( CDibObject *pDibObject )
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( -1 );

	unsigned char *pBuffer, *pBits;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, i;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH：获得位图信息头地址
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors：获得调色板中的颜色数。图像为16位色或更高时为0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette：获得调色板数据地址
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits：获得位图数据地址
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];
	if (pBIH->biBitCount != 8) return (-1);

	//获取灰度图像的宽度和高度（以像素表示）
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;
	
	int *nHistogramBuffer = GetHistogram();
	
	float Sigma[256];
	float Mean1, Mean2;
	
	//Sigma[256]数组清零
	for( i = 0;i < 256; i++) Sigma[i] = 0.0;
	
	int nLevelLow = 0;
	int nLevelHigh = 0;
	
	//查找最小灰度值
	i = 0;
	while(nLevelLow == 0)
	{
		if( nHistogramBuffer[i] != 0 )
		{
			nLevelLow = i;
		}
		i++;
	}

	//查找最大灰度值
	i = 255;
	while(nLevelHigh == 0)
	{
		if( nHistogramBuffer[i] != 0 )
		{
			nLevelHigh = i;
		}
		i--;
	}

	//th循环，小于阈值th的像素归为第一类，大于th的像素归为第二类
	//th=0、1、2、…、nLevel。
	for(int th = nLevelLow; th <= nLevelHigh; th++)   
	{
		long lGrayLow = 0;
		long lGrayHigh = 0;
		long lSumLow = 0;
		long lSumHigh = 0;
		for(i = nLevelLow; i < th; i++)
		{
			lSumLow += nHistogramBuffer[i];
			lGrayLow += i * nHistogramBuffer[i];
		}
		for(i = th; i <= nLevelHigh; i++)
		{
			lSumHigh += nHistogramBuffer[i];
			lGrayHigh += i * nHistogramBuffer[i];
		}
		
		if( lSumLow > 0 )
		{
			Mean1 = (float)lGrayLow / lSumLow;
		}
		else
		{
			Mean1 = 0.0;
		}
		if( lSumHigh > 0)
		{
			Mean2 = (float)lGrayHigh / lSumHigh;
		}
		else
		{
			Mean2 = 0.0;
		}
		
		//计算两类之间的方差
		Sigma[th] = (float)lSumLow * lSumHigh 
			* (Mean1 - Mean2) * (Mean1 - Mean2);
	}
	
	//th循环结束，求方差最大的阈值
	float MaxSigma = Sigma[nLevelLow];
	int Thresh = 0;
	for( i = nLevelLow + 1; i <= nLevelHigh; i++)
	{
		if(MaxSigma < Sigma[i])
		{
			MaxSigma = Sigma[i];
			Thresh = i;
		}
	}
	
	delete [] nHistogramBuffer;
	return(Thresh);
	
}

////////////////////////////////////////////////////////////////////////
//BOOL Threshold( )   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像用给定的阈值进行阈值化
//			(只对8位灰度图像有效）
//----------------------------------------------------------------------
//参数说明：int Thresh					阈值
//			int Mode					阈值化方法，1对象置黑，0对象置白
//			BOOL bSetPalette			是否设置调色板
//			CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::Threshold(int Thresh, int Mode, BOOL bSetPalette, CDibObject *pDibObject)
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( -1 );
	if( !IsGray256() ) return( -1 );

	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, i;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH：获得位图信息头地址
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors：获得调色板中的颜色数。图像为16位色或更高时为0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette：获得调色板数据地址
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits：获得位图数据地址
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];

	//获取灰度图像的宽度和高度（以像素表示）
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;

	// 灰度映射表
	unsigned char pMap[256];
	
	switch (Mode)
	{
	case 1:
		for( i = 0; i <= Thresh; i++) pMap[i] = 255;
		for( i = Thresh + 1; i <= 255; i++) pMap[i] = 0;
		break;
	case 0:
		for( i = 0; i <= Thresh; i++) pMap[i] = 0;
		for( i = Thresh + 1; i <= 255; i++) pMap[i] = 255;
		break;
	}

	for ( i = 0; i < nHeight; i ++)
	{
		pTemp = pBits;
		pTemp += i * nWidthBytes;	//位图数据下一行起始指针
		for ( int j = 0; j < nWidth; j ++)
		{
			pTemp[j] = pMap[pTemp[j]];
		}
	}
	m_pDibObject->ProcessPalette();
		if( bSetPalette && m_pDibObject->m_nPaletteBytes > 0 )
		{
			CWindowDC WindowDC( NULL );
			m_pDibObject->SetPalette( &WindowDC );
		}

	::GlobalUnlock( m_pDibObject->GetDib() );

	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;


	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL IsGray256(CDibObject *pDibObject)
//----------------------------------------------------------------------
//基本功能：本函数判断传入的CDibObject对象中的图像是否为256级灰度图像。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL
//----------------------------------------------------------------------
//返回：BOOL：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::IsGray256( CDibObject *pDibObject )
{
	//使用传入的CDibObject对象
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//不是8位图像，必不是256级灰度图像，不处理返回FALSE
	if( m_pDibObject->GetNumBits() != 8 ) return( FALSE );

	//定义变量
	unsigned char *pBuffer;
	RGBQUAD *pPalette;
	int nWidthBytes, i;

	//pBuffer: 获得位图数据指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );

	//pPalette：获得调色板数据地址
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];

	//检查是否为256色灰度调色板
	for( i=0; i<256; i++ )
	{
		if( pPalette[i].rgbRed != pPalette[i].rgbGreen 
			|| pPalette[i].rgbRed != pPalette[i].rgbBlue
			|| pPalette[i].rgbGreen != pPalette[i].rgbBlue )
			return( FALSE);
	}
	::GlobalUnlock( m_pDibObject->GetDib() );

	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL LinerTran(CImagObject *pDibObject, float fa, float fb)   
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像进行灰度的线性变换。如
//			果进行此调整之前没有指定一个CDibObject对象指针，则必须在调
//			整时加以指定。任何未传入的坐标值或默认的-1坐标值都将被置为图
//			像的最大值或最大植。变量nX1和nY1将被置为0，nX2将被置为图像宽
//			度减1，nY2将被置为图像高度减1。想要在整个图像上进行操作时，
//			最好的方法是不传入nX1、nY1、nX2和nY2值。这样它们会被默认为整
//			个图像。
//----------------------------------------------------------------------
//参数说明：float fa			线性变换的斜率
//			float fb			线性变换的截距
//			int	nX1,			默认为-1
//			int	nY1,			默认为-1
//			int	nX2,			默认为-1
//			int	nY2,			默认为-1
//			CDibObject *pDibObject, 默认为NULL
//----------------------------------------------------------------------
//返回：BOOL；成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::LinerTran(float fa, float fb,
									 int nX1,
									 int nY1,
									 int nX2,
									 int nY2,
									 CDibObject *pDibObject)
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	BOOL bLessThanHalf, bCompleteImage;	

	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2,
										&bCompleteImage, 
										&bLessThanHalf );
	//定义变量
	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, x, y, i;
	
	//pBuffer: 获得位图数据指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );
	
	//pBIH：获得位图信息头地址
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors：获得调色板中的颜色数。图像为16位色或更高时为0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette：获得调色板数据地址
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits：获得位图数据地址
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
		nNumColors*sizeof(RGBQUAD)];
	
	LOGPALETTE *pLogPal = m_pDibObject->GetLogPal();
	CPalette *_pPalette = m_pDibObject->GetPalette();
	
	//创建灰度映射表
	unsigned char *pMap = PointSlopeLinerTran(fa, fb);
			
	switch( m_pDibObject->GetNumBits() )
	{
	case 1:
		for( y=nY1; y<=nY2; y++ )
		{
			unsigned char Mask;
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 / 8 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				Mask = 0x80 >> ( x & 7 );
				if( pTemp[0] & Mask ) pTemp[0] &= ( Mask ^ 0xff );
				else pTemp[0] |= Mask;
				if( ( x & 7 ) == 7 ) pTemp++;
			}
		}
		break;
	case 4:
		if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( 
								pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + ( nX1 / 2 ) );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					if( !( x & 1 ) ) Data >>= 4;
					else Data &= 0x0f;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, ucGreen, 
						ucBlue, pPalette, nNumColors );
					if( !( x & 1 ) )
					{
						Data <<= 4;
						(*pTemp) &= 0x0f;
						(*pTemp) |= Data;
					}
					else
					{
						(*pTemp) &= 0xf0;
						(*pTemp) |= Data;
						pTemp++;
					}
				}
			}
		}
		break;
	case 8:
		if(IsGray256())
		{
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + nX1 );
				for( x=nX1; x<=nX2; x++ )
				{
					pTemp[x] = pMap[pTemp[x]];
				}
			}
		}
		else if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + nX1 );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, 
						ucGreen, ucBlue, pPalette, nNumColors );
					*pTemp++ = Data;
				}
			}
		}
		break;
	case 16:
		unsigned char ucRed, ucGreen, ucBlue;
		for( y=0; y< nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( m_pDibObject->GetHeight() - 1 - y ) 
						* nWidthBytes );
			pTemp += ( nX1 * 2 );
			for( x=nX1; x<=nX2; x++ )
			{
				GETRGB555( ucRed, ucGreen, ucBlue, pTemp );
				ucRed = pMap[ucRed];
				ucGreen = pMap[ucGreen];
				ucBlue = pMap[ucBlue];
				PUTRGB555( ucRed, ucGreen, ucBlue, pTemp );
				pTemp += 2;
			}
		}
		break;
	case 24:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
				* nWidthBytes ) + ( nX1 * 3 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp += 3;
			}
		}
		break;
	case 32:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 * 4 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp[3] = pMap[pTemp[3]];
				pTemp += 4;
			}
		}
		break;
	}
	
	delete [] pMap;
	
	::GlobalUnlock( m_pDibObject->GetDib() );
	
	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;
	
	// 返回
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL GrayStretch()
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像进行灰度拉伸变换。如果
//			进行此调整之前没有指定一个CDibObject对象指针，则必须在调整
//			时加以指定。任何未传入的坐标值或默认的-1坐标值都将被置为图像
//			的最大值或最大植。变量nX1和nY1将被置为0，nX2将被置为图像宽度
//			减1，nY2将被置为图像高度减1。想要在整个图像上进行操作时，最
//			好的方法是不传入nX1、nY1、nX2和nY2值。这样它们会被默认为整个
//			图像。
//----------------------------------------------------------------------
//参数说明：BYTE bX1	灰度拉伸第一个点的x坐标
//			BYTE bY1	灰度拉伸第一个点的y坐标
//			BYTE bX2	灰度拉伸第二个点的x坐标
//			BYTE bY2	灰度拉伸第二个点的y坐标
//			int	nX1, 默认为-1
//			int	nY1, 默认为-1
//			int	nX2, 默认为-1
//			int	nY2, 默认为-1
//			CDibObject *pDibObject, 默认为NULL
//----------------------------------------------------------------------
//返回：BOOL：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::GrayStretch(BYTE bX1,
									   BYTE bY1,
									   BYTE bX2,
									   BYTE bY2,
									   int nX1,
									   int nY1,
									   int nX2,
									   int nY2, 
									   CDibObject *pDibObject)
{
	//创建灰度映射表
	unsigned char *pMap = DoublePointLinerTran(bX1, bY1, bX2, bY2);

	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	BOOL bLessThanHalf, bCompleteImage;	

	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2,
										&bCompleteImage, 
										&bLessThanHalf );
	//定义变量
	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, x, y, i;
	
	//pBuffer: 获得位图数据指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );
	
	//pBIH：获得位图信息头地址
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors：获得调色板中的颜色数。图像为16位色或更高时为0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette：获得调色板数据地址
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits：获得位图数据地址
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
		nNumColors*sizeof(RGBQUAD)];
	
	LOGPALETTE *pLogPal = m_pDibObject->GetLogPal();
	CPalette *_pPalette = m_pDibObject->GetPalette();
	
	switch( m_pDibObject->GetNumBits() )
	{
	case 1:
		for( y=nY1; y<=nY2; y++ )
		{
			unsigned char Mask;
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 / 8 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				Mask = 0x80 >> ( x & 7 );
				if( pTemp[0] & Mask ) pTemp[0] &= ( Mask ^ 0xff );
				else pTemp[0] |= Mask;
				if( ( x & 7 ) == 7 ) pTemp++;
			}
		}
		break;
	case 4:
		if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( 
								pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + ( nX1 / 2 ) );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					if( !( x & 1 ) ) Data >>= 4;
					else Data &= 0x0f;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, ucGreen, 
						ucBlue, pPalette, nNumColors );
					if( !( x & 1 ) )
					{
						Data <<= 4;
						(*pTemp) &= 0x0f;
						(*pTemp) |= Data;
					}
					else
					{
						(*pTemp) &= 0xf0;
						(*pTemp) |= Data;
						pTemp++;
					}
				}
			}
		}
		break;
	case 8:
		if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + nX1 );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, 
						ucGreen, ucBlue, pPalette, nNumColors );
					*pTemp++ = Data;
				}
			}
		}
		break;
	case 16:
		unsigned char ucRed, ucGreen, ucBlue;
		for( y=0; y< nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( m_pDibObject->GetHeight() - 1 - y ) 
						* nWidthBytes );
			pTemp += ( nX1 * 2 );
			for( x=nX1; x<=nX2; x++ )
			{
				GETRGB555( ucRed, ucGreen, ucBlue, pTemp );
				ucRed = pMap[ucRed];
				ucGreen = pMap[ucGreen];
				ucBlue = pMap[ucBlue];
				PUTRGB555( ucRed, ucGreen, ucBlue, pTemp );
				pTemp += 2;
			}
		}
		break;
	case 24:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
				* nWidthBytes ) + ( nX1 * 3 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp += 3;
			}
		}
		break;
	case 32:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 * 4 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp[3] = pMap[pTemp[3]];
				pTemp += 4;
			}
		}
		break;
	}
	
	delete [] pMap;
	
	::GlobalUnlock( m_pDibObject->GetDib() );
	
	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;
	
	// 返回
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL InteEqualize(CDibObject *pDibObject)
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像进行直方图均衡。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL
//----------------------------------------------------------------------
//返回：BOOL：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::InteEqualize(CDibObject *pDibObject)
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//只处理8位图像
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("目前只支持8位灰度图像的处理！");
		return( FALSE );
	}
	
	//定义变量
	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, x, y;

	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//pBuffer: 获得位图数据指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );

	//pBIH：获得位图信息头地址
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors：获得调色板中的颜色数。图像为16位色或更高时为0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette：获得调色板数据地址
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits：获得位图数据地址
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];

	// 创建直方图数据
	int *pHistogram = GetHistogram();

	// 灰度映射表
	BYTE bMap[256];

	int i, j;

	for(i = 0; i < 256; i++)
	{
		bMap[i] = 0;
	}
	
	// 计算灰度映射表
	for (i = 0; i < 256; i++)
	{
		// 初始为0
		long lTemp = 0;
		
		for (j = 0; j <= i ; j++)
		{
			lTemp += pHistogram[j];
		}
		
		// 计算对应的新灰度值
		bMap[i] = (BYTE) (lTemp * 255 / nHeight / nWidth);
	}

	delete [] pHistogram;

	for(y = 0; y < nHeight; y++)
	{
		pTemp = pBits;
		pTemp += y * nWidthBytes;	//位图数据下一行起始指针
		for(x = 0; x < nWidth; x++)
		{
			long lpSrc = pTemp[x];
			// 计算新的灰度值
			pTemp[x] = bMap[lpSrc];			
		}
	}
	
	::GlobalUnlock( m_pDibObject->GetDib() );

	// 返回
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL ReplaceColorPal()
//----------------------------------------------------------------------
//基本功能：本函数对传入的CDibObject对象中的图像用指定的伪彩色编码表来
//			替换图像的调试板。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL
//			BYTE * bpColorsTable	- 伪彩色编码表
//----------------------------------------------------------------------
//返回：BOOL：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::ReplaceColorPal(CDibObject *pDibObject, 
										   BYTE *bpColorsTable)
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	
	//定义变量
	unsigned char *pBuffer;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//pBuffer: 获得位图数据指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );

	//pBIH：获得位图信息头地址
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors：获得调色板中的颜色数。图像为16位色或更高时为0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette：获得调色板数据地址
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits：获得位图数据地址

	// 读取伪彩色编码，更新DIB调色板
	for(int i=0; i<256; i++ )
	{
		// 更新DIB调色板红色分量
		pPalette[i].rgbRed = bpColorsTable[i * 4];
		// 更新DIB调色板绿色分量
		pPalette[i].rgbGreen = bpColorsTable[i * 4 + 1];
		// 更新DIB调色板蓝色分量
		pPalette[i].rgbBlue	= bpColorsTable[i * 4 + 2];
		// 更新DIB调色板保留位
		pPalette[i].rgbReserved	= 0;
	}
	::GlobalUnlock( m_pDibObject->GetDib() );
	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//unsigned char *PointSlopeLinerTran(float fa, float fb)
//----------------------------------------------------------------------
//基本功能：本函数计算点斜线性变换的灰度映射表，它是一个辅助函数，供点斜
//			线性变换函数使用。
//----------------------------------------------------------------------
//参数说明：float fa			线性变换的斜率
//			float fb			线性变换的截距
//----------------------------------------------------------------------
//返回：unsigned char *型的灰度映射表。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char *CPointPro::PointSlopeLinerTran(float fa,
														 float fb)
{
	// 灰度映射表
	unsigned char *pMap = new unsigned char[256];
	
	// 计算灰度映射表
	for (int i = 0; i <= 255; i++)
	{
		float fTemp = fa * (float)(i) + fb;
		// 判断是否超出范围
		if (fTemp > 255)
		{
			// 直接赋值为255
			pMap[i] = 255;
		}
		else if (fTemp < 0)
		{
			// 直接赋值为0
			pMap[i] = 0;
		}
		else
		{
			// 四舍五入
			pMap[i] = (unsigned char) (fTemp + 0.5);
		}
	}
	return(pMap);
}

////////////////////////////////////////////////////////////////////////
//unsigned char *DoublePointLinerTran()
//----------------------------------------------------------------------
//基本功能：本函数计算点斜线性变换的灰度映射表，它是一个辅助函数，供点斜
//			线性变换函数使用。
//----------------------------------------------------------------------
//参数说明：BYTE bX1	灰度拉伸第一个点的x坐标
//			BYTE bY1	灰度拉伸第一个点的y坐标
//			BYTE bX2	灰度拉伸第二个点的x坐标
//			BYTE bY2	灰度拉伸第二个点的y坐标
//----------------------------------------------------------------------
//返回：unsigned char *型的灰度映射表。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char * CPointPro::DoublePointLinerTran(BYTE bX1,
														   BYTE bY1,
														   BYTE bX2,
														   BYTE bY2)
{
	// 灰度映射表
	unsigned char *pMap = new unsigned char[256];
	int i;
	// 计算灰度映射表
	for (int i = 0; i <= bX1; i++)
	{
		// 判断bX1是否大于0（防止分母为0）
		if (bX1 > 0)
		{
			// 线性变换
			pMap[i] = (BYTE) bY1 * i / bX1;
		}
		else
		{
			// 直接赋值为0
			pMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)
	{
		// 判断bX1是否等于bX2（防止分母为0）
		if (bX2 != bX1)
		{
			// 线性变换
			pMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// 直接赋值为bY1
			pMap[i] = bY1;
		}
	}

	for (; i < 256; i++)
	{
		// 判断bX2是否等于255（防止分母为0）
		if (bX2 != 255)
		{
			// 线性变换
			pMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// 直接赋值为255
			pMap[i] = 255;
		}
	}
	return(pMap);
}

////////////////////////////////////////////////////////////////////////
//BOOL IsBlackandWhite256(CDibObject *pDibObject)
//----------------------------------------------------------------------
//基本功能：本函数判断传入的CDibObject对象中的图像是否为256级二值图像。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL
//----------------------------------------------------------------------
//返回：BOOL：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::IsBlackandWhite256(CDibObject *pDibObject)
{
	//使用传入的CDibObject对象
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//不是8位图像，必不是256级灰度图像，不处理返回FALSE
	if( !IsGray256() ) return( FALSE );

	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH：获得位图信息头地址
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors：获得调色板中的颜色数。图像为16位色或更高时为0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette：获得调色板数据地址
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits：获得位图数据地址
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];
	//获取灰度图像的宽度和高度（以像素表示）
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;

	for(int y = 0; y < nHeight; y++)
	{
		pTemp = pBits;
		pTemp += y * nWidthBytes;	//位图数据下一行起始指针
		for(int x = 0; x < nWidth; x++)
		{
			if( pTemp[x] != 0 && pTemp[x] != 255) return(FALSE);			
		}
	}

	return(TRUE);
}


////////////////////////////////////////////////////////////////////////
//int *GetHistogram()   
//----------------------------------------------------------------------
//基本功能：本函数获得传入的CDibObject对象中图像的直方图。如果进行此调
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
//			CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：直方图数组指针*pBuffer其中：
//			nHistogramBuffer[]		存储亮度直方图数据
//			nHistogramBuffer[256]	存储红色直方图数据
//			nHistogramBuffer[512]	存储绿色直方图数据
//			nHistogramBuffer[768]	存储蓝直方图数据
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int *CPointPro::GetHistogram( int nX1, 
									int nY1,
									int nX2,
									int nY2,
									CDibObject *pDibObject)
{
	//使用传入的CDibObject对象
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//坐标规整化
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	//创建直方图数据
	int *nHistogramBuffer = CreateHistogram( nX1, nY1, nX2, nY2, pBits, pPalette, nWidthBytes );

	::GlobalUnlock( m_pDibObject->GetDib() );

	return( nHistogramBuffer );

}

////////////////////////////////////////////////////////////////////////
//BOOL SplitChannels()   
//----------------------------------------------------------------------
//基本功能：本函数将传入的CDibObject对象中的图像分离成由红、绿、蓝分量
//			构成的256色灰度图像。
//----------------------------------------------------------------------
//参数说明：CDibObject *pRed		指向红色通道的图像指针
//			CDibObject *pGreen	指向绿色通道的图像指针
//			CDibObject *pBlue		指向蓝色通道的图像指针
//----------------------------------------------------------------------
//返    回：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::SplitChannels(CDibObject *pRed, 
							  CDibObject *pGreen, 
							  CDibObject *pBlue)
{
	//没有传入相应指针无法完成操作
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("没有传入原始图像，无法完成操作！");
		return FALSE;
	}

	if(pRed == NULL)
	{
		AfxMessageBox("没有传入红色通道指针，无法完成操作！");
		return FALSE;
	}

	if(pGreen == NULL)
	{
		AfxMessageBox("没有传入绿色通道指针，无法完成操作！");
		return FALSE;
	}

	if(pBlue == NULL)
	{
		AfxMessageBox("没有传入蓝色通道指针，无法完成操作！");
		return FALSE;
	}

	//已是256色灰度图像不用再转换
	if(IsGray256())
	{
		AfxMessageBox("已是256色灰度图像，不能进行通道分离！");
		return FALSE;
	}

	//红色通道
	MakeGray256(1, pRed);

	//绿色通道
	MakeGray256(2, pGreen);
	
	//蓝色通道
	MakeGray256(3, pBlue);

	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL GetMixedChannel()   
//----------------------------------------------------------------------
//基本功能：本函数获得传入的CDibObject对象中的图像中的混合分量通道。
//----------------------------------------------------------------------
//参数说明：CDibObject *pMixed	指向混合通道的图像指针
//----------------------------------------------------------------------
//返    回：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::GetMixedChannel(CDibObject *pMixed)
{
	//没有传入相应指针无法完成操作
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("没有传入原始图像，无法完成操作！");
		return FALSE;
	}

	if(pMixed == NULL)
	{
		AfxMessageBox("没有传入通道指针，无法完成操作！");
		return FALSE;
	}

	//已是256色灰度图像不用再转换
	if(IsGray256())
	{
		AfxMessageBox("已是256色灰度图像，不能进行通道分离！");
		return FALSE;
	}

	MakeGray256(0, pMixed);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL GetRedChannel()   
//----------------------------------------------------------------------
//基本功能：本函数获得传入的CDibObject对象中的图像中的红色分量通道。
//----------------------------------------------------------------------
//参数说明：CDibObject *pRed	指向红色通道的图像指针
//----------------------------------------------------------------------
//返    回：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::GetRedChannel(CDibObject *pRed)
{
	//没有传入相应指针无法完成操作
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("没有传入原始图像，无法完成操作！");
		return FALSE;
	}

	if(pRed == NULL)
	{
		AfxMessageBox("没有传入红色通道指针，无法完成操作！");
		return FALSE;
	}

	//已是256色灰度图像不用再转换
	if(IsGray256())
	{
		AfxMessageBox("已是256色灰度图像，不能进行通道分离！");
		return FALSE;
	}

	//红色通道
	MakeGray256(1, pRed);
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL GetGreenChannel()   
//----------------------------------------------------------------------
//基本功能：本函数获得传入的CDibObject对象中的图像中的绿色分量通道。
//----------------------------------------------------------------------
//参数说明：CDibObject *pGreen	指向混合通道的图像指针
//----------------------------------------------------------------------
//返    回：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::GetGreenChannel(CDibObject *pGreen)
{
	//没有传入相应指针无法完成操作
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("没有传入原始图像，无法完成操作！");
		return FALSE;
	}

	if(pGreen == NULL)
	{
		AfxMessageBox("没有传入通道指针，无法完成操作！");
		return FALSE;
	}

	//已是256色灰度图像不用再转换
	if(IsGray256())
	{
		AfxMessageBox("已是256色灰度图像，不能进行通道分离！");
		return FALSE;
	}

	MakeGray256(2, pGreen);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL GetBlueChannel()   
//----------------------------------------------------------------------
//基本功能：本函数获得传入的CDibObject对象中的图像中的蓝色分量通道。
//----------------------------------------------------------------------
//参数说明：CDibObject *pBlue	指向蓝色通道的图像指针
//----------------------------------------------------------------------
//返    回：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::GetBlueChannel(CDibObject *pBlue)
{
	//没有传入相应指针无法完成操作
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("没有传入原始图像，无法完成操作！");
		return FALSE;
	}

	if(pBlue == NULL)
	{
		AfxMessageBox("没有传入通道指针，无法完成操作！");
		return FALSE;
	}

	//已是256色灰度图像不用再转换
	if(IsGray256())
	{
		AfxMessageBox("已是256色灰度图像，不能进行通道分离！");
		return FALSE;
	}

	MakeGray256(3, pBlue);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL CombineChannels()   
//----------------------------------------------------------------------
//基本功能：本函数将传入的由红、绿、蓝分量构成的256色灰度图像合成为采色图
//			像。
//----------------------------------------------------------------------
//参数说明：CDibObject *pRed		指向红色通道的图像指针
//			CDibObject *pGreen	指向绿色通道的图像指针
//			CDibObject *pBlue		指向蓝色通道的图像指针
//----------------------------------------------------------------------
//返    回：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::CombineChannels(CDibObject *pRed, 
								CDibObject *pGreen, 
								CDibObject *pBlue)
{
	//没有传入相应指针无法完成操作
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("没有传入创建新图像，无法完成操作！");
		return FALSE;
	}

	if(pRed == NULL)
	{
		AfxMessageBox("没有传入红色通道指针，无法完成操作！");
		return FALSE;
	}

	if(pGreen == NULL)
	{
		AfxMessageBox("没有传入绿色通道指针，无法完成操作！");
		return FALSE;
	}

	if(pBlue == NULL)
	{
		AfxMessageBox("没有传入蓝色通道指针，无法完成操作！");
		return FALSE;
	}

	//传入的图像不符合需要的格式，无法完成操作
	if(pRed->GetNumBits() != 8)
	{
		AfxMessageBox("传入的红色通道不是8位图像，无法完成操作！");
		return FALSE;
	}

	if(pGreen->GetNumBits() != 8)
	{
		AfxMessageBox("传入的绿色通道不是8位图像，无法完成操作！");
		return FALSE;
	}

	if(pBlue->GetNumBits() != 8)
	{
		AfxMessageBox("传入的蓝色通道不是8位图像，无法完成操作！");
		return FALSE;
	}

	//创建的图像不是24位图像将其转换为24位图像
	if(m_pDibObject->GetNumBits() != 24)
	{
		m_pDibObject->ChangeFormat(24);
	}

	//获得各个图像的大小
	int nCombineWidth, nRedWidth, nGreenWidth, nBlueWidth;
	int nCombineHeight, nRedHeight, nGreenHeight, nBlueHeight;
	
	//合并后图像的大小
	nCombineWidth = m_pDibObject->GetWidth();
	nCombineHeight = m_pDibObject->GetHeight();

	//红色通道图像大小
	nRedWidth = pRed->GetWidth();
	nRedHeight = pRed->GetHeight();

	//绿色通道图像大小
	nGreenWidth = pGreen->GetWidth();
	nGreenHeight = pGreen->GetHeight();

	//蓝色通道图像大小
	nBlueWidth = pBlue->GetWidth();
	nBlueHeight = pBlue->GetHeight();

	CRect rectCombine(0, 0, nCombineWidth, nCombineHeight);
	CRect rectRed(0, 0, nRedWidth, nRedHeight);
	CRect rectGreen(0, 0, nGreenWidth, nGreenHeight);
	CRect rectBlue(0, 0, nBlueWidth, nBlueHeight);
	
	//所有图像的大小应一致
	if (rectCombine != rectRed 
		|| rectRed != rectGreen 
		|| rectGreen != rectBlue 
		|| rectBlue != rectCombine)
	{
		AfxMessageBox("各个通道大小不一致，无法完成操作！");
		return FALSE;
	}

	//获取各个通道原图像字节宽度
	int nCombineWidthBytes, nRedWidthBytes, nGreenWidthBytes, nBlueWidthBytes;
	
	//合并的图像的字节宽度
	char *pCombineBuffer = (char *) m_pDibObject->GetDIBPointer(&nCombineWidthBytes);
	if( pCombineBuffer == NULL ) return( FALSE );
	
	//红色通道的图像的字节宽度
	char *pRedBuffer = (char *) pRed->GetDIBPointer(&nRedWidthBytes);
	if( pRedBuffer == NULL ) return( FALSE );
	
	//绿色通道的图像的字节宽度
	char *pGreenBuffer = (char *) pGreen->GetDIBPointer(&nGreenWidthBytes);
	if( pGreenBuffer == NULL ) return( FALSE );
	
	//蓝色通道的图像的字节宽度
	char *pBlueBuffer = (char *) pBlue->GetDIBPointer(&nBlueWidthBytes);
	if( pBlueBuffer == NULL ) return( FALSE );

	//定义变量
	BITMAPINFOHEADER *pCombineBIH, *pRedBIH, *pGreenBIH, *pBlueBIH;
	BITMAPFILEHEADER *pCombineBFH, *pRedBFH, *pGreenBFH, *pBlueBFH;
	RGBQUAD *pCombineRGBPalette, *pRedRGBPalette, *pGreenRGBPalette, *pBlueRGBPalette;
	unsigned char *pCombineBits, *pRedBits, *pGreenBits, *pBlueBits;
	unsigned char *pCombineTemp, *pRedTemp, *pGreenTemp, *pBlueTemp;
	int nCombineNumColors, nRedNumColors, nGreenNumColors, nBlueNumColors;

	//合并后的图像信息
	//获取文件头指针
	pCombineBFH = (BITMAPFILEHEADER *) pCombineBuffer;
	//获取信息头指针
	pCombineBIH = (BITMAPINFOHEADER *) &pCombineBuffer[sizeof(BITMAPFILEHEADER)];
	//获取调色板指针
	pCombineRGBPalette = (RGBQUAD *) &pCombineBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//图像颜色数
	nCombineNumColors = m_pDibObject->GetNumColors();
	//获取原图像数据指针
	pCombineBits = (unsigned char *) &pCombineBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nCombineNumColors*sizeof(RGBQUAD)];

	//红色通道的图像信息
	//获取文件头指针
	pRedBFH = (BITMAPFILEHEADER *) pRedBuffer;
	//获取信息头指针
	pRedBIH = (BITMAPINFOHEADER *) &pRedBuffer[sizeof(BITMAPFILEHEADER)];
	//获取调色板指针
	pRedRGBPalette = (RGBQUAD *) &pRedBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//图像颜色数
	nRedNumColors = pRed->GetNumColors();
	//获取原图像数据指针
	pRedBits = (unsigned char *) &pRedBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nRedNumColors*sizeof(RGBQUAD)];

	//绿色通道的图像信息
	//获取文件头指针
	pGreenBFH = (BITMAPFILEHEADER *) pGreenBuffer;
	//获取信息头指针
	pGreenBIH = (BITMAPINFOHEADER *) &pGreenBuffer[sizeof(BITMAPFILEHEADER)];
	//获取调色板指针
	pGreenRGBPalette = (RGBQUAD *) &pGreenBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//图像颜色数
	nGreenNumColors = pGreen->GetNumColors();
	//获取原图像数据指针
	pGreenBits = (unsigned char *) &pGreenBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nGreenNumColors*sizeof(RGBQUAD)];

	//蓝色通道的图像信息
	//获取文件头指针
	pBlueBFH = (BITMAPFILEHEADER *) pBlueBuffer;
	//获取信息头指针
	pBlueBIH = (BITMAPINFOHEADER *) &pBlueBuffer[sizeof(BITMAPFILEHEADER)];
	//获取调色板指针
	pBlueRGBPalette = (RGBQUAD *) &pBlueBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//图像颜色数
	nBlueNumColors = pBlue->GetNumColors();
	//获取原图像数据指针
	pBlueBits = (unsigned char *) &pBlueBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nBlueNumColors*sizeof(RGBQUAD)];

	//循环变量定义
	int x, y;

	//行位置
	for(y = 0; y < nCombineHeight; y++ )
	{
		//合并后的图像数据指针定位到起始位置
		pCombineTemp = pCombineBits;
		//合并后的图像数据指针定位到图像数据每行的起始零位置
		pCombineTemp += y * nCombineWidthBytes;
		
		//红色通道图像数据指针定位到起始位置
		pRedTemp = pRedBits;
		//红色通道图像数据指针定位到图像数据每行的起始零位置
		pRedTemp += y * nRedWidthBytes;
		
		//绿色通道图像数据指针定位到起始位置
		pGreenTemp = pGreenBits;
		//绿色通道图像数据指针定位到图像数据每行的起始零位置
		pGreenTemp += y * nGreenWidthBytes;
		
		//蓝色通道图像数据指针定位到起始位置
		pBlueTemp = pBlueBits;
		//蓝色通道图像数据指针定位到图像数据每行的起始零位置
		pBlueTemp += y * nBlueWidthBytes;
		
		//CString s, message = "";

		//列位置
		for(x = 0; x < nCombineWidth; x++)
		{
			//进行合并操作
			pCombineTemp[2] = *pRedTemp;
			pCombineTemp[1] = *pGreenTemp;
			pCombineTemp[0] = *pBlueTemp;

			//调整图像数据指针
			pCombineTemp += 3;
			pRedTemp++;
			pGreenTemp++;
			pBlueTemp++;
		}
	}

	


	return TRUE;
}
