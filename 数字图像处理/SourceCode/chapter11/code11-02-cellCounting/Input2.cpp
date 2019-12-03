// Input2.cpp : implementation file
//

#include "stdafx.h"
#include "imagetest.h"
#include "Input2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInput2 dialog


CInput2::CInput2(CWnd* pParent /*=NULL*/)
	: CDialog(CInput2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInput2)
	m_size = 0;
	m_thresh = 0;
	m_sigma = 0.0;
	m_gausswidth = 0;
	//}}AFX_DATA_INIT
}


void CInput2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInput2)
	DDX_Text(pDX, IDC_EDIT1, m_size);
	DDV_MinMaxInt(pDX, m_size, 3, 15);
	DDX_Text(pDX, IDC_EDIT2, m_thresh);
	DDV_MinMaxInt(pDX, m_thresh, 100, 50000);
	DDX_Text(pDX, IDC_EDIT3, m_sigma);
	DDV_MinMaxDouble(pDX, m_sigma, 0.2, 3.);
	DDX_Text(pDX, IDC_EDIT4, m_gausswidth);
	DDV_MinMaxInt(pDX, m_gausswidth, 3, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInput2, CDialog)
	//{{AFX_MSG_MAP(CInput2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInput2 message handlers
