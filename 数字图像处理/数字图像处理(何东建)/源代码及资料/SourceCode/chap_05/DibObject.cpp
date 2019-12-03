//DibObject.cpp

#include "stdafx.h"
#include "DibObject.h"

IMPLEMENT_DYNCREATE(CDibObject, CObject)

char *CDibObject::szExtensions[] = { ".BMP", ".GIF", ".PCX", ".TGA", ".JPG", ".TIF", "" };
        
//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CDibObject::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
	//now do the stuff for our specific class
	dc<<"File Name:"<<m_pszFilename<<"\n";
}
#endif

#ifdef _DEBUG
void CDibObject::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//check CDibObject members...
	ASSERT(m_pszFilename != NULL); //must exist
	ASSERT(m_hDib != NULL); //must exist
}
#endif

////////////////////////////////////////////////////////////////////////
//CDibObject类的构造函数		    
//----------------------------------------------------------------------
//基本功能：这是一个无参数的构造函数。它简单地创建一个CDibObject对象并
//			初始化其内部变量。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：无
////////////////////////////////////////////////////////////////////////
CDibObject::CDibObject()
{

	//调用辅助函数初始化其内部变量。
	InitVars();

}

////////////////////////////////////////////////////////////////////////
//CDibObject类的构造函数		    
//----------------------------------------------------------------------
//基本功能：用一个CDibObject对象通过复制操作来创建一个CDibObject对象。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject
//----------------------------------------------------------------------
//返 回 值：无
////////////////////////////////////////////////////////////////////////
CDibObject::CDibObject(CDibObject *pDibObject)
{
	InitVars();

	KillImage();
	
	m_nWidth = pDibObject->m_nWidth;
	m_nHeight = pDibObject->m_nHeight;
	m_nPlanes = pDibObject->m_nPlanes;
	m_nBits = pDibObject->m_nBits;
	m_nColors = pDibObject->m_nColors;
	m_nImageType = pDibObject->m_nImageType;
	m_nX = pDibObject->m_nX;
	m_nY = pDibObject->m_nY;
	m_nLastError = pDibObject->m_nLastError;
	m_nScreenPlanes = pDibObject->m_nScreenPlanes;
	m_nScreenBits = pDibObject->m_nScreenBits;
	m_nPaletteBytes = pDibObject->m_nPaletteBytes;
	m_nQuality = pDibObject->m_nQuality;
	m_nPaletteCreationType = pDibObject->m_nPaletteCreationType;
	
	int nNumColors = m_nColors;
	int nWidthBytes = WidthBytes( m_nBits, m_nWidth );
	
	if( pDibObject->m_hDib != NULL )
	{
		DWORD dwSize = ::GlobalSize( pDibObject->m_hDib );
		char *pData = (char *) ::GlobalLock( pDibObject->m_hDib );
		if( pData != NULL )
		{
			HGLOBAL hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
			if( hGlobal != NULL ){
				
				char *pDestData = (char *) ::GlobalLock( hGlobal );
				if( pDestData != NULL )
				{
					memcpy( pDestData, pData, dwSize );
					::GlobalUnlock( hGlobal );
					m_hDib = hGlobal;
				}
				else ::GlobalFree( hGlobal );
			}
			::GlobalUnlock( pDibObject->m_hDib );
		}
	}
}

////////////////////////////////////////////////////////////////////////
//CDibObject类的构造函数		    
//----------------------------------------------------------------------
//基本功能：构造一个CDibObject对象。唯一的要求的一个参数是文件名。如果
//			给出了CDC设备上下文参数，图像加载后会立即在该设备上下文中显示
//			出来。如果给出了nX或nY参数，图像会显示在该坐标指定的位置，否
//			则，图像总是显示在坐标为（0, 0）的位置。
//----------------------------------------------------------------------
//参数说明：const char *pszFilename
//			CDC *pDC，默认为NULL
//			int nX，默认为-1
//			int nY，默认为-1
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CDibObject::CDibObject( const char *pszFilename,
						   CDC *pDC, int nX, int nY )
{

	InitVars();
	Load( pszFilename, pDC, nX, nY );

}

////////////////////////////////////////////////////////////////////////
//CDibObject类的析构函数		    
//----------------------------------------------------------------------
//基本功能：析构CDibObject对象，删除该对象中的图像及相应的变量
//----------------------------------------------------------------------
//参数说明：无
////////////////////////////////////////////////////////////////////////
CDibObject::~CDibObject()
{

	if( m_hDib ) ::GlobalFree( m_hDib );
	if( m_pszFilename != NULL ) delete [] m_pszFilename;
	if( m_pLogPal != NULL ) delete [] m_pLogPal;

}

////////////////////////////////////////////////////////////////////////
//InitVars()函数		    
//----------------------------------------------------------------------
//基本功能：初始化CDibObject类内部变量
//----------------------------------------------------------------------
//参数说明：BOOL bFlag	标记类中是否有图像打开，默认值为FALSE
////////////////////////////////////////////////////////////////////////
void CDibObject::InitVars( BOOL bFlag )
{

	m_nWidth = m_nHeight = m_nBits = m_nColors = m_nImageType = 0;
	m_nX = m_nY = 0;
	m_nLastError = 0;
	m_hDib = NULL;
	m_nPaletteCreationType = FIXED_PALETTE;
	if( !bFlag )
	{
		m_pszFilename = NULL;
		m_pLogPal = NULL;
	}
	m_nQuality = 50;

}

