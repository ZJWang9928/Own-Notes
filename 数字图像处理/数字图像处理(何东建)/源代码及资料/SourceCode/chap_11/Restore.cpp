// Restore.cpp: implementation of the CRestore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dip.h"
#include "Restore.h"


IMPLEMENT_DYNCREATE(CRestore, CObject)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRestore::CRestore()
{


}

CRestore::~CRestore()
{
	exitM();
}

CRestore::CRestore(CDibObject *pDibObject)
{
	m_pDibObject=pDibObject;
	initM(MATCOM_VERSION);
}

/******************************************************************
*
*    函数名：InvFilter()
*
*    参  数：无
*  
*    返回值：布尔类型
*
*    功  能：对一幅图像进行逆滤波复原
*  
*    备  注：该函数适用于对由卷积所造成的图像模糊进行复原
*
*******************************************************************/
BOOL CRestore::InvFilter()
{
	/************************************读取图像数据******************************/

	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//对1位及4位图像不作任何操作直接返回
	if(m_pDibObject->GetNumBits() != 8) return( FALSE );
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
			
	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	

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
	if( pBuffer == NULL ) return( NULL );
 
    

    /******************************将图像数据转换成二维矩阵，以进行矩阵运算*******************************/
	
	//图像数据总字节数，其中nWidthBytes为每行图像数据占用的字节数
	DWORD SizeImage = nWidthBytes * nHeight;

	//通过创建一个1×SizeImage的一维矩阵0矩阵，为矩阵变量分配内存
	m_matBits = zeros(1, SizeImage);

	//将矩阵由double型转换为unsigned char型，以便将图像数据赋给矩阵
	m_matBits = muint8(m_matBits);

	//首先利用m_matBits.addr()获得矩阵实部的内存指针，
	//然后利用memcpy()将图像数据一次性赋给矩阵变量。
	memcpy(m_matBits.addr(), pBits, SizeImage);

	//由于Mm型的矩阵在内存中是按先列后行的顺序存储的，
	//先用Matrix<LIB>C++的库函数reshape()将一维矩阵m_matBits变维为
	//nWidthBytes×nHeight的二维矩阵，再用rot90()函数将其逆时针旋转90度，
	//将矩阵变为nHeight×nWidthBytes的二维矩阵，使图像的第nHeight-1行数据
	//存储在矩阵的第0行，从而完成将图像数据按从下到上的顺序存储。
	m_matBits = rot90(reshape(m_matBits, nWidthBytes, nHeight));

	//若图像宽度与其字节宽度不同，则将由系统补齐为每行字节数为4的整数倍的
	//各列0删除，以减小矩阵大小加快处理速度。
	if(nWidthBytes != nWidth)
	{
		//相当于MATLAB中的X=X(:, 1: nWidth)操作
		m_matBits = m_matBits(c_p, colon(1, 1, nWidth));
	}
	//将矩阵由unsigned char型转换为double型，以便进行运算
	m_matBits = mdouble(m_matBits);
    
    //定义模板，大小为7x7
	Mm m_matFilter;
	m_matFilter=zeros(nHeight,nWidth);
	int i,j;
	for(i=1;i<7;i++)
	{
		for(j=1;j<7;j++){
			m_matFilter.r(i,j)=0.02;
		}

	}


    /**********************************对图像进行卷积模糊处理****************************/

	//对原图像进行快速傅立叶变换，将图像从空域变换到频域
	m_matBits=fft2(m_matBits);
	//对模板进行快速傅立叶变换
	m_matFilter=fft2(m_matFilter);

	//频率相乘
	double a,b,c,d;
	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			a=m_matBits.r(i,j);
			b=m_matBits.i(i,j);
			c=m_matFilter.r(i,j);
			d=m_matFilter.i(i,j);
			if(c*c+d*d>0.003)
			{
				m_matBits.r(i,j)=(a*c+b*d)/(c*c+d*d);
				m_matBits.i(i,j)=(b*c-a*d)/(c*c+d*d);
			}
		}
	}

	//对图像进行快速逆傅立叶变换，将图像从频域变换到空域
	m_matBits=ifft2(m_matBits);


	/**********************************图像处理结果保存**********************************/

	//用Matrix<LIB>C++的库函数将矩阵数据范围限定于0～255
	m_matBits = mabs(m_matBits);
	Mm L = m_matBits > 255.0;
	Mm NotL = !L;
	m_matBits = times(m_matBits, NotL) + L * 255.0;

	//将矩阵由double型转换为unsigned char型
	m_matBits = muint8(m_matBits);
	//将每行数据的字节数用0补齐为4的整数倍
	int nTmp = (int)rem(nWidth, 4);
	int nPadding;
	if(nTmp > 0)
	{
		nPadding = 4 - nTmp;
		m_matBits = cat(2, m_matBits, repmat(muint8(0), 
	(BR(size(m_matBits, 1)), nPadding)));
	}
	//将图像数据矩阵顺时针旋转90度（原因同前）
	m_matBits = rot90(m_matBits, -1);
	//将图像数据矩阵存回图像数据区
	memcpy(pBits, m_matBits.addr(), (nWidthBytes * nHeight)*sizeof(unsigned char));
	
	return TRUE;
}

