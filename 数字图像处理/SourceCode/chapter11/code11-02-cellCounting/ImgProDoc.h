// ImgProDoc.h : interface of the CImgProDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGPRODOC_H__4698CFA3_78B1_4166_AAD6_C4A6DD371FC7__INCLUDED_)
#define AFX_IMGPRODOC_H__4698CFA3_78B1_4166_AAD6_C4A6DD371FC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImgProDoc : public CDocument
{
protected: // create from serialization only
	CImgProDoc();
	DECLARE_DYNCREATE(CImgProDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgProDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	double * difference(double * ,int ,int ,double *,int ,int );
	BOOL ReadImgToDoc();
	BOOL m_bImageLoaded;
	CDibObject *m_pDibObject;
	float depart_factor;
	bool * corner;
	virtual ~CImgProDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
		BOOL m_bBinary;
// Generated message map functions
protected:

	//{{AFX_MSG(CImgProDoc)
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnConvert();
	afx_msg void OnAutoLight();
	afx_msg void OnBellySep();
	afx_msg void OnCharacter();
	afx_msg void OnIdentify();
	afx_msg void OnLapulasiSharp();
	afx_msg void OnLineLight();
	afx_msg void OnMidianTemp();
	afx_msg void OnTiduSharp();
	afx_msg void OnWingSep();
	afx_msg void OnAntennaSep();
	afx_msg void OnTempSmooth();
	afx_msg void OnThresholdSep();
	afx_msg void OnDelate();
	afx_msg void OnCancleErsion();
	afx_msg void OnMorphOpen();
	afx_msg void OnMorphClose();
	afx_msg void OnGeomZoom();
	afx_msg void OnGeomMirh();
	afx_msg void OnGeomTran();
	afx_msg void OnGeomMirv();
	afx_msg void OnGeomTrpo();
	afx_msg void OnGeomRota();
	afx_msg void OnInfo();
	afx_msg void OnThin();
	afx_msg void OnAnteenaFactor();
	afx_msg void OnBellyFactor();
	afx_msg void OnAnteenaAvgFactor();
	afx_msg void OnBellyAvgFactor();
	afx_msg void OnContourTrace();
	afx_msg void OnEdgeTrace();
	afx_msg void OnUpdateAntennaSep(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBellySep(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCancleErsion(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCharacter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateContourTrace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdgeTrace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdentify(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLapulasiSharp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTiduSharp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThresholdSep(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTempSmooth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMidianTemp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMorphClose(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWingSep(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMorphOpen(CCmdUI* pCmdUI);
	afx_msg void OnHelp();
	afx_msg void OnSalientPoint();
	afx_msg void OnSalientDisplay();
	afx_msg void OnFull();
	afx_msg void OnCountNumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGPRODOC_H__4698CFA3_78B1_4166_AAD6_C4A6DD371FC7__INCLUDED_)
