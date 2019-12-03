// MorphProc.cpp: implementation of the CMorphPro class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MorphProc.h"

IMPLEMENT_DYNCREATE(CMorphPro, CObject)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CMorphPro::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CMorphPro::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif

/************************************************************************
*                                                                       *
*                           数学形态学处理类                            *
*                                                                       *
************************************************************************/

////////////////////////////////////////////////////////////////////////
//构造函数CMorphPro()    
//----------------------------------------------------------------------
//基本功能：构造一个CMorphPro类的对象，如不传入CDibObject对象。第一次调   
//			用某一个处理函数时必须给出一个CDibObject对象指针。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CMorphPro::CMorphPro()
{
}

////////////////////////////////////////////////////////////////////////
//构造函数CMorphPro()    
//----------------------------------------------------------------------
//基本功能：构造一个CMorphPro类的对象并传入CDibObject对象。所有的操作都
//			针对该对象，直到另一个对象作为参数被传给图像处理函数。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CMorphPro::CMorphPro(CDibObject *pDibObject)
{
	m_pDibObject = pDibObject;
}

//析构函数
CMorphPro::~CMorphPro()
{

}


/************************************************************************
*                                                                       *
*            以下函数完成图象区膨胀、腐蚀、开、闭等运算操作                 *
*	                                                                    *
************************************************************************/

////////////////////////////////////////////////////////////////////////
//void MoveBuffer()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的两幅图像数据进行移动操作
//----------------------------------------------------------------------
//参数说明：unsigned char *pOut		指向输出图像数据的指针
//			unsigned char *pIn		指向输入图像数据的指针
//			LONG lSize				图像数据的大小（nWidthBytes*nHeight）
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CMorphPro类中使用。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::MoveBuffer(unsigned char *pOut, unsigned char *pIn, 
							LONG lSize)
{
	if(pOut == NULL || pIn == NULL)
	{
		AfxMessageBox("传入的数据绶冲区指针为空！");
		return;
	}

	//用pIn中的数据填充pOut
	memcpy(pOut, pIn, lSize);
	//将pIn中的数据置0
	memset(pIn, 0x00, lSize);
}
////////////////////////////////////////////////////////////////////////
//unsigned char GetBitsPoint()   
//----------------------------------------------------------------------
//基本功能：本函数获取传入的pDibObject指定的CMorphPro类的图像对象的数
//			据区的指针。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：unsigned char
//          返回图像数据区的指针。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CMorphPro类中使用。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char *CMorphPro::GetBitsPoint()
{
	//图像指针为空
	if(m_pDibObject == NULL) return(NULL);
		
	//定义变量
	unsigned char *pBuffer, *pBits; 
	int nWidthBytes;
	int nNumColors = m_pDibObject->GetNumColors();

	//原图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
											m_pDibObject->GetNumBits() );
	if( pBuffer == NULL )
	{
		return(NULL);
	}

	//图像数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+
								sizeof(BITMAPINFOHEADER)+
								nNumColors*sizeof(RGBQUAD)];
	//返回图像数据指针
	return( pBits );
}

