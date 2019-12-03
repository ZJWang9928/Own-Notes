#if !defined(AFX_CDlgMorph_H__82B6CF03_D5E6_11D4_9AF2_0000B4C23285__INCLUDED_)
#define AFX_CDlgMorph_H__82B6CF03_D5E6_11D4_9AF2_0000B4C23285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgMorph.h : header file
//
#include "BtnSt.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMorph dialog

class CDlgMorph : public CDialog
{
// Construction
public:
	int m_nMaskLen;
	int *m_npMask;
	CDlgMorph(CWnd* pParent = NULL);   // standard constructor

	void UpdateEdit();

// Dialog Data
	//{{AFX_DATA(CDlgMorph)
	enum { IDD = IDD_DLG_MORPH };
	CButtonST	m_ctrlStru00;
	CButtonST	m_ctrlStru01;
	CButtonST	m_ctrlStru02;
	CButtonST	m_ctrlStru03;
	CButtonST	m_ctrlStru04;
	CButtonST	m_ctrlStru05;
	CButtonST	m_ctrlStru06;
	CButtonST	m_ctrlStru07;
	CButtonST	m_ctrlStru08;
	CButtonST	m_ctrlStru09;
	CButtonST	m_ctrlStru10;
	CButtonST	m_ctrlStru11;
	CButtonST	m_ctrlStru12;
	CButtonST	m_ctrlStru13;
	CButtonST	m_ctrlStru14;
	CButtonST	m_ctrlStru15;
	CButtonST	m_ctrlStru16;
	CButtonST	m_ctrlStru17;
	CButtonST	m_ctrlStru18;
	CButtonST	m_ctrlStru19;
	CButtonST	m_ctrlStru20;
	CButtonST	m_ctrlStru21;
	CButtonST	m_ctrlStru22;
	CButtonST	m_ctrlStru23;
	CButtonST	m_ctrlStru24;
	CSpinButtonCtrl	m_ctrlStruYCSpin;
	CSpinButtonCtrl	m_ctrlStruXCSpin;
	CSpinButtonCtrl	m_ctrlStruWSpin;
	CSpinButtonCtrl	m_ctrlStruHSpin;
	int		m_nStruH;
	int		m_nStruW;
	int		m_nStruXc;
	int		m_nStruYc;
	BOOL	m_bStru00;
	BOOL	m_bStru01;
	BOOL	m_bStru02;
	BOOL	m_bStru03;
	BOOL	m_bStru04;
	BOOL	m_bStru05;
	BOOL	m_bStru06;
	BOOL	m_bStru07;
	BOOL	m_bStru08;
	BOOL	m_bStru09;
	BOOL	m_bStru10;
	BOOL	m_bStru11;
	BOOL	m_bStru12;
	BOOL	m_bStru13;
	BOOL	m_bStru14;
	BOOL	m_bStru15;
	BOOL	m_bStru16;
	BOOL	m_bStru17;
	BOOL	m_bStru18;
	BOOL	m_bStru19;
	BOOL	m_bStru20;
	BOOL	m_bStru21;
	BOOL	m_bStru22;
	BOOL	m_bStru23;
	BOOL	m_bStru24;
	int		m_nMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMorph)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMorph)
	afx_msg void OnCustom();
	afx_msg void OnVert();
	afx_msg void OnHori();
	virtual BOOL OnInitDialog();
	afx_msg void OnSquare();
	afx_msg void OnRect();
	afx_msg void OnDiamond();
	afx_msg void OnCircle();
	afx_msg void OnKillfocusEditStruH();
	afx_msg void OnKillfocusEditStruW();
	afx_msg void OnKillfocusEditStruXc();
	afx_msg void OnKillfocusEditStruYc();
	virtual void OnOK();
	afx_msg void OnChangeEditStruXc();
	afx_msg void OnChangeEditStruYc();
	afx_msg void OnChangeEditStruH();
	afx_msg void OnChangeEditStruW();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDlgMorph_H__82B6CF03_D5E6_11D4_9AF2_0000B4C23285__INCLUDED_)
