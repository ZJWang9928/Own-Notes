// DipDoc.h : interface of the CDipDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIPDOC_H__23C0C851_92CB_4683_A0C4_7AAD45B78F53__INCLUDED_)
#define AFX_DIPDOC_H__23C0C851_92CB_4683_A0C4_7AAD45B78F53__INCLUDED_

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
	int m_nColorIndex;

// Operations
public:
	BOOL LoadImageToDocument();

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
	long m_lHeight;
	long m_lWidth;

// Generated message map functions
protected:
	BOOL m_bBinary;
	//{{AFX_MSG(CDipDoc)
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnPointEqua();
	afx_msg void OnPointLiner();
	afx_msg void OnPointStre();
	afx_msg void OnPointContauto();
	afx_msg void OnPointThre();
	afx_msg void OnEnhaSmooth();
	afx_msg void OnPointPseudocolor();
	afx_msg void OnEnhaGradsharp();
	afx_msg void OnEnhaTempsharp();
	afx_msg void OnEnhaLapsharp();
	afx_msg void OnPointInvert();
	afx_msg void OnChannelSplit();
	afx_msg void OnUpdateChannelSplit(CCmdUI* pCmdUI);
	afx_msg void OnChannelBlue();
	afx_msg void OnUpdateChannelBlue(CCmdUI* pCmdUI);
	afx_msg void OnChannelGreen();
	afx_msg void OnChannelMixed();
	afx_msg void OnChannelRed();
	afx_msg void OnUpdateChannelGreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChannelMixed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChannelRed(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointThre(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePointPseudocolor(CCmdUI* pCmdUI);
	afx_msg void OnEnhaMidianF();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPDOC_H__23C0C851_92CB_4683_A0C4_7AAD45B78F53__INCLUDED_)
