#if !defined(AFX_DLGMEDIANFILTER_H__E1EDCED9_6248_4A98_A55D_A606D901C610__INCLUDED_)
#define AFX_DLGMEDIANFILTER_H__E1EDCED9_6248_4A98_A55D_A606D901C610__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMedFilter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMedFilter dialog

class CDlgMedFilter : public CDialog
{
// Construction
public:
	void UpdateEdit();
	CDlgMedFilter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMedFilter)
	enum { IDD = IDD_DIg_MedianFilter };
	CSpinButtonCtrl	m_ctrlTempWSpin;
	CSpinButtonCtrl	m_ctrlTempHSpin;
	CSpinButtonCtrl	m_ctrlTempCSpin;
	CSpinButtonCtrl	m_ctrlMYSpin;
	CSpinButtonCtrl	m_ctrlMXSpin;
	int		m_iTempH;
	int		m_iTempW;
	int		m_iTempMX;
	int		m_iTempMY;
	int		m_nSortType;
	int		m_nWindowSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMedFilter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMedFilter)
	virtual BOOL OnInitDialog();
	afx_msg void OnRad1();
	afx_msg void OnRad2();
	afx_msg void OnRad3();
	afx_msg void OnRad4();
	virtual void OnOK();
	afx_msg void OnKillfocusEditTemph();
	afx_msg void OnKillfocusEditTempw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMEDIANFILTER_H__E1EDCED9_6248_4A98_A55D_A606D901C610__INCLUDED_)
