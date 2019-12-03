// AreaPro.cpp: implementation of the CFreqPro class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FreqPro.h"

#define AVERAGE   0
#define DISK      1
#define GAUSSIAN  2
#define LAPLACIAN 3
#define LOG       4
#define MOTION    5
#define PREWITT   6
#define SOBEL     7
#define UNSHARP   8

IMPLEMENT_DYNCREATE(CFreqPro, CObject)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CFreqPro::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CFreqPro::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif

/***********************************************************************
*                                                                      *
*                              频域处理类                              *
*                                                                      *
***********************************************************************/

////////////////////////////////////////////////////////////////////////
//构造函数CFreqPro()    
//----------------------------------------------------------------------
//基本功能：构造一个CFreqPro类的对象，如不传入CDibObject对象。第一次调   
//			用某一个处理函数时必须给出一个CDibObject对象指针。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////

CFreqPro::CFreqPro()
{
	//初始化Matrix<LIB>类库
	initM(MATCOM_VERSION);
}

////////////////////////////////////////////////////////////////////////
//构造函数CFreqPro()    
//----------------------------------------------------------------------
//基本功能：构造一个CFreqPro类的对象并传入CDibObject对象。所有的操作都
//			针对该对象，直到另一个对象作为参数被传给图像处理函数。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject
//----------------------------------------------------------------------
//返    回：无
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CFreqPro::CFreqPro(CDibObject *pDibObject)
{
	//初始化Matrix<LIB>类库
	initM(MATCOM_VERSION);

	m_pDibObject = pDibObject;
}

//析构函数
CFreqPro::~CFreqPro()
{
	//结束Matrix<LIB>类库调用
	exitM();	
}

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//基本功能：本函数为CFreqPro类对象指定一个CDibObject对象指针
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject, 默认为NULL。
//----------------------------------------------------------------------
//返    回：无。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CFreqPro::SetDibObjectClass( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}

////////////////////////////////////////////////////////////////////////
//Mm GetMatData()   
//----------------------------------------------------------------------
//基本功能：本函数根据CDibObject对象指针指定的图像，生成图像数据矩阵。
//----------------------------------------------------------------------
//参数说明：无。
//----------------------------------------------------------------------
//返    回：Mm	图像数据矩阵
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::GetMatData()
{
	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(NULL);

	//对不是8位的图像不作任何操作直接返回
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("目前只支持8位图像的处理！");
		return( NULL );
	}

	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits());
	if( pBuffer == NULL ) return( NULL );

	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	//图像数据区大小（字节总数）
	DWORD SizeImage = nWidthBytes * nHeight;

	//创建图像数据矩阵，并将其元素初始值设为0
	m_matBits = zeros(1, SizeImage);

	//默认的矩阵数据类型是double，
	//首先将其转换为unsigned char型，
	//以便用memcpy内存拷贝命令将图像数据赋给矩阵。
	m_matBits = muint8(m_matBits);

	//通过Matrix<LIB>C++库的.addr()函数返回矩阵变量的内存指针，以完成对矩阵单元的访问。
	//用memcpy内存拷贝命令将图像数据赋给矩阵。
	memcpy(m_matBits.addr(), pBits, SizeImage);

	//由于Mm类型的矩阵是按先列后行的顺序排列，
	//在此用reshape()函数将创建的一维矩阵m_matBits变维为nWidthBytes×nHeight的二维矩阵。
	//再用rot90()函数将二维矩阵逆时针旋转90度，将矩阵变为nHeight×nWidthBytes的二维矩阵，
	//并使的矩阵的第nHeight行对应图像数据的第一行数据。
	m_matBits = rot90(reshape(m_matBits, nWidthBytes, nHeight));

	//若图像宽度与其字节宽度不同，
	//则将由系统补齐的每行字节数为4的整数倍的各列0删除，以减小矩阵大小加快处理速度。
	if(nWidthBytes != nWidth)
	{
		//相当于Matlab中的X=X(:,1:nWidth)操作
		m_matBits = m_matBits(c_p, colon(1, 1, nWidth));
	}

	//将矩阵由unsigned char型转换为double型，以便进行运算
	m_matBits = mdouble(m_matBits);

	//内存解锁
	::GlobalUnlock(m_pDibObject->GetDib());

	return( m_matBits );
}

