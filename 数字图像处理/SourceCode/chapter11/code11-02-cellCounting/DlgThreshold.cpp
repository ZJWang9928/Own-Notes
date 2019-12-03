// DlgThreshold.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "DlgThreshold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgThreshold dialog


CDlgThreshold::CDlgThreshold(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgThreshold::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgThreshold)
	m_nCurrentThreshold = 0;
	m_nThresholdType = 0;
	//}}AFX_DATA_INIT

	m_ThresholdAuto= 0;
	m_nThresholdJustAnalyze = 0;
}


void CDlgThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgThreshold)
	DDX_Control(pDX, IDC_CURRENT_THRESHOLD, m_ctrlEditThreshold);
	DDX_Text(pDX, IDC_CURRENT_THRESHOLD, m_nCurrentThreshold);
	DDX_Radio(pDX, IDC_THR_JUST, m_nThresholdType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgThreshold, CDialog)
	//{{AFX_MSG_MAP(CDlgThreshold)
	ON_BN_CLICKED(IDC_THR_JUST, OnThrJust)
	ON_BN_CLICKED(IDC_THR_ANYLYSIS, OnThrAnylysis)
	ON_BN_CLICKED(IDC_THR_SELF, OnThrSelf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgThreshold message handlers

BOOL CDlgThreshold::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ctrlEditThreshold.SetReadOnly(TRUE);
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgThreshold::OnThrJust() 
{
	m_bIsSelf = FALSE;
	UpdateData(TRUE);
	m_ctrlEditThreshold.SetReadOnly(TRUE);
	m_nCurrentThreshold = m_ThresholdAuto;
	UpdateData(FALSE);
}

void CDlgThreshold::OnThrAnylysis() 
{
	m_bIsSelf = FALSE;
	UpdateData(TRUE);
	m_ctrlEditThreshold.SetReadOnly(TRUE);
	m_nCurrentThreshold = m_nThresholdJustAnalyze;
	UpdateData(FALSE);
}

void CDlgThreshold::OnThrSelf() 
{
	m_bIsSelf = TRUE;
	UpdateData(TRUE);
	m_ctrlEditThreshold.SetReadOnly(FALSE);
	m_nCurrentThreshold =240;
	UpdateData(FALSE);
}
