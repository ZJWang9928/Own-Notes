// DipDoc.h : interface of the CDipDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIPDOC_H__23C0C851_92CB_4683_A0C4_7AAD45B78F53__INCLUDED_)
#define AFX_DIPDOC_H__23C0C851_92CB_4683_A0C4_7AAD45B78F53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "matlib.h"

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
	Mm m_matTransed;
	BOOL m_bFft;
	BOOL m_bDct;
	BOOL m_bWht;

	BOOL m_bIsLabeled;
	BOOL m_bIsTraced;

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
	afx_msg void OnEdgeTrace();
	afx_msg void OnUpdateEdgeTrace(CCmdUI* pCmdUI);
	afx_msg void OnEdgeContour();
	afx_msg void OnEdgeFill();
	afx_msg void OnUpdateEdgeContour(CCmdUI* pCmdUI);
	afx_msg void OnDetectMinus();
	afx_msg void OnUpdateDetectMinus(CCmdUI* pCmdUI);
	afx_msg void OnDetectTemplate();
	afx_msg void OnUpdateDetectTemplate(CCmdUI* pCmdUI);
	afx_msg void OnDetectProject();
	afx_msg void OnUpdateDetectProject(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdgeFill(CCmdUI* pCmdUI);
	afx_msg void OnGeomMirh();
	afx_msg void OnGeomMirv();
	afx_msg void OnGeomRota();
	afx_msg void OnGeomTran();
	afx_msg void OnGeomTrpo();
	afx_msg void OnGeomZoom();
	afx_msg void OnUpdateGeomMirh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGeomMirv(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGeomRota(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGeomTran(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGeomTrpo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGeomZoom(CCmdUI* pCmdUI);
	afx_msg void OnFreqDct();
	afx_msg void OnFreqFft();
	afx_msg void OnFreqFftFilte();
	afx_msg void OnFreqNfft();
	afx_msg void OnFreqWalh();
	afx_msg void OnUpdateFreqDct(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFreqFftFilte(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFreqNfft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFreqWalh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFreqFft(CCmdUI* pCmdUI);
	afx_msg void OnFreqNdct();
	afx_msg void OnUpdateFreqNdct(CCmdUI* pCmdUI);
	afx_msg void OnFreqNwalh();
	afx_msg void OnUpdateFreqNwalh(CCmdUI* pCmdUI);
	afx_msg void OnMorphClose();
	afx_msg void OnMorphDilation();
	afx_msg void OnMorphErosion();
	afx_msg void OnMorphOpen();
	afx_msg void OnMorphThining();
	afx_msg void OnUpdateMorphClose(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMorphDilation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMorphErosion(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMorphOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMorphThining(CCmdUI* pCmdUI);
	afx_msg void OnMorphHitmissing();
	afx_msg void OnUpdateMorphHitmissing(CCmdUI* pCmdUI);
	afx_msg void OnAnalyseFeature();
	afx_msg void OnAnalyseLabel();
	afx_msg void OnAnalyseTrace();
	afx_msg void OnUpdateAnalyseFeature(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAnalyseLabel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAnalyseTrace(CCmdUI* pCmdUI);
	afx_msg void OnCodeArith();
	afx_msg void OnCodeHuffman();
	afx_msg void OnCodeShannon();
	afx_msg void OnUpdateCodeArith(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCodeHuffman(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCodeShannon(CCmdUI* pCmdUI);
	afx_msg void OnRestoreBlur();
	afx_msg void OnRestoreConvnoise();
	afx_msg void OnRestoreInvfilt();
	afx_msg void OnRestoreWinner();
	afx_msg void OnUpdateRestoreBlur(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRestoreConvnoise(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRestoreInvfilt(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRestoreWinner(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPDOC_H__23C0C851_92CB_4683_A0C4_7AAD45B78F53__INCLUDED_)
