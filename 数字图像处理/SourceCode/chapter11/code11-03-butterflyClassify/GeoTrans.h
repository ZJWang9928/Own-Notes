// GeoTrans.h: interface for the CGeoTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOTRANS_H__526243D9_08A2_47D0_87CA_6AB2414B073D__INCLUDED_)
#define AFX_GEOTRANS_H__526243D9_08A2_47D0_87CA_6AB2414B073D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DibObject.h"

// 常数π
#define PI 3.1415926535

//角度到弧度转化的宏
#define RADIAN(angle) ((angle)*PI/180.0) 

class CGeoTrans : public CObject  
{
	DECLARE_DYNCREATE(CGeoTrans)
public:
	//构造函数
	CGeoTrans();
	CGeoTrans( CDibObject * );
	//析构函数
	virtual ~CGeoTrans();
	//设置DibObject类对象
	void SetDibObjectClass( CDibObject * );

public:
	//图像旋转
	BOOL Rotate(int nRotateAngle = 0,  CDibObject *pDibObject = NULL);
	//图像缩放
	BOOL ZoomImage(float fXZoomRatio = 1.0, float fYZoomRatio = 1.0, CDibObject *pDibObject = NULL);
	//图像转置
	BOOL Transpose(CDibObject *pDibObject = NULL);
	//图像镜像
	BOOL Mirror(BOOL bDirection = TRUE, CDibObject *pDibObject=NULL);
	//图像平移
	BOOL TranslationPixel(long lXOffset = 0, long lYOffset =0, CDibObject *pDibObject = NULL);
	
//诊断调试函数
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

protected:
	CDibObject * m_pDibObject;
};

#endif // !defined(AFX_GEOTRANS_H__526243D9_08A2_47D0_87CA_6AB2414B073D__INCLUDED_)