/******************************************************************
*
*    函数名：ConvNoiseBlur()
*
*    参  数：无
*  
*    返回值：布尔类型
*
*    功  能：对一幅图像进行卷积噪音模糊
*  
*    备  注：无
*
*******************************************************************/
BOOL CRestore::ConvNoiseBlur()
{

	/************************************读取图像数据******************************/

	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//对1位及4位图像不作任何操作直接返回
	if(m_pDibObject->GetNumBits() != 8) return( FALSE );
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
			
	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	

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
	if( pBuffer == NULL ) return( NULL );
 
    

    /******************************将图像数据转换成二维矩阵，以进行矩阵运算*******************************/
	
	//图像数据总字节数，其中nWidthBytes为每行图像数据占用的字节数
	DWORD SizeImage = nWidthBytes * nHeight;

	//通过创建一个1×SizeImage的一维矩阵0矩阵，为矩阵变量分配内存
	m_matBits = zeros(1, SizeImage);

	//将矩阵由double型转换为unsigned char型，以便将图像数据赋给矩阵
	m_matBits = muint8(m_matBits);

	//首先利用m_matBits.addr()获得矩阵实部的内存指针，
	//然后利用memcpy()将图像数据一次性赋给矩阵变量。
	memcpy(m_matBits.addr(), pBits, SizeImage);

	//由于Mm型的矩阵在内存中是按先列后行的顺序存储的，
	//先用Matrix<LIB>C++的库函数reshape()将一维矩阵m_matBits变维为
	//nWidthBytes×nHeight的二维矩阵，再用rot90()函数将其逆时针旋转90度，
	//将矩阵变为nHeight×nWidthBytes的二维矩阵，使图像的第nHeight-1行数据
	//存储在矩阵的第0行，从而完成将图像数据按从下到上的顺序存储。
	m_matBits = rot90(reshape(m_matBits, nWidthBytes, nHeight));

	//若图像宽度与其字节宽度不同，则将由系统补齐为每行字节数为4的整数倍的
	//各列0删除，以减小矩阵大小加快处理速度。
	if(nWidthBytes != nWidth)
	{
		//相当于MATLAB中的X=X(:, 1: nWidth)操作
		m_matBits = m_matBits(c_p, colon(1, 1, nWidth));
	}
	//将矩阵由unsigned char型转换为double型，以便进行运算
	m_matBits = mdouble(m_matBits);
    


    //定义模板，大小为7x7
	Mm m_matFilter;
	m_matFilter=zeros(nHeight,nWidth);
	int i,j;
	for(i=1;i<7;i++)
	{
		for(j=1;j<7;j++){
			m_matFilter.r(i,j)=0.02;
		}

	}


   /**************************************对图像进行卷积噪声模糊处理******************************/

	//对原图像进行快速傅立叶变换，将图像从空域变换到频域
	m_matBits=fft2(m_matBits);
	//对模板进行快速傅立叶变换
	m_matFilter=fft2(m_matFilter);

	//频率相乘
	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			m_matBits.r(i,j)=m_matBits.r(i,j)*m_matFilter.r(i,j)-m_matBits.i(i,j)*m_matFilter.i(i,j);
			m_matBits.i(i,j)=m_matBits.r(i,j)*m_matFilter.i(i,j)+m_matBits.i(i,j)*m_matFilter.r(i,j);
		}
	}

	//对图像做快速逆傅立叶变换,将图像从频域变换到空域
	m_matBits=ifft2(m_matBits);

	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			if((i+j)==((int) (i+j)/8)*8)
			{
				m_matBits.r(i,j)=m_matBits.r(i,j)-16;
			}
		}
	}

	   
	
	/**********************************图像处理结果保存**********************************/

	//用Matrix<LIB>C++的库函数将矩阵数据范围限定于0～255
	m_matBits = mabs(m_matBits);
	Mm L = m_matBits > 255.0;
	Mm NotL = !L;
	m_matBits = times(m_matBits, NotL) + L * 255.0;

	//将矩阵由double型转换为unsigned char型
	m_matBits = muint8(m_matBits);
	//将每行数据的字节数用0补齐为4的整数倍
	int nTmp = (int)rem(nWidth, 4);
	int nPadding;
	if(nTmp > 0)
	{
		nPadding = 4 - nTmp;
		m_matBits = cat(2, m_matBits, repmat(muint8(0), 
	(BR(size(m_matBits, 1)), nPadding)));
	}
	//将图像数据矩阵顺时针旋转90度（原因同前）
	m_matBits = rot90(m_matBits, -1);
	//将图像数据矩阵存回图像数据区
	memcpy(pBits, m_matBits.addr(), (nWidthBytes * nHeight)*sizeof(unsigned char));

	return TRUE;
}

