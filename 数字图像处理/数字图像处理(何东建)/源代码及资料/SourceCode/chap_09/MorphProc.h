// MorphProc.h: interface for the CMorphPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MORPHPROC_H__DF1FC1F5_F917_4601_BE84_F740376CBE4B__INCLUDED_)
#define AFX_MORPHPROC_H__DF1FC1F5_F917_4601_BE84_F740376CBE4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DibObject.h"

class CMorphPro : public CObject  
{
	DECLARE_DYNCREATE(CMorphPro)
public:
	//无参构造函数
	CMorphPro();
	//有参构造函数
	CMorphPro(CDibObject *pDibObject);
	//析构函数
	virtual ~CMorphPro();

public:
	//设置DibObject类对象
	void SetDibObjectClass(CDibObject *pDibObject);

	//腐蚀运算
	BOOL Erosion(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//膨胀运算
	BOOL Dilation(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//开运算
	BOOL Opening(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//闭运算
	BOOL Closing(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//击中击不中变换
	BOOL HitMissTran(int *nHmask = NULL, int nHmaskLen = 9, 
				int *nMmask = NULL, int nMmaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//检测外边界
	BOOL OutEdge(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//检测内边界
	BOOL InEdge(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//检测形态学梯度
	BOOL MorphGrads(int *nMask = NULL, int nMaskLen = 9, 
				CDibObject *pDibObject = NULL);
	//薄化运算
	BOOL Thining(int *nHmask = NULL, int nHmaskLen = 9, 
				int *nMmask = NULL, int nMmaskLen = 9, 
				CDibObject *pDibObject = NULL,
				BOOL *bIsBlank = FALSE);
	//厚化运算
	BOOL Thicking(int *nHmask = NULL, int nHmaskLen = 9, 
				int *nMmask = NULL, int nMmaskLen = 9, 
				CDibObject *pDibObject = NULL);

	BOOL ThiningDIB(CDibObject *pDibObject=NULL);

protected:
	unsigned char *GetBitsPoint();
	//腐蚀运算
	BOOL MakeErosion(int *nMask, int nMaskLen, 
 					unsigned char *pOut, unsigned char *pIn,
					int nWidthBytes, int nWidth, int nHeight);
	//膨胀运算
	BOOL MakeDilation(int *nMask, int nMaskLen, 
 					unsigned char *pOut, unsigned char *pIn,
					int nWidthBytes, int nWidth, int nHeight);
	
	void AndBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	void ReverseBuffer(unsigned char *pIn, LONG lSize);
	void OrBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	void XorBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	void CopyBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	void MoveBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	void CleanBuffer(unsigned char *pIn, LONG lSize);
	BOOL IsResultBlank(unsigned char *pIn, LONG lSize);

//诊断调试函数
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

private:
	CDibObject *m_pDibObject;

};

#endif // !defined(AFX_MORPHPROC_H__DF1FC1F5_F917_4601_BE84_F740376CBE4B__INCLUDED_)
