#if !defined(AFX_DLGMORTH_H__BF4CF31D_823B_4F66_B929_D7E93FA947B9__INCLUDED_)
#define AFX_DLGMORTH_H__BF4CF31D_823B_4F66_B929_D7E93FA947B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMorth.h : header file
//
#include "ButtonST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMorth dialog

class CDlgMorth : public CDialog
{
// Construction
public:
	int m_nMaskLen;
	int *m_npMask;
	void UpdateEdit();
	CDlgMorth(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMorth)
	enum { IDD = IDD_Dlg_Morth };
	CButtonST	m_ctrlStruct9;
	CButtonST	m_ctrlStruct8;
	CButtonST	m_ctrlStruct7;
	CButtonST	m_ctrlStruct6;
	CButtonST	m_ctrlStruct5;
	CButtonST	m_ctrlStruct4;
	CButtonST	m_ctrlStruct3;
	CButtonST	m_ctrlStruct25;
	CButtonST	m_ctrlStruct24;
	CButtonST	m_ctrlStruct23;
	CButtonST	m_ctrlStruct22;
	CButtonST	m_ctrlStruct21;
	CButtonST	m_ctrlStruct20;
	CButtonST	m_ctrlStruct2;
	CButtonST	m_ctrlStruct19;
	CButtonST	m_ctrlStruct18;
	CButtonST	m_ctrlStruct17;
	CButtonST	m_ctrlStruct16;
	CButtonST	m_ctrlStruct15;
	CButtonST	m_ctrlStruct14;
	CButtonST	m_ctrlStruct13;
	CButtonST	m_ctrlStruct12;
	CButtonST	m_ctrlStruct11;
	CButtonST	m_ctrlStruct10;
	CButtonST	m_ctrlStruct1;
	CSpinButtonCtrl	m_ctrlStruct_Y;
	CSpinButtonCtrl	m_ctrlStruct_X;
	CSpinButtonCtrl	m_ctrlStructW;
	CSpinButtonCtrl	m_ctrlStructH;
	int		m_nMode;
	int		m_nStructH;
	int		m_nStructW;
	int		m_nStruct_X;
	int		m_nStruct_Y;
	BOOL	m_bStruct1;
	BOOL	m_bStruct2;
	BOOL	m_bStruct12;
	BOOL	m_bStruct13;
	BOOL	m_bStruct14;
	BOOL	m_bStruct15;
	BOOL	m_bStruct16;
	BOOL	m_bStruct17;
	BOOL	m_bStruct18;
	BOOL	m_bStruct19;
	BOOL	m_bStruct20;
	BOOL	m_bStruct21;
	BOOL	m_bStruct22;
	BOOL	m_bStruct23;
	BOOL	m_bStruct24;
	BOOL	m_bStruct25;
	BOOL	m_bStruct3;
	BOOL	m_bStruct4;
	BOOL	m_bStruct5;
	BOOL	m_bStruct6;
	BOOL	m_bStruct7;
	BOOL	m_bStruct8;
	BOOL	m_bStruct9;
	BOOL	m_bStruct10;
	BOOL	m_bStruct11;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMorth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMorth)
	afx_msg void OnDiamond();
	afx_msg void OnHorizon();
	afx_msg void OnVertical();
	afx_msg void OnCircle();
	afx_msg void OnDefineSelf();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditX();
	afx_msg void OnChangeEditY();
	afx_msg void OnChangeEditHeight();
	afx_msg void OnChangeEditWidth();
	afx_msg void OnKillfocusEditX();
	afx_msg void OnKillfocusEditY();
	afx_msg void OnKillfocusEditHeight();
	afx_msg void OnKillfocusEditWidth();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMORTH_H__BF4CF31D_823B_4F66_B929_D7E93FA947B9__INCLUDED_)
