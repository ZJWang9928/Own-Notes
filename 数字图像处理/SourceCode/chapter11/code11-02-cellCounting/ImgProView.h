// ImgProView.h : interface of the CImgProView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGPROVIEW_H__0350DE52_BC57_4916_9005_94C038BCB5A9__INCLUDED_)
#define AFX_IMGPROVIEW_H__0350DE52_BC57_4916_9005_94C038BCB5A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImgProView : public CScrollView
{
protected: // create from serialization only
	CImgProView();
	DECLARE_DYNCREATE(CImgProView)

// Attributes
public:
	CImgProDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgProView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:

	CDibObject * m_pDibObject;
	virtual ~CImgProView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImgProView)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSalientDisplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImgProView.cpp
inline CImgProDoc* CImgProView::GetDocument()
   { return (CImgProDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGPROVIEW_H__0350DE52_BC57_4916_9005_94C038BCB5A9__INCLUDED_)
