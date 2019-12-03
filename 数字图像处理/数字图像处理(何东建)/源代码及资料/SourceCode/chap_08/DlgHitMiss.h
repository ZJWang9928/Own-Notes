#if !defined(AFX_DLGHITMISS_H__2D69B3D4_1FD1_4838_9EAB_41CF5B53C0A6__INCLUDED_)
#define AFX_DLGHITMISS_H__2D69B3D4_1FD1_4838_9EAB_41CF5B53C0A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHitMiss.h : header file

/////////////////////////////////////////////////////////////////////////////
// CDlgHitMiss dialog

class CDlgHitMiss : public CDialog
{
// Construction
public:

	int m_nHmaskLen;
	int *m_npHmask;
	int m_nMmaskLen;
	int *m_npMmask;

	CDlgHitMiss(CWnd* pParent = NULL);   // standard constructor

	void UpdateEdit();

// Dialog Data
	//{{AFX_DATA(CDlgHitMiss)
	enum { IDD = IDD_DLG_HitMiss };
	CButton	m_ctrlStru00;
	CButton	m_ctrlStru01;
	CButton	m_ctrlStru02;
	CButton	m_ctrlStru03;
	CButton	m_ctrlStru04;
	CButton	m_ctrlStru05;
	CButton	m_ctrlStru06;
	CButton	m_ctrlStru07;
	CButton	m_ctrlStru08;
	CButton	m_ctrlStru09;
	CButton	m_ctrlStru10;
	CButton	m_ctrlStru11;
	CButton	m_ctrlStru12;
	CButton	m_ctrlStru13;
	CButton	m_ctrlStru14;
	CButton	m_ctrlStru15;
	CButton	m_ctrlStru16;
	CButton	m_ctrlStru17;
	CButton	m_ctrlStru18;
	CButton	m_ctrlStru19;
	CButton	m_ctrlStru20;
	CButton	m_ctrlStru21;
	CButton	m_ctrlStru22;
	CButton	m_ctrlStru23;
	CButton	m_ctrlStru24;
	CSpinButtonCtrl	m_ctrlStruYCSpin;
	CSpinButtonCtrl	m_ctrlStruXCSpin;
	CSpinButtonCtrl	m_ctrlStruWSpin;
	CSpinButtonCtrl	m_ctrlStruHSpin;
	int		m_nStruH;
	int		m_nStruW;
	int		m_nStruXc;
	int		m_nStruYc;
	int	m_bStru00;
	int	m_bStru01;
	int	m_bStru02;
	int	m_bStru03;
	int	m_bStru04;
	int	m_bStru05;
	int	m_bStru06;
	int	m_bStru07;
	int	m_bStru08;
	int	m_bStru09;
	int	m_bStru10;
	int	m_bStru11;
	int	m_bStru12;
	int	m_bStru13;
	int	m_bStru14;
	int	m_bStru15;
	int	m_bStru16;
	int	m_bStru17;
	int	m_bStru18;
	int	m_bStru19;
	int	m_bStru20;
	int	m_bStru21;
	int	m_bStru22;
	int	m_bStru23;
	int	m_bStru24;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHitMiss)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHitMiss)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusEditStruH();
	afx_msg void OnKillfocusEditStruW();
	afx_msg void OnKillfocusEditStruXc();
	afx_msg void OnKillfocusEditStruYc();
	afx_msg void OnChangeEditStruH();
	afx_msg void OnChangeEditStruW();
	afx_msg void OnChangeEditStruXc();
	afx_msg void OnChangeEditStruYc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHITMISS_H__2D69B3D4_1FD1_4838_9EAB_41CF5B53C0A6__INCLUDED_)
