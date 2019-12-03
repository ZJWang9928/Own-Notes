#if !defined(AFX_DLGGENZOOM_H__6E35BBF9_C532_42ED_BF75_E86FD3516ADD__INCLUDED_)
#define AFX_DLGGENZOOM_H__6E35BBF9_C532_42ED_BF75_E86FD3516ADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGenZoom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGenZoom dialog

class CDlgGenZoom : public CDialog
{
// Construction
public:
	CDlgGenZoom(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGenZoom)
	enum { IDD = IDD_Dlg_Zoom };
	float	m_XZoom;
	float	m_YZoom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGenZoom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGenZoom)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGENZOOM_H__6E35BBF9_C532_42ED_BF75_E86FD3516ADD__INCLUDED_)
