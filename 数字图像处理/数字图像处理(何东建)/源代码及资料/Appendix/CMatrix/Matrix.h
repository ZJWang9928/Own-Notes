// CMatrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMATRIX_H__CC2F3E15_9F82_11D4_A203_0000E81BB9DD__INCLUDED_)
#define AFX_CMATRIX_H__CC2F3E15_9F82_11D4_A203_0000E81BB9DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//用TYPE来定义double数据类型
#define TYPE double

class CMatrix  
{
public:
	//构造函数
	//无参构造函数
	CMatrix();
	//利用参数RowNumber和ColumnNumber构造矩阵
	CMatrix(int RowNumber, int ColumnNumber);
	//拷贝构造函数，利用传入的矩阵Matrix拷贝构造另一个矩阵
	CMatrix(CMatrix& Matrix);

	//析构函数
	virtual ~CMatrix();

	//用指定的RowNumber和ColumnNumber创建矩阵
	Create(int RowNumber, int ColumnNumber);

	//获得矩阵的行数
	int GetRowNumber();
	//获得矩阵的列数
	int GetColumnNumber();

	//操作符重载
	// * 操作符
	CMatrix operator *(CMatrix &Num2);
	// [] 操作符
	TYPE* CMatrix::operator[](int Row);
	// *= 操作符
	void CMatrix::operator *=(TYPE Number);
	// += 操作符
	void CMatrix::operator +=(CMatrix& Other);
	// = 操作符
	void CMatrix::operator =(CMatrix& Other);
	// + 操作符
	CMatrix CMatrix::operator +(CMatrix &Other);

	//用于调试的转储函数
	void Dump();
	void Fill(TYPE FillNumber);
	//生成单位阵
	void MakeUnit();
private:
	//矩阵列数
	int ColumnNumber;
	//矩阵行数
	int RowNumber;
	//矩阵元素
	TYPE *data;
};

#endif // !defined(AFX_CMATRIX_H__CC2F3E15_9F82_11D4_A203_0000E81BB9DD__INCLUDED_)
