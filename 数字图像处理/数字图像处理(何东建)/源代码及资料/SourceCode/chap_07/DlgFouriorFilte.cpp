// DlgFouriorFilte.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgFouriorFilte.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFouriorFilte dialog


CDlgFouriorFilte::CDlgFouriorFilte(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFouriorFilte::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFouriorFilte)
	m_nFilteType = -1;
	m_nFitleParaA = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgFouriorFilte::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFouriorFilte)
	DDX_Radio(pDX, IDC_Filter_TYPE, m_nFilteType);
	DDX_Text(pDX, IDC_Freq_Fitler_a, m_nFitleParaA);
	DDV_MinMaxDouble(pDX, m_nFitleParaA, 0., 1.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFouriorFilte, CDialog)
	//{{AFX_MSG_MAP(CDlgFouriorFilte)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFouriorFilte message handlers

BOOL CDlgFouriorFilte::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
