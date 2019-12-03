#if !defined(AFX_DLGFOURIORFILTE_H__55D1FE58_1B1B_455D_92C9_01E54A3AF89D__INCLUDED_)
#define AFX_DLGFOURIORFILTE_H__55D1FE58_1B1B_455D_92C9_01E54A3AF89D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFouriorFilte.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFouriorFilte dialog

class CDlgFouriorFilte : public CDialog
{
// Construction
public:
	CDlgFouriorFilte(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFouriorFilte)
	enum { IDD = IDD_DLG_Fourior_Filter };
	int		m_nFilteType;
	double	m_nFitleParaA;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFouriorFilte)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFouriorFilte)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFOURIORFILTE_H__55D1FE58_1B1B_455D_92C9_01E54A3AF89D__INCLUDED_)