/******************************************************************
*
*    函数名：wiener()
*
*    参  数：无
*  
*    返回值：布尔类型
*
*    功  能：对一幅图像进行维纳滤波复原
*  
*    备  注：该函数适用于对由卷积噪声所造成的图像模糊进行复原
*
*******************************************************************/
BOOL CRestore::Wiener()
{

	/************************************读取图像数据******************************/

	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//对1位及4位图像不作任何操作直接返回
	if(m_pDibObject->GetNumBits() != 8) return( FALSE );
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
			
	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	

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
	if( pBuffer == NULL ) return( NULL );
 
    
    /******************************将图像数据转换成二维矩阵，以进行矩阵运算*******************************/
	
	//图像数据总字节数，其中nWidthBytes为每行图像数据占用的字节数
	DWORD SizeImage = nWidthBytes * nHeight;

	//通过创建一个1×SizeImage的一维矩阵0矩阵，为矩阵变量分配内存
	m_matBits = zeros(1, SizeImage);

	//将矩阵由double型转换为unsigned char型，以便将图像数据赋给矩阵
	m_matBits = muint8(m_matBits);

	//首先利用m_matBits.addr()获得矩阵实部的内存指针，
	//然后利用memcpy()将图像数据一次性赋给矩阵变量。
	memcpy(m_matBits.addr(), pBits, SizeImage);

	//由于Mm型的矩阵在内存中是按先列后行的顺序存储的，
	//先用Matrix<LIB>C++的库函数reshape()将一维矩阵m_matBits变维为
	//nWidthBytes×nHeight的二维矩阵，再用rot90()函数将其逆时针旋转90度，
	//将矩阵变为nHeight×nWidthBytes的二维矩阵，使图像的第nHeight-1行数据
	//存储在矩阵的第0行，从而完成将图像数据按从下到上的顺序存储。
	m_matBits = rot90(reshape(m_matBits, nWidthBytes, nHeight));

	//若图像宽度与其字节宽度不同，则将由系统补齐为每行字节数为4的整数倍的
	//各列0删除，以减小矩阵大小加快处理速度。
	if(nWidthBytes != nWidth)
	{
		//相当于MATLAB中的X=X(:, 1: nWidth)操作
		m_matBits = m_matBits(c_p, colon(1, 1, nWidth));
	}
	//将矩阵由unsigned char型转换为double型，以便进行运算
	m_matBits = mdouble(m_matBits);


    //定义模糊模板，大小为7x7
	Mm m_matFilter;
	m_matFilter=zeros(nHeight,nWidth);
	int i,j;
	for(i=1;i<7;i++)
	{
		for(j=1;j<7;j++){
			m_matFilter.r(i,j)=0.02;
		}

	}

	//定义噪声模板
	Mm m_matNoise;
	m_matNoise=zeros(nHeight,nWidth);
	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			if((i+j)==((int) ((i+j)/8))*8)
			{
				m_matNoise.r(i,j)=-16;
			}
		}
	}
 

    /************************************对图像进行Wiener滤波复原***************************/

	//对原图像进行快速傅立叶变换，将图像从空域变换到频域
	m_matBits=fft2(m_matBits);
	//对模糊模板进行快速傅立叶变换
	m_matFilter=fft2(m_matFilter);
	//对噪声模板进行快速傅立叶变换
	m_matNoise=fft2(m_matNoise);
	double a,b,c,d,e,f,multi;
	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			a=m_matBits.r(i,j);
			b=m_matBits.i(i,j);
			c=m_matFilter.r(i,j);
			d=m_matFilter.i(i,j);
			e=m_matNoise.r(i,j);
			f=m_matNoise.i(i,j);

			multi=(a*a+b*b)/(a*a+b*b-e*e-f*f);
			if(c*c+d*d>0.003)
			{
				m_matBits.r(i,j)=(a*c+b*d)/(c*c+d*d)/multi;
				m_matBits.i(i,j)=(b*c-a*d)/(c*c+d*d)/multi;
			}
		}
	}

	//对处理结果进行快速逆傅立叶变换，将图像从频域变换到空域
	m_matBits=ifft2(m_matBits);


	/**********************************图像处理结果保存**********************************/

	//用Matrix<LIB>C++的库函数将矩阵数据范围限定于0～255
	m_matBits = mabs(m_matBits);
	Mm L = m_matBits > 255.0;
	Mm NotL = !L;
	m_matBits = times(m_matBits, NotL) + L * 255.0;

	//将矩阵由double型转换为unsigned char型
	m_matBits = muint8(m_matBits);
	//将每行数据的字节数用0补齐为4的整数倍
	int nTmp = (int)rem(nWidth, 4);
	int nPadding;
	if(nTmp > 0)
	{
		nPadding = 4 - nTmp;
		m_matBits = cat(2, m_matBits, repmat(muint8(0), 
	(BR(size(m_matBits, 1)), nPadding)));
	}
	//将图像数据矩阵顺时针旋转90度（原因同前）
	m_matBits = rot90(m_matBits, -1);
	//将图像数据矩阵存回图像数据区
	memcpy(pBits, m_matBits.addr(), (nWidthBytes * nHeight)*sizeof(unsigned char));

	return TRUE;
}


