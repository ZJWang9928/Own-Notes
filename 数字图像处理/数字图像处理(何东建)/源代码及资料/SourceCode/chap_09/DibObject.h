//DibObject.h

#ifndef __DibObject_H__
#define __DibObject_H__

#include "ImageErrors.h"
#include "ImageLoad.h"

#define POPULARITY_PALETTE 0
#define MEDIAN_CUT_PALETTE 1
#define FIXED_PALETTE 2

#define GETRGB555( a, b, c, d ) { WORD *wData = (WORD *) d; a = (unsigned char) ( ( (*wData) & 0x7c00 ) >> 7 ); b = (unsigned char) ( ( (*wData) & 0x03e0 ) >> 2 ); c = (unsigned char) ( ( (*wData) & 0x001f ) << 3 ); }
#define GETRGB565( a, b, c, d ) { WORD *wData = (WORD *) d; a = (unsigned char) ( ( (*wData) & 0xf800 ) >> 8 ); b = (unsigned char) ( ( (*wData) & 0x07e0 ) >> 3 ); c = (unsigned char) ( ( (*wData) & 0x001f ) << 3 ); }
#define GETRGB888( a, b, c, d ) { DWORD *dwData = (DWORD *) d; a = (unsigned char) ( (*dwData) >> 16 ); b = (unsigned char) ( ( (*dwData) & 0x0000ff00 ) >> 8 ); c = (unsigned char) ( (*dwData) & 0x000000ff ); }
#define PUTRGB555( a, b, c, d ) { WORD *wData = (WORD *) d; *wData = ( ( ( (WORD) a & 0x00f8 ) << 7 ) | ( ( (WORD) b & 0x00f8 ) << 2 ) | ( (WORD) c >> 3 ) ); }
#define PUTRGB565( a, b, c, d ) { WORD *wData = (WORD *) d; *wData = ( ( ( (WORD) a & 0x00f8 ) << 8 ) | ( ( (WORD) b & 0x00fc ) << 3 ) | ( (WORD) c >> 3 ) ); }
#define PUTRGB888( a, b, c, d ) { DWORD *dwData = (DWORD *) d; *dwData = ( (DWORD) a << 16 ) | ( (DWORD) b << 8 ) | (DWORD) c; }

#define _RGB(r,g,b) (WORD)(((b)&~7)<<7)|(((g)&~7)<<2)|((r)>>3)

class CDibObject : public CObject
{
	DECLARE_DYNCREATE(CDibObject)
public:
	//无参构造函数
	CDibObject();
	//拷贝构造函数
	CDibObject(CDibObject* pDibObject);
	//有参构造函数
	CDibObject( const char *, CDC *pDC = NULL, int nX = -1, int nY = -1 );
	//析构函数
	~CDibObject();

	//等号操作符重载
	void operator= (const CDibObject &DibObject);
	
	//读入图像
	BOOL Load( const char *, CDC *pDC = NULL, int nX = -1, int nY = -1 );
	
	//得到最近发生的错误码
	int GetLastError( void );
	//保存图像
	BOOL Save( const char *, int nType = -1 );
	//得到宽度（像素单位）
	int GetWidth( void );
	//得到高度（像素单位）
	int GetHeight( void );
	//得到每像素颜色位数
	int GetNumBits( void );
	//得到颜色数
	int GetNumColors( void );
	//得到调色板
	BOOL GetPaletteData( RGBQUAD * );
	RGBQUAD *GetPaletteData( void );
	//得到文件类型
	int GetImageType( const char * );
	int GetImageType( void );
	//得到图像信息
	BOOL GetImageInfo( const char *, int *pnWidth = NULL,
					   int *pnHeight = NULL, int *pnPlanes = NULL,
					   int *pnBitsPerPixel = NULL, int *pnNumColors = NULL );
	//绘制图像
	BOOL Draw( CDC *, int x = -1, int y = -1 );
	//设置调色板
	BOOL SetPalette( CDC * );
	//设置调色板类型
	void SetPaletteCreationType( int );
	//得到调色板类型
	int GetPaletteCreationType( void );

	//图像是否被载入
	BOOL IsLoaded( void );
	//扩展名索引
	int ExtensionIndex( const char * );

	//改变图像格式
	BOOL ChangeFormat( int );

