// FreqPro.h: interface for the CFreqPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AREAPRO_H__AA3E7726_2EE5_40E0_8074_2039A3018333__INCLUDED_)
#define AFX_AREAPRO_H__AA3E7726_2EE5_40E0_8074_2039A3018333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "matlib.h"



class CFreqPro : public CObject  
{
	DECLARE_DYNCREATE(CFreqPro)
public:
	CFreqPro();
	CFreqPro(CDibObject *pDibObject);
	virtual ~CFreqPro();

public:
	//进行图像傅立叶变换
	Mm FFT2(CDibObject *pDibObject = NULL);
	//进行图像傅立叶逆变换
	BOOL NFFT2(Mm matTransed);

	//进行图像余弦变换
	Mm DCT2(CDibObject *pDibObject = NULL);
	//进行图像余弦逆变换
	BOOL NDCT2(Mm matTransed);

	//进行图像沃尔什-哈达玛变换
	Mm WHT2(CDibObject *pDibObject = NULL);
	//进行图像沃尔什-哈达玛逆变换
	BOOL NWHT2(Mm matTransed);

	//设置DibObject类对象
	void SetDibObjectClass( CDibObject *pDibObject );

protected:
	//为图像数据赋给矩阵变量
	Mm GetMatData();
	//为矩阵变量赋给图像数据
	BOOL SetBits();

//诊断调试函数
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

private:
	BOOL bIs2Power(int nNum);
	CDibObject *m_pDibObject;

protected:

	//完成一维离散余弦变换
	Mm dct(Mm a, Mm n);
	Mm dct(Mm a);
	Mm dct();

	//完成二维离散余弦变换
	Mm dct2(Mm arg1, Mm mrows, Mm ncols);
	Mm dct2(Mm arg1, Mm mrows);
	Mm dct2(Mm arg1);

	//完成一维离散余弦逆变换
	Mm idct(Mm b, Mm n);
	Mm idct(Mm b);
	Mm idct();

	//完成二维离散余弦逆变换
	Mm idct2(Mm arg1, Mm mrows, Mm ncols);
	Mm idct2(Mm arg1, Mm mrows);
	Mm idct2(Mm arg1);

	//内部中间变量
	Mm m_matBits, m_matFilter, m_matTransed;
;
};

#endif // !defined(AFX_AREAPRO_H__AA3E7726_2EE5_40E0_8074_2039A3018333__INCLUDED_)
