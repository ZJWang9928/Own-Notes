#if !defined(AFX_INPUT2_H__D5960788_3E7B_41EC_95A2_B39D18A0F3AA__INCLUDED_)
#define AFX_INPUT2_H__D5960788_3E7B_41EC_95A2_B39D18A0F3AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Input2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInput2 dialog

class CInput2 : public CDialog
{
// Construction
public:
	CInput2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInput2)
	enum { IDD = IDD_DIALOG2 };
	int		m_size;
	int		m_thresh;
	double	m_sigma;
	int		m_gausswidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInput2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInput2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUT2_H__D5960788_3E7B_41EC_95A2_B39D18A0F3AA__INCLUDED_)
