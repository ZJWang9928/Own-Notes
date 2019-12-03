#if !defined(AFX_DLGSHANNON_H__478D0D9F_E920_496E_8EC9_DE3453BEB7BB__INCLUDED_)
#define AFX_DLGSHANNON_H__478D0D9F_E920_496E_8EC9_DE3453BEB7BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShannon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon dialog

class CDlgShannon : public CDialog
{
// Construction
public:
	void ShannonCode();
	// 灰度级别数目
	int m_iColorNum;
	
	// 各个灰度值出现频率
	FLOAT *	m_fFreq;
	
	//频率不为0的各灰度的映射关系
	BYTE *m_uMap;
	
	// 香农－弗诺编码表
	CString	* m_strCode;

	CDlgShannon(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgShannon)
	enum { IDD = IDD_DLG_Shannon };
	CListCtrl	m_lstTable;
	double	m_dEfficiency;
	double	m_dAvgCodeLen;
	double	m_dEntropy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShannon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgShannon)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHANNON_H__478D0D9F_E920_496E_8EC9_DE3453BEB7BB__INCLUDED_)
