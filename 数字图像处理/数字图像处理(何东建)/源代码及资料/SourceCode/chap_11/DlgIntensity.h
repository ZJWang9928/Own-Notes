#if !defined(AFX_DLGINTENSITY_H__504BB8CE_7CF6_4D13_B5B7_DCC1BC84FEA5__INCLUDED_)
#define AFX_DLGINTENSITY_H__504BB8CE_7CF6_4D13_B5B7_DCC1BC84FEA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIntensity.h : header file
//
class CDibObject;
/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity dialog

class CDlgIntensity : public CDialog
{
// Construction
public:
	BOOL m_bIsGray256;
	//直方图数据指针
	int *m_pnHistogram;
	int *m_pHistTemp;
	
	// 当前鼠标拖动状态：	0表示未拖动，
	//						1表示正在拖动下限
	//						2表示正在拖动上限。
	int		m_iIsDraging;
	BOOL	m_bIsInRect;
	int		m_nGrayValue;
	
	// 相应鼠标事件的矩形区域
	CRect	m_MouseRect;
	
	// standard constructor
	CDlgIntensity(CWnd* pParent = NULL);   

	// Dialog Data
	//{{AFX_DATA(CDlgIntensity)
	enum { IDD = IDD_DLG_Intensity };
	CSpinButtonCtrl	m_ctrlUpGraySpin;
	CSpinButtonCtrl	m_ctrlLowGraySpin;
	CSpinButtonCtrl	m_ctrlCurrentGraySpin;
	int		m_iLowGray;
	int		m_iUpGray;
	UINT	m_nCurrentGray;
	long	m_nCurrentPiexsNum;
	int		m_nHistogramType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIntensity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CButton& GrayTypeRedRadio(){return *(CButton*)GetDlgItem(IDC_RED_RADIO);}
	CButton& GrayTypeGreenRadio(){return *(CButton*)GetDlgItem(IDC_GREEN_RADIO);}
	CButton& GrayTypeBlueRadio(){return *(CButton*)GetDlgItem(IDC_BLUE_RADIO);}

// Implementation
protected:
	int m_nPenType;
	BOOL m_bRMouseDown;
	CDibObject *pDibObject;
	// Generated message map functions
	//{{AFX_MSG(CDlgIntensity)
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChangeEDITLowGray();
	afx_msg void OnChangeEDITUpGray();
	afx_msg void OnChangeEditCurGray();
	afx_msg void OnHistogramtype();
	afx_msg void OnGreenRadio();
	afx_msg void OnRedRadio();
	afx_msg void OnBlueRadio();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINTENSITY_H__504BB8CE_7CF6_4D13_B5B7_DCC1BC84FEA5__INCLUDED_)
