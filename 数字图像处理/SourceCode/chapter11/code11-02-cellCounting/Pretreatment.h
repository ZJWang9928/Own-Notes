// Pretreatment.h: interface for the CPretreatment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRETREATMENT_H__6AB836AB_4FEE_4A8D_A0BA_C0B4C139E089__INCLUDED_)
#define AFX_PRETREATMENT_H__6AB836AB_4FEE_4A8D_A0BA_C0B4C139E089__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DibObject.h"

class CPretreatment : public CDibObject  
{
	
public:
	int finalcount;
	int *area;	

	void CountNumber();
	void FillHole(CDibObject * m_pDibObject);
	BOOL IsBlackandWhite256(CDibObject *pDibObject=NULL);
	int ThresholdJustAnlyze(CDibObject *pDibObject=NULL );
	BOOL Threshold(int Thresh = 128, int Mode = 1, BOOL bSetPalette = TRUE, CDibObject *pDibObject = NULL);
	int ThresholdAuto(CDibObject *pDibObject=NULL);
	BOOL TemplateSmooth(float *fpArray = NULL, float fCoef = 9, int nTempW = 3, int nTempH = 3, int nTempXc = 1, int nTempYc = 1,int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);
	BOOL MedianFilter(int nType = 0, int nTempW = 3, int nTempH = 3, int nTempXc = 1, 
		              int nTempYc = 1, int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);
	
	int * GetHistogram(int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1, CDibObject *pDibObject = NULL );
	BOOL AutoLight(BYTE bX1 = 0, BYTE bY1 = 0, BYTE bX2 = 255, BYTE bY2 = 255, int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1, CDibObject *pDibObject = NULL );
	BOOL LinerTran(float fa = 1.0, float fb = 0.0, int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1, CDibObject *pDibObject = NULL);

	//LaplacianÈñ»¯²Ù×÷
	BOOL LapSharping(float *fpArray = NULL, float fCoef = 1, int nTempW = 3, 
				   int nTempH = 3, int nTempXc = 1, int nTempYc = 1, 
				   int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);
	BOOL GradSharping(int Threshold = 128, int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1,
		CDibObject *pDibObject = NULL);
	BOOL MakeGray(CDibObject *pDibObject);

	BOOL IsGrayImage(CDibObject *pDibObject=NULL);
	CPretreatment();
	CPretreatment(CDibObject *);

	virtual ~CPretreatment();
protected:
	int * CreateHistogram( int, int, int, int, unsigned char *, RGBQUAD *, int, CDibObject *pDibObject = NULL );
	BOOL TemplateOperation(float *fpArray, float fCoef, int nTempW, int nTempH, int nTempXc, int nTempYc, 
		                   unsigned char *pData, int nWidthBytes, 
						   int nX1, int nY1, int nX2, int nY2);
		void Convolution(float *fpData, float *fpKernel, float fCoef, int nSize, unsigned char *nResult);
	CPretreatment *m_pDibObject;
};

#endif 
// !defined(AFX_PRETREATMENT_H__6AB836AB_4FEE_4A8D_A0BA_C0B4C139E089__INCLUDED_)
