#if !defined(AFX_DLGFEATURE_H__DBE65A42_A91B_4140_930B_6E25653E1A79__INCLUDED_)
#define AFX_DLGFEATURE_H__DBE65A42_A91B_4140_930B_6E25653E1A79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFeature.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFeature dialog

class CDlgFeature : public CDialog
{
// Construction
public:
	BOOL GetFourier();
	int m_iNumPoints;
	//计算各参数
	void CalPara(float * X_Scale,float *YScale,float * XY_Scale);
	CDlgFeature(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFeature)
	enum { IDD = IDD_DLG_FEATURE };
	float	m_fXScale;
	float	m_fXYScale;
	float	m_fYScale;
	float	m_fPermeter;
	float	m_fCompact;
	float	m_fArea;
	BOOL	m_IsUseCurValue;
	int		m_SelOpt;
	CString	m_strCxAndCy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFeature)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	double * m_Fourier;//傅立叶描述子数据指针

	// Generated message map functions
	//{{AFX_MSG(CDlgFeature)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnRadioCustom();
	afx_msg void OnCheckRetain();
	afx_msg void OnRadioRecalib();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFEATURE_H__DBE65A42_A91B_4140_930B_6E25653E1A79__INCLUDED_)