/******************************************************************
*
*   函数名：ConvBlur()
*
*   参  数：无
*  
*   返回值：布尔类型
*
*   功  能：对一幅图像进行卷积模糊
*  
*   备  注：无
*
*******************************************************************/
BOOL CRestore::ConvBlur()
{

	/************************************读取图像数据******************************/

	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);
	
	//对1位及4位图像不作任何操作直接返回
	if(m_pDibObject->GetNumBits() != 8) return( FALSE );
	
	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
			
	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	

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
	if( pBuffer == NULL ) return( NULL );
 
    
    /******************************将图像数据转换成二维矩阵，以进行矩阵运算*******************************/
	
	//图像数据总字节数，其中nWidthBytes为每行图像数据占用的字节数
	DWORD SizeImage = nWidthBytes * nHeight;

	//通过创建一个1×SizeImage的一维矩阵0矩阵，为矩阵变量分配内存
	m_matBits = zeros(1, SizeImage);

	//将矩阵由double型转换为unsigned char型，以便将图像数据赋给矩阵
	m_matBits = muint8(m_matBits);

	//首先利用m_matBits.addr()获得矩阵实部的内存指针，
	//然后利用memcpy()将图像数据一次性赋给矩阵变量。
	memcpy(m_matBits.addr(), pBits, SizeImage);

	//由于Mm型的矩阵在内存中是按先列后行的顺序存储的，
	//先用Matrix<LIB>C++的库函数reshape()将一维矩阵m_matBits变维为
	//nWidthBytes×nHeight的二维矩阵，再用rot90()函数将其逆时针旋转90度，
	//将矩阵变为nHeight×nWidthBytes的二维矩阵，使图像的第nHeight-1行数据
	//存储在矩阵的第0行，从而完成将图像数据按从下到上的顺序存储。
	m_matBits = rot90(reshape(m_matBits, nWidthBytes, nHeight));

	//若图像宽度与其字节宽度不同，则将由系统补齐为每行字节数为4的整数倍的
	//各列0删除，以减小矩阵大小加快处理速度。
	if(nWidthBytes != nWidth)
	{
		//相当于MATLAB中的X=X(:, 1: nWidth)操作
		m_matBits = m_matBits(c_p, colon(1, 1, nWidth));
	}
	//将矩阵由unsigned char型转换为double型，以便进行运算
	m_matBits = mdouble(m_matBits);


    
    //定义模板，大小为7x7
	Mm m_matFilter;
	m_matFilter=zeros(nHeight,nWidth);
	int i,j;
	for(i=1;i<7;i++)
	{
		for(j=1;j<7;j++){
			m_matFilter.r(i,j)=0.02;
		}

	}

    /**************************************对图像进行卷积模糊处理****************************/

	//对原图像进行快速傅立叶变换，将图像从空域变换到频域
	m_matBits=fft2(m_matBits);
	//对模板进行快速傅立叶变换
	m_matFilter=fft2(m_matFilter);

	//频率相乘
	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			m_matBits.r(i,j)=m_matBits.r(i,j)*m_matFilter.r(i,j)-m_matBits.i(i,j)*m_matFilter.i(i,j);
			m_matBits.i(i,j)=m_matBits.r(i,j)*m_matFilter.i(i,j)+m_matBits.i(i,j)*m_matFilter.r(i,j);
		}
	}
	//对图像做逆傅立叶变换，将图像从频域变换到空域
	m_matBits=ifft2(m_matBits);


	/**********************************图像处理结果保存**********************************/

	//用Matrix<LIB>C++的库函数将矩阵数据范围限定于0～255
	m_matBits = mabs(m_matBits);
	Mm L = m_matBits > 255.0;
	Mm NotL = !L;
	m_matBits = times(m_matBits, NotL) + L * 255.0;

	//将矩阵由double型转换为unsigned char型
	m_matBits = muint8(m_matBits);
	//将每行数据的字节数用0补齐为4的整数倍
	int nTmp = (int)rem(nWidth, 4);
	int nPadding;
	if(nTmp > 0)
	{
		nPadding = 4 - nTmp;
		m_matBits = cat(2, m_matBits, repmat(muint8(0), 
	(BR(size(m_matBits, 1)), nPadding)));
	}
	//将图像数据矩阵顺时针旋转90度（原因同前）
	m_matBits = rot90(m_matBits, -1);
	//将图像数据矩阵存回图像数据区
	memcpy(pBits, m_matBits.addr(), (nWidthBytes * nHeight)*sizeof(unsigned char));

	return TRUE;
}
