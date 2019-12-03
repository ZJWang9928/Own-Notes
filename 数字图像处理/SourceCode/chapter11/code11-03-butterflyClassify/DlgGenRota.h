#if !defined(AFX_DLGGENROTA_H__DABBA42C_9B57_410F_88B0_1011A5312B78__INCLUDED_)
#define AFX_DLGGENROTA_H__DABBA42C_9B57_410F_88B0_1011A5312B78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGenRota.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGenRota dialog

class CDlgGenRota : public CDialog
{
// Construction
public:
	CDlgGenRota(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGenRota)
	enum { IDD = IDD_Dlg_Rota };
	int		m_iRotateAngle;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGenRota)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGenRota)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGENROTA_H__DABBA42C_9B57_410F_88B0_1011A5312B78__INCLUDED_)