////////////////////////////////////////////////////////////////////////
//BOOL SetBits()   
//----------------------------------------------------------------------
//基本功能：本函数根据用图像数据矩阵重设图像数据区
//----------------------------------------------------------------------
//参数说明：无。
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CFreqPro::SetBits()
{
	//图像指针为空，无法操作返回
	if(m_pDibObject == NULL) return(FALSE);

	//对1位及4位图像不作任何操作直接返回
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("目前只支持8位灰度图像的处理！");
		return( FALSE );
	}

	//获取图像宽度和高度（以像素为单位）
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//定义变量
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//获得图像指针
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits());
	if( pBuffer == NULL ) return( NULL );

	//获得颜色数
	nNumColors = m_pDibObject->GetNumColors();
	//获得调色板指针
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//获得位图数据指针
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

		//创建矩阵变换，其元素初始值设为0
	if(isempty(m_matBits) == 1)
	{
		return FALSE;
	}

	//将矩阵数据范围限定于（0——255）
	m_matBits = mabs(m_matBits);
	Mm L = m_matBits > 255.0;
	Mm NotL = !L;
	m_matBits = times(m_matBits, NotL) + L * 255.0;

	//将矩阵由double型转换为unsigned char型，以便将图像数据赋给矩阵
	m_matBits = muint8(m_matBits);

	//补0，以满足BMP图像对行宽字节数为4的整数倍的要求。
	int nTmp = (int)rem(nWidth, 4);
	int nPadding;

	if(nTmp > 0)
	{
		nPadding = 4 - nTmp;
		m_matBits = cat(2, m_matBits, 
			repmat(muint8(0), (BR(size(m_matBits, 1)), nPadding)));
	}
	else
	{
		nPadding = 0;
	}

	//对矩阵进行转置操作
	m_matBits = rot90(m_matBits, -1);

	//将图像数据赋绘矩阵
	memcpy(pBits, m_matBits.addr(), (nWidthBytes * nHeight)*sizeof(unsigned char));

	//内存解锁
	::GlobalUnlock(m_pDibObject->GetDib());

	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//Mm FFT2()   
//----------------------------------------------------------------------
//基本功能：本函数完成图像的快速傅立叶变换。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	图像对象指针。
//----------------------------------------------------------------------
//返    回：Mm	返回变换结果矩阵
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::FFT2(CDibObject *pDibObject)
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = pDibObject;

	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//创建图像数据矩阵
	GetMatData();

	//获得矩阵的行数和列数
	Mm mSize = size(m_matBits);

	//调用Matrix<Lib>C++库函数fft2()完成二维离散傅立叶变换
	Mm ff = fft2(m_matBits);

	Mm matTransed = ff;

	//调用Matrix<Lib>C++库函数fftshift()将频域中心移到矩阵中心
	ff = fftshift(ff);

	//调用Matrix<Lib>C++库函数mabs()计算频谱
	m_matBits = mabs(ff) / sqrt(mSize.r(1,1)*mSize.r(1,2));

	//将矩阵数据赋给图像数据区
	SetBits();
	
	return matTransed;
}

////////////////////////////////////////////////////////////////////////
//BOOL NFFT2()   
//----------------------------------------------------------------------
//基本功能：本函数完成图像的快速傅立叶逆变换。
//----------------------------------------------------------------------
//参数说明：Mm matTransed	被变换的矩阵
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CFreqPro::NFFT2(Mm matTransed)
{
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//调用Matrix<Lib>C++库函数fft2()完成二维离散傅立叶变换
	m_matBits = ifft2(matTransed);

	//将矩阵数据赋给图像数据区
	SetBits();
	
	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//Mm DCT2()   
//----------------------------------------------------------------------
//基本功能：本函数完成图像的离散余弦变换。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	图像对象指针。
//----------------------------------------------------------------------
//返    回：Mm	返回变换结果矩阵
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::DCT2(CDibObject *pDibObject)
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//创建图像数据矩阵
	GetMatData();

	Mm ff = dct2(m_matBits);

	Mm matTransed = ff;

	//调用Matrix<Lib>C++库函数mabs()计算频谱
	m_matBits = mabs(ff);

	//将矩阵数据赋给图像数据区
	SetBits();
	
	return matTransed;
}

