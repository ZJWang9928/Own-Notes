// AreaPro.h: interface for the CAreaPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AREAPRO_H__88D90509_32B8_464E_8420_B91A8F4652F8__INCLUDED_)
#define AFX_AREAPRO_H__88D90509_32B8_464E_8420_B91A8F4652F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DibObject.h"

class CAreaPro : public CObject  
{
	DECLARE_DYNCREATE(CAreaPro)
public:
	CAreaPro();
	CAreaPro(CDibObject *pDibObject);
	virtual ~CAreaPro();

public:
	//Laplacian锐化操作
	BOOL LapSharping(float *fpArray = NULL, float fCoef = 1, int nTempW = 3, 
				   int nTempH = 3, int nTempXc = 1, int nTempYc = 1, 
				   int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);
	BOOL GradSharping(int Threshold = 128, int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1,
		CDibObject *pDibObject = NULL);
	//平滑操作
	BOOL Smoothing(float *fpArray = NULL, float fCoef = 9, int nTempW = 3, 
				   int nTempH = 3, int nTempXc = 1, int nTempYc = 1, 
				   int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);
	//锐化操作
	BOOL Sharping(float *fpArray = NULL, int *nDirection = NULL, int nTempW = 3, 
				   int nTempH = 3, int nTempXc = 1, int nTempYc = 1, 
				   int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);
	//中值滤波
	BOOL MedianFilter(int nType = 0, int nTempW = 3, int nTempH = 3, int nTempXc = 1, 
		              int nTempYc = 1, int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);

	//设置DibObject类对象
	void SetDibObjectClass( CDibObject *pDibObject );
//诊断调试函数
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

protected:
	//对模板进行旋转（每执行一次模板顺时针旋转45°）
	void TemplateRotating(float *fpTemplate = NULL);

	//模板操作函数（仅用模板处理图像数据区）
	BOOL TemplateOperation(float *fpArray, float fCoef, int nTempW, int nTempH, int nTempXc, int nTempYc, 
		                   unsigned char *pData, int nWidthBytes, 
						   int nX1, int nY1, int nX2, int nY2);
	//获取直方图
	int *GetHistogram( int = -1, int = -1, int = -1, int = -1 );
	//创建直方图
	int *CreateHistogram( int, int, int, int, unsigned char *, RGBQUAD *, int );

private:
	unsigned char GetMedian(unsigned char *pData = NULL, int nSize = 9, int nType = 1);
	//卷积函数
	void Convolution(float *fpData, float *fpKernel, float fCoef, int nSize, unsigned char *nResult);
	CDibObject *m_pDibObject;

};

#endif // !defined(AFX_AREAPRO_H__88D90509_32B8_464E_8420_B91A8F4652F8__INCLUDED_)
