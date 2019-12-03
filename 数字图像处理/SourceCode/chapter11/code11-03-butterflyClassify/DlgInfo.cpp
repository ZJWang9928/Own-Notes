// DlgInfo.cpp : implementation file

/*==========图象信息对话框类，用以读取位图的一些基本信息===============*/

#include "stdafx.h"
#include "Insects.h"
#include "DlgInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo dialog


CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInfo)
	m_nBits = 0;
	m_nHeight = 0;
	m_nWidth = 0;
	m_nSize = 0;
	//}}AFX_DATA_INIT
}


void CDlgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInfo)
	DDX_Text(pDX, IDC_BITS, m_nBits);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_SIZES, m_nSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgInfo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo message handlers
