// DlgStrInput.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgStrInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStrInput dialog


CDlgStrInput::CDlgStrInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStrInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStrInput)
	m_strInput = _T("");
	//}}AFX_DATA_INIT
}


void CDlgStrInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStrInput)
	DDX_Text(pDX, IDC_EDIT_STRINPUT, m_strInput);
	DDV_MaxChars(pDX, m_strInput, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStrInput, CDialog)
	//{{AFX_MSG_MAP(CDlgStrInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStrInput message handlers

