// PointPro.h: interface for the CPointPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTPRO_H__EEBA051A_B9D4_4C71_8ADC_DE79C869C8E4__INCLUDED_)
#define AFX_POINTPRO_H__EEBA051A_B9D4_4C71_8ADC_DE79C869C8E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPointPro : public CObject  
{
	DECLARE_DYNCREATE(CPointPro)
public:
	BOOL IsBlackandWhite256(CDibObject *pDibObject = NULL);
	BOOL ReplaceColorPal(CDibObject *pDibObject , BYTE *bpColorsTable);
	BOOL InteEqualize(CDibObject *pDibObject = NULL);

	BOOL LinerTran(float fa = 1.0, float fb = 0.0, int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1, CDibObject *pDibObject = NULL);

	BOOL GrayStretch(BYTE bX1 = 0, BYTE bY1 = 0, BYTE bX2 = 255, BYTE bY2 = 255, int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1, CDibObject *pDibObject = NULL );
	BOOL IsGray256( CDibObject *pDibObject = NULL );
	int *CreateHueHist(int Thresh, CDibObject *pDibObject = NULL );
	BOOL Threshold(int Thresh = 128, int Mode = 1, BOOL bSetPalette = TRUE, CDibObject *pDibObject = NULL);
	int FindThresh_Auto( CDibObject *pDibObject = NULL );
	int FindThresh_Discriminant( CDibObject *pDibObject = NULL );
	CPointPro();
	CPointPro( CDibObject * );

	BOOL MakeGray256( BYTE mGrayType = 0, CDibObject *pDibObject = NULL );

	void SetDibObjectClass( CDibObject * );

	int *GetHistogram( int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1, CDibObject *pDibObject = NULL );

	//通道操作

	//分离通道
	BOOL SplitChannels(CDibObject *pRed, CDibObject *pGreen, CDibObject *pBlue);
	//合并通道
	BOOL CombineChannels(CDibObject *pRed, CDibObject *pGreen, CDibObject *pBlue);
	//获取合成通道
	BOOL GetMixedChannel(CDibObject *pMixed);
	//获取红色通道
	BOOL GetRedChannel(CDibObject *pRed);
	//获取绿色通道
	BOOL GetGreenChannel(CDibObject *pGreen);
	//获取蓝色通道
	BOOL GetBlueChannel(CDibObject *pBlue);
	


//诊断调试函数
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

protected:

	int *CreateHistogram( int, int, int, int, unsigned char *, RGBQUAD *, int, CDibObject *pDibObject = NULL );

	CDibObject *m_pDibObject;

private:
	unsigned char * DoublePointLinerTran(BYTE bX1 = 0, BYTE bY1 = 0, BYTE bX2 = 255, BYTE bY2 = 255);
	unsigned char *PointSlopeLinerTran(float fa, float fb);
};

#endif // !defined(AFX_POINTPRO_H__EEBA051A_B9D4_4C71_8ADC_DE79C869C8E4__INCLUDED_)
