// Set.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "Set.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSet dialog


CSet::CSet(CWnd* pParent /*=NULL*/)
	: CDialog(CSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSet)
	m_nPreCount = 0;
	//}}AFX_DATA_INIT
}


void CSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSet)
	DDX_Text(pDX, IDC_PRE_COUNT, m_nPreCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSet, CDialog)
	//{{AFX_MSG_MAP(CSet)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSet message handlers
