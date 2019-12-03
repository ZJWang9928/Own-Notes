#if !defined(AFX_DLGSTRINPUT_H__D9D24A00_26CE_465E_B8D9_EFF5AFAF94F1__INCLUDED_)
#define AFX_DLGSTRINPUT_H__D9D24A00_26CE_465E_B8D9_EFF5AFAF94F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStrInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStrInput dialog

class CDlgStrInput : public CDialog
{
// Construction
public:
	CDlgStrInput(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgStrInput)
	enum { IDD = IDD_DLG_STRINPUT };
	CString	m_strInput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStrInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStrInput)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTRINPUT_H__D9D24A00_26CE_465E_B8D9_EFF5AFAF94F1__INCLUDED_)
