// DlgGenRota.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "DlgGenRota.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGenRota dialog


CDlgGenRota::CDlgGenRota(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGenRota::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGeoRota)
	m_iRotateAngle = 0;
	//}}AFX_DATA_INIT
}


void CDlgGenRota::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGeoRota)
	DDX_Text(pDX, IDC_EDIT_Rotate, m_iRotateAngle);
	DDV_MinMaxInt(pDX, m_iRotateAngle, 0, 360);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGenRota, CDialog)
	//{{AFX_MSG_MAP(CDlgGenRota)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGenRota message handlers
