// DlgSeedFill.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgSeedFill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSeedFill dialog


CDlgSeedFill::CDlgSeedFill(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSeedFill::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSeedFill)
	m_iSeedMethod = 0;
	//}}AFX_DATA_INIT
}


void CDlgSeedFill::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSeedFill)
	DDX_Radio(pDX, IDC_RADIO_SEEDFILL1, m_iSeedMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSeedFill, CDialog)
	//{{AFX_MSG_MAP(CDlgSeedFill)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSeedFill message handlers
