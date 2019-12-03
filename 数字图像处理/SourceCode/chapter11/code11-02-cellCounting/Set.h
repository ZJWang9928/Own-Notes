#if !defined(AFX_SET_H__96D0B02A_BE3A_470A_B5B6_2065256FC648__INCLUDED_)
#define AFX_SET_H__96D0B02A_BE3A_470A_B5B6_2065256FC648__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Set.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSet dialog

class CSet : public CDialog
{
// Construction
public:
	CSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSet)
	enum { IDD = IDD_SET };
	int		m_nPreCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSet)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SET_H__96D0B02A_BE3A_470A_B5B6_2065256FC648__INCLUDED_)
