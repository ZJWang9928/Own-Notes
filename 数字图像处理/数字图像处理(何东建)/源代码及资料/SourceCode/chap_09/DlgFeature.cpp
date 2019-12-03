// DlgFeature.cpp : implementation file

#include "stdafx.h"
#include "dip.h"
#include "DlgFeature.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DipDoc.h"
#include "DibObject.h"
#include "ImgAnalyse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFeature dialog


CDlgFeature::CDlgFeature(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFeature::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFeature)
	m_fXScale = 1.0f;
	m_fXYScale = 1.414f;
	m_fYScale = 1.0f;
	m_fPermeter = 0.0f;
	m_fCompact = 0.0f;
	m_fArea = 0.0f;
	m_IsUseCurValue = TRUE;
	m_SelOpt = -1;
	m_strCxAndCy = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFeature::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFeature)
	DDX_Text(pDX, IDC_EDIT_XSCALE, m_fXScale);
	DDX_Text(pDX, IDC_EDIT_XYSCALE, m_fXYScale);
	DDX_Text(pDX, IDC_EDIT_YSCALE, m_fYScale);
	DDX_Text(pDX, IDC_EDIT_PERIMETER, m_fPermeter);
	DDX_Text(pDX, IDC_EDIT_COMPACT, m_fCompact);
	DDX_Text(pDX, IDC_EDIT_AREA, m_fArea);
	DDX_Check(pDX, IDC_CHECK_RETAIN, m_IsUseCurValue);
	DDX_Radio(pDX, IDC_RADIO_RECALIB, m_SelOpt);
	DDX_Text(pDX, IDC_EDIT_CENTROID, m_strCxAndCy);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgFeature, CDialog)
	//{{AFX_MSG_MAP(CDlgFeature)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, OnRadioCustom)
	ON_BN_CLICKED(IDC_CHECK_RETAIN, OnCheckRetain)
	ON_BN_CLICKED(IDC_RADIO_RECALIB, OnRadioRecalib)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFeature message handlers

BOOL CDlgFeature::OnInitDialog() 
{
	CDialog::OnInitDialog();


	CalPara(&m_fXScale,&m_fYScale,&m_fXYScale);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFeature::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//获取对话框窗口大小,它的坐标是相对于屏幕客户区的。
	RECT rect1;
	GetWindowRect(&rect1);

	//获取Group框的指针
	CWnd *pWnd;
	pWnd=GetDlgItem(IDC_STATIC_FOURIER);

	//获得绘制直方图的组合框的尺寸，它的坐标也是相对于屏幕客户区的
	RECT rect2;
	pWnd->GetWindowRect(&rect2);

	//定义画刷和画笔
	COLORREF color=GetSysColor(COLOR_BTNFACE);
	CBrush brush(color);
	CPen pen( PS_SOLID, 1, RGB(0, 0, 0));
	CBrush *pOldBrush;
	CPen *pOldPen;

	//画笔选入设备环境
	pOldBrush = (CBrush *) dc.SelectObject( &brush );
	pOldPen = (CPen *) dc.SelectObject( &pen );

	//定义绘制曲线的相对坐标，因为在对话框上绘图时其坐标是相对对话框的
	int x1 = rect2.left-rect1.left;
	int y1 = rect2.top-rect1.top-5;
	int x2 = rect2.right - rect2.left;
	int y2 = rect2.bottom - rect1.top-50;

	//绘制坐标系
	dc.MoveTo(x1,y2);
	dc.LineTo(x1,y1);
	dc.MoveTo(x1,y2);
	dc.LineTo(x2,y2);
	dc.MoveTo(x1+32,y2);
	dc.LineTo(x1+32,y2+3);
	dc.MoveTo(x1+64,y2);
	dc.LineTo(x1+64,y2+3);
	dc.MoveTo(x1+96,y2);
	dc.LineTo(x1+96,y2+3);
	dc.MoveTo(x1+128,y2);
	dc.LineTo(x1+128,y2+3);
	dc.MoveTo(x1+160,y2);
	dc.LineTo(x1+160,y2+3);
	dc.MoveTo(x1+192,y2);
	dc.LineTo(x1+192,y2+3);
	dc.MoveTo(x1+224,y2);
	dc.LineTo(x1+224,y2+3);
	dc.MoveTo(x1+256,y2);
	dc.LineTo(x1+256,y2+3);
	//设置文本输出时的背景色
	dc.SetBkColor( color );
	dc.TextOut(x1-2,y1-14,"‖S(ω)‖");
	dc.TextOut(x2-5,y2+3,"N");
	dc.TextOut(x1,y2+3,"0");
	dc.TextOut(x1+28,y2+3,"32");
	dc.TextOut(x1+60,y2+3,"64");
	dc.TextOut(x1+92,y2+3,"96");
	dc.TextOut(x1+120,y2+3,"128");
	dc.TextOut(x1+152,y2+3,"160");
	dc.TextOut(x1+184,y2+3,"192");
	dc.TextOut(x1+216,y2+3,"224");
	dc.TextOut(x1+248,y2+3,"256");

	GetFourier();

	//定义绘制的最大点数
	int nMaxNumP = 400;
	if(m_iNumPoints <= nMaxNumP) nMaxNumP = m_iNumPoints;

	//绘制傅立叶频谱幅度
	for(int i=0; i<nMaxNumP; i++ )
	{
		dc.MoveTo(x1+i+1, y2);
		
		//防止绘制时超出边框
		if(m_Fourier[i] <= y2-y1-20)
			dc.LineTo( x1+i+1,y2-(int)(m_Fourier[i]));
		else
			dc.LineTo( x1+i+1,y1+10);
	}

	//把原画刷和画笔选回设备环境
	dc.SelectObject( pOldBrush );
	dc.SelectObject( pOldPen );

	//Do not call CDialog::OnPaint() for painting messages
}

