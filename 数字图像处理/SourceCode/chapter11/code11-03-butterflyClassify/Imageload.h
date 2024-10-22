// LoadPics.h


/////////////////////////////////// Generic /////////////////////////////////

#define IMAGETYPE_NONE 0
#define IMAGETYPE_BMP 1
#define IMAGETYPE_GIF 2
#define IMAGETYPE_PCX 3
#define IMAGETYPE_TGA 4
#define IMAGETYPE_JPG 5
#define IMAGETYPE_TIF 6

#define IMAGETYPE_FIRSTTYPE IMAGETYPE_BMP
#define IMAGETYPE_LASTTYPE IMAGETYPE_TIF

#ifdef __cplusplus
extern "C" {
#endif

// Error
int __declspec (dllexport) GetLastPicLibError( void );

// Type                              filename
int __declspec (dllexport) FileType( const char * );


///////////////////////////////////// BMP ///////////////////////////////////

// hDib                                 filename
HGLOBAL __declspec (dllexport) LoadBMP( const char * );

// Success                              filename      w      h      planes bits   colors
BOOL __declspec (dllexport) GetBMPInfo( const char *, int *, int *, int *, int *, int * );

// Success                           filename      hDib
BOOL __declspec (dllexport) SaveBMP( const char *, HGLOBAL );


///////////////////////////////////// GIF ///////////////////////////////////

// hDib                                 filename
HGLOBAL __declspec (dllexport) LoadGIF( const char * );

// Success                              filename      w      h      planes bits   colors
BOOL __declspec (dllexport) GetGIFInfo( const char *, int *, int *, int *, int *, int * );

// Success                           filename      hDib
BOOL __declspec (dllexport) SaveGIF( const char *, HGLOBAL );


///////////////////////////////////// PCX ///////////////////////////////////

// hDib                                 filename
HGLOBAL __declspec (dllexport) LoadPCX( const char * );

// Success                              filename      w      h      planes bits   colors
BOOL __declspec (dllexport) GetPCXInfo( const char *, int *, int *, int *, int *, int * );

// Success                           filename      hDib
BOOL __declspec (dllexport) SavePCX( const char *, HGLOBAL );


///////////////////////////////////// TGA ///////////////////////////////////

// hDib                                 filename
HGLOBAL __declspec (dllexport) LoadTGA( const char * );

// Success                              filename      w      h      planes bits   colors
BOOL __declspec (dllexport) GetTGAInfo( const char *, int *, int *, int *, int *, int * );

// Success                           filename      hDib
BOOL __declspec (dllexport) SaveTGA( const char *, HGLOBAL );


///////////////////////////////////// TIF ///////////////////////////////////

// hDib                                 filename
HGLOBAL __declspec (dllexport) LoadTIF( const char * );

// Success                              filename      w      h      planes bits   colors
BOOL __declspec (dllexport) GetTIFInfo( const char *, int *, int *, int *, int *, int * );

// Success                           filename      hDib
BOOL __declspec (dllexport) SaveTIF( const char *, HGLOBAL );


///////////////////////////////////// JPG ///////////////////////////////////

// hDib                                 filename
HGLOBAL __declspec (dllexport) LoadJPG( const char * );

// Success                              filename      w      h      planes bits   colors
BOOL __declspec (dllexport) GetJPGInfo( const char *, int *, int *, int *, int *, int * );

// Success                           filename      hDib
BOOL __declspec (dllexport) SaveJPG( const char *, HGLOBAL, int );


/////////////////////////////////// PALETTE //////////////////////////////////

// Count                               RGB History  Colormap          MaxColors
WORD __declspec (dllexport) MedianCut( WORD Hist[], BYTE ColMap[][3], int );

WORD __declspec (dllexport) Popularity( unsigned char *pBits, int nBits, int nWidth, int nHeight, BYTE ColorMap[][3] );

#ifdef __cplusplus
}
#endif

