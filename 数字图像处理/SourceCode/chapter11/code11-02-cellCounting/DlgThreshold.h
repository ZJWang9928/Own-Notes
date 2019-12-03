#if !defined(AFX_DLGTHRESHOLD_H__33399579_887D_4DD1_A431_194E0DA716DC__INCLUDED_)
#define AFX_DLGTHRESHOLD_H__33399579_887D_4DD1_A431_194E0DA716DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgThreshold.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgThreshold dialog

class CDlgThreshold : public CDialog
{
// Construction
public:
	int *m_pnHistogram;
	int m_ThresholdAuto;
	int m_nThresholdJustAnalyze;
	BOOL m_bIsSelf;
	CDlgThreshold(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgThreshold)
	enum { IDD = IDD_Dlg_Threshold };
	CEdit	m_ctrlEditThreshold;
	UINT	m_nCurrentThreshold;
	int		m_nThresholdType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgThreshold)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgThreshold)
	virtual BOOL OnInitDialog();
	afx_msg void OnThrJust();
	afx_msg void OnThrAnylysis();
	afx_msg void OnThrSelf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTHRESHOLD_H__33399579_887D_4DD1_A431_194E0DA716DC__INCLUDED_)