//计算各参数
void CDlgFeature::CalPara(float *X_Scale, float *YScale, float *XY_Scale)
{
	//保存数据到控件成员变量
	UpdateData();

	CMainFrame *pMainFrame=(CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChildFrame=(CChildFrame *)pMainFrame->MDIGetActive();
	CDipDoc *pDoc = (CDipDoc *)pChildFrame->GetActiveDocument();
	CImgAnalyse ImgAnalyse(pDoc->m_pDibObject);
	//ImgAnalyse.Calibrate(X_Scale,YScale,XY_Scale,26.0f);
	//由链码计算周长
	if(!ImgAnalyse.PeriByChainCode(*X_Scale,*YScale,*XY_Scale,&m_fPermeter))
		return;
	//由边界坐标计算面积
	if(!ImgAnalyse.AreaByGreen(*X_Scale,*YScale,&m_fArea))
		return;
	
	//返回由区域标记所得的质心
	POINT m_CxCy = ImgAnalyse.GetCentroid();
	m_strCxAndCy.Format("%d , %d",m_CxCy.x,m_CxCy.y);
	
	//计算致密度,已经规范化
	m_fCompact = m_fPermeter*m_fPermeter/(4*3.1415926f*m_fArea);

	//显示变量中存储的数据
	UpdateData(false);
}

void CDlgFeature::OnCheckRetain() 
{
	if(m_IsUseCurValue == TRUE)
	{
		//使单选框可用
		GetDlgItem(IDC_RADIO_CUSTOM)->EnableWindow();
		GetDlgItem(IDC_RADIO_RECALIB)->EnableWindow();
		m_IsUseCurValue = FALSE;
		
		//在取消复选框选择时，如果当前单选是自定，则使文本框可用	
		if(m_SelOpt == 1)
		{
			GetDlgItem(IDC_EDIT_XSCALE)->EnableWindow();
			GetDlgItem(IDC_EDIT_YSCALE)->EnableWindow();
			GetDlgItem(IDC_EDIT_XYSCALE)->EnableWindow();
		}
	}
	else
	{
		//使单选框不可用
		GetDlgItem(IDC_RADIO_CUSTOM)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RECALIB)->EnableWindow(FALSE);
		m_IsUseCurValue = TRUE;
		
		//重新计算并显示参数
		CalPara(&m_fXScale,&m_fYScale,&m_fXYScale);
	
		//使文本框可用,此时可自定义标定系数	
		GetDlgItem(IDC_EDIT_XSCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_YSCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_XYSCALE)->EnableWindow(FALSE);
	}
}
//自定义标定系数
void CDlgFeature::OnRadioCustom() 
{
	//使文本框可用	
	GetDlgItem(IDC_EDIT_XSCALE)->EnableWindow();
	GetDlgItem(IDC_EDIT_YSCALE)->EnableWindow();
	GetDlgItem(IDC_EDIT_XYSCALE)->EnableWindow();
}

BOOL CDlgFeature::GetFourier()
{
	CImgAnalyse ImgAnalyse;
	//获取傅立叶描述子数据指针
	//m_iNumPoints为边界点数，即傅立叶变换的系数个数
	m_Fourier = ImgAnalyse.FourierFactor(&m_iNumPoints);
	if(m_Fourier == NULL) return false;
	else
		return true;
}

void CDlgFeature::OnRadioRecalib() 
{
	char szBdFilter[] = "位图文件(*.BMP)|*.BMP||";

	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szBdFilter);

	if( FileDlg.DoModal() == IDOK )
	{
		CString PathName = FileDlg.GetPathName();
		PathName.MakeUpper();
		AfxGetApp()->OpenDocumentFile( PathName );

		CDibObject pDibObject(PathName);

		CImgAnalyse ImgAnalyse(&pDibObject);
		ImgAnalyse.Calibrate(&m_fXScale,&m_fYScale,&m_fXYScale,26.0f);
		UpdateData(false);
	}	
}
