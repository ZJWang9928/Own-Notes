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
	
	void MoveBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);

//诊断调试函数
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

private:
	CDibObject *m_pDibObject;

};

#endif // !defined(AFX_MORPHPROC_H__DF1FC1F5_F917_4601_BE84_F740376CBE4B__INCLUDED_)
