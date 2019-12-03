// DlgGeoTran.cpp : implementation file
//

#include "stdafx.h"
#include "Insects.h"
#include "DlgGenTran.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoTran dialog


CDlgGenTran::CDlgGenTran(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGenTran::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGeoTran)
	m_XOffset = 0;
	m_YOffset = 0;
	//}}AFX_DATA_INIT
}

void CDlgGenTran::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGeoTran)
	DDX_Control(pDX, IDC_YSPIN, m_ctrlYSpin);
	DDX_Control(pDX, IDC_XSPIN, m_ctrlXSpin);
	DDX_Text(pDX, IDC_EDIT_XOffset, m_XOffset);
	DDX_Text(pDX, IDC_EDIT_YOffset, m_YOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGenTran, CDialog)
	//{{AFX_MSG_MAP(CDlgGeoTran)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoTran message handlers

BOOL CDlgGenTran::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//ÉèÖÃÎ¢µ÷¿Ø¼þµÄ·¶Î§ºÍ»ï°é±à¼­¿ò
	m_ctrlXSpin.SetRange(100000,-100000);
	m_ctrlXSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_XOffset));

	m_ctrlYSpin.SetRange(100000,-100000);
	m_ctrlYSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_YOffset));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
