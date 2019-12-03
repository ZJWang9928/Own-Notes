#if !defined(AFX_DLGINFO_H__5BADFDD3_EBF6_4F42_9CC1_33C5971BDA30__INCLUDED_)
#define AFX_DLGINFO_H__5BADFDD3_EBF6_4F42_9CC1_33C5971BDA30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo dialog

class CDlgInfo : public CDialog
{
// Construction
public:
	CDlgInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInfo)
	enum { IDD = IDD_Dlg_Info };
	int		m_nBits; //位图深度
	int		m_nHeight;//位图高度
	int		m_nWidth;//位图宽度
	int		m_nSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInfo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINFO_H__5BADFDD3_EBF6_4F42_9CC1_33C5971BDA30__INCLUDED_)
