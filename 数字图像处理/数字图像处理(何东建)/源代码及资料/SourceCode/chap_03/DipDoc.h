// DipDoc.h : interface of the CDipDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIPDOC_H__3976E474_EA92_4AC9_A01B_0A1461A0422A__INCLUDED_)
#define AFX_DIPDOC_H__3976E474_EA92_4AC9_A01B_0A1461A0422A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDipDoc : public CDocument
{
protected: // create from serialization only
	CDipDoc();
	DECLARE_DYNCREATE(CDipDoc)

// Attributes
public:
	BOOL m_bImageLoaded;
	CDibObject *m_pDibObject;

// Operations
public:
	BOOL ReadImgToDoc();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDipDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDipDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDipDoc)
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPDOC_H__3976E474_EA92_4AC9_A01B_0A1461A0422A__INCLUDED_)
