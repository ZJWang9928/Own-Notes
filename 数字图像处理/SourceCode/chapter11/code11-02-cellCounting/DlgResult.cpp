// DlgResult.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "DlgResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgResult dialog


CDlgResult::CDlgResult(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgResult)
	m_Result = _T("");
	m_nAntenna = 0.0f;
	m_nBelly = 0.0f;
	m_nSwing = 0;
	m_nBellyFactor = 0.0f;
	m_nEccentricity = 0.0f;
	m_nArea = 0.0f;
	m_nSquare = 0.0f;
	m_nWeight = _T("");
	//}}AFX_DATA_INIT
}


void CDlgResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgResult)
	DDX_Text(pDX, IDC_RESULT, m_Result);
	DDX_Text(pDX, IDC_EDIT_ANTENNA, m_nAntenna);
	DDX_Text(pDX, IDC_EDIT_BELLY, m_nBelly);
	DDX_Text(pDX, IDC_EDIT_ECCENTRICITY, m_nEccentricity);
	DDX_Text(pDX, IDC_EDIT_AREA, m_nArea);
	DDX_Text(pDX, IDC_EDIT_SQUARE, m_nSquare);
	DDX_Text(pDX, IDC_WEIGHT, m_nWeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgResult, CDialog)
	//{{AFX_MSG_MAP(CDlgResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgResult message handlers

void CDlgResult::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
