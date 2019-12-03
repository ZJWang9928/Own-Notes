#ifndef FOURIOR
#define FOURIOR

//复数计算类
class CComplex
{
protected:
	//实部和虚部
	double Re,Im;
public:
	//构造函数
	CComplex();
	CComplex(double x,double y);
	//析构函数
	~CComplex();
	//获得实部
	double GetRe();
	//获得虚部	
	double GetIm();
	//=号操作符重载
	void operator=(CComplex &cm);
	//+=号操作符重载
	void operator+=(CComplex &cm);
	//-=号操作符重载
	void operator-=(CComplex &cm);
	//*=号操作符重载
	void operator*=(CComplex &cm);
	//*号操作符重载
	CComplex operator*(CComplex &cm);
	//*=号操作符重载
	void operator*=(float var);
	// /=号操作符重载
	void operator/=(double x);
	// /号操作符重载
	CComplex operator/(double x);
	// +号操作符重载
	CComplex operator+(CComplex &cm);
	// -号操作符重载
	CComplex operator-(CComplex &cm);
	//求模运算
	double abs();
};

////////////////////////////////////////////////////

//傅立叶变换类
class CFourior
{
private:
	CComplex *Wn;
	BYTE *bWn;
	BOOL bFlag;
	int nBitsNum;
	int nByteNum;
public:
	//构造函数
	CFourior();
	CFourior(int N);
	//析构函数
	~CFourior();
	//DFT变换
	void DFT(CComplex *Input);
	//FFT变换
	void FFT(CComplex *Input);
	//求共轭
	void SetInverseDFT();
	void SetInverseFFT();
	//设置傅立叶变换数据点数
	void SetCount(int N);
	//设置快速傅立叶变换系数
	BOOL SetFftWn();
	//设置傅立叶变换系数
	BOOL SetDftWn();
protected:
};

#endif
