#if !defined(AFX_DLGTEMPSMOOTH_H__2F993D02_AE6C_47AA_B088_B3A84AF6AD9B__INCLUDED_)
#define AFX_DLGTEMPSMOOTH_H__2F993D02_AE6C_47AA_B088_B3A84AF6AD9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTempSmooth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTempSmooth dialog

class CDlgTempSmooth : public CDialog
{
// Construction
public:
	void UpdateEdit();
	float *m_fpArray;
	CDlgTempSmooth(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTempSmooth)
	enum { IDD = IDD_TemplateSmooth };
	
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
	//{{AFX_VIRTUAL(CDlgTempSmooth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTempSmooth)
	afx_msg void OnTemp1();
	afx_msg void OnTemp2();
	afx_msg void OnTemp3();
	afx_msg void OnChangeEditTempw();
	virtual void OnOK();
	afx_msg void OnKillfocusEditTemph();
	afx_msg void OnKillfocusEditTempw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEMPSMOOTH_H__2F993D02_AE6C_47AA_B088_B3A84AF6AD9B__INCLUDED_)
