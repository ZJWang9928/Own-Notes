#if !defined(AFX_DLGRESULT_H__B250261B_45A1_4CFF_9267_964F970197E1__INCLUDED_)
#define AFX_DLGRESULT_H__B250261B_45A1_4CFF_9267_964F970197E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgResult dialog

class CDlgResult : public CDialog
{
// Construction
public:
	CDlgResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgResult)
	enum { IDD = IDD_Dlg_Result };
	CString	m_Result;
	float	m_nAntenna;
	float	m_nBelly;
	int		m_nSwing;
	float	m_nBellyFactor;
	float	m_nEccentricity;
	float	m_nArea;
	float	m_nSquare;
	CString	m_nWeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgResult)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRESULT_H__B250261B_45A1_4CFF_9267_964F970197E1__INCLUDED_)