////////////////////////////////////////////////////////////////////////
//BOOL Load()		    
//----------------------------------------------------------------------
//基本功能：本函数把一个图像文件载入CDibObject类。必需的一个参数是文件
//			名。如果给出了CDC设备上下文参数，图像一加载就被绘制。如果给
//			出了nX或nY参数，图像将显示在该坐标指定的位置。否则，图像总是
//			显示在坐标为（0,0）的位置。
//----------------------------------------------------------------------
//参数说明：const char *pszFilename
//			CDC *pDC，默认为NULL
//			int nX，默认为-1
//			int nY，默认为-1
//----------------------------------------------------------------------
//返 回 值：BOOL：成功返回TRUE，失败返回FALSE
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::Load( const char *pszFilename, 
						CDC *pDC, int nX, int nY )
{

	//获取图像文件类型
	m_nImageType = FileType( pszFilename );
	if( m_nImageType == 0 )
	{
		m_nLastError = IMAGELIB_UNSUPPORTED_FILETYPE;
		return( FALSE );
	}

	//删除已存在的图像
	KillImage();

	m_pszFilename = new char [strlen(pszFilename)+1];
	if( m_pszFilename != NULL ) strcpy( m_pszFilename, pszFilename );

	//根据文件类型调用ImageLoad.dll动态链接库中的相应函数打开图像文件
	switch( m_nImageType )
	{
	case IMAGETYPE_BMP:
		m_hDib = ::LoadBMP( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	case IMAGETYPE_GIF:
		m_hDib = ::LoadGIF( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	case IMAGETYPE_JPG:
		m_hDib = ::LoadJPG( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	case IMAGETYPE_PCX:
		m_hDib = ::LoadPCX( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	case IMAGETYPE_TGA:
		m_hDib = ::LoadTGA( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	case IMAGETYPE_TIF:
		m_hDib = ::LoadTIF( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	}

	//处理图像信息头
	ProcessImageHeader();
	//处理调色板
	ProcessPalette();

	//若传入了设备上下文指针，就在指定的设备上下文中绘制图像
	if( pDC != NULL ) Draw( pDC, nX, nY );

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//BOOL GetImageInfo()		    
//----------------------------------------------------------------------
//基本功能：本函数获取图像宽度、高度、位平面数、每个像素点的位数、以及颜
//			色数的信息。值为NULL的参数将不被传入。
//----------------------------------------------------------------------
//参数说明：const char *pszFilename
//			int *pnWidth，			默认为NULL
//			int *pnHeight，			默认为NULL
//			int *pnPlanes，			默认为NULL
//			int *pnBitsPerPixel，	默认为NULL
//			int *pnNumColors，		默认为NULL
//----------------------------------------------------------------------
//返 回 值：BOOL：成功返回TRUE，失败返回FALSE
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::GetImageInfo( const char *pszFilename, int *pnWidth,
	int *pnHeight, int *pnPlanes, int *pnBitsPerPixel, int *pnNumColors )
{
	int nImageType;

	//获取图像文件类型
	nImageType = FileType( pszFilename );
	if( nImageType == 0 )
	{
		m_nLastError = IMAGELIB_UNSUPPORTED_FILETYPE;
		return( FALSE );
	}
	
	//根据文件类型调用ImageLoad.dll动态链接库中的相应函数获取图像信息
	switch( nImageType )
	{
	case IMAGETYPE_BMP:
		return( ::GetBMPInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	case IMAGETYPE_GIF:
		return( ::GetGIFInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	case IMAGETYPE_JPG:
		return( ::GetJPGInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	case IMAGETYPE_PCX:
		return( ::GetPCXInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	case IMAGETYPE_TGA:
		return( ::GetTGAInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	case IMAGETYPE_TIF:
		return( ::GetTIFInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	}
	
	return( FALSE );

}

////////////////////////////////////////////////////////////////////////
//int GetLastError()		    
//----------------------------------------------------------------------
//基本功能：本函数返回所发生的最后一次CDibObject类错误。详细错误信息见
//			"ImageErrors.h"头文件。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：int nErrorNumber
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::GetLastError( void )
{

	return( m_nLastError );

}

////////////////////////////////////////////////////////////////////////
//BOOL Save()		    
//----------------------------------------------------------------------
//基本功能：本函数保存驻留于CDibObject对象中的当前图像（图像类型定义见
//			GetImageType()函数。
//----------------------------------------------------------------------
//参数说明：const char *pszFilename
//			int nType，默认为-1
//----------------------------------------------------------------------
//返 回 值：BOOL：成功返回TRUE，失败返回FALSE
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::Save( const char *pszFilename, int nType )
{

	//若没有指定文件类型，则根据文件名判断其文件类型
	if( nType == -1 ) nType = ExtensionIndex( pszFilename );

	if( nType < IMAGETYPE_FIRSTTYPE || nType > IMAGETYPE_LASTTYPE ) return( FALSE );
	m_nImageType = nType;
	delete [] m_pszFilename;
	m_pszFilename = new char [strlen(pszFilename)+1];
	if( m_pszFilename != NULL ) strcpy( m_pszFilename, pszFilename );

	//根据文件类型调用ImageLoad.dll动态链接库中的相应函数保存图像
	switch( m_nImageType )
	{
	case IMAGETYPE_BMP:
		return( ::SaveBMP( pszFilename, m_hDib ) );
		break;
	case IMAGETYPE_GIF:
		return( ::SaveGIF( pszFilename, m_hDib ) );
		break;
	case IMAGETYPE_JPG:
		return( ::SaveJPG( pszFilename, m_hDib, m_nQuality ) );
		break;
	case IMAGETYPE_PCX:
		return( ::SavePCX( pszFilename, m_hDib ) );
		break;
	case IMAGETYPE_TGA:
		return( ::SaveTGA( pszFilename, m_hDib ) );
		break;
	case IMAGETYPE_TIF:
		return( ::SaveTIF( pszFilename, m_hDib ) );
		break;
	}
	
	return( TRUE );
	
}

////////////////////////////////////////////////////////////////////////
//int ExtensionIndex()
//----------------------------------------------------------------------
//基本功能：本函数返回文件名指定的图像类型。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：int nWidth
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::ExtensionIndex( const char *pszFilename )
{
	int Index = 0;
	char *pszExtension;

	pszExtension = (char *) &pszFilename[strlen(pszFilename)-4];

	while( szExtensions[Index][0] )
	{
		if( !stricmp( pszExtension, szExtensions[Index] ) ) return( Index + 1 );
		Index++;
	}

	return( -1 );

}

////////////////////////////////////////////////////////////////////////
//int GetWidth()
//----------------------------------------------------------------------
//基本功能：本函数返回图像的像素宽度数。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：int nWidth
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::GetWidth( void )
{

	return( m_nWidth );

}

////////////////////////////////////////////////////////////////////////
//int GetHeight( void )
//----------------------------------------------------------------------
//基本功能：本函数返回图像的像素高度数。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：int nHeight
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::GetHeight( void )
{

	return( m_nHeight );

}

////////////////////////////////////////////////////////////////////////
//int GetNumBits( void )
//----------------------------------------------------------------------
//基本功能：本函数返回图像的像素数：1、4、8、16、24、32。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：int nBits
////////////////////////////////////////////////////////////////////////
int CDibObject::GetNumBits( void )
{

	return( m_nBits );

}

////////////////////////////////////////////////////////////////////////
//int GetNumColors( void )
//----------------------------------------------------------------------
//基本功能：本函数返回图像调色板中的颜色。图像为16位色或更高时返回值为0。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：int nColors
////////////////////////////////////////////////////////////////////////
int CDibObject::GetNumColors( void )
{

	return( m_nColors );

}

////////////////////////////////////////////////////////////////////////
//BOOL GetPaletteData()		    
//----------------------------------------------------------------------
//基本功能：本函数将当前RGB调色板数据填入到所传入的指针中。如果图像是16
//			位色或更高时则不进行任何操作。
//----------------------------------------------------------------------
//参数说明：RGBQUAD *pRGBPalette
//----------------------------------------------------------------------
//返 回 值：BOOL：成功返回TRUE，失败返回FALSE
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::GetPaletteData( RGBQUAD *pRGBPalette )
{

	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return( FALSE );

	char *pTemp;
	pTemp = (char *) ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL ) return( FALSE );

	memcpy( pRGBPalette, &pTemp[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)],
								m_nPaletteBytes );

	m_nLastError = IMAGELIB_SUCCESS;
	::GlobalUnlock( m_hDib );

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//RGBQUAD GetPaletteData()		    
//----------------------------------------------------------------------
//基本功能：本函数返回一个指向CDibObject调色板数据序列的指针。如果图像
//			是16位色或更高时则返回NULL。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：返回一个指向CDibObject调色板数据序列的指针
////////////////////////////////////////////////////////////////////////
RGBQUAD *CDibObject::GetPaletteData( void )
{
	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return( NULL );

	m_nLastError = IMAGELIB_NO_PALETTE_FOR_HIGH_COLOR;
	if( m_nColors <= 0 || m_nColors > 256 ) return( NULL );
	
	RGBQUAD *pRGBPalette;
	pRGBPalette = new RGBQUAD [m_nColors];
	if( pRGBPalette == NULL )
	{
		m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		return( NULL );
	}
	
	char *pTemp;
	pTemp = (char *) ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL )
	{
		delete [] pRGBPalette;
		return( NULL );
	}
	
	memcpy( pRGBPalette, &pTemp[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], 
								m_nColors * sizeof( RGBQUAD ) );
	
	m_nLastError = IMAGELIB_SUCCESS;
	::GlobalUnlock( m_hDib );
	
	return( pRGBPalette );
	
}

////////////////////////////////////////////////////////////////////////
//int GetImageType()		    
//----------------------------------------------------------------------
//基本功能：本函数返回图像的类型。图像类型定义如下：
//			#define IMAGETYPE_NONE		0
//			#define IMAGETYPE_BMP		1
//			#define IMAGETYPE_GIF		2
//			#define IMAGETYPE_PCX		3
//			#define IMAGETYPE_TGA		4
//			#define IMAGETYPE_JPG		5
//			#define IMAGETYPE_TIF		6
//----------------------------------------------------------------------
//参数说明：const char *pFilename
//----------------------------------------------------------------------
//返 回 值：int nImageType
////////////////////////////////////////////////////////////////////////
int CDibObject::GetImageType( const char *pFilename )
{

	return( ::FileType( pFilename ) );

}

////////////////////////////////////////////////////////////////////////
//int GetImageType()		    
//----------------------------------------------------------------------
//基本功能：本函数返回图像的类型。图像类型定义如下：
//			#define IMAGETYPE_NONE		0
//			#define IMAGETYPE_BMP		1
//			#define IMAGETYPE_GIF		2
//			#define IMAGETYPE_PCX		3
//			#define IMAGETYPE_TGA		4
//			#define IMAGETYPE_JPG		5
//			#define IMAGETYPE_TIF		6
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：int nImageType
////////////////////////////////////////////////////////////////////////
int CDibObject::GetImageType( void )
{

	return( m_nImageType );

}

////////////////////////////////////////////////////////////////////////
//BOOL Draw( CDC *pDC, int nX, int nY )		    
//----------------------------------------------------------------------
//基本功能：本函数是在设备描述表上绘制图像。如果X和Y坐标没有给出，图像会
//			被画在(0,0)坐标或上次传入的有效坐标处。
//----------------------------------------------------------------------
//参数说明：CDC *pDC
//			int nX，默认为-1
//			int nY，默认为-1
//----------------------------------------------------------------------
//返 回 值：BOOL：成功返回TRUE，失败返回FALSE
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::Draw( CDC *pDC, int nX, int nY )
{
	if( nX != -1 ) m_nX = nX;
	if( nY != -1 ) m_nY = nY;

	m_nLastError = IMAGELIB_HDIB_NULL;

	//没有打开图像
	if( m_hDib == NULL ) return( FALSE );

	char *pTemp;
	//锁定图像对象句柄
	pTemp = (char *) ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL ) return( NULL );

	//图像信息头
	BITMAPINFOHEADER *pBIH;
	pBIH = (BITMAPINFOHEADER *) &pTemp[sizeof(BITMAPFILEHEADER)];

	int nRet = ::StretchDIBits( pDC->m_hDC, m_nX, m_nY, m_nWidth, m_nHeight, 0, 0, 
								m_nWidth, m_nHeight, 
								(const void FAR *) &pTemp[sizeof(BITMAPFILEHEADER)+
														  sizeof(BITMAPINFOHEADER)+
														  m_nPaletteBytes], 
								(BITMAPINFO *) pBIH, DIB_RGB_COLORS, SRCCOPY );

	::GlobalUnlock( m_hDib );
	m_nLastError = IMAGELIB_STRETCHDIBITS_ERROR;
	if( nRet != m_nHeight ) return( FALSE );
	m_nLastError = IMAGELIB_SUCCESS;

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//BOOL CreateLogPalette()		    
//----------------------------------------------------------------------
//基本功能：本函数根据指定的调色板指针及颜色数创建一个逻辑调色板。
//----------------------------------------------------------------------
//参数说明：CDC *pDC
//			int nX，默认为-1
//			int nY，默认为-1
//----------------------------------------------------------------------
//返 回 值：创建的逻辑调色板指针
////////////////////////////////////////////////////////////////////////
LOGPALETTE *CDibObject::CreateLogPalette( RGBQUAD *pPalette, 
										   int nNumColors )
{
	LOGPALETTE *pLogPal;
	int i;

	if( pPalette == NULL ) return( NULL );

	pLogPal = (LOGPALETTE *) new char [sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY)];
	if( pLogPal == NULL ) return( NULL );

	//指定调色板版本号
	pLogPal->palVersion = 0x300;
	//指定调色板颜色数
	pLogPal->palNumEntries = (unsigned short) nNumColors;
	
	//复制调色板各颜色项
	for( i=0; i<nNumColors; i++ )
	{
		pLogPal->palPalEntry[i].peRed = pPalette[i].rgbRed;
		pLogPal->palPalEntry[i].peGreen = pPalette[i].rgbGreen;
		pLogPal->palPalEntry[i].peBlue = pPalette[i].rgbBlue;
		pLogPal->palPalEntry[i].peFlags = 0;
	}

	return( pLogPal );

}

////////////////////////////////////////////////////////////////////////
//void ProcessImageHeader()		    
//----------------------------------------------------------------------
//基本功能：本函数处理图像信息头各项内容。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：无
////////////////////////////////////////////////////////////////////////
void CDibObject::ProcessImageHeader( void )
{

	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return;

	char *pTemp;
	BITMAPINFOHEADER *pBIH;
	pTemp = (char *) ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL ) return;
	
	//得到图像信息头指针
	pBIH = (BITMAPINFOHEADER *) &pTemp[sizeof(BITMAPFILEHEADER)];
	m_nWidth = pBIH->biWidth;
	m_nHeight = pBIH->biHeight;
	m_nPlanes = pBIH->biPlanes;
	m_nBits = pBIH->biBitCount;
	m_nColors = 1 << m_nBits;
	if( m_nPlanes > 1 ) m_nColors <<= ( m_nPlanes - 1 );
	if( m_nBits >= 16 ) m_nColors = 0;

		
	int nWidthBytes = WidthBytes(m_nBits, m_nWidth);
	pBIH->biSizeImage = nWidthBytes * m_nHeight;

	::GlobalUnlock( m_hDib );

	m_nLastError = IMAGELIB_SUCCESS;

}

////////////////////////////////////////////////////////////////////////
//void ProcessPalette()		    
//----------------------------------------------------------------------
//基本功能：处理体调色板。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：无
////////////////////////////////////////////////////////////////////////
void CDibObject::ProcessPalette( void )
{

	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return;

	CWindowDC WindowDC( NULL );
	m_nScreenPlanes = WindowDC.GetDeviceCaps( PLANES );
	m_nScreenBits = WindowDC.GetDeviceCaps( BITSPIXEL );

	m_nPaletteBytes = 0;
	m_Palette.DeleteObject();

	if( m_nBits <= 8 ) m_nPaletteBytes = m_nColors * sizeof( RGBQUAD );
	if( m_nScreenBits >= 16 ) return;

	char *pTemp;
	pTemp = (char *) ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL ) return;

	if( m_nBits <= 8 ){
		RGBQUAD *pRGBPalette;
		pRGBPalette = (RGBQUAD *) &pTemp[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
		LOGPALETTE *pPalette;
		pPalette = CreateLogPalette( pRGBPalette, m_nColors );
		if( pPalette == NULL ){
			m_nLastError = IMAGELIB_LOGICAL_PALETTE_CREATION_ERROR;
			goto ProcessPaletteExit;
			}
		m_Palette.CreatePalette( pPalette );
		delete [] pPalette;
		}

	m_nLastError = IMAGELIB_SUCCESS;

ProcessPaletteExit:
	::GlobalUnlock( m_hDib );

}

////////////////////////////////////////////////////////////////////////
//void KillImage()		    
//----------------------------------------------------------------------
//基本功能：清除当前CDibObject类中的图像对象。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：无
////////////////////////////////////////////////////////////////////////
void CDibObject::KillImage( void )
{

	if( m_hDib ) ::GlobalFree( m_hDib );
	m_hDib = NULL;

	if( m_pLogPal != NULL ) delete [] m_pLogPal;
	m_pLogPal = NULL;

	if( m_pszFilename != NULL ) delete [] m_pszFilename;
	m_pszFilename = NULL;

	m_Palette.DeleteObject();

}

////////////////////////////////////////////////////////////////////////
//BOOL SetPalette( CDC *pDC )		    
//----------------------------------------------------------------------
//基本功能：本函数将该设备描述表的调色板分配给相应的图像。当一个图像被载
//			入时，它的调色板信息便被存入CDibObject对象中。
//----------------------------------------------------------------------
//参数说明：CDC *pDC
//----------------------------------------------------------------------
//返 回 值：BOOL：成功返回TRUE，失败返回FALSE
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::SetPalette( CDC *pDC )
{

	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return( FALSE );

	pDC->SelectPalette( &m_Palette, FALSE );
	pDC->RealizePalette();

	m_nLastError = IMAGELIB_SUCCESS;

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//BOOL IsLoaded( void )		    
//----------------------------------------------------------------------
//基本功能：本函数判断是否已经载入了一幅图像
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：BOOL：成功返回TRUE，失败返回FALSE
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::IsLoaded( void )
{

	return( m_hDib != NULL );

}

////////////////////////////////////////////////////////////////////////
//void GetDIBPointer()		    
//----------------------------------------------------------------------
//基本功能：得到图像DIB数据指针。
//----------------------------------------------------------------------
//参数说明：int *nWidthBytes	图像的字节宽度
//			int nNewBits		新设置的图像颜色位数
//			int *nNewWidthBytes	图像颜色位数改变后的字节宽度
//			int nNewWidth		新设置的图像宽度（像素）
//----------------------------------------------------------------------
//返 回 值：无
////////////////////////////////////////////////////////////////////////
void *CDibObject::GetDIBPointer( int *nWidthBytes,
								 int nNewBits, 
								 int *nNewWidthBytes,
								 int nNewWidth )
{

	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return( NULL );

	void *pTemp;
	pTemp = ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL ) return( NULL );

	if( nWidthBytes != NULL ) *nWidthBytes = WidthBytes( m_nBits, m_nWidth );

	if( nNewWidthBytes != NULL )
	{
		if( nNewWidth == -1 ) nNewWidth = m_nWidth;
		*nNewWidthBytes = WidthBytes( nNewBits, nNewWidth );
	}

	return( pTemp );

}

////////////////////////////////////////////////////////////////////////
//int WidthBytes()		    
//----------------------------------------------------------------------
//基本功能：本函数根据传入的像素位数及图像宽度（像素）计算其字节宽度。
//----------------------------------------------------------------------
//参数说明：int nBits	每像素位数
//			int nWidth	图像的宽度
//----------------------------------------------------------------------
//返 回 值：int	图像的字节宽度
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::WidthBytes( int nBits, int nWidth )
{
	int nWidthBytes;

	nWidthBytes = nWidth;
	if( nBits == 1 ) nWidthBytes = ( nWidth + 7 ) / 8;
	else if( nBits == 4 ) nWidthBytes = ( nWidth + 1 ) / 2;
	else if( nBits == 16 ) nWidthBytes = nWidth * 2;
	else if( nBits == 24 ) nWidthBytes = nWidth * 3;
	else if( nBits == 32 ) nWidthBytes = nWidth * 4;

	//一个数的后两位为零，则该数为4的整数倍
	while( ( nWidthBytes & 3 ) != 0 ) nWidthBytes++;

	return( nWidthBytes );

}


////////////////////////////////////////////////////////////////////////
//void NormalizeCoordinates()		    
//----------------------------------------------------------------------
//基本功能：规格化处理的图像区域。
//----------------------------------------------------------------------
//参数说明：int *nX1				处理区域左上角X坐标
//			int *nY1				处理区域左上角Y坐标
//			int *nX2				处理区域右下角X坐标
//			int *nY2				处理区域右下角Y坐标
//			BOOL *bCompleteImage	处理区域是否为全部区域
//			BOOL *bLessThanHalf		处理区域是否为一半区域
//----------------------------------------------------------------------
//返 回 值：无
////////////////////////////////////////////////////////////////////////
void CDibObject::NormalizeCoordinates( int *nX1, int *nY1, int *nX2, int *nY2, 
									  BOOL *bCompleteImage, BOOL *bLessThanHalf )
{

	if( *nX1 == -1 ) *nX1 = 0;
	if( *nY1 == -1 ) *nY1 = 0;
	if( *nX2 == -1 ) *nX2 = m_nWidth - 1;
	if( *nY2 == -1 ) *nY2 = m_nHeight - 1;

	if( *nX1 > *nX2 ){
		int nTemp;
		nTemp = *nX1;
		*nX1 = *nX2;
		*nX2 = nTemp;		
		}

	if( *nY1 > *nY2 ){
		int nTemp;
		nTemp = *nY1;
		*nY1 = *nY2;
		*nY2 = nTemp;		
		}

	if( *nX1 < 0 ) *nX1 = 0;
	if( *nX2 > m_nWidth - 1 ) *nX2 = m_nWidth - 1;

	if( *nX2 < 0 ) *nX2 = 0;
	if( *nX2 > m_nWidth - 1 ) *nX2 = m_nWidth - 1;

	if( *nY1 < 0 ) *nY1 = 0;
	if( *nY1 > m_nHeight - 1 ) *nY1 = m_nHeight - 1;

	if( *nY2 < 0 ) *nY2 = 0;
	if( *nY2 > m_nHeight - 1 ) *nY2 = m_nHeight - 1;

	if( bCompleteImage != NULL ) *bCompleteImage = TRUE;
	if( bLessThanHalf != NULL ) *bLessThanHalf = FALSE;
	if( *nX1 > 0 || *nY1 > 0 || *nX2 < m_nWidth - 1 || *nY2 < m_nHeight - 1 ){
		if( bCompleteImage != NULL ) *bCompleteImage = FALSE;
		DWORD dwTotalPixels, dwThesePixels;
		dwTotalPixels = (DWORD) m_nWidth * m_nHeight;
		dwThesePixels = (DWORD) ( (*nX2) - (*nX1) + 1 ) * (DWORD) ( (*nY2) - (*nY1) + 1 );
		if( bLessThanHalf != NULL && dwThesePixels <= dwTotalPixels / 2 ) *bLessThanHalf = TRUE;
		}

}

////////////////////////////////////////////////////////////////////////
//void operator=()		    
//----------------------------------------------------------------------
//基本功能：“=”操作符重载。
//----------------------------------------------------------------------
//参数说明：CDibObject &DibObject	另一个CDibObject图像对象。
//----------------------------------------------------------------------
//返 回 值：无
////////////////////////////////////////////////////////////////////////
void CDibObject::operator= (const CDibObject &DibObject)
{

	KillImage();

	m_nWidth = DibObject.m_nWidth;
	m_nHeight = DibObject.m_nHeight;
	m_nPlanes = DibObject.m_nPlanes;
	m_nBits = DibObject.m_nBits;
	m_nColors = DibObject.m_nColors;
	m_nImageType = DibObject.m_nImageType;
	m_nX = DibObject.m_nX;
	m_nY = DibObject.m_nY;
	m_nLastError = DibObject.m_nLastError;
	m_nScreenPlanes = DibObject.m_nScreenPlanes;
	m_nScreenBits = DibObject.m_nScreenBits;
	m_nPaletteBytes = DibObject.m_nPaletteBytes;
	m_nQuality = DibObject.m_nQuality;
	m_nPaletteCreationType = DibObject.m_nPaletteCreationType;

	int nNumColors = m_nColors;
	int nWidthBytes = WidthBytes( m_nBits, m_nWidth );

	if( DibObject.m_hDib != NULL ){

		DWORD dwSize = ::GlobalSize( DibObject.m_hDib );

		char *pData = (char *) ::GlobalLock( DibObject.m_hDib );
		if( pData != NULL ){

			HGLOBAL hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
			if( hGlobal != NULL ){

				char *pDestData = (char *) ::GlobalLock( hGlobal );
				if( pDestData != NULL ){

					memcpy( pDestData, pData, dwSize );

					if( nNumColors != 0 ) CreatePaletteFromDIB( (RGBQUAD *) &pData[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], nNumColors );

					else if( DibObject.m_pLogPal != NULL ){
						m_pLogPal = (LOGPALETTE *) new char [sizeof(LOGPALETTE)+DibObject.m_pLogPal->palNumEntries*sizeof(PALETTEENTRY)];
						if( m_pLogPal != NULL ){
							for( int i=0; i<DibObject.m_pLogPal->palNumEntries; i++ ) m_pLogPal[i] = DibObject.m_pLogPal[i];
							m_Palette.CreatePalette( m_pLogPal );
							}
						}

					::GlobalUnlock( hGlobal );
					m_hDib = hGlobal;
					}
				else ::GlobalFree( hGlobal );
				}
			::GlobalUnlock( DibObject.m_hDib );
			}
		}

	if( DibObject.m_pszFilename != NULL ){
		m_pszFilename = new char [strlen(DibObject.m_pszFilename)+1];
		strcpy( m_pszFilename, DibObject.m_pszFilename );
		}
}

////////////////////////////////////////////////////////////////////////
//void CreatePaletteFromDIB()		    
//----------------------------------------------------------------------
//基本功能：从当前CDibObject位图对象中创建调色板。
//----------------------------------------------------------------------
//参数说明：RGBQUAD *pRGBPalette	调色板指针
//			int nNumColors			图像颜色数
//----------------------------------------------------------------------
//返 回 值：无
////////////////////////////////////////////////////////////////////////
void CDibObject::CreatePaletteFromDIB( RGBQUAD *pRGBPalette, int nNumColors )
{

	if( pRGBPalette != NULL ){
		LOGPALETTE *pPalette;
		pPalette = CreateLogPalette( pRGBPalette, nNumColors );
		if( pPalette != NULL ){
			m_Palette.CreatePalette( pPalette );
			delete [] pPalette;
			}
		else m_nLastError = IMAGELIB_LOGICAL_PALETTE_CREATION_ERROR;
		}

}

////////////////////////////////////////////////////////////////////////
//int GetNearestIndex()		    
//----------------------------------------------------------------------
//基本功能：获取相近颜色索引值。
//----------------------------------------------------------------------
//参数说明：unsigned char ucRed		红色分量
//			unsigned char ucGreen	绿色分量
//			unsigned char ucBlue	蓝色分量
//			RGBQUAD *pRGBPalette	调色板指针
//			int nNumColors			颜色数
//----------------------------------------------------------------------
//返 回 值：int	相近颜色索引值
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::GetNearestIndex( unsigned char ucRed,
								 unsigned char ucGreen, 
								 unsigned char ucBlue, 
								 RGBQUAD *pRGBPalette, 
								 int nNumColors )
{
	int i, Index = 0;
	DWORD NewDiff, Diff = 100000L;
	DWORD Red, Green, Blue;
	
	for( i=0; i<nNumColors; i++ )
	{
		if( ucRed > pRGBPalette[i].rgbRed ) Red = (DWORD) ( pRGBPalette[i].rgbRed - ucRed );
		else Red = (DWORD) ( ucRed - pRGBPalette[i].rgbRed );
		if( ucGreen > pRGBPalette[i].rgbGreen ) Green = (DWORD) ( pRGBPalette[i].rgbGreen - ucGreen );
		else Green = (DWORD) ( ucGreen - pRGBPalette[i].rgbGreen );
		if( ucBlue > pRGBPalette[i].rgbBlue ) Blue = (DWORD) ( pRGBPalette[i].rgbBlue - ucBlue );
		else Blue = (DWORD) ( ucBlue - pRGBPalette[i].rgbBlue );
		NewDiff = ( Red * Red ) + ( Green * Green ) + ( Blue * Blue );
		if( NewDiff < Diff )
		{
			if( NewDiff <= 1 ) return( i );
			Diff = NewDiff;
			Index = i;
		}
	}
	
	return( Index );

}

////////////////////////////////////////////////////////////////////////
//BOOL ChangeFormat( int nNewBitsPerPixel )		    
//----------------------------------------------------------------------
//基本功能：本函数将图像从当前分辨率转化为指定分辨率。
//----------------------------------------------------------------------
//参数说明：int nNewBitsPerPixel
//----------------------------------------------------------------------
//返 回 值：BOOL：成功返回TRUE，失败返回FALSE
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::ChangeFormat( int nNewBitsPerPixel )
{

	m_nLastError = IMAGELIB_SUCCESS;
	if( nNewBitsPerPixel == m_nBits ) return( TRUE );

	int nOldWidthBytes, nNewWidthBytes;
	char *pBuffer = (char *) GetDIBPointer( &nOldWidthBytes, nNewBitsPerPixel, &nNewWidthBytes );
	if( pBuffer == NULL ) return( FALSE );

	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	RGBQUAD *pOldRGBPalette, *pNewRGBPalette;
	unsigned char *pOldBits, *pNewBits;
	int nNumColors, nNumNewColors;

	pOldBFH = (BITMAPFILEHEADER *) pBuffer;
	pOldBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	pOldRGBPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	nNumColors = m_nColors;
	nNumNewColors = 1 << nNewBitsPerPixel;
	if( nNewBitsPerPixel > 8 ) nNumNewColors = 0;
	pOldBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	if( m_nBits >= 16 && nNewBitsPerPixel < 16 ){
		if( m_pLogPal != NULL ) delete [] m_pLogPal;
		m_pLogPal = CreatePaletteFromBitmap( nNumNewColors, pOldBits, m_nBits, m_nWidth, m_nHeight );
		}

	HGLOBAL hGlobal;
	DWORD dwSize;
	dwSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNumNewColors * sizeof( RGBQUAD ) + m_nHeight * nNewWidthBytes;
	hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
	if( hGlobal == NULL ){
		::GlobalUnlock( m_hDib );
		return( FALSE );
		}

	pBuffer = (char *) ::GlobalLock( hGlobal );
	if( pBuffer == NULL ){
		::GlobalFree( hGlobal );
		::GlobalUnlock( m_hDib );
		return( FALSE );
		}

	pNewBFH = (BITMAPFILEHEADER *) pBuffer;
	pNewBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	pNewRGBPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	*pNewBFH = *pOldBFH;
	*pNewBIH = *pOldBIH;
	int i, j = nNumNewColors;

	if( m_nBits < 16 && nNewBitsPerPixel < 16 ){
		for( i=0; i<j; i++ ) pNewRGBPalette[i] = pOldRGBPalette[i];
		}
	else if( m_nBits >= 16 ){
 		for( i=0; i<j; i++ ){
			pNewRGBPalette[i].rgbRed = m_pLogPal->palPalEntry[i].peRed;
			pNewRGBPalette[i].rgbGreen = m_pLogPal->palPalEntry[i].peGreen;
			pNewRGBPalette[i].rgbBlue = m_pLogPal->palPalEntry[i].peBlue;
			}
		}
	pNewBIH->biBitCount = nNewBitsPerPixel;
	pNewBIH->biSizeImage = nNewWidthBytes * m_nHeight;
	pNewBIH->biClrUsed = nNumNewColors;
	pNewBFH->bfSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNumNewColors * sizeof( RGBQUAD ) + pNewBIH->biSizeImage;
	pNewBFH->bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNumNewColors * sizeof( RGBQUAD );
	pNewBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumNewColors*sizeof(RGBQUAD)];
	m_nPaletteBytes = nNumNewColors * sizeof( RGBQUAD );

	for( int y=0; y<m_nHeight; y++ ){
		unsigned char ucRed, ucGreen, ucBlue;
		unsigned char *pSrc, *pDest;
		pSrc = pOldBits;
		pSrc += ( nOldWidthBytes * y );
		pDest = pNewBits;
		pDest += ( nNewWidthBytes * y );
		for( int x=0; x<m_nWidth; x++ ){
			switch( m_nBits ){
				case 1:
					if( pSrc[x/8] & ( 0x80 >> ( x & 7 ) ) )
						ucRed = ucGreen = ucBlue = 0xff;
					else
						ucRed = ucGreen = ucBlue = 0x00;
					break;
				case 4:
					if( !( x & 1 ) ){
						ucRed = pOldRGBPalette[pSrc[x/2]>>4].rgbRed;
						ucGreen = pOldRGBPalette[pSrc[x/2]>>4].rgbGreen;
						ucBlue = pOldRGBPalette[pSrc[x/2]>>4].rgbBlue;
						}
					else{
						ucRed = pOldRGBPalette[pSrc[x/2]&15].rgbRed;
						ucGreen = pOldRGBPalette[pSrc[x/2]&15].rgbGreen;
						ucBlue = pOldRGBPalette[pSrc[x/2]&15].rgbBlue;
						}
					break;
				case 8:
					ucRed = pOldRGBPalette[pSrc[x]].rgbRed;
					ucGreen = pOldRGBPalette[pSrc[x]].rgbGreen;
					ucBlue = pOldRGBPalette[pSrc[x]].rgbBlue;
					break;
				case 16:
					GETRGB555( ucRed, ucGreen, ucBlue, &pSrc[x*2] );
					break;
				case 24:
					ucRed = pSrc[x*3+2];
					ucGreen = pSrc[x*3+1];
					ucBlue = pSrc[x*3];
					break;
				case 32:
					GETRGB888( ucRed, ucGreen, ucBlue, &pSrc[x*4] );
					break;
				}

			switch( nNewBitsPerPixel ){
				case 1:
					if( !( x & 7 ) ) pDest[x/8] = 0;
					pDest[x/8] |= (unsigned char) GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNumNewColors ) << ( x & 7 );
					break;
				case 4:
					if( !( x & 1 ) )
						pDest[x/2] = (unsigned char) GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNumNewColors ) << 4;
					else
						pDest[x/2] |= (unsigned char) GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNumNewColors );
					break;
				case 8:
					pDest[x] = (unsigned char) GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNumNewColors );
					break;
				case 16:
					PUTRGB555( ucRed, ucGreen, ucBlue, &pDest[x*2] );
					break;
				case 24:
					pDest[x*3+2] = ucRed;
					pDest[x*3+1] = ucGreen;
					pDest[x*3] = ucBlue;
					break;
				case 32:
					PUTRGB888( ucRed, ucGreen, ucBlue, &pDest[x*4] );
					break;
				}
			}
		}

	::GlobalUnlock( m_hDib );
	::GlobalFree( m_hDib );
	::GlobalUnlock( hGlobal );
	m_hDib = hGlobal;

	ProcessImageHeader();

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//LOGPALETTE CreatePaletteFromBitmap()		    
//----------------------------------------------------------------------
//基本功能：从当前CDibObject位图对象中创建逻辑调色板。
//----------------------------------------------------------------------
//参数说明：int nNumColors			颜色数
//			unsigned char *pBits	位图数据指针
//			int nBits				颜色位数
//			int nWidth				图像宽度
//			int nHeight				图像高度
//----------------------------------------------------------------------
//返 回 值：LOGPALETTE	逻辑调色板指针
////////////////////////////////////////////////////////////////////////
LOGPALETTE *CDibObject::CreatePaletteFromBitmap(int nNumColors, 
												unsigned char *pBits, int nBits, 
												int nWidth, int nHeight )
{

	RGBQUAD *pRGBPalette;
	if( nBits != 8 && m_nPaletteCreationType == POPULARITY_PALETTE ) pRGBPalette = MakePopularityPalette( nNumColors, pBits, nBits, nWidth, nHeight );
	else if( nBits != 8 && m_nPaletteCreationType == MEDIAN_CUT_PALETTE ) pRGBPalette = MakeMedianCutPalette( nNumColors, pBits, nBits, nWidth, nHeight );
	else if( m_nPaletteCreationType == FIXED_PALETTE ) pRGBPalette = MakeFixedPalette( nNumColors );
	if( pRGBPalette == NULL ) return( NULL );

	LOGPALETTE *pLogPal = CreateLogPalette( pRGBPalette, nNumColors );

	delete [] pRGBPalette;

	return( pLogPal );

}

////////////////////////////////////////////////////////////////////////
//void SetPaletteCreationType( int nType )		    
//----------------------------------------------------------------------
//基本功能：当希望通过颜色还原得到一幅具有调色板的图像时，本函数负责设置
//			调色板的创建类型。可以设置如下：
//			#define POPULARITY_PALETTE	0
//			#define MEDIAN_CUT_PALETTE	1
//			#define FIXED_PALETTE		2
//----------------------------------------------------------------------
//参数说明：int nType
//----------------------------------------------------------------------
//返 回 值：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CDibObject::SetPaletteCreationType( int nType )
{

	m_nPaletteCreationType = nType;

}

////////////////////////////////////////////////////////////////////////
//int GetPaletteCreationType( void )		    
//----------------------------------------------------------------------
//基本功能：本函数返回用于下一次颜色还原操作的调色板类型。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：int nPaletteCreationType
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::GetPaletteCreationType( void )
{

	return( m_nPaletteCreationType );

}

RGBQUAD *CDibObject::MakePopularityPalette( int nNumColors, unsigned char *pBits, int nBits, int nWidth, int nHeight )
{
	RGBQUAD *pRGBQuad = new RGBQUAD[nNumColors];
	if( pRGBQuad == NULL ) return( MakeFixedPalette( nNumColors ) );
	memset( pRGBQuad, 0, nNumColors * sizeof( RGBQUAD ) );

	BYTE ColMap[256][3];
	if( !Popularity( pBits, nBits, nWidth, nHeight, ColMap ) ){
		delete [] pRGBQuad;
		return( MakeFixedPalette( nNumColors ) );
		}

	for( int i=0; i<nNumColors; i++ ){
		pRGBQuad[i].rgbRed = ColMap[i][0];
		pRGBQuad[i].rgbGreen = ColMap[i][1];
		pRGBQuad[i].rgbBlue = ColMap[i][2];
		}

	return( pRGBQuad );

}

RGBQUAD *CDibObject::MakeMedianCutPalette( int nNumColors, unsigned char *pBits, int nBits, int nWidth, int nHeight )
{
	RGBQUAD *pRGBQuad = new RGBQUAD[nNumColors];
	if( pRGBQuad == NULL ) return( MakeFixedPalette( nNumColors ) );
	memset( pRGBQuad, 0, nNumColors * sizeof( RGBQUAD ) );

	BYTE ColMap[256][3];
	WORD *Hist = new WORD[32768];
	if( Hist == NULL ){
		delete [] pRGBQuad;
		return( MakeFixedPalette( nNumColors ) );
		}
	memset( Hist, 0, 32768 * sizeof( WORD ) );

	int nWidthBytes = WidthBytes( nBits, nWidth );

	for( int y=0; y<nHeight; y++ ){
		unsigned char *pData = pBits;
		unsigned char ucRed, ucGreen, ucBlue;
		WORD color;
		pData += ( y * nWidthBytes );
		for( int x=0; x<nWidth; x++ ){
			switch( nBits ){
				case 16:
					GETRGB555( ucRed, ucGreen, ucBlue, &pData[x*2] );
					break;
				case 24:
					ucRed = pData[x*3+2];
					ucGreen = pData[x*3+1];
					ucBlue = pData[x*3];
					break;
				case 32:
					GETRGB888( ucRed, ucGreen, ucBlue, &pData[x*4] );
					break;
				}
			color = _RGB( ucRed, ucGreen, ucBlue );
			if( Hist[color] < 65535 ) Hist[color]++;
			}
		}

	MedianCut( Hist, ColMap, (int) 256 );
	for( int i=0; i<nNumColors; i++ ){
		pRGBQuad[i].rgbRed = ColMap[i][0];
		pRGBQuad[i].rgbGreen = ColMap[i][1];
		pRGBQuad[i].rgbBlue = ColMap[i][2];
		}
	delete [] Hist;

	return( pRGBQuad );

}

RGBQUAD *CDibObject::MakeFixedPalette( int nNumColors )
{
	RGBQUAD *pRGBQuad = new RGBQUAD[nNumColors];
	if( pRGBQuad == NULL ) return( NULL );

	static int colors[] = {
		255, 255, 255,
		0, 0, 0,
		255, 0, 0,
		0, 0, 255,
		0, 255, 0,
		150, 150, 150,
		255, 255, 0,
		0, 150, 150,
		150, 0, 150,
		150, 150, 0,
		0, 255, 255,
		255, 0, 255,
		255, 120, 120,
		120, 255, 120,
		120, 120, 255,
		90, 90, 90 };

	int nSteps = ( ( nNumColors + 15 ) / 16 );

	for( int i=0; i<nSteps; i++ ){
		for( int j=0; j<16; j++ ){
			if( i * 16 + j < nNumColors ){
				int r, g, b;
				r = colors[j*3];
				g = colors[j*3+1];
				b = colors[j*3+2];
				r = ( ( nSteps - i ) * r ) / nSteps;
				g = ( ( nSteps - i ) * g ) / nSteps;
				b = ( ( nSteps - i ) * b ) / nSteps;
				pRGBQuad[i*16+j].rgbRed = (unsigned char) r;
				pRGBQuad[i*16+j].rgbGreen = (unsigned char) g;
				pRGBQuad[i*16+j].rgbBlue = (unsigned char) b;
				}
			}
		}

	return( pRGBQuad );

}

//得到调色板字节数
int CDibObject::GetPaletteBytes( void )
{

	return( m_nPaletteBytes );

}

////////////////////////////////////////////////////////////////////////
//HGLOBALBOOL GetDib( void )	    
//----------------------------------------------------------------------
//基本功能：本函数返回当前载入的图像Dib句柄。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：HGLOBALBOOL hDib
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
HGLOBAL CDibObject::GetDib( void )
{

	return( m_hDib );

}

////////////////////////////////////////////////////////////////////////
//CPalette GetPalette( void )	    
//----------------------------------------------------------------------
//基本功能：本函数返回一个指向CPalette对象的指针。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返 回 值：CPalette *Palette
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CPalette *CDibObject::GetPalette( void )
{

	return( &m_Palette );

}

//得到逻辑调色板
LOGPALETTE *CDibObject::GetLogPal( void )
{

	return( m_pLogPal );

}

void CDibObject::ProcImgHead()
{
	ProcessImageHeader();
}

char * CDibObject::GetImageName()
{
	return( m_pszFilename );
}
