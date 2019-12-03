// Dip.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Dip.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DipDoc.h"
#include "DipView.h"
#include "CombineDlg.h"
#include "PointPro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDipApp* GetApp()
{
  return (CDipApp*) AfxGetApp();
}

CMainFrame* GetMainFrame()
{
  return (CMainFrame*) AfxGetMainWnd();
}

//Inserted code begin
char szFilter[] = "位图文件(*.BMP)|*.BMP|图形交换格式文件(*.GIF)|*.GIF|PCX文件(*.PCX)|*.PCX|TGA文件(*.TGA)|*.TGA|JPEG文件(*.JPG)|*.JPG|标记图像文件(*.TIF)|*.TIF|所有支持图片|*.BMP,*.GIF,*.PCX,*.TGA,*.JPG,*.TIF|所有文件(*.*)|*.*||";
//Inserted code end

/////////////////////////////////////////////////////////////////////////////
// CDipApp

BEGIN_MESSAGE_MAP(CDipApp, CWinApp)
	//{{AFX_MSG_MAP(CDipApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_CHANNEL_COMBINE, OnChannelCombine)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_COMBINE, OnUpdateChannelCombine)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDipApp construction

CDipApp::CDipApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDipApp object

CDipApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDipApp initialization

BOOL CDipApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_DIPTYPE,
		RUNTIME_CLASS(CDipDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDipView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	m_nCmdShow = SW_MAXIMIZE;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDipApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDipApp message handlers


void CDipApp::OnFileNew() 
{
	// TODO: Add your command handler code here
	
}

void CDipApp::OnFileOpen() 
{
	static int nIndex = 1;

	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	FileDlg.m_ofn.nFilterIndex = (DWORD) nIndex;

	if( FileDlg.DoModal() == IDOK )
	{
		CString PathName = FileDlg.GetPathName();
		PathName.MakeUpper();
		OpenDocumentFile( PathName );
		nIndex = (int) FileDlg.m_ofn.nFilterIndex;
	}
}

int CDipApp::NumberOfOpenDocuments() const
{
	//打开的文档数
	int nOpenDocuments = 0;

	//获得第一个文档的位置
	POSITION posTemplate = GetFirstDocTemplatePosition();

	//统计文档数
	while (posTemplate != NULL) 
	{
		CDocTemplate* pTemplate = GetNextDocTemplate(posTemplate);
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		POSITION posDoc = pTemplate->GetFirstDocPosition();
		while (posDoc != NULL) 
		{
			pTemplate->GetNextDoc(posDoc);
			++nOpenDocuments;
		}
	}
	
	return nOpenDocuments;	
}


void CDipApp::OnChannelCombine() 
{
	CCombineDlg dlg;
	//获取主框架窗口指针
	CMainFrame *pFrame = GetMainFrame();
	if (dlg.DoModal() == IDOK)
	{
		CDibObject *dib = new CDibObject(dlg.GetRedImage());
		//创建点处理CPointPro类对象
		CPointPro PointOperation( dib );
		
		// 更改光标形状
		BeginWaitCursor();
		if (PointOperation.CombineChannels(dlg.GetRedImage(), dlg.GetGreenImage(), dlg.GetBlueImage()))
		{
			POSITION posTemplate = GetFirstDocTemplatePosition();

			CDocTemplate* pDocTemplate = GetNextDocTemplate(posTemplate);
			CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
			pDocument->m_pDibObject = dib;
			pDocument->m_bImageLoaded = TRUE;
			CString sSplitCaption;
			AfxFormatString1(sSplitCaption, IDS_COMBINE_CAPTION, dlg.m_strTitle);
			pDocument->SetTitle(sSplitCaption);
			
			//获取子框架窗口指针
			CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
			pChild->m_nWidth = dib->GetWidth(); 
			pChild->m_nHeight = dib->GetHeight();
			pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
				pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );
			
			pDocument->UpdateAllViews(NULL);
			
		}
		else
			AfxMessageBox(IDS_FAIL_COMBINE);

		// 恢复光标
		EndWaitCursor();
	}
}

void CDipApp::OnUpdateChannelCombine(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(NumberOfOpenDocuments() >= 3);
}