	//得到调色板字节数
	int GetPaletteBytes( void );
	//得到位图句柄
	HGLOBAL GetDib( void );
	//得到调色板指针
	CPalette *GetPalette( void );

	//对输入的坐标进行调整，看是否超出图像大小
	void NormalizeCoordinates( int *, int *, int *, int *, BOOL *bCompleteImage = NULL, BOOL *bLessThanHalf = NULL );
	//得到DIB位图内存指针
	void *GetDIBPointer( int *nWidthBytes = NULL, int nNewBits = 0, int *nNewWidthBytes = NULL, int nNewWidth = -1 );
	//得到逻辑调色板
	LOGPALETTE *GetLogPal( void );
	//创建逻辑调色板
	LOGPALETTE *CreateLogPalette( RGBQUAD *Palette, int NumColors );
	//得到最近颜色索引
	int GetNearestIndex( unsigned char, unsigned char, unsigned char, RGBQUAD *, int );

	//设置质量
	void SetQuality( int nQual ){ m_nQuality = nQual; }
	//设置宽度
	void SetWidth( int nWidth ){ m_nWidth = nWidth; }
	//设置高度
	void SetHeight( int nHeight ){ m_nHeight = nHeight; }
	//设置平面数
	void SetPlanes( int nPlanes ){ m_nPlanes = nPlanes; }
	//设置平面数
	void SetScreenPlanes( int nScreenPlanes ){ m_nScreenPlanes = nScreenPlanes; }
	//设置颜色位数
	void SetBits( int nBits ){ m_nBits = nBits; }
	//设置颜色位数
	void SetScreenBits( int nScreenBits ){ m_nScreenBits = nScreenBits; }
	//设置颜色数
	void SetColors( int nColors ){ m_nColors = nColors; }
	//设置图像类型
	void SetImageType( int nImageType ){ m_nImageType = nImageType; }
	//设置最后错误类型
	void SetLastError( int nLastError ){ m_nLastError = nLastError; }
	//设置最后错误类型
	void SetPaletteBytes( int nPaletteBytes ){ m_nPaletteBytes = nPaletteBytes; }
	//设置m_nX
	void SetX( int nX ){ m_nX = nX; }
	//设置m_nY
	void SetY( int nY ){ m_nY = nY; }
	//获取m_nPlans值
	int GetPlanes( ){ return m_nPlanes; }
	//获取m_nScreenPlans值
	int GetScreenPlanes( ){ return m_nScreenPlanes; }
	//获取m_nScreenBits值
	int GetScreenBits( ){ return m_nScreenBits; }
	//获取m_nQuality值
	int GetQuality( ){ return m_nQuality; }
	//获取m_nColors值
	int GetColors( ){ return m_nColors; }
	//获取m_nX值
	int GetX( ){ return m_nX; }
	//获取m_nY值
	int GetY( ){ return m_nY; }

	//处理调色板
	void ProcessPalette( void );
	//字节宽度
	int WidthBytes( int, int );
	//设置位图
	void SetDib( HGLOBAL hDib ){ m_hDib = hDib; }
	//设置逻辑调色板
	void SetLogPal( LOGPALETTE *pLogPal ){ m_pLogPal = pLogPal; }
	//清除图像
	void KillImage( void );
	//处理图像头
	void ProcessImageHeader( void );

	static char *szExtensions[7];			//扩展名
	int m_nLastError;						//最近错误
	int m_nScreenPlanes, m_nScreenBits, m_nPaletteBytes;			

//诊断调试函数
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

protected:
	//初始化参数
	void InitVars( BOOL bFlag = FALSE );
	//从位图创建调色板
	void CreatePaletteFromDIB( RGBQUAD *, int );
	LOGPALETTE *CreatePaletteFromBitmap( int, unsigned char *, int, int, int );

	RGBQUAD *MakePopularityPalette( int, unsigned char *, int, int, int );
	RGBQUAD *MakeMedianCutPalette( int, unsigned char *, int, int, int );
	RGBQUAD *MakeFixedPalette( int );


	int m_nWidth, m_nHeight, m_nPlanes, m_nBits, m_nColors, m_nImageType;
	int m_nX, m_nY;
	int m_nQuality;
	int m_nPaletteCreationType;
	CPalette m_Palette;
	HGLOBAL m_hDib;
public:
	char * GetImageName();
	void ProcImgHead(void);
	char *m_pszFilename;
	LOGPALETTE *m_pLogPal;


};

#endif