////////////////////////////////////////////////////////////////////////
//BOOL MakeErosion()   
//----------------------------------------------------------------------
//基本功能：本函数对图像数据执行腐蚀操作。
//----------------------------------------------------------------------
//参数说明：int		*nMask			结构元素数组指针
//			int		 nMaskLen		结构元素长度（以点数为计数单位）
//			unsigned char *pOut		输出图像数据指针
//			unsigned char *pIn		输入图像数据指针
//			int		 nWidthBytes	图像宽度（以字节表示）
//			int		 nWidth			图像宽度（以像素表示）
//			int		 nHeight		图像高度
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CMorphPro类中使用。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::MakeErosion(int *nMask, int nMaskLen, 
 					unsigned char *pOut, unsigned char *pIn,
					int nWidthBytes, int nWidth, int nHeight)
{
	//若传入的图像数据为空，将无法完成操作，直接返回。
	if(pOut == NULL || pIn == NULL) return FALSE;
	
	//定义变量
	int x, y, k;
	unsigned char Mark;

	//执行腐蚀操作
	for( y = 0; y < nHeight; y++ )
	{
		unsigned char *pOutTemp = pOut;
		pOutTemp +=  y * nWidthBytes;
		for( x = 0; x < nWidth; x++ )
		{
			Mark = 0;
			for (k = 0; k < nMaskLen; k++)
			{
				//不能处理边界像素
				if ((x + nMask[2 * k] >= 0 ) &&
					(x + nMask[2 * k] < nWidth) &&
					(y + nMask[2 * k + 1] >= 0) &&
					(y + nMask[2 * k + 1] < nHeight))
				{
					unsigned char Data;
					//取与模板中位置相对应的像素值
					unsigned char *pTemp = pIn;
					pTemp +=  y * nWidthBytes;
					pTemp += nMask[2 * k + 1] * nWidthBytes;
					Data = pTemp[x + nMask[2 * k]];
					if(Data == 255)
					{ 
						Mark = 1;
						k = nMaskLen;
					}
				}
			}
			if (Mark == 1)	pOutTemp[x] = 255;
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL MakeDilation()   
//----------------------------------------------------------------------
//基本功能：本函数对图像数据执行膨胀操作。
//----------------------------------------------------------------------
//参数说明：int		*nMask			结构元素数组指针
//			int		 nMaskLen		结构元素长度（以点数为计数单位）
//			unsigned char *pOut		输出图像数据指针
//			unsigned char *pIn		输入图像数据指针
//			int		 nWidthBytes	图像宽度（以字节表示）
//			int		 nWidth			图像宽度（以像素表示）
//			int		 nHeight		图像高度
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CMorphPro类中使用。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::MakeDilation(int *nMask, int nMaskLen, 
 					unsigned char *pOut, unsigned char *pIn,
					int nWidthBytes, int nWidth, int nHeight)
{
	//若传入的图像数据为空，将无法完成操作，直接返回。
	if(pOut == NULL || pIn == NULL) return FALSE;
	
	//定义变量
	int x, y, k;
	unsigned char Mark;

	for( y = 0; y < nHeight; y++ )
	{
		unsigned char *pOutTemp = pOut;
		pOutTemp +=  y *nWidthBytes;
		for( x = 0; x < nWidth; x++ )
		{
			Mark = 1;
			for (k = 0; k < nMaskLen; k++)
			{
				//不能处理边界像素
				if ((x + nMask[2 * k] >= 0 ) &&
					(x + nMask[2 * k] < nWidth) &&
					(y + nMask[2 * k + 1] >= 0) &&
					(y + nMask[2 * k + 1] < nHeight))
				{
					unsigned char Data;
					//取模板中的像素值
					unsigned char *pTemp = pIn;
					pTemp +=  y * nWidthBytes;
					pTemp += nMask[2 * k + 1] * nWidthBytes;
					Data = pTemp[x + nMask[2 * k]];
					if(Data != 255)
					{ 
						Mark = 0;
						k = nMaskLen;
					}
				}
				else
				{
					Mark=1;
					k=nMaskLen;
				}
			}
			if (Mark == 1)	pOutTemp[x] = 255;
		}
	}
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL Erosion()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行腐蚀运算。
//----------------------------------------------------------------------
//参数说明：int			 *nMask			结构元素数组指针
//			int			 nMaskLen		结构元素长度（以点数为计数单位）
//			CDibObject *pDibObject	输出图像数据指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Erosion(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//使用传入的CDibObject对象
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	
	//不是8位灰度图像
	int nNumBits = m_pDibObject->GetNumBits(); 
	if( nNumBits != 8 ) return( FALSE );

	//获得图像宽度和高度及字节宽度
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	int nWidthBytes = m_pDibObject->WidthBytes(8, nWidth);

	//图像数据区大小
	DWORD dwSize = nWidthBytes * nHeight;
	
	//获得图像数据区指针
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//为新图像分配内存
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );

	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像数据指针
	unsigned char *pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	
    if (pNewBuffer == NULL || pOldBuffer == NULL)	
	{
		AfxMessageBox("数据绶冲区定位出错！");
		return (FALSE);
	}

	//将原图像数据移动到新图像中（原图像数据清零）
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);

	//调用MakeErosion()保护型函数进行腐蚀操作
	MakeErosion(nMask, nMaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);
	
	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL Dilation()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行膨胀运算。
//----------------------------------------------------------------------
//参数说明：int			 *nMask			结构元素数组指针
//			int			 nMaskLen		结构元素长度（以点数为计数单位）
//			CDibObject *pDibObject	输出图像数据指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Dilation(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//使用传入的CDibObject对象
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	
	//不是8位灰度图像
	int nNumBits = m_pDibObject->GetNumBits(); 
	if( nNumBits != 8 ) return( FALSE );

	//获得图像宽度和高度
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	int nWidthBytes = m_pDibObject->WidthBytes(8, nWidth);

	//图像数据区大小
	DWORD dwSize = nWidthBytes * nHeight;
	
	//获得图像数据区指针
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//为新图像分配内存
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );

	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//新图像数据指针
	unsigned char *pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	
    if (pNewBuffer == NULL || pOldBuffer == NULL)	
	{
		AfxMessageBox("数据绶冲区定位出错！");
		return (FALSE);
	}

	//将原图像数据移动到新图像中（原图像数据清零）
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);

	//调用MakeDilation()保护型函数进行腐蚀操作
	MakeDilation(nMask, nMaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);
	
	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}
