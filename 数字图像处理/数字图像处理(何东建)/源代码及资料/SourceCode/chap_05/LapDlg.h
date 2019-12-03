#if !defined(AFX_LAPDLG_H__7A64182D_D9FA_4068_A6FB_674CE0E86724__INCLUDED_)
#define AFX_LAPDLG_H__7A64182D_D9FA_4068_A6FB_674CE0E86724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LapDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLapDlg dialog

class CLapDlg : public CDialog
{
// Construction
public:
	void UpdateEdit();
	
	// 模板元素数组指针
	float * m_fpArray;

	CLapDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLapDlg)
	enum { IDD = IDD_DLG_LaplacianDet };
	CSpinButtonCtrl	m_ctrlTempWSpin;
	CSpinButtonCtrl	m_ctrlTempHSpin;
	CSpinButtonCtrl	m_ctrlTempCSpin;
	CSpinButtonCtrl	m_ctrlMYSpin;
	CSpinButtonCtrl	m_ctrlMXSpin;


	// 模板类型
	int		m_intType;
	
	// 模板高度
	int		m_iTempH;
	
	// 模板宽度
	int		m_iTempW;
	
	// 模板中心元素X坐标
	int		m_iTempMX;
	
	// 模板中心元素Y坐标
	int		m_iTempMY;
	
	// 模板系数
	float	m_fTempC;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLapDlg)
	virtual void OnOK();
	afx_msg void OnKillfocusEditTemph();
	afx_msg void OnKillfocusEditTempw();
	afx_msg void OnRad1();
	afx_msg void OnRad2();
	afx_msg void OnRad3();
	afx_msg void OnRad4();
	afx_msg void OnRad5();
	afx_msg void OnRad6();
	afx_msg void OnRad7();
	afx_msg void OnRad8();
	virtual BOOL OnInitDialog();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAPDLG_H__7A64182D_D9FA_4068_A6FB_674CE0E86724__INCLUDED_)
