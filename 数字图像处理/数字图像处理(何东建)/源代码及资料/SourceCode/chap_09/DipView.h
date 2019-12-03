// DipView.h : interface of the CDipView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIPVIEW_H__C724A826_122B_4204_9DC7_05C206B96AF9__INCLUDED_)
#define AFX_DIPVIEW_H__C724A826_122B_4204_9DC7_05C206B96AF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDipView : public CScrollView
{
protected: // create from serialization only
	CDipView();
	DECLARE_DYNCREATE(CDipView)

// Attributes
public:
	CDipDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDipView)
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
	virtual ~CDipView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDipView)
	afx_msg void OnViewHist();
	afx_msg void OnUpdateViewHist(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DipView.cpp
inline CDipDoc* CDipView::GetDocument()
   { return (CDipDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPVIEW_H__C724A826_122B_4204_9DC7_05C206B96AF9__INCLUDED_)
