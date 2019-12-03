// DlgMedFilter.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "DlgMedFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMedFilter dialog


CDlgMedFilter::CDlgMedFilter(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMedFilter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMedFilter)
	m_iTempH = 0;
	m_iTempW = 0;
	m_iTempMX = 0;
	m_iTempMY = 0;
	m_nWindowSize = -1;
	//}}AFX_DATA_INIT
}


void CDlgMedFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMedFilter)
	DDX_Control(pDX, IDC_EDIT_TEMPW_SPIN, m_ctrlTempWSpin);
	DDX_Control(pDX, IDC_EDIT_TEMPH_SPIN, m_ctrlTempHSpin);
	DDX_Control(pDX, IDC_EDIT_MY_SPIN, m_ctrlMYSpin);
	DDX_Control(pDX, IDC_EDIT_MX_SPIN, m_ctrlMXSpin);
	DDX_Text(pDX, IDC_EDIT_TEMPH, m_iTempH);
	DDX_Text(pDX, IDC_EDIT_TEMPW, m_iTempW);
	DDX_Text(pDX, IDC_EDIT_MX, m_iTempMX);
	DDX_Text(pDX, IDC_EDIT_MY, m_iTempMY);
	DDX_Radio(pDX, IDC_SORT1, m_nSortType);
	DDX_Radio(pDX, IDC_WND1, m_nWindowSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMedFilter, CDialog)
	//{{AFX_MSG_MAP(CDlgMedFilter)
	ON_BN_CLICKED(IDC_WND1, OnRad1)
	ON_BN_CLICKED(IDC_WND2, OnRad2)
	ON_BN_CLICKED(IDC_WND3, OnRad3)
	ON_BN_CLICKED(IDC_WND4, OnRad4)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPH, OnKillfocusEditTemph)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPW, OnKillfocusEditTempw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMedFilter message handlers

BOOL CDlgMedFilter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//设置微调控件的范围和伙伴编辑框
	m_ctrlTempWSpin.SetRange(1, 99);
	m_ctrlTempWSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_TEMPW));
	
	m_ctrlTempHSpin.SetRange(1, 99);
	m_ctrlTempHSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_TEMPH));
	
	m_ctrlMXSpin.SetRange(0, 99);
	m_ctrlMXSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_MX));

	m_ctrlMYSpin.SetRange(0, 99);
	m_ctrlMYSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_MY));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMedFilter::UpdateEdit()
{
	BOOL	bEnable;
	
	// 判断是不是自定义模板
	if (m_nWindowSize == 3) 
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	// 设置文本框及其微调控件的状态
	(CEdit *) GetDlgItem(IDC_EDIT_TEMPH)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_TEMPH_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_TEMPW)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_TEMPW_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_MX)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_MX_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_MY)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_MY_SPIN)->EnableWindow(bEnable);
	
}

void CDlgMedFilter::OnRad1() 
{
	// 3×3窗口
	m_nWindowSize = 0;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	
	// 更新文本框状态
	UpdateEdit();
	
	// 更新
	UpdateData(FALSE);
}

void CDlgMedFilter::OnRad2() 
{
	// 5×5窗口
	m_nWindowSize = 1;
	m_iTempH  = 5;
	m_iTempW  = 5;
	m_iTempMX = 2;
	m_iTempMY = 2;
	
	// 更新文本框状态
	UpdateEdit();
	
	// 更新
	UpdateData(FALSE);
}

void CDlgMedFilter::OnRad3() 
{
	// 7×7窗口
	m_nWindowSize = 2;
	m_iTempH  = 7;
	m_iTempW  = 7;
	m_iTempMX = 3;
	m_iTempMY = 3;
	
	// 更新文本框状态
	UpdateEdit();
	
	// 更新
	UpdateData(FALSE);
}

void CDlgMedFilter::OnRad4() 
{
	// 自定义窗口
	m_nWindowSize = 3;
	
	// 更新文本框状态
	UpdateEdit();
}

void CDlgMedFilter::OnOK() 
{
	// 获取用户设置（更新）
	UpdateData(TRUE);
	
	// 判断设置是否有效
	if ((m_iTempMX < 0) || (m_iTempMX > m_iTempW - 1) ||
		(m_iTempMY < 0) || (m_iTempMY > m_iTempH - 1))
	{
		// 提示用户参数设置错误
		MessageBox("中心元素参数设置错误！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}

	BOOL bError = FALSE;
	
	if (m_iTempW % 2 == 0)
	{
		// 提示用户参数设置错误
		MessageBox("窗口宽度必须为奇数！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 返回
		bError = TRUE;
	}

	if (m_iTempH % 2 == 0)
	{
		// 提示用户参数设置错误
		MessageBox("窗口高度必须为奇数！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 返回
		bError = TRUE;
	}

	if(bError) return;

	// 更新
	UpdateData(FALSE);

	CDialog::OnOK();
}

void CDlgMedFilter::OnKillfocusEditTemph() 
{
	// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();
}

void CDlgMedFilter::OnKillfocusEditTempw() 
{
	// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();
}
