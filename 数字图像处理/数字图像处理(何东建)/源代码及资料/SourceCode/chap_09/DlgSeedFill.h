#if !defined(AFX_DLGSEEDFILL_H__CC4CA8E2_8AC0_450A_A185_4D8B8CFA23F2__INCLUDED_)
#define AFX_DLGSEEDFILL_H__CC4CA8E2_8AC0_450A_A185_4D8B8CFA23F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSeedFill.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSeedFill dialog

class CDlgSeedFill : public CDialog
{
// Construction
public:
	CDlgSeedFill(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSeedFill)
	enum { IDD = IDD_DLG_SEEDFILL };
	int		m_iSeedMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSeedFill)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSeedFill)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSEEDFILL_H__CC4CA8E2_8AC0_450A_A185_4D8B8CFA23F2__INCLUDED_)
