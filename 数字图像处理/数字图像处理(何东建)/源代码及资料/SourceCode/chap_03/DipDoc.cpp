// DipDoc.cpp : implementation of the CDipDoc class
//

#include "stdafx.h"
#include "Dip.h"

#include "DipDoc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DipView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//外部变量声明
extern char szFilter[];

/////////////////////////////////////////////////////////////////////////////
// CDipDoc

IMPLEMENT_DYNCREATE(CDipDoc, CDocument)

BEGIN_MESSAGE_MAP(CDipDoc, CDocument)
	//{{AFX_MSG_MAP(CDipDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDipDoc construction/destruction

CDipDoc::CDipDoc()
{
	m_pDibObject = NULL;
	m_bImageLoaded = FALSE;
}

CDipDoc::~CDipDoc()
{
	if(m_pDibObject != NULL)
	{
		delete m_pDibObject;
		m_pDibObject = NULL;
	}
}

BOOL CDipDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDipDoc serialization

void CDipDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDipDoc diagnostics

#ifdef _DEBUG
void CDipDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDipDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDipDoc commands

void CDipDoc::OnFileNew() 
{
	// TODO: Add your command handler code here
	
}

//打开图像文件
void CDipDoc::OnFileOpen() 
{
	static int nIndex = 1;
	
	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	FileDlg.m_ofn.nFilterIndex = (DWORD) nIndex;
	
	if( FileDlg.DoModal() == IDOK )
	{
		CString strPathName = FileDlg.GetPathName();
		AfxGetApp()->OpenDocumentFile( strPathName );
		nIndex = (int) FileDlg.m_ofn.nFilterIndex;
		if( !ReadImgToDoc() )
		{
			AfxMessageBox("无法载入图像文件！");
			return;
		}
	}
}

//另存图像文件
void CDipDoc::OnFileSaveAs() 
{
	static int nIndex = 1;

	CFileDialog DialogSaveAs( FALSE, NULL, m_pDibObject->GetImageName(),
		                      OFN_HIDEREADONLY, szFilter );

	DialogSaveAs.m_ofn.nFilterIndex = (DWORD) nIndex;

	if( DialogSaveAs.DoModal() == IDOK )
	{
		CMainFrame *pMainFrame = ( CMainFrame *)AfxGetMainWnd();
		CChildFrame *pChildFrame = ( CChildFrame *)pMainFrame->MDIGetActive();
		CDipView *pDipView = ( CDipView * )pChildFrame->GetActiveView();
		
		nIndex = (int) DialogSaveAs.m_ofn.nFilterIndex;

		//是JPEG格式图像，判断其是否为24位真彩色图像
		if( nIndex == 5 )
		{
			if( m_pDibObject->GetNumBits() != 24 )
			{
				AfxMessageBox("只有24位真彩色图像才能存为JPEG格式！");
				return;
			}
		}
		
		//CDibObject类中有图像才能保存
		if( m_pDibObject != NULL )
		{
			//获取指定的文件名（含路径名）
			CString strPathName = DialogSaveAs.GetPathName();
			int nFindIndex = strPathName.Find(".");

			if( nFindIndex != -1)
				strPathName = strPathName.Left( nFindIndex );

			//加上指定的扩展名以构成完整文件名
			strPathName += CDibObject::szExtensions[ nIndex - 1 ];
			//调用CDibObject类成员函数Save()保存图像
			m_pDibObject->Save( strPathName );
			
			//设置窗口标题
			CString strFileName = DialogSaveAs.GetFileName();
			nFindIndex = strFileName.Find(".");

			if ( nFindIndex != -1 )
				strFileName = strFileName.Left( nFindIndex );

			strFileName += CDibObject::szExtensions[ nIndex - 1 ];
			pChildFrame->SetWindowText( strFileName );
			
			SetPathName( strPathName );

			//若保存为JPEG图像，重新将图像载入并更新视图的显示
			if( nIndex == 5 )
			{
				m_pDibObject->Load( strPathName );
				pDipView->InvalidateRect( NULL, FALSE );
				pDipView->UpdateWindow();
			}
		}
	}
}

//将图像文件读入CDibObject类对象中
BOOL CDipDoc::ReadImgToDoc()
{
	CString strPathName = GetPathName();
	
	//设置等待光标
	BeginWaitCursor();
	m_pDibObject = new CDibObject( strPathName.GetBuffer(3) );
	//取消等待光标
	EndWaitCursor();
	
	//读入图像文件失败
	if( m_pDibObject == NULL )
	{	
		AfxMessageBox("无法创建图像类对象！");
		//返回FALSE
		return(FALSE);
	}
	
	//读入图像文件成功，设置相应变量
	m_bImageLoaded = TRUE;
	
	//返回TRUE
	return(TRUE);
}
