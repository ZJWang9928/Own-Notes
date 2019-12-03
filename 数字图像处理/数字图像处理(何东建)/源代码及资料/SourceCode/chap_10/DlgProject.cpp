// DlgProject.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgProject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProject dialog


CDlgProject::CDlgProject(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProject)
	m_iProjectMode = 0;
	//}}AFX_DATA_INIT
}


void CDlgProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProject)
	DDX_Radio(pDX, IDC_RADIO_PROJECTH, m_iProjectMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProject, CDialog)
	//{{AFX_MSG_MAP(CDlgProject)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProject message handlers