////////////////////////////////////////////////////////////////////////
//Mm WHT2()   
//----------------------------------------------------------------------
//基本功能：本函数完成图像的离散沃尔什哈达玛变换。
//----------------------------------------------------------------------
//参数说明：CDibObject *pDibObject	图像对象指针。
//----------------------------------------------------------------------
//返    回：Mm	返回变换结果矩阵
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::WHT2(CDibObject *pDibObject)
{
	//CDibObject对象指针
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//创建图像数据矩阵
	GetMatData();

	//获得矩阵的行数和列数
	Mm mSize = size(m_matBits);

	BOOL bPowered = FALSE;

	if(bIs2Power((int)mSize.r(1,1))) bPowered = TRUE;
	if(bIs2Power((int)mSize.r(1,1)/12)) bPowered = TRUE;
	if(bIs2Power((int)mSize.r(1,1)/20)) bPowered = TRUE;

	if(bIs2Power((int)mSize.r(1,2))) bPowered = TRUE;
	if(bIs2Power((int)mSize.r(1,2)/12)) bPowered = TRUE;
	if(bIs2Power((int)mSize.r(1,2)/20)) bPowered = TRUE;

	if(!bPowered)
	{
		// 提示用户参数设置错误
		AfxMessageBox("尺寸不是2^N、12*2^N、20*2^N，无法生成哈达玛矩阵！");
		
		// 返回
		return( FALSE );
	}


	//生成哈达玛矩阵
	Mm HColum = hadamard(mSize.r(1,1));
	Mm HRow = hadamard(mSize.r(1,2)); 


	//进行离散哈达玛变换
	Mm ff = HRow * m_matBits * HColum;

	Mm matTransed = ff / (mSize.r(1,1)*mSize.r(1,2));

	//调整系数以便以图像方式显示结果
	ff = ff / sqrt(mSize.r(1,1)*mSize.r(1,2));

	//调用Matrix<Lib>C++库函数mabs()计算频谱
	m_matBits = mabs(ff);

	//将矩阵数据赋给图像数据区
	SetBits();
	
	return matTransed;
}

