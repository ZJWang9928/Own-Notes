#if !defined(AFX_EDGEDETDLG_H__E0B9A38C_8860_4448_9822_CB07FC17A4D1__INCLUDED_)
#define AFX_EDGEDETDLG_H__E0B9A38C_8860_4448_9822_CB07FC17A4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdgeDetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEdgeDetDlg dialog

class CEdgeDetDlg : public CDialog
{
// Construction
public:
	void UpdateEdit();
	
	// 模板元素数组指针
	float * m_fpArray;

	// 模板方向数组指针
	int *m_nDirection;


	CEdgeDetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEdgeDetDlg)
	enum { IDD = IDD_DLG_EdgeDet };
	CSpinButtonCtrl	m_ctrlTempWSpin;
	CSpinButtonCtrl	m_ctrlTempHSpin;
	CSpinButtonCtrl	m_ctrlTempCSpin;
	CSpinButtonCtrl	m_ctrlMYSpin;
	CSpinButtonCtrl	m_ctrlMXSpin;
	int		m_intType;
	int		m_iTempH;
	int		m_iTempW;
	int		m_iTempMX;
	int		m_iTempMY;
	float	m_fTempC;
	BOOL	m_bDirection0;
	BOOL	m_bDirection1;
	BOOL	m_bDirection2;
	BOOL	m_bDirection3;
	BOOL	m_bDirection4;
	BOOL	m_bDirection5;
	BOOL	m_bDirection6;
	BOOL	m_bDirection7;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdgeDetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEdgeDetDlg)
	virtual void OnOK();
	afx_msg void OnKillfocusEditTemph();
	afx_msg void OnKillfocusEditTempw();
	afx_msg void OnRad1();
	afx_msg void OnRad2();
	afx_msg void OnRad3();
	afx_msg void OnRad4();
	afx_msg void OnRad5();
	afx_msg void OnRad6();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDGEDETDLG_H__E0B9A38C_8860_4448_9822_CB07FC17A4D1__INCLUDED_)
