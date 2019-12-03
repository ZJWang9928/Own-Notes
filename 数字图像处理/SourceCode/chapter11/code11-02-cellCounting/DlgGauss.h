#if !defined(AFX_DLGGAUSS_H__820D694A_13A1_4DF3_B892_46AD5DDBAF19__INCLUDED_)
#define AFX_DLGGAUSS_H__820D694A_13A1_4DF3_B892_46AD5DDBAF19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGauss.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGauss dialog

class CDlgGauss : public CDialog
{
// Construction
public:
	CDlgGauss(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGauss)
	enum { IDD = IDD_DIALOG2 };
	double	m_sigma;
	int		m_size;
	int		m_thresh;
	int		m_gausswidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGauss)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGauss)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGAUSS_H__820D694A_13A1_4DF3_B892_46AD5DDBAF19__INCLUDED_)
