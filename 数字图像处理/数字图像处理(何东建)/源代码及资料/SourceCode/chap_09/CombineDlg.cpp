#include "stdafx.h"
#include "Dip.h"
#include "CombineDlg.h"
#include "DipDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCombineDlg::CCombineDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCombineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCombineDlg)
	//}}AFX_DATA_INIT
	
	m_pRedImage = NULL;
	m_pGreenImage = NULL;
	m_pBlueImage = NULL;
	m_strTitle = "";
}


void CCombineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCombineDlg)
	DDX_Control(pDX, IDC_RED_CHANNEL, m_ctrlRedChannel);
	DDX_Control(pDX, IDC_GREEN_CHANNEL, m_ctrlGreenChannel);
	DDX_Control(pDX, IDC_BLUE_CHANNEL, m_ctrlBlueChannel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCombineDlg, CDialog)
//{{AFX_MSG_MAP(CCombineDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCombineDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString s;
	
	POSITION posTemplate = AfxGetApp()->GetFirstDocTemplatePosition();
	while (posTemplate != NULL) 
	{
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(posTemplate);
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		POSITION posDoc = pTemplate->GetFirstDocPosition();
		while (posDoc != NULL) 
		{
			CDipDoc* pDoc = (CDipDoc*) pTemplate->GetNextDoc(posDoc);
			ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CDipDoc)));
			m_ctrlRedChannel.AddString(pDoc->GetTitle());
			m_ctrlRedChannel.SetItemData(m_ctrlRedChannel.GetCount()-1, (DWORD) pDoc);
			m_ctrlGreenChannel.AddString(pDoc->GetTitle());
			m_ctrlGreenChannel.SetItemData(m_ctrlGreenChannel.GetCount()-1, (DWORD) pDoc);
			m_ctrlBlueChannel.AddString(pDoc->GetTitle());
			m_ctrlBlueChannel.SetItemData(m_ctrlBlueChannel.GetCount()-1, (DWORD) pDoc);
		}
	}
	
	m_ctrlRedChannel.SetCurSel(0);
	m_ctrlGreenChannel.SetCurSel(0);
	m_ctrlBlueChannel.SetCurSel(0);
	
	return TRUE;
}

void CCombineDlg::OnOK() 
{
	CString s;
	m_ctrlRedChannel.GetLBText(m_ctrlRedChannel.GetCurSel(), s);
	s =s.Left(s.Find("[", 0)) +  "[" + s.Mid((s.Find("[", 0) + 1), 4) + "+";
	m_strTitle += s; 

	CDipDoc* pRedDoc = (CDipDoc*) m_ctrlRedChannel.GetItemData(m_ctrlRedChannel.GetCurSel());
	m_pRedImage = pRedDoc->m_pDibObject;
	
	m_ctrlGreenChannel.GetLBText(m_ctrlGreenChannel.GetCurSel(), s);
	s = s.Mid((s.Find("[", 0) + 1), 4) + "+";
	m_strTitle += s; 

	CDipDoc* pGreenDoc = (CDipDoc*) m_ctrlGreenChannel.GetItemData(m_ctrlGreenChannel.GetCurSel());
	m_pGreenImage = pGreenDoc->m_pDibObject;
	
	m_ctrlBlueChannel.GetLBText(m_ctrlBlueChannel.GetCurSel(), s);
	s = s.Mid((s.Find("[", 0) + 1), 4);
	m_strTitle += s; 

	CDipDoc* pBlueDoc = (CDipDoc*) m_ctrlBlueChannel.GetItemData(m_ctrlBlueChannel.GetCurSel());
	m_pBlueImage = pBlueDoc->m_pDibObject;
	
	CDialog::OnOK();
}
