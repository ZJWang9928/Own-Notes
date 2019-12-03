// Pretreatment.cpp: implementation of the CPretreatment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImgPro.h"
#include "Pretreatment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPretreatment::CPretreatment()
{
	m_pDibObject=NULL;
}

CPretreatment::CPretreatment(CDibObject *pDibObject)
{
	m_pDibObject=(CPretreatment *)pDibObject;
}

CPretreatment::~CPretreatment()
{

}
////////////////////////////////////////////////////////////////////////
//BOOL IsGrayImage(CDibObject *pDibObject)
//----------------------------------------------------------------------
//基本功能：本函数判断传入的CDibObject对象中的图像是否为256级灰度图像。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL
//----------------------------------------------------------------------
//返回：BOOL：成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPretreatment::IsGrayImage(CDibObject *pDibObject)
{
//使用传入的CDibObject对象
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
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

BOOL CPretreatment::MakeGray(CDibObject *pDibObject)
{
	//没有传入相应指针无法完成操作
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("没有传入原始图像，无法完成操作！");
		return FALSE;
	}

	if(pDibObject == NULL)
	{
		AfxMessageBox("没有传入通道指针，无法完成操作！");
		return FALSE;
	}

	//已是256色灰度图像不用再转换
	if(IsGrayImage(m_pDibObject))
	{
		AfxMessageBox("已是256色灰度图像，不能进行通道分离！");
		return FALSE;
	}

	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = (CPretreatment *)pDibObject;
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

		//按亮度Y=0.3R+0.59G+0.11B将彩色图像转换为灰度图像
		for( i=0; i<256; i++ )
		{
			dwGray = ( (DWORD) pOldRGBPalette[i].rgbRed * 30 +
				(DWORD) pOldRGBPalette[i].rgbGreen * 59 +
				(DWORD) pOldRGBPalette[i].rgbBlue * 11 ) / 100;
			pLookup[i] = (unsigned char) dwGray;
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

			for( x=0; x<pOldBIH->biWidth; x++ )
			{
				GETRGB555( ucRed, ucGreen, ucBlue, &pTemp[x*2] );
				//按亮度Y=0.3R+0.59G+0.11B将彩色图像转换为灰度图像
				dwGray = (ucRed * 30 + ucGreen * 59 +ucBlue * 11) / 100;
				//给新图像数据赋值
				pNewTemp[x] = (unsigned char)dwGray;
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

			//按亮度Y=0.3R+0.59G+0.11B将彩色图像转换为灰度图像
			for( x=0; x<pOldBIH->biWidth; x++ )
			{
				dwGray = ( (DWORD) pTemp[x*3+2] * 30	//红色
					+(DWORD) pTemp[x*3+1] * 59		//绿色
					+(DWORD) pTemp[x*3] * 11		//兰色
					) / 100;
					//给新图像数据赋值
				pNewTemp[x] = (unsigned char)dwGray;
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
//BOOL AutoLight()
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
BOOL CPretreatment::AutoLight(
									   BYTE bX1,
									   BYTE bY1,
									   BYTE bX2,
									   BYTE bY2,
									   int nX1,
									   int nY1,
									   int nX2,
									   int nY2, 
									   CDibObject *pDibObject)
{
	// 灰度映射表
	unsigned char *pMap = new unsigned char[256];

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

	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
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
	int nWidthBytes, nNumColors, x, y;
	
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

int * CPretreatment::GetHistogram(  int nX1, 
									int nY1,
									int nX2,
									int nY2,
									CDibObject *pDibObject)
{
	//使用传入的CDibObject对象
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
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

int * CPretreatment::CreateHistogram(int nX1, int nY1, int nX2, int nY2, unsigned char *pData, RGBQUAD *pPalette, int nWidthBytes,CDibObject *pDibObject)
{

	//图像指针为空，无法操作返回
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
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


BOOL CPretreatment::LinerTran(float fa, float fb,
									 int nX1,
									 int nY1,
									 int nX2,
									 int nY2,
									 CDibObject *pDibObject)
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
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

	unsigned char *pMap = new unsigned char[256];
	
	// 计算灰度映射表
	for (i = 0; i <255; i++)
	{
		float fTemp = fa * (float)(i) + fb;
		// 判断是否超出范围
		if (fTemp >= 255)
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
	pMap[255]=255;		
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
		if(IsGrayImage())
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

BOOL CPretreatment::MedianFilter(int nType, int nTempW, int nTempH, int nTempXc, int nTempYc, int nX1, int nY1, int nX2, int nY2)
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

	//8位图像
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

				//指定的排序算法不可识别，用冒泡排序算法。
				if(nType < 0 || nType > 1) nType =0;

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
								if (pGray[i] > pGray[i + 1])
								{	
									nTemp =pGray[i];
									pGray[i] = pGray[i + 1];
									pGray[i + 1] = nTemp;
									bSorted = false;
								}
								
							}
							nLast--;
						} while (!bSorted);
						
					}
					nData = pGray[nSize / 2];
					break;
				
				case 1://快速排序算法
					{
						int i;
						int nHisto[256];
						for( i = 0; i < 256; i++) nHisto[ i ] = 0;
						//统计相同灰度值出现的次数				
						for( i = 0; i < nSize; i++)
						{    
							nHisto[(int)pGray[i]]++;
						}

						//取直方图的中值
						int nCount = 0;
						for( i = 0; i < 256; i++)
						{ 
							//存放直方图中相同灰度值出现次数的累加和
							nCount += nHisto[i];
							if(nCount >= (int)(nSize / 2 + 1))
							{  
								nData = i;
								break;
							}
						}			
					}
					break;
					
				}

				//将计算结果赋于新图像
				*pNewTemp = (unsigned char) m_pDibObject->GetNearestIndex( 
					pNewPalette[nData].rgbRed, pNewPalette[nData].rgbGreen, 
					pNewPalette[nData].rgbBlue,	pNewPalette, nNumColors );

				//新旧图像数据指针加1
				pOldTemp++;
				pNewTemp++;
				
			}
		}
	
	//释放内存
	delete [] pGray;

	::GlobalUnlock( m_pDibObject->GetDib() );
    ::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
    m_pDibObject->SetDib( hNewDib );
    return(TRUE);
}

BOOL CPretreatment::TemplateSmooth(float *fpArray, float fCoef, int nTempW, int nTempH, int nTempXc, int nTempYc, int nX1, int nY1, int nX2, int nY2)
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


//获取图像高度（以像素为单位）
	nHeight = m_pDibObject->GetHeight();

	//定义变量
	unsigned char Data;

	//定义与图像数据操作有关的变量
	unsigned char *pOldBits, *pNewBits, 
				  *pOldTemp, *pNewTemp,
				  *pNeighborTemp;
	int  x, y, i, j;

	//原图像颜色数
	nNumColors = m_pDibObject->GetNumColors();

	//原图像数据指针
	pOldBits = pBits;

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
		//	Convolution(pGray, fpArray, fCoef, nSize, &Data);

			float Sum = 0, fResult;

			//计算卷积
			for(i = 0; i < nSize; i++)
			{
				Sum += pGray[i] * fpArray[i];
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
			Data = (unsigned char) (fResult + 0.5);		
			
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

	//内存解锁
	::GlobalUnlock(m_pDibObject->GetDib());

	return( TRUE );


}

int CPretreatment::ThresholdAuto(CDibObject *pDibObject)
{
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
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

BOOL CPretreatment::Threshold(int Thresh, int Mode, BOOL bSetPalette, CDibObject *pDibObject)
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( -1 );
	if( !IsGrayImage() ) return( -1 );

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
	
	for( i = 0; i <= Thresh; i++) pMap[i] = 0;
	for( i = Thresh + 1; i <= 255; i++) pMap[i] =255;

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
int CPretreatment::ThresholdJustAnlyze(CDibObject *pDibObject)
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
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
BOOL CPretreatment::TemplateOperation(float *fpArray, float fCoef, 
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

BOOL CPretreatment::GradSharping(int Threshold, int nX1, int nY1, int nX2, int nY2,  
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
BOOL CPretreatment::LapSharping(float *fpArray, float fCoef, int nTempW, 
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
void CPretreatment::Convolution(float *fpData, float *fpKernel, float fCoef, 
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

BOOL CPretreatment::IsBlackandWhite256(CDibObject *pDibObject)
{
//使用传入的CDibObject对象
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//不是8位图像，必不是256级灰度图像，不处理返回FALSE
	if( !IsGrayImage() ) return( FALSE );

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
//void CPretreatment::FillHole(CDibObject *m_pDibObject)
//----------------------------------------------------------------------
//基本功能：填充孔洞
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject
//----------------------------------------------------------------------
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CPretreatment::FillHole(CDibObject *m_pDibObject)
{
	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return;


	//坐标规整化
	//m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;
	//DWORD dwNewSize;

	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return;

	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];



	//定义与图像数据操作有关的变量
	unsigned char *pOldTemp, *pOldBits;
	int i, j,k;

    pOldTemp=pBits;
	//pNewTemp=pNew;
	
	int flag_right=0;
	int flag_left=0;
	int flag_up=0;
	int flag_down=0;

	for(i=nHeight;i>0;i--)
	{
		pOldBits=pOldTemp;
		//原图像数据指针定位到图像数据每行的中心位置
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes;
		//pNewBits=pNewTemp;
		//新图像数据指针定位到图像数据每行的中心位置
		//pNewBits =pNewBits + (i - 1 ) * nWidthBytes;
		
		for(j=0;j<nWidthBytes;j++)
		{
		//复制数据
			if(*pOldBits==255) 
			{
				for(k=1;k<20;k++)
				{
					if((pOldBits+k)<(pOldBits-j+nWidthBytes) && *(pOldBits+k)==0 ) flag_right=1;
					if((k<j) && (*(pOldBits-k)==0)) flag_left=1;
					if(((pOldBits+(k-1)*nWidthBytes)<pOldTemp+(nHeight-1)*nWidthBytes) && (*(pOldBits+(k-1)*nWidthBytes)==0)) flag_up=1;
					if((pOldBits-(k-1)*nWidthBytes)>pOldTemp && (*(pOldBits-(k-1)*nWidthBytes)==0) ) flag_down=1;
				
					if(flag_right==1 && flag_left==1 && flag_up==1 &&flag_down==1) 
					
					{
						*pOldBits=0;
						//*pNewBits=0;
					}
				}
			}

			flag_right=0;
			flag_left=0;
			flag_up=0;
			flag_down=0;

			//pNewBits++;
			pOldBits++;
		}
	}

	::GlobalUnlock(m_pDibObject->GetDib());
	return;
}
////////////////////////////////////////////////////////////////////////
//void CPretreatment::CountNumber()
//----------------------------------------------------------------------
//基本功能：统计个数及计算面积
//----------------------------------------------------------------------
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CPretreatment::CountNumber()
{
	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return;

	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	//定义变量
	unsigned char *pBuffer, *pBits,*pTemp;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return;


	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	//创建标记矩阵，用于存储标号
	int *label1=new int[nWidthBytes*nHeight];
	int *label2=new int[nWidthBytes*nHeight];
	//将标号矩阵清零
	memset(label1, 0, nWidthBytes*nHeight*sizeof(int));
	memset(label2, 0, nWidthBytes*nHeight*sizeof(int));

	//定义宏
	#define label1(ROW,COL) label1[nWidthBytes*ROW+COL]
	#define label2(ROW,COL) label2[nWidthBytes*ROW+COL]
	//CString str;
	//str.Format("%d",count);
	
	int count;
	int i,j,k;
	int left,above;
	pTemp=pBits;
	// 第一遍扫描，注意由上到下，由左到右的扫描顺序
	count=0;
	for(i = 0; i < nHeight; i++)
		for(j = 0; j < nWidthBytes; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			//lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
			pBits =pTemp + (nHeight-i) * nWidthBytes+j;
			/*if(i==1)
				for(k=0;k<cxDIB;k++)
					TRACE("%d ",label1(0,k));*/
			
			if(*pBits==255)//当前像素为白色，则标记为0
			 	label1(i,j)=0;  
			else//遇到黑色像素，则判断周围像素是否被标号的情况来决定是否给本像素标号
			{
				if(i==0)//对头行单独处理
				{
					if(label1(i,j-1)==0)//如果左边的像素未被标号								
						label1(i,j)=++count;
					else //否则，设置为跟左边像素相同的标号
						label1(i,j)=label1(i,j-1);
					continue;
				}
				if(j==0)//对头列单独处理
				{
					if(label1(i-1,j)==0)//如果上边的像素未被标号
						label1(i,j)=++count;
					else //否则，设置为跟上边像素相同的标号
						label1(i,j)=label1(i-1,j);
					continue;
				}

				//接下来处理非头行、头列的一般像素。对标号图像进行处理
				left=label1(i,(j-1));
				above=label1((i-1),j);
	
				//如果左边和上边都已经被标号了，但二者标号不同，则取上边的值，并将左边错标的重标
				if( left!=0 && above!=0 && left!=above)
				{
					label1(i,j)=above;
					//同时改变本行之前的、与该像素相邻接的错误标号
					for(k=j-1;k>=0;k--)
					{						
						if(label1(i,k)!=0)
							label1(i,k)=above;
						else   break;
					}
				}
				//如果左边和上边都已经被标号了，且二者标号相同，则任取上、左的标号作为当前像素的标号即可
				else if( left!=0 && above!=0 && left==above)
					label1(i,j)=left;
				
				else if( left!=0) //如果只是左边的被标号了，则取左边的标号
					label1(i,j)=left;
				
				else if( above!=0) //如果只是上边的被标号了，则取上边的标号
					label1(i,j)=above;
				
				else if(above==0 && left==0) //如果左边、上边都是白色区域，则创建一个新标号
					label1(i,j)=++count;
				else ;
			}	
		}
			
	//再一遍扫描，统计细菌数目，并统计每个细菌的面积
	int temp;
	const int NMAX=250;
	//int area[250];
	//area=new int[250];//area[0]不用
	area=new int[NMAX];//area[0]不用

	//从1开始，对每个标号的像素进行重新排列
	count=0;
	for(k=1;k<NMAX;k++)
	{
		temp=0;
		for(i = 0; i < nHeight; i++)
			for(j = 0; j < nWidthBytes; j++)
				if(label1(i,j)==k)  temp++;			
		if(temp>50)	 //面积小于50的当作毛刺和噪声
		{ 
			count++;
			area[count]=temp; 

			for(i = 0; i < nHeight; i++)
				for(j = 0; j < nWidthBytes; j++)
				 	if(label1(i,j)==k)
					 	label2(i,j)=count;				
		}

		 if(temp>1000)
		{
		count ++;
		area[count]=temp/2; 

		count ++;
		area[count]=temp/2; 
		 }
	}

	finalcount=count;//存储最终统计的细菌数目

	//输出细胞个数及面积
	CString str,str1;

	str.Format("共有细胞 %2d 个,\n其中 \n",count);
	for(int ii=1;ii<=finalcount;ii++)
	{
		
		str1.Format ("第 %2d 个细胞的面积为：%4d ",ii,area[ii]);
		if(ii % 3==0)
		{
			str1=str1+"\r\n";
		}
		str.Insert (str.GetLength(),str1);
		}
	AfxMessageBox(str,MB_ICONINFORMATION);

	//释放内存

	::GlobalUnlock( m_pDibObject->GetDib() );

	return;
}

