/**************************************************************************************
*
*   类    名：CRestore
*
*   功    能：实现图像复原
*
*   函数列表：ConvBlur()      对图像进行卷积模糊处理，生成一幅待复原的退化图像
*
*             InvFilter()     逆滤波复原图像，对由卷积模糊所造成的退化图像进行复原 
*             
*             ConvNoiseBlur() 对图像进行卷积噪声模糊处理，生成一幅待复原的退化图像
*
*             Wiener()        维纳滤波复原图像，对由卷积噪声所造成的退化图像进行复原
*
**************************************************************************************/





#include "matlib.h"



// Restore.h: interface for the CRestore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESTORE_H__617CFF60_A93F_49C1_8D14_71AB03BA3B8A__INCLUDED_)
#define AFX_RESTORE_H__617CFF60_A93F_49C1_8D14_71AB03BA3B8A__INCLUDED_

#include "DibObject.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "matlib.h"

class CRestore : public CObject  
{
public:

	BOOL ConvBlur();
	BOOL Wiener();
	BOOL ConvNoiseBlur();
	BOOL InvFilter();
	Mm m_matBits;
	Mm m_matTemplate;

	CDibObject *m_pDibObject;

	DECLARE_DYNCREATE(CRestore)
	CRestore();
	CRestore(CDibObject *pDibObject);
	virtual ~CRestore();
protected:
	
};

#endif // !defined(AFX_RESTORE_H__617CFF60_A93F_49C1_8D14_71AB03BA3B8A__INCLUDED_)
