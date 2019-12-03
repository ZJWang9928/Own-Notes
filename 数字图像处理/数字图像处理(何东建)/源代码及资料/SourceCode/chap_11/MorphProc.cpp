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
*注意：在数学形态学类中，为处理方便要求的CDibObject图像仍为256色灰度图*
*	   像不过在处理中只用到其调色板中的0和255两个值。                   *
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

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//基本功能：本函数为CMorphPro类对象指定一个CDibObject对象指针
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：无。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::SetDibObjectClass(CDibObject *pDibObject)
{
	m_pDibObject = pDibObject;
}

/************************************************************************
*                                                                       *
*            以下函数完成数据区反相、与、或、异或等操作                 *
*	                                                                    *
************************************************************************/

////////////////////////////////////////////////////////////////////////
//BOOL IsResultBlank()   
//----------------------------------------------------------------------
//基本功能：本函数判断图像数据是否全为零
//----------------------------------------------------------------------
//参数说明：unsigned char *pIn		指向图像数据的指针
//			LONG lSize				图像数据的大小（nWidthBytes*nHeight）
//----------------------------------------------------------------------
//返    回：BOOL
//			不为零时返回TRUE，为零时返回FALSE。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CMorphPro类中使用。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::IsResultBlank(unsigned char *pIn, LONG lSize)
{
	BOOL bMark = TRUE;
	LONG i;
	for(i = 0; i < lSize; i++)
	{
		if(*(pIn + i) != 0x00)
		{
			i = lSize;
			bMark = FALSE;
		}
	}
	return(bMark);
}

////////////////////////////////////////////////////////////////////////
//void ReverseBuffer()   
//----------------------------------------------------------------------
//基本功能：本函数判断图像数据进行反相操作
//----------------------------------------------------------------------
//参数说明：unsigned char *pIn		指向图像数据的指针
//			LONG lSize				图像数据的大小（nWidthBytes*nHeight）
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CMorphPro类中使用。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::ReverseBuffer(unsigned char *pIn, LONG lSize)
{
	LONG i;
	BYTE  Buffer;
	for(i = 0;i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//进行反相运算
		*(pIn + i) = (BYTE)(255 - Buffer);
	}
}

////////////////////////////////////////////////////////////////////////
//void AndBuffer()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的两幅图像数据进行与操作
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
void CMorphPro::AndBuffer(unsigned char *pOut, unsigned char *pIn,
						   LONG lSize)
{
	LONG i;
	BYTE Buffer;
	for(i = 0; i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//执行与操作
		*(pOut + i) &= Buffer;
	}
}

////////////////////////////////////////////////////////////////////////
//void OrBuffer()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的两幅图像数据进行或操作
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
void CMorphPro::OrBuffer(unsigned char *pOut, unsigned char *pIn, 
						  LONG lSize)
{
	LONG i;
	BYTE Buffer;
	for(i = 0; i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//执行或操作
		*(pOut + i) |= Buffer;
	}
}

////////////////////////////////////////////////////////////////////////
//void XorBuffer()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的两幅图像数据进行异或操作
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
void CMorphPro::XorBuffer(unsigned char *pOut, unsigned char *pIn, 
						   LONG lSize)
{
	LONG i;
	BYTE Buffer;
	for(i = 0; i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//执行异或操作
		*(pOut + i) ^= Buffer;
	}
}

