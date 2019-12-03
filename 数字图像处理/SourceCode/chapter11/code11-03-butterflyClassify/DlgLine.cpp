// DlgLine.cpp : implementation file
//

#include "stdafx.h"
#include "Insects.h"
#include "DlgLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLine dialog


CDlgLine::CDlgLine(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLine::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLine)
	m_f1 = 0.0f;
	m_f2 = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLine)
	DDX_Text(pDX, IDC_EDIT1, m_f1);
	DDX_Text(pDX, IDC_EDIT2, m_f2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLine, CDialog)
	//{{AFX_MSG_MAP(CDlgLine)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnKillfocusEdit2)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_CANCLE, OnCancle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLine message handlers

BOOL CDlgLine::OnInitDialog() 
{	
	// 调用默认OnInitDialog函数
	CDialog::OnInitDialog();	
	// 返回TRUE
	return TRUE; 
}

void CDlgLine::OnKillfocusEdit1() 
{
	// 保存用户设置
	UpdateData(TRUE);
	
}

void CDlgLine::OnKillfocusEdit2() 
{
	// 保存用户设置
	UpdateData(TRUE);	
}

void CDlgLine::OnOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CDlgLine::OnCancle() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
