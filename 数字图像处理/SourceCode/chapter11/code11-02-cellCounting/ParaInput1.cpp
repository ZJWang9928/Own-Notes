// ParaInput1.cpp : implementation file
//

#include "stdafx.h"
#include "imagetest.h"
#include "ParaInput1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParaInput1 dialog


CParaInput1::CParaInput1(CWnd* pParent /*=NULL*/)
	: CDialog(CParaInput1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParaInput1)
	m_input1 = 0;
	//}}AFX_DATA_INIT
}


void CParaInput1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParaInput1)
	DDX_Text(pDX, IDC_EDIT1, m_input1);
	DDV_MinMaxInt(pDX, m_input1, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParaInput1, CDialog)
	//{{AFX_MSG_MAP(CParaInput1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParaInput1 message handlers