////////////////////////////////////////////////////////////////////////
//void CopyBuffer()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的两幅图像数据进行复制操作
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
void CMorphPro::CopyBuffer(unsigned char *pOut, unsigned char *pIn,
							LONG lSize)
{
	if(pOut == NULL || pIn == NULL)
	{
		AfxMessageBox("传入的数据绶冲区指针为空！");
		return;
	}
	//用pIn中的数据填充pOut
	memcpy(pOut, pIn, lSize);
}

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
//void CleanBuffer()   
//----------------------------------------------------------------------
//基本功能：本函数对传入的两幅图像数据进行清除操作
//----------------------------------------------------------------------
//参数说明：unsigned char *pIn		指向图像数据的指针
//			LONG lSize				图像数据的大小（nWidthBytes*nHeight）
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CMorphPro类中使用。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::CleanBuffer(unsigned char *pIn, LONG lSize)
{
	if(pIn == NULL)
	{
		AfxMessageBox("传入的数据绶冲区指针为空！");
		return;
	}

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
					//取与模板中位置相对应的像素值
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
					Mark = 0;
					k = nMaskLen;
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
					//取模板中的像素值
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

////////////////////////////////////////////////////////////////////////
//BOOL Closing()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行击中击不中变换。
//----------------------------------------------------------------------
//参数说明：int			 *nHmask		击中结构元素数组指针
//			int			 nHmaskLen		击中结构元素长度（点数）
//			int			 *nMmask		击不中结构元素数组指针
//			int			 nMmaskLen		击不中结构元素长度（点数）
//			CDibObject *pDibObject	默认为NULL。
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::HitMissTran(int *nHmask, int nHmaskLen,
							int *nMmask, int nMmaskLen,
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

	//图像数据区大小（字节）
	DWORD dwSize = nWidthBytes * nHeight;
	
	//图像数据区指针
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//为新图像分配内存
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 2 * dwSize );

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

	//复制图像数据
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);

	//击中变换
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeErosion(nHmask, nHmaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//击不中变换
	ReverseBuffer(pNewBuffer + dwSize, (LONG)dwSize);
	MoveBuffer(pNewBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	MakeErosion(nMmask, nMmaskLen, pNewBuffer + dwSize, pNewBuffer, nWidthBytes, 
		        nWidth, nHeight);

	//求交集
	AndBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL Thining()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行薄化运算。
//----------------------------------------------------------------------
//参数说明：int			 *nHmask		击中结构元素数组指针
//			int			 nHmaskLen		击中结构元素长度（点数）
//			int			 *nMmask		击不中结构元素数组指针
//			int			 nMmaskLen		击不中结构元素长度（点数）
//			CDibObject *pDibObject	默认为NULL。
//			BOOL		 *bIsBlank		结果是否为全黑。
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Thining(int *nHmask, int nHmaskLen,
							int *nMmask, int nMmaskLen,
							CDibObject *pDibObject,
							BOOL *bIsBlank)
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

	//图像数据区大小（字节）
	DWORD dwSize = nWidthBytes * nHeight;
	
	//图像数据区指针
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//为新图像分配内存
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 3 * dwSize );

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

	//复制图像数据
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);
	CopyBuffer(pNewBuffer + 2 * dwSize, pOldBuffer, (LONG)dwSize);

	//击中变换
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeErosion(nHmask, nHmaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//击不中变换
	ReverseBuffer(pNewBuffer + dwSize, (LONG)dwSize);
	MoveBuffer(pNewBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	MakeErosion(nMmask, nMmaskLen, 	pNewBuffer + dwSize, pNewBuffer, nWidthBytes, 
				nWidth, nHeight);

	//求交集
	AndBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//用原图像减去击中击不中后的图像
	XorBuffer(pOldBuffer, pNewBuffer + 2 * dwSize, (LONG)dwSize);

	XorBuffer(pNewBuffer + 2 * dwSize, pOldBuffer, (LONG)dwSize);

	*bIsBlank = IsResultBlank(pNewBuffer + 2 * dwSize, (LONG)dwSize);

	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL Thicking()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行厚化运算。
//----------------------------------------------------------------------
//参数说明：int			 *nHmask		击中结构元素数组指针
//			int			 nHmaskLen		击中结构元素长度（点数）
//			int			 *nMmask		击不中结构元素数组指针
//			int			 nMmaskLen		击不中结构元素长度（点数）
//			CDibObject *pDibObject	默认为NULL。
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Thicking(int *nHmask, int nHmaskLen,
							int *nMmask, int nMmaskLen,
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
	
	//图像数据区指针
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//为新图像分配内存
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 3 * dwSize );

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

	//复制图像数据
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);
	CopyBuffer(pNewBuffer + 2 * dwSize, pOldBuffer, (LONG)dwSize);

	//击中变换
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeErosion(nHmask, nHmaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//击不中变换
	ReverseBuffer(pNewBuffer + dwSize, (LONG)dwSize);
	MoveBuffer(pNewBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	MakeErosion(nMmask, nMmaskLen, pNewBuffer + dwSize, pNewBuffer, nWidthBytes, 
				nWidth, nHeight);

	//求交集
	AndBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//用原图像并上击中击不中变换后的图像
	OrBuffer(pOldBuffer, pNewBuffer + 2 * dwSize, (LONG)dwSize);
	
	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL OutEdge()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行外边界检测。
//----------------------------------------------------------------------
//参数说明：int			 *nmask			击中结构元素数组指针
//			int			 nmaskLen		击中结构元素长度（点数）
//			CDibObject *pDibObject	默认为NULL。
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::OutEdge(int *nMask, int nMaskLen,
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

	//图像数据区大小（字节）
	DWORD dwSize = nWidthBytes * nHeight;
	
	//图像数据区指针
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//为新图像分配内存
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 2 * dwSize );

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

	//复制图像数据
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);

	//膨胀
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeDilation(nMask, nMaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//用膨胀后的结果减原图
	XorBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL InEdge()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行内边界检测。
//----------------------------------------------------------------------
//参数说明：int			 *nmask			击中结构元素数组指针
//			int			 nmaskLen		击中结构元素长度（点数）
//			CDibObject *pDibObject	默认为NULL。
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::InEdge(int *nMask, int nMaskLen,
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

	//图像数据区大小（字节）
	DWORD dwSize = nWidthBytes * nHeight;
	
	//图像数据区指针
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//为新图像分配内存
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 2 * dwSize );

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

	//复制图像数据
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);

	//腐蚀
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeErosion(nMask, nMaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//用原图减腐蚀后的结果
	XorBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL MorphGrads()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行形态学梯度运算。
//----------------------------------------------------------------------
//参数说明：int			 *nMask		击中结构元素数组指针
//			int			 nMaskLen		击中结构元素长度（点数）
//			CDibObject *pDibObject	默认为NULL。
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::MorphGrads(int *nMask, int nMaskLen,
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

	//图像数据区大小（字节）
	DWORD dwSize = nWidthBytes * nHeight;
	
	//图像数据区指针
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//为新图像分配内存
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 2 * dwSize );

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

	//复制图像数据
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);

	//膨胀
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeDilation(nMask, nMaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//腐蚀
	MoveBuffer(pNewBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	MakeErosion(nMask, nMaskLen, pNewBuffer + dwSize, pNewBuffer, nWidthBytes, 
				nWidth, nHeight);

	//用膨胀后的结果减腐蚀后的结果
	XorBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//将内存解锁和将不再使用的内存释放
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL ThiningDIB()   
//----------------------------------------------------------------------
//基本功能：本函数对CDibObject对象中的图象进行细化运算。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	默认为NULL。
//----------------------------------------------------------------------
//返    回：BOOL
//			成功返回TRUE，失败返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::ThiningDIB(CDibObject *pDibObject)
{
	//使用传入的CDibObject对象
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//无CDibObject对象, 返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );


	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;
	int lWidth,lHeight;
	
	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	lWidth=m_pDibObject->GetWidth();
	lHeight=m_pDibObject->GetHeight();

	/////////////////////////////////////////////////////////
		// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//脏标记
	BOOL bModified;

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//四个条件
	BOOL bCondition1;
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;

	//计数器
	unsigned char nCount;
	
	//像素值
	unsigned char pixel;

	//5×5相邻区域像素值
	unsigned char neighbour[5][5];

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	bModified=TRUE;

	while(bModified)
	{

		bModified = FALSE;
		// 初始化新分配的内存，设定初始值为255
		lpDst = (char *)lpNewDIBBits;
		memset(lpDst, (BYTE)255, lWidth * lHeight);

		for(j = 2; j <lHeight-2; j++)
		{
			for(i = 2;i <lWidth-2; i++)
			{

				bCondition1 = FALSE;
				bCondition2 = FALSE;
				bCondition3 = FALSE;
				bCondition4 = FALSE;

				//由于使用5×5的结构元素，为防止越界，所以不处理外围的几行和几列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char *)pBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if(pixel != 255 && *lpSrc != 0)
					//return FALSE;
					continue;
				//如果源图像中当前点为白色，则跳过
				else if(pixel == 255)
					continue;

				//获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
				for (m = 0;m < 5;m++ )
				{
					for (n = 0;n < 5;n++)
					{
						neighbour[m][n] =(255 - (unsigned char)*(lpSrc + ((4 - m) - 2)*lWidth + n - 2 )) / 255;
					}
				}
				//逐个判断条件。
				//判断2<=NZ(P1)<=6
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
						+ neighbour[2][1] + neighbour[2][3] + \
						+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if ( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;

				//判断Z0(P1)=1
				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					bCondition2 = TRUE;

				//判断P2*P4*P8=0 or Z0(p2)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[2][3] == 0)
					bCondition3 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount++;
					if (nCount != 1)
						bCondition3 = TRUE;
				}

				//判断P2*P4*P6=0 or Z0(p4)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[3][2] == 0)
					bCondition4 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						bCondition4 = TRUE;
				}
				if(bCondition1 && bCondition2 && bCondition3 && bCondition4)
				{
					*lpDst = (unsigned char)255;
					bModified = TRUE;
				}
				else
				{
					*lpDst = (unsigned char)0;
				}
			}
		}
			// 复制腐蚀后的图像
			memcpy(pBits, lpNewDIBBits, lWidth * lHeight);


	}
	// 复制腐蚀后的图像
	memcpy(pBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}
