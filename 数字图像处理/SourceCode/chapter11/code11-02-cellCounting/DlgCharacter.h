#if !defined(AFX_DLGCHARACTER_H__F8FBA583_DFC5_4A99_BAC8_73B78CE30693__INCLUDED_)
#define AFX_DLGCHARACTER_H__F8FBA583_DFC5_4A99_BAC8_73B78CE30693__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCharacter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCharacter dialog

class CDlgCharacter : public CDialog
{
// Construction
public:
	CDlgCharacter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCharacter)
	enum { IDD = IDD_Dlg_Character };
	float	m_nAntenna;
	float	m_nBelly;
	int		m_nSwing;
	float	m_nEccentricity;
	float	m_nArea;
	float	m_nPerimeter;
	float	m_nCircularity;
	float	m_nFormFactor;
	int		m_Center_x;
	int		m_Center_y;
	float	m_Center_Square;
	float	m_Square;
	int		m_Heights;
	int		m_Widths;
	float	m_Indense;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCharacter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCharacter)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHARACTER_H__F8FBA583_DFC5_4A99_BAC8_73B78CE30693__INCLUDED_)