////////////////////////////////////////////////////////////////////////
//BOOL NWHT2()   
//----------------------------------------------------------------------
//基本功能：本函数完成图像的快速傅立叶逆变换。
//----------------------------------------------------------------------
//参数说明：Mm matTransed	被变换的矩阵
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CFreqPro::NWHT2(Mm matTransed)
{
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//获得矩阵的行数和列数
	Mm mSize = size(matTransed);

	BOOL bPowered = FALSE;

	if(bIs2Power((int)mSize.r(1,1))) bPowered = TRUE;
	if(bIs2Power((int)mSize.r(1,1)/12)) bPowered = TRUE;
	if(bIs2Power((int)mSize.r(1,1)/20)) bPowered = TRUE;

	if(bIs2Power((int)mSize.r(1,2))) bPowered = TRUE;
	if(bIs2Power((int)mSize.r(1,2)/12)) bPowered = TRUE;
	if(bIs2Power((int)mSize.r(1,2)/20)) bPowered = TRUE;

	if(!bPowered)
	{
		// 提示用户参数设置错误
		AfxMessageBox("尺寸不是2^N、12*2^N、20*2^N，无法生成哈达玛矩阵！");
		
		// 返回
		return( FALSE );
	}


	//生成哈达玛矩阵
	Mm HColum = hadamard(mSize.r(1,1));
	Mm HRow = hadamard(mSize.r(1,2)); 

	//进行离散哈达玛变换
	m_matBits = HRow * matTransed * HColum;

	//将矩阵数据赋给图像数据区
	SetBits();
	
	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//Mm dct()   
//----------------------------------------------------------------------
//基本功能：本函数完成一个向量X的一维离散余弦变换。
//----------------------------------------------------------------------
//参数说明：Mm a	待变换向量（若为矩阵，按列进行运算）
//			Mm n	变换长度，若指定该参数，则自动将向量a补0或自动裁剪到
//					指定长度。
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::dct(Mm a, Mm n)
{
	//定义相关矩阵变量
	a.setname("a"); n.setname("n"); 
    dMm(b); dMm(do_trans); dMm(m); dMm(aa); dMm(ww); dMm(y); dMm(yy); 

    //判断输入参数是否正确
    double old_nargin=nargin_val;

	if (!nargin_set) nargin_val=2.0;

    nargin_set=0;
    double old_nargout=nargout_val;

	if (!nargout_set) nargout_val=1.0;

    nargout_set=0;

	if (istrue(nargin()==0.0))
	{
		error(TM("输入参数为空，无法完成操作！"));
    }
    
	if (istrue(isempty(a)))
	{
		error(TM("输入矩阵为空，无法完成操作！"));
		b = nop_M;

		nargin_val=old_nargin; nargout_val=old_nargout;		
		a.setname(NULL); n.setname(NULL); 

		return b;
    }
    
    //如果输入为一行向量，将其转换为列向量。
	do_trans = (size(a,1.0)==1.0);
	if (istrue(do_trans))
	{
		//是行向量，转换为列向量。
		//相当于Matlab的a=a(:)。
		a = a(c_p);
    }

	//只有一个输入参数，未指定变换点数n。	
	if (istrue(nargin()==1.0))
	{
		//得到第一维长度（矩阵列长度）
		n = size(a,1.0);
    }
	//得到第二维长度（矩阵行长度）
	m = size(a,2.0);
    
    //根据需要为矩阵补零或截去多余元素
	if (istrue(size(a,1.0)<n))
	{
		//矩阵列长度小于指定的变换点数
		//生成n×m的零矩阵
		aa = zeros(n,m);
		//从输入矩阵a中取得所需元素
		//相当于Matlab的aa(1:size(a,1),:) = a。
		aa(colon(1.0,1.0,size(a,1.0)),c_p) = a;
    } 
	else
	{
		//矩阵列长度大于或等于指定的变换点数
		//从输入矩阵a中取得所需元素
		//相当于Matlab的aa = a(1:n,:)。
		aa = a(colon(1.0,1.0,n),c_p);
    }

    //计算用于DFT计算的系数
	ww = transpose((exp(-i*(colon(0.0,1.0,n-1.0))*pi/(2.0*n))/msqrt(2.0*n)));
	//调整第一个系数
	ww(1.0) = ww(1.0)/msqrt(2.0);
    
	//进行变换
	if (istrue(rem(n,2.0)==1.0)||istrue(!isreal(a)))
	{
		//n为奇数
		//生成中间变换矩阵

		//生成2.0*n×m的零矩阵
		y = zeros(2.0*n,m);
		//相当于Matlab的y(1:n,:) = aa和y(n+1:2*n,:) = flipud(aa);。
		y(colon(1.0,1.0,n),c_p) = aa;
		y(colon(n+1.0,1.0,2.0*n),c_p) = flipud(aa);
		
		//调用Matrix<Lib>C++库函数FFT()进行快速离散傅立叶变换，
		yy = fft(y);
		//保留需要的变换结果数据
		//相当于Matlab的yy = yy(1:n,:)。
		yy = yy(colon(1.0,1.0,n),c_p);
		
    }
	else
	{
		//n为偶数
		//对aa矩阵的各列元素重新进行排序
		//相当于Matlab的y = [ aa(1:2:n,:); aa(n:-2:2,:) ]。
		y = (BR(aa(colon(1.0,2.0,n),c_p)),semi,
			aa(colon(n,-2.0,2.0),c_p));
		//调用Matrix<Lib>C++库函数FFT()进行快速离散傅立叶变换，
		yy = fft(y);
		//调整变换系数矩阵
		ww = 2.0*ww;
    }
    
    //对FFT变换结果乘以变换系数
	//相当于Matlab的b = ww(:,ones(1,m)) .* yy。
	b = times(ww(c_p,ones(1.0,m)),yy);
    
	//如果输入矩阵是实数矩阵，输出取变换结果的实部。
	if (istrue(isreal(a)))
	{
		b = real(b);
    }
	//如果输入的是行向量，对结果进行转置操作。
	if (istrue(do_trans))
	{
		b = transpose(b);
    }
	
	nargin_val=old_nargin; nargout_val=old_nargout;
	
	a.setname(NULL); n.setname(NULL); 
	return b;					 
}

////////////////////////////////////////////////////////////////////////
//Mm dct()   
//----------------------------------------------------------------------
//基本功能：本函数为离散余弦变换的无参重载版本。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::dct()
{
    begin_scope
		double old_nargin=nargin_val; nargin_val=0.0; nargin_set=1;
    dMm(a); dMm(n); 
    dMm(ret1a);
    ret1a=dct(a, n);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
}

////////////////////////////////////////////////////////////////////////
//Mm dct()   
//----------------------------------------------------------------------
//基本功能：本函数为离散余弦变换的无指定变换长度重载版本。
//----------------------------------------------------------------------
//参数说明：Mm a	待变换向量（若为矩阵，按列进行运算）
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::dct(Mm a)
{
    begin_scope
		double old_nargin=nargin_val; nargin_val=1.0; nargin_set=1;
    dMm(n); 
    dMm(ret1a);
    ret1a=dct(a, n);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
}

////////////////////////////////////////////////////////////////////////
//Mm dct2()   
//----------------------------------------------------------------------
//基本功能：本函数完成矩阵的一维离散余弦变换。
//----------------------------------------------------------------------
//参数说明：Mm arg1		待变换矩阵
//			Mm mrows	行变换长度，若指定该参数，则自动将向量a补0或自动
//						裁剪到指定长度。
//			Mm ncols	列变换长度，若指定该参数，则自动将向量a补0或自动
//						裁剪到指定长度。
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用。该函数是先对矩阵
//			按列进行一维离散余弦变换，再对其按列进行一维离散余弦变换，最
//			终得到其二维离散余弦变换结果。对程序在此不加详细注释，请参考
//			一维离散余弦变换程序注释。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::dct2(Mm arg1, Mm mrows, Mm ncols)
{
	arg1.setname("arg1"); mrows.setname("mrows"); ncols.setname("ncols"); 
	dMm(b); dMm(m); dMm(n); dMm(a); dMm(mpad); dMm(npad); 
	
    double old_nargin=nargin_val;
	if (!nargin_set) nargin_val=3.0;
    nargin_set=0;
    double old_nargout=nargout_val;
	if (!nargout_set) nargout_val=1.0;
    nargout_set=0;

	size(arg1,i_o,m,n);
	if (istrue(nargin()==1.0))
	{
		if (istrue((m>1.0))&&istrue((n>1.0)))
		{
			b = transpose(dct(transpose(dct(arg1))));
			nargin_val=old_nargin; nargout_val=old_nargout;
			
			arg1.setname(NULL); mrows.setname(NULL); ncols.setname(NULL); 
			return b;
		}
		else
		{
			
			mrows = m;
			ncols = n;
		}
	}
	a = arg1;
	if (istrue(nargin()==2.0))
	{
		ncols = mrows(2.0);
		mrows = mrows(1.0);
	}
	mpad = mrows;
	npad = ncols;
	if (istrue(m==1.0)&&istrue(mpad>m))
	{
		a(2.0,1.0) = 0.0;
		m = 2.0;
	}
	if (istrue(n==1.0)&&istrue(npad>n))
	{
		a(1.0,2.0) = 0.0;
		n = 2.0;
	}
	if (istrue(m==1.0))
	{
		mpad = npad;
		npad = 1.0;
	}
	b = dct(a,mpad);
	if (istrue(m>1.0)&&istrue(n>1.0))
	{
		b = transpose(dct(transpose(b),npad));
	}
	arg1.setname(NULL); mrows.setname(NULL); ncols.setname(NULL); 
	return b;
}

////////////////////////////////////////////////////////////////////////
//Mm dct2()   
//----------------------------------------------------------------------
//基本功能：本函数为二维离散余弦变换的无指定变换长度重载版本。
//----------------------------------------------------------------------
//参数说明：Mm arg1	待变换矩阵
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::dct2(Mm arg1)
{
    begin_scope
		double old_nargin=nargin_val; nargin_val=1.0; nargin_set=1;
    dMm(mrows); dMm(ncols); 
    dMm(ret1a);
    ret1a=dct2(arg1, mrows, ncols);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
}

////////////////////////////////////////////////////////////////////////
//Mm dct2()   
//----------------------------------------------------------------------
//基本功能：本函数为二维离散余弦变换的无指定列变换长度重载版本。
//----------------------------------------------------------------------
//参数说明：Mm arg1		待变换矩阵
//			Mm mrows	行变换长度，若指定该参数，则自动将向量a补0或自动
//						裁剪到指定长度。
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::dct2(Mm arg1, Mm mrows)
{
    begin_scope
		double old_nargin=nargin_val; nargin_val=2.0; nargin_set=1;
    dMm(ncols); 
    dMm(ret1a);
    ret1a=dct2(arg1, mrows, ncols);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
}

////////////////////////////////////////////////////////////////////////
//Mm bIs2Power()   
//----------------------------------------------------------------------
//基本功能：本函数为判断一个数是否为2的整次幂。
//----------------------------------------------------------------------
//参数说明：int nNum		输入的整数
//----------------------------------------------------------------------
//返    回：BOOL
//			是返回真，否返回假。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CFreqPro::bIs2Power(int nNum)
{
	LONG	a;
	
	a = 1;

	while(a * 2 <= nNum)
	{
		a *= 2;
	}

	if(a == nNum) return TRUE;
	else
		return FALSE;
}

////////////////////////////////////////////////////////////////////////
//Mm idct()   
//----------------------------------------------------------------------
//基本功能：本函数完成一个向量X的一维离散余弦变换。
//----------------------------------------------------------------------
//参数说明：Mm b	待变换向量（若为矩阵，按列进行运算）
//			Mm n	变换长度，若指定该参数，则自动将向量a补0或自动裁剪到
//					指定长度。
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::idct(Mm b, Mm n)
{
	//定义相关矩阵变量
	b.setname("b"); n.setname("n"); 
    dMm(a); dMm(do_trans); dMm(m); dMm(bb); dMm(ww); dMm(W); dMm(yy); dMm(y); 
    
    //判断输入参数是否正确
	double old_nargin=nargin_val;

	if (!nargin_set) nargin_val=2.0;

    nargin_set=0;
    double old_nargout=nargout_val; 

	if (!nargout_set) nargout_val=1.0;

    nargout_set=0;

	if (istrue(nargin()==0.0))
	{
		error(TM("输入参数为空，无法完成操作！."));
    }
    
	if (istrue(isempty(b))) 
	{
		
		error(TM("输入矩阵为空，无法完成操作！"));

		a = nop_M;
		
		nargin_val=old_nargin; nargout_val=old_nargout;		
		b.setname(NULL); n.setname(NULL); 

		return a;
	}
	
	//如果输入为一行向量，将其转换为列向量。
	do_trans = (size(b,1.0)==1.0);
	if (istrue(do_trans)) 
	{
		//是行向量，转换为列向量。
		//相当于Matlab的b=b(:)。
		b = b(c_p);
	}
	
	//只有一个输入参数，未指定变换点数n。	
	if (istrue(nargin()==1.0)) 
	{
		//得到第一维长度（矩阵列长度）
		n = size(b,1.0);
	}
	//得到第二维长度（矩阵行长度）
	m = size(b,2.0);

    //根据需要为矩阵补零或截去多余元素
	if (istrue(size(b,1.0)<n)) 
	{
		//矩阵列长度小于指定的变换点数
		//生成n×m的零矩阵
		bb = zeros(n,m);

		//从输入矩阵b中取得所需元素
		//相当于Matlab的bb(1:size(b,1),:) = b。
		bb(colon(1.0,1.0,size(b,1.0)),c_p) = b;
	} 
	else
	{
		//矩阵列长度大于或等于指定的变换点数
		//从输入矩阵b中取得所需元素
		//相当于Matlab的b = b(1:n,:)。
		bb = b(colon(1.0,1.0,n),c_p);
	}
	
    //计算用于IDFT计算的系数
	ww = msqrt(2.0*n)*transpose(exp(j*(colon(0.0,1.0,n-1.0))*pi/(2.0*n)));
	
	//进行变换
	if (istrue(rem(n,2.0)==1.0)||istrue(!isreal(b)))
	{
		//n为偶数
		//生成中间变换矩阵
		ww(1.0) = ww(1.0)*msqrt(2.0);
		W = ww(c_p,ones(1.0,m));
		//生成2.0*n×m的零矩阵
		yy = zeros(2.0*n,m);
		//相当于Matlab的yy(1:n,:) = W.*bb
		yy(colon(1.0,1.0,n),c_p) = times(W,bb);
		//相当于Matlab的yy(n+2:2*n,:) 。
		yy(colon(n+2.0,1.0,2.0*n),c_p) = -times(j*W(colon(2.0,1.0,n),c_p),flipud(bb(colon(2.0,1.0,n),c_p)));
		
		//调用Matrix<Lib>C++库函数IFFT()进行快速离散傅立叶逆变换，
		y = ifft(yy);
		
		//保留需要的变换结果数据
		//相当于Matlab的a = y(1:n,:)。
		a = y(colon(1.0,1.0,n),c_p);		
	}
	else
	{
		//n为奇数
		//计算校正系数
		ww(1.0) = ww(1.0)/msqrt(2.0);
		W = ww(c_p,ones(1.0,m));
		yy = times(W,bb);
		
		//调用Matrix<Lib>C++库函数IFFT()进行快速离散傅立叶逆变换，
		y = ifft(yy);
		
		//对矩阵的各列元素重新进行排序
		//生成n×m的零矩阵
		a = zeros(n,m);
		//相当于Matlab的a(1:2:n,:) = y(1:n/2,:)		
		a(colon(1.0,2.0,n),c_p) = y(colon(1.0,1.0,n/2.0),c_p);
		//相当于Matlab的a(2:2:n,:) = y(n:-1:n/2+1,:)
		a(colon(2.0,2.0,n),c_p) = y(colon(n,-1.0,n/2.0+1.0),c_p);
	}
	
	//如果输入矩阵是实数矩阵，输出取变换结果的实部。
	if (istrue(isreal(b)))
	{
		a = real(a);
	}
	//如果输入的是行向量，对结果进行转置操作。
	if (istrue(do_trans))
	{
		a = transpose(a);
	}
	
	nargin_val=old_nargin; nargout_val=old_nargout;

	b.setname(NULL); n.setname(NULL); 

	return a;
}

////////////////////////////////////////////////////////////////////////
//Mm idct()   
//----------------------------------------------------------------------
//基本功能：本函数为离散余弦变换的无参重载版本。
//----------------------------------------------------------------------
//参数说明：无
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::idct()
{
	begin_scope
		double old_nargin=nargin_val; nargin_val=0.0; nargin_set=1;
	dMm(b); dMm(n); 
	dMm(ret1a);
	ret1a=idct(b, n);
	nargin_val=old_nargin;
	return ret1a;
	end_scope
}

////////////////////////////////////////////////////////////////////////
//Mm idct()   
//----------------------------------------------------------------------
//基本功能：本函数为离散余弦变换的无指定变换长度重载版本。
//----------------------------------------------------------------------
//参数说明：Mm b	待变换向量（若为矩阵，按列进行运算）
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::idct(Mm b)
{
	begin_scope
		double old_nargin=nargin_val; nargin_val=1.0; nargin_set=1;
	dMm(n); 
	dMm(ret1a);
	ret1a=idct(b, n);
	nargin_val=old_nargin;
	return ret1a;
	end_scope
}

////////////////////////////////////////////////////////////////////////
//Mm idct2()   
//----------------------------------------------------------------------
//基本功能：本函数完成矩阵的一维离散余弦逆变换。
//----------------------------------------------------------------------
//参数说明：Mm arg1		待变换矩阵
//			Mm mrows	行变换长度，若指定该参数，则自动将向量a补0或自动
//						裁剪到指定长度。
//			Mm ncols	列变换长度，若指定该参数，则自动将向量a补0或自动
//						裁剪到指定长度。
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用。该函数是先对矩阵
//			按列进行一维离散余弦逆变换，再对其按列进行一维离散余弦逆变换，
//			最终得到其二维离散余弦逆变换结果。对程序在此不加详细注释，请
//			参考一维离散余弦逆变换程序注释。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::idct2(Mm arg1, Mm mrows, Mm ncols)
{
	arg1.setname("arg1"); mrows.setname("mrows"); ncols.setname("ncols"); 
	dMm(a); dMm(m); dMm(n); dMm(b); dMm(mpad); dMm(npad); 
	
	double old_nargin=nargin_val;
	if (!nargin_set) nargin_val=3.0;
	nargin_set=0;
	double old_nargout=nargout_val;
	if (!nargout_set) nargout_val=1.0;
	nargout_set=0;

	size(arg1,i_o,m,n);	

	if (istrue(nargin()==1.0))
	{
		if (istrue((m>1.0))&&istrue((n>1.0)))
		{
			a = transpose(idct(transpose(idct(arg1))));
			nargin_val=old_nargin; nargout_val=old_nargout;
			
			arg1.setname(NULL); mrows.setname(NULL); ncols.setname(NULL); 
			return a;
		}
		else
		{
			mrows = m;
			ncols = n;
		}
	}
	b = arg1;
	if (istrue(nargin()==2.0))
	{
		ncols = mrows(2.0);
		mrows = mrows(1.0);
	}
	mpad = mrows;
	npad = ncols;
	if (istrue(m==1.0)&&istrue(mpad>m))
	{
		b(2.0,1.0) = 0.0;
		m = 2.0;
	}
	if (istrue(n==1.0)&&istrue(npad>n))
	{
		b(1.0,2.0) = 0.0;
		n = 2.0;
	}
	if (istrue(m==1.0))
	{
		mpad = npad;
		npad = 1.0;
	}
	
	a = idct(b,mpad);
	if (istrue(m>1.0)&&istrue(n>1.0))
	{
		a = transpose(idct(transpose(a),npad));
	}
	
	nargin_val=old_nargin; nargout_val=old_nargout;
	
	arg1.setname(NULL); mrows.setname(NULL); ncols.setname(NULL); 
	return a;
}


////////////////////////////////////////////////////////////////////////
//Mm idct2()   
//----------------------------------------------------------------------
//基本功能：本函数为二维离散余弦逆变换的无指定变换长度重载版本。
//----------------------------------------------------------------------
//参数说明：Mm arg1	待变换矩阵
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::idct2(Mm arg1)
{
	begin_scope
		double old_nargin=nargin_val; nargin_val=1.0; nargin_set=1;
	dMm(mrows); dMm(ncols); 
	dMm(ret1a);
	ret1a=idct2(arg1, mrows, ncols);
	nargin_val=old_nargin;
	return ret1a;
	end_scope
}

////////////////////////////////////////////////////////////////////////
//Mm idct2()   
//----------------------------------------------------------------------
//基本功能：本函数为二维离散余弦逆变换的无指定列变换长度重载版本。
//----------------------------------------------------------------------
//参数说明：Mm arg1		待变换矩阵
//			Mm mrows	行变换长度，若指定该参数，则自动将向量a补0或自动
//						裁剪到指定长度。
//----------------------------------------------------------------------
//返    回：Mm
//			变换结果矩阵。
//----------------------------------------------------------------------
//注    意：此函数声明为保护型，只能在CFreqPro类中使用
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
Mm CFreqPro::idct2(Mm arg1, Mm mrows)
{
	begin_scope
		double old_nargin=nargin_val; nargin_val=2.0; nargin_set=1;
	dMm(ncols); 
	dMm(ret1a);
	ret1a=idct2(arg1, mrows, ncols);
	nargin_val=old_nargin;
	return ret1a;
	end_scope
}

////////////////////////////////////////////////////////////////////////
//BOOL NDCT2()   
//----------------------------------------------------------------------
//基本功能：本函数完成图像的快速余弦逆变换。
//----------------------------------------------------------------------
//参数说明：Mm matTransed	被变换的矩阵
//----------------------------------------------------------------------
//返    回：BOOL
//			成功时返回TRUE，失败时返回FALSE。
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CFreqPro::NDCT2(Mm matTransed)
{
	//若未指定 CDibObject 对象指针返回FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//调用函数idct2()完成二维离散余弦变换
	m_matBits = idct2(matTransed);

	//将矩阵数据赋给图像数据区
	SetBits();
	
	return(TRUE);
}


