#if !defined(AFX_PARAINPUT1_H__A36E4712_55EB_4D39_8096_C70D4755959F__INCLUDED_)
#define AFX_PARAINPUT1_H__A36E4712_55EB_4D39_8096_C70D4755959F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParaInput1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParaInput1 dialog

class CParaInput1 : public CDialog
{
// Construction
public:
	CParaInput1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParaInput1)
	enum { IDD = IDD_DIALOG1 };
	int		m_input1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaInput1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParaInput1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAINPUT1_H__A36E4712_55EB_4D39_8096_C70D4755959F__INCLUDED_)