////////////////////////////////////////////////////////////////////////
//BOOL Opening()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行开运算。
//----------------------------------------------------------------------
//参数说明：int			 *nMask			结构元素数组指针
//			int			 nMaskLen		结构元素长度（以点数为计数单位）
//			CDibObject *pDibObject	输出图像数据指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Opening(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//对传入的CDibObject对象进行腐蚀
	if(Erosion(nMask, nMaskLen, pDibObject) != TRUE) return( FALSE );

	//检查结构元素是否对称
	int nSumX = 0, nSumY = 0;
	int i;
	for(i = 0; i < nMaskLen; i++)
	{
		nSumX += nMask[2 * i];
		nSumY += nMask[2 * i + 1];
	}

	//CString s;
	//s.Format("nSumX=%d, nSumY=%d",nSumX, nSumY);
	//AfxMessageBox(s);

	if(nSumX != 0 || nSumY != 0)
	{
		for(i = 0; i < nMaskLen; i++)
		{
			nMask[2 * i] = -nMask[2 * i];
			nMask[2 * i + 1] = -nMask[2 * i + 1];
		}
	}
	
	//对传入的CDibObject对象进行膨胀
	if(Dilation(nMask, nMaskLen, pDibObject) != TRUE) return( FALSE );
	
	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL Closing()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行闭运算。
//----------------------------------------------------------------------
//参数说明：int			 *nMask			结构元素数组指针
//			int			 nMaskLen		结构元素长度（以点数为计数单位）
//			CDibObject *pDibObject	输出图像数据指针
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Closing(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//对传入的CDibObject对象进行膨胀
	if(Dilation(nMask, nMaskLen, pDibObject) != TRUE) return( FALSE );

	//检查结构元素是否对称
	int nSumX = 0, nSumY = 0;
	int i;
	for(i = 0; i < nMaskLen; i++)
	{
		nSumX += nMask[2 * i];
		nSumY += nMask[2 * i + 1];
	}

	if(nSumX != 0 || nSumY != 0)
	{
		for(i = 0; i < nMaskLen; i++)
		{
			nMask[2 * i] = -nMask[2 * i];
			nMask[2 * i + 1] = -nMask[2 * i + 1];
		}
	}

	//对传入的CDibObject对象进行腐蚀
	if(Erosion(nMask, nMaskLen, pDibObject) != TRUE) return( FALSE );
	
	return TRUE;
}

