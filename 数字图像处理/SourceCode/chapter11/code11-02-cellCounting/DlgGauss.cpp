// DlgGauss.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "DlgGauss.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGauss dialog


CDlgGauss::CDlgGauss(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGauss::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGauss)
	m_sigma = 0.0;
	m_size = 0;
	m_thresh = 0;
	m_gausswidth = 0;
	//}}AFX_DATA_INIT
}


void CDlgGauss::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGauss)
	DDX_Text(pDX, ID_SIGMA, m_sigma);
	DDX_Text(pDX, ID_SIZE, m_size);
	DDX_Text(pDX, ID_THRESHOLD, m_thresh);
	DDX_Text(pDX, ID_WIDTH, m_gausswidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGauss, CDialog)
	//{{AFX_MSG_MAP(CDlgGauss)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGauss message handlers
