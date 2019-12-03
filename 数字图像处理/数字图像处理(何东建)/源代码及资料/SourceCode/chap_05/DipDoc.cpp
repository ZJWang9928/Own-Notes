// DipDoc.cpp : implementation of the CDipDoc class
//

#include "stdafx.h"
#include "Dip.h"

#include "DipDoc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DipView.h"
#include "PointPro.h"
#include "AreaPro.h"

#include "DlgLinerPara.h"
#include "DlgPointStre.h"
#include "DlgThreshold.h"
#include "DlgSmooth.h"
#include "EdgeDetDlg.h"
#include "LapDlg.h"
#include "DlgColor.h"
#include "DlgMedianFilter.h"
#include "DlgSeedFill.h"
#include "DlgProject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char szFilter[];

/////////////////////////////////////////////////////////////////////////////
// CDipDoc

IMPLEMENT_DYNCREATE(CDipDoc, CDocument)

BEGIN_MESSAGE_MAP(CDipDoc, CDocument)
	//{{AFX_MSG_MAP(CDipDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_POINT_EQUA, OnPointEqua)
	ON_COMMAND(ID_POINT_LINER, OnPointLiner)
	ON_COMMAND(ID_POINT_STRE, OnPointStre)
	ON_COMMAND(ID_POINT_CONTAUTO, OnPointContauto)
	ON_COMMAND(ID_POINT_THRE, OnPointThre)
	ON_COMMAND(ID_ENHA_SMOOTH, OnEnhaSmooth)
	ON_COMMAND(ID_POINT_PSEUDOCOLOR, OnPointPseudocolor)
	ON_COMMAND(ID_ENHA_GRADSHARP, OnEnhaGradsharp)
	ON_COMMAND(ID_ENHA_TEMPSHARP, OnEnhaTempsharp)
	ON_COMMAND(ID_ENHA_LAPSHARP, OnEnhaLapsharp)
	ON_COMMAND(ID_POINT_INVERT, OnPointInvert)
	ON_COMMAND(ID_CHANNEL_SPLIT, OnChannelSplit)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_SPLIT, OnUpdateChannelSplit)
	ON_COMMAND(ID_CHANNEL_BLUE, OnChannelBlue)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_BLUE, OnUpdateChannelBlue)
	ON_COMMAND(ID_CHANNEL_GREEN, OnChannelGreen)
	ON_COMMAND(ID_CHANNEL_MIXED, OnChannelMixed)
	ON_COMMAND(ID_CHANNEL_RED, OnChannelRed)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_GREEN, OnUpdateChannelGreen)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_MIXED, OnUpdateChannelMixed)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_RED, OnUpdateChannelRed)
	ON_UPDATE_COMMAND_UI(ID_POINT_THRE, OnUpdatePointThre)
	ON_UPDATE_COMMAND_UI(ID_POINT_PSEUDOCOLOR, OnUpdatePointPseudocolor)
	ON_COMMAND(ID_ENHA_MidianF, OnEnhaMidianF)
	ON_COMMAND(ID_EDGE_TRACE, OnEdgeTrace)
	ON_UPDATE_COMMAND_UI(ID_EDGE_TRACE, OnUpdateEdgeTrace)
	ON_COMMAND(ID_EDGE_CONTOUR, OnEdgeContour)
	ON_COMMAND(ID_EDGE_FILL, OnEdgeFill)
	ON_UPDATE_COMMAND_UI(ID_EDGE_CONTOUR, OnUpdateEdgeContour)
	ON_COMMAND(ID_DETECT_MINUS, OnDetectMinus)
	ON_UPDATE_COMMAND_UI(ID_DETECT_MINUS, OnUpdateDetectMinus)
	ON_COMMAND(ID_DETECT_TEMPLATE, OnDetectTemplate)
	ON_UPDATE_COMMAND_UI(ID_DETECT_TEMPLATE, OnUpdateDetectTemplate)
	ON_COMMAND(ID_DETECT_PROJECT, OnDetectProject)
	ON_UPDATE_COMMAND_UI(ID_DETECT_PROJECT, OnUpdateDetectProject)
	ON_UPDATE_COMMAND_UI(ID_EDGE_FILL, OnUpdateEdgeFill)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDipDoc construction/destruction

CDipDoc::CDipDoc()
{
	// TODO: add one-time construction code here
	m_pDibObject = NULL;
	m_bImageLoaded = FALSE;
	m_nColorIndex = 0;
	m_bBinary = FALSE;
	m_lHeight = 0;
	m_lWidth = 0;
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

void CDipDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	static int nIndex = 1;

	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	FileDlg.m_ofn.nFilterIndex = (DWORD) nIndex;

	if( FileDlg.DoModal() == IDOK )
	{
		CString strPathName = FileDlg.GetPathName();
		AfxGetApp()->OpenDocumentFile( strPathName );
		nIndex = (int) FileDlg.m_ofn.nFilterIndex;
		if( !LoadImageToDocument() )
		{
			AfxMessageBox("无法载入图像文件！");
			return;
		}
	}
}

BOOL CDipDoc::LoadImageToDocument()
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

	//获取主框架窗口指针
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	//获取子框架窗口指针
	CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
	pChild->m_nWidth = m_pDibObject->GetWidth(); 
	pChild->m_nHeight = m_pDibObject->GetHeight();
	pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
		pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );
	

	//读入图像文件成功，设置相应变量
	m_bImageLoaded = TRUE;

	//返回TRUE
	return(TRUE);
}


void CDipDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	static int nIndex = 1;
	
	CFileDialog DialogSaveAs( FALSE, NULL, m_pDibObject->GetImageName(),
							OFN_HIDEREADONLY, szFilter );

	DialogSaveAs.m_ofn.nFilterIndex = (DWORD) nIndex;
	
	if( DialogSaveAs.DoModal() == IDOK )
	{
		CMainFrame *pMainFrame = ( CMainFrame * )AfxGetMainWnd();
		CChildFrame *pChildFrame = ( CChildFrame * )pMainFrame->MDIGetActive();
		CDipView *pDipView = ( CDipView * )pChildFrame->GetActiveView();
		
		nIndex = (int) DialogSaveAs.m_ofn.nFilterIndex;
		if( nIndex == 5 )
		{
			if( m_pDibObject->GetNumBits() != 24 )
			{
				AfxMessageBox("必须是24位真彩色图像才能存为JPEG格式！");
				return;
			}
		}

		if( m_pDibObject != NULL )
		{
			CString strPathName = DialogSaveAs.GetPathName();
			int nFindIndex = strPathName.Find(".");
			if( nFindIndex != -1)
				strPathName = strPathName.Left( nFindIndex );
			strPathName += CDibObject::szExtensions[ nIndex - 1 ];
			//m_pDibObject->ProcessImageHeader();
			//m_pDibObject->ProcessPalette();
			m_pDibObject->Save( strPathName );

			CString strFileName = DialogSaveAs.GetFileName();
			nFindIndex = strFileName.Find(".");
			if ( nFindIndex != -1 )
				strFileName = strFileName.Left( nFindIndex );
			strFileName += CDibObject::szExtensions[ nIndex - 1 ];
			pChildFrame->SetWindowText( strFileName );

			SetPathName( strPathName );
			if( nIndex == 5 )
			{
				m_pDibObject->Load( strPathName );
				pDipView->InvalidateRect( NULL, FALSE );
				pDipView->UpdateWindow();
			}
		}
	}
}

void CDipDoc::OnPointEqua() 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	
	// 更改光标形状
	BeginWaitCursor();

	// 调用LinerTrans()函数进行线性变换
	PointOperation.InteEqualize( m_pDibObject );

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnPointLiner() 
{
	// 创建对话框
	CDlgLinerPara dlgPara;
	
	// 线性变换的斜率
	float fa;	
	// 线性变换的截距
	float fb;

	// 初始化变量值
	dlgPara.m_fA = 1.0;
	dlgPara.m_fB = 0.0;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	fa = dlgPara.m_fA;
	fb = dlgPara.m_fB;

	// 删除对话框
	delete dlgPara;	
	
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	
	// 更改光标形状
	BeginWaitCursor();
	// 调用LinerTrans()函数进行线性变换
	PointOperation.LinerTran(fa, fb);

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();	
}

void CDipDoc::OnPointStre() 
{
	// 创建对话框
	CDlgPointStre dlgPara;
	
	// 点1坐标
	BYTE	bX1;
	BYTE	bY1;
	
	// 点2坐标
	BYTE	bX2;
	BYTE	bY2;

	// 初始化变量值
	dlgPara.m_bX1 = 50;
	dlgPara.m_bY1 = 30;
	dlgPara.m_bX2 = 200;
	dlgPara.m_bY2 = 220;
	
	// 显示对话框，提示用户设定拉伸位置
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户的设定
	bX1 = dlgPara.m_bX1;
	bY1 = dlgPara.m_bY1;
	bX2 = dlgPara.m_bX2;
	bY2 = dlgPara.m_bY2;
	
	// 删除对话框
	delete dlgPara;	
	
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	
	// 更改光标形状
	BeginWaitCursor();
	// 调用LinerTrans()函数进行线性变换
	PointOperation.GrayStretch(bX1, bY1, bX2, bY2);

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnPointContauto() 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation( m_pDibObject );

	int *pHistogram = PointOperation.GetHistogram();
	
	int i = 0;
	int nGrayMax = 0, nGrayMin = 0;

	//查找最小灰度值
	while(nGrayMin == 0 && i <= 255)
	{
		if( pHistogram[i] >= 100 )
		{
			nGrayMin = i;
		}
		i++;
	}

	//查找最大灰度值
	i = 255;
	while(nGrayMax == 0 && i >= 0)
	{
		if( pHistogram[i] >= 100 )
		{
			nGrayMax = i;
		}
		i--;
	}

	delete [] pHistogram;

	// 更改光标形状
	BeginWaitCursor();
	// 调用LinerTrans()函数进行线性变换
	PointOperation.GrayStretch(nGrayMin,0,nGrayMax,255);

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnPointThre() 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation( m_pDibObject );

	int *pHistogram = PointOperation.GetHistogram();
	
	//生成一个对话框CHistDlg类的实例
	CDlgThreshold ThreshDlg;

	//将绘制直方图的数据传递给CHistDlg对话框类的公有成员变量m_pnHistogram
	if( pHistogram != NULL )
	{
		ThreshDlg.m_pnHistogram = pHistogram;
		//设置当前像素值为0的像素数
		//HistDlg.m_nCurrentPiexsNum = pHistogram[0];

	}
	
	// 更改光标形状
	BeginWaitCursor();

	ThreshDlg.m_nThresholdAutoAdjust = PointOperation.FindThresh_Discriminant();

	ThreshDlg.m_nThresholdDiscriminant = PointOperation.FindThresh_Auto();

	ThreshDlg.m_nCurrentThreshold = ThreshDlg.m_nThresholdAutoAdjust;
	
	//显示对话框
	if ( ThreshDlg.DoModal() != IDOK)
		return;

	delete [] pHistogram;

	

	// 调用Threshold()函数进行阈值化操作
	PointOperation.Threshold(ThreshDlg.m_nCurrentThreshold, 
		ThreshDlg.m_nThresholdType, TRUE);

	m_bBinary = TRUE;

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnPointInvert() 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	
	BeginWaitCursor();
	//反色：fa=-1.0，fb=255
	PointOperation.LinerTran(-1.0, 255);
	EndWaitCursor();

	//SetModifiedFlag(TRUE);

	UpdateAllViews(NULL);
}

void CDipDoc::OnEnhaSmooth() 
{
	// 模板高度
	int	nTempH;
	
	// 模板宽度
	int nTempW;
	
	// 模板系数
	float fTempC;
	
	// 模板中心元素X坐标
	int nTempXc;
	
	// 模板中心元素Y坐标
	int nTempYc;
	
	// 模板元素数组
	float aValue[25];

	// 创建对话框
	CDlgSmooth dlgPara;
	
	// 给模板数组赋初值（为平均模板）
	aValue[0] = 1.0;
	aValue[1] = 1.0;
	aValue[2] = 1.0;
	aValue[3] = 1.0;
	aValue[4] = 1.0;
	aValue[5] = 1.0;
	aValue[6] = 1.0;
	aValue[7] = 1.0;
	aValue[8] = 1.0;
	aValue[9] = 1.0;
	aValue[10] = 1.0;
	aValue[11] = 1.0;
	aValue[12] = 1.0;
	aValue[13] = 1.0;
	aValue[14] = 1.0;
	aValue[15] = 1.0;
	aValue[16] = 1.0;
	aValue[17] = 1.0;
	aValue[18] = 1.0;
	aValue[19] = 1.0;
	aValue[20] = 1.0;
	aValue[21] = 1.0;
	aValue[22] = 1.0;
	aValue[23] = 1.0;
	aValue[24] = 1.0;
	
	// 初始化对话框变量值
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC  = 9.0;
	dlgPara.m_fpArray = aValue;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	nTempH  = dlgPara.m_iTempH;
	nTempW  = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC  = dlgPara.m_fTempC;
	
	// 删除对话框
	delete dlgPara;	
	
	//创建点处理CPointPro类对象
	CAreaPro AreaPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.Smoothing(aValue, fTempC, nTempW, nTempH, nTempXc, nTempYc);

	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
	
}

void CDipDoc::OnPointPseudocolor() 
{
	CDlgColor dlgPara;
	
	// 初始化变量值
	if (m_nColorIndex >= 0)
	{
		// 初始选中当前的伪彩色
		dlgPara.m_nColor = m_nColorIndex;
	}
	else
	{
		// 初始选中灰度伪彩色编码表
		dlgPara.m_nColor = 0;
	}
	
	// 指向名称数组的指针
	dlgPara.m_lpColorName = (LPSTR) ColorScaleName;
	
	// 伪彩色编码数目
	dlgPara.m_nColorCount = COLOR_SCALE_COUNT;
	
	// 名称字符串长度
	dlgPara.m_nNameLen = sizeof(ColorScaleName) / COLOR_SCALE_COUNT;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户的设定
	int nColor = dlgPara.m_nColor;
	
	// 删除对话框
	delete dlgPara;	
	
	CPointPro PointOperation( m_pDibObject );
	
	// 更改光标形状
	BeginWaitCursor();

	// 调用ReplaceColorPal()函数变换调色板
	PointOperation.ReplaceColorPal(m_pDibObject, (BYTE*) ColorsTable[nColor]);

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();

}

void CDipDoc::OnEnhaGradsharp() 
{
	//创建点处理CPointPro类对象
	CAreaPro AreaPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.GradSharping(10);

	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
}


void CDipDoc::OnEnhaMidianF() 
{
	// 窗口高度
	int	nTempH;
	
	// 窗口宽度
	int nTempW;
	
	// 窗口中心元素X坐标
	int nTempXc;
	
	// 窗口中心元素Y坐标
	int nTempYc;

	// 排序方式
	int nSortType;

	
	// 创建对话框
	CDlgMedianFilter dlgPara;
	
	// 初始化对话框变量值
	dlgPara.m_nWindowSize = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_nSortType = 0;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	nTempH  = dlgPara.m_iTempH;
	nTempW  = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	nSortType  = dlgPara.m_nSortType;

	
	// 删除对话框
	delete dlgPara;	
	
	//创建点处理CPointPro类对象
	CAreaPro AreaPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.MedianFilter(nSortType, nTempW, nTempH, nTempXc, nTempYc);

	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnEnhaTempsharp() 
{
	// 模板高度
	int	nTempH;
	
	// 模板宽度
	int nTempW;
	
	// 模板系数
	float fTempC;
	
	// 模板中心元素X坐标
	int nTempXc;
	
	// 模板中心元素Y坐标
	int nTempYc;
	
	// 模板元素数组
	float aValue[25];

	// 方向元素数组
	int nDirection[8];

	// 创建对话框
	CEdgeDetDlg dlgPara;
	
	// 给模板数组赋初值（为Roberts模板1）
	aValue[0]  = -1.0;
	aValue[1]  =  0.0;
	aValue[2]  =  0.0;
	aValue[3]  =  0.0;
	aValue[4]  =  0.0;
	aValue[5]  =  0.0;
	aValue[6]  =  1.0;
	aValue[7]  =  0.0;
	aValue[8]  =  0.0;
	aValue[9]  =  0.0;
	aValue[10] =  0.0;
	aValue[11] =  0.0;
	aValue[12] =  0.0;
	aValue[13] =  0.0;
	aValue[14] =  0.0;
	aValue[15] =  0.0;
	aValue[16] =  0.0;
	aValue[17] =  0.0;
	aValue[18] =  0.0;
	aValue[19] =  0.0;
	aValue[20] =  0.0;
	aValue[21] =  0.0;
	aValue[22] =  0.0;
	aValue[23] =  0.0;
	aValue[24] =  0.0;
	
	// 初始化对话框变量值
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC  = 1.0;
	dlgPara.m_fpArray = aValue;
	dlgPara.m_nDirection = nDirection;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	nTempH  = dlgPara.m_iTempH;
	nTempW  = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC  = dlgPara.m_fTempC;

	
	// 删除对话框
	delete dlgPara;	
	
	//创建点处理CPointPro类对象
	CAreaPro AreaPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.Sharping(aValue, nDirection);
	
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
	
}

void CDipDoc::OnEnhaLapsharp() 
{
	// 模板高度
	int	nTempH;
	
	// 模板宽度
	int nTempW;
	
	// 模板系数
	float fTempC;
	
	// 模板中心元素X坐标
	int nTempXc;
	
	// 模板中心元素Y坐标
	int nTempYc;
	
	// 模板元素数组
	float aValue[25];

	// 创建对话框
	CLapDlg dlgPara;
	
	// 给模板数组赋初值（为平均模板）
	aValue[0]  = -1.0;
	aValue[1]  = -1.0;
	aValue[2]  = -1.0;
	aValue[3]  =  0.0;
	aValue[4]  =  0.0;
	aValue[5]  = -1.0;
	aValue[6]  =  9.0;
	aValue[7]  = -1.0;
	aValue[8]  =  0.0;
	aValue[9]  =  0.0;
	aValue[10] = -1.0;
	aValue[11] = -1.0;
	aValue[12] = -1.0;
	aValue[13] =  0.0;
	aValue[14] =  0.0;
	aValue[15] =  0.0;
	aValue[16] =  0.0;
	aValue[17] =  0.0;
	aValue[18] =  0.0;
	aValue[19] =  0.0;
	aValue[20] =  0.0;
	aValue[21] =  0.0;
	aValue[22] =  0.0;
	aValue[23] =  0.0;
	aValue[24] =  0.0;
	
	// 初始化对话框变量值
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC  = 1.0;
	dlgPara.m_fpArray = aValue;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	nTempH  = dlgPara.m_iTempH;
	nTempW  = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC  = dlgPara.m_fTempC;
	
	// 删除对话框
	delete dlgPara;	
	
	//创建点处理CPointPro类对象
	CAreaPro AreaPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.LapSharping(aValue, fTempC, nTempW, nTempH, nTempXc, nTempYc);
	
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
	
}


void CDipDoc::OnChannelSplit() 
{
	//获取应用程序类指针
	CDipApp *pApp = GetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame = GetMainFrame();

	CDibObject *red = new CDibObject(m_pDibObject);
	CDibObject *green = new CDibObject(m_pDibObject);
	CDibObject *blue = new CDibObject(m_pDibObject);
	
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	
	// 更改光标形状
	BeginWaitCursor();
	
	if (PointOperation.SplitChannels(red, green, blue))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = red;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_RED_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
		pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = green;
		pDocument->m_bImageLoaded = TRUE;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_GREEN_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//获取子框架窗口指针
		pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
		pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = blue;
		pDocument->m_bImageLoaded = TRUE;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_BLUE_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//获取子框架窗口指针
		pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);
	
	// 恢复光标
	EndWaitCursor();

}

void CDipDoc::OnChannelBlue() 
{
	//获取应用程序类指针
	CDipApp *pApp = GetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame = GetMainFrame();

	CDibObject *blue = new CDibObject(m_pDibObject);
	
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	
	// 更改光标形状
	BeginWaitCursor();
	
	if (PointOperation.GetMixedChannel(blue))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = blue;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_BLUE_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnChannelGreen() 
{
	//获取应用程序类指针
	CDipApp *pApp = GetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame = GetMainFrame();

	CDibObject *green = new CDibObject(m_pDibObject);
	
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	
	// 更改光标形状
	BeginWaitCursor();
	
	if (PointOperation.GetMixedChannel(green))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = green;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_GREEN_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnChannelMixed() 
{
	//获取应用程序类指针
	CDipApp *pApp = GetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame = GetMainFrame();

	CDibObject *mixed = new CDibObject(m_pDibObject);
	
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	
	// 更改光标形状
	BeginWaitCursor();
	
	if (PointOperation.GetMixedChannel(mixed))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = mixed;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_MIXED_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnChannelRed() 
{
	//获取应用程序类指针
	CDipApp *pApp = GetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame = GetMainFrame();

	CDibObject *red = new CDibObject(m_pDibObject);
	
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	
	// 更改光标形状
	BeginWaitCursor();
	
	if (PointOperation.GetMixedChannel(red))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CDipDoc* pDocument = (CDipDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = red;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_RED_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnUpdateChannelGreen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateChannelMixed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateChannelRed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateChannelBlue(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateChannelSplit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdatePointThre(CCmdUI* pCmdUI) 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation( m_pDibObject );

	//判断是否为256色灰度图像
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnUpdatePointPseudocolor(CCmdUI* pCmdUI) 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation( m_pDibObject );

	//判断是否为256色灰度图像
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnEdgeTrace() 
{
	//创建点处理CPointPro类对象
	CAreaPro AreaPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.EdgeTracing();

	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnEdgeContour() 
{
	//创建点处理CPointPro类对象
	CAreaPro AreaPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.ContourPickUp();

	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnEdgeFill() 
{
	//创建种子填充方式选择对话框
	CDlgSeedFill seedfill;
	if(seedfill.DoModal() == IDOK)
	{
		//创建点处理CPointPro类对象
		CAreaPro AreaPro( m_pDibObject );
		// 更改光标形状
		BeginWaitCursor();
		//选择种子填充方法
		if(seedfill.m_iSeedMethod == 0)
			AreaPro.Feeding1();
		else
			AreaPro.Feeding2();
		// 恢复光标
		EndWaitCursor();
		
		// 更新视图
		UpdateAllViews(NULL);
	}
}

void CDipDoc::OnUpdateEdgeTrace(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnUpdateEdgeContour(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnDetectMinus() 
{
	// TODO: Add your command handler code here
	//模板图像数据指针
	unsigned char *pBitsTemp;

	static int nIndex = 1;

	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	FileDlg.m_ofn.nFilterIndex = (DWORD) nIndex;

	if( FileDlg.DoModal() == IDOK )
	{
		CString strPathName = FileDlg.GetPathName();
		nIndex = (int) FileDlg.m_ofn.nFilterIndex;

		//设置等待光标
		BeginWaitCursor();

		CDibObject *pTemplate = new CDibObject( strPathName.GetBuffer(3) );

		//取消等待光标
		EndWaitCursor();

		//读入图像文件失败
		if( pTemplate == NULL )
		{
			AfxMessageBox("无法创建差影模板图像类对象！");
			//返回FALSE
			return;
		}

		//在点处理CPointPro类中创建用来绘制直方图的数据
		CPointPro PointOperation( pTemplate );
		
		//判断是否为256色灰度图像
		if(!PointOperation.IsGray256())
		{
			AfxMessageBox("差影模板图像不是256级灰度图像，无法进行操作！");
			//返回FALSE
			return;
		}

		//原始图像大小
		int nOldWidth = m_pDibObject->GetWidth();
		int nOldHeight = m_pDibObject->GetHeight();
		
		//模板图像大小
		int nTempWidth = pTemplate->GetWidth();
		int nTempHeight = pTemplate->GetHeight();
		
		CRect rectOld(0, 0, nOldWidth, nOldHeight);
		CRect rectTemp(0, 0, nTempWidth, nTempHeight);
		
		//两个图像的大小应一致
		if (rectOld != rectTemp)
		{
			AfxMessageBox("模板图像和原图像大小不一致，无法完成操作！");
			return;
		}

		//创建点处理CPointPro类对象
		CAreaPro AreaPro1( pTemplate );
		// 更改光标形状
		BeginWaitCursor();
		
		pBitsTemp = AreaPro1.GetBitsPointer();
		
		// 恢复光标
		EndWaitCursor();

		//创建点处理CPointPro类对象
		CAreaPro AreaPro2( m_pDibObject );

		// 更改光标形状
		BeginWaitCursor();
		
		AreaPro2.Minusing(pBitsTemp);
		
		// 恢复光标
		EndWaitCursor();
		
		// 更新视图
		UpdateAllViews(NULL);
		
	}
}

void CDipDoc::OnUpdateDetectMinus(CCmdUI* pCmdUI) 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation( m_pDibObject );

	//判断是否为256色灰度图像
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnDetectTemplate() 
{
	// TODO: Add your command handler code here
	CDibObject *pTemplate;
	static int nIndex = 1;

	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	FileDlg.m_ofn.nFilterIndex = (DWORD) nIndex;

	if( FileDlg.DoModal() == IDOK )
	{
		CString strPathName = FileDlg.GetPathName();
		nIndex = (int) FileDlg.m_ofn.nFilterIndex;

		//设置等待光标
		BeginWaitCursor();

		pTemplate = new CDibObject( strPathName.GetBuffer(3) );

		//取消等待光标
		EndWaitCursor();

		//读入图像文件失败
		if( pTemplate == NULL )
		{
			AfxMessageBox("无法创建匹配模板图像类对象！");
			//返回FALSE
			return;
		}

		//在点处理CPointPro类中创建用来绘制直方图的数据
		CPointPro PointOperation( pTemplate );
		
		//判断是否为256色灰度图像
		if(!PointOperation.IsGray256())
		{
			AfxMessageBox("匹配模板图像不是256级灰度图像，无法进行操作！");
			//返回FALSE
			return;
		}

		//原始图像大小
		int nOldWidth = m_pDibObject->GetWidth();
		int nOldHeight = m_pDibObject->GetHeight();
		
		//模板图像大小
		int nTempWidth = pTemplate->GetWidth();
		int nTempHeight = pTemplate->GetHeight();
		
		//两个图像的大小应一致
		if (nTempWidth > nOldWidth || nTempHeight > nOldHeight)
		{
			AfxMessageBox("模板图像太大，无法完成操作！");
			return;
		}

		//创建点处理CPointPro类对象
		CAreaPro AreaPro( m_pDibObject );
		// 更改光标形状
		BeginWaitCursor();
		
		AreaPro.TemplateMatch(pTemplate);
		
		// 恢复光标
		EndWaitCursor();
		
		// 更新视图
		UpdateAllViews(NULL);
	}
}

void CDipDoc::OnUpdateDetectTemplate(CCmdUI* pCmdUI) 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation( m_pDibObject );

	//判断是否为256色灰度图像
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnDetectProject() 
{
	//创建投影模式选择对话框
	CDlgProject projectmode;
	if(projectmode.DoModal() == IDOK)
	{
		//创建点处理CPointPro类对象
		CAreaPro AreaPro( m_pDibObject );
		// 更改光标形状
		BeginWaitCursor();
		//选择投影模式
		if(projectmode.m_iProjectMode == 0)
			AreaPro.ProjectH();
		else
			AreaPro.ProjectV();
		// 恢复光标
		EndWaitCursor();
		
		// 更新视图
		UpdateAllViews(NULL);
	}
}

void CDipDoc::OnUpdateDetectProject(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	//只对8位二值图像起作用
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnUpdateEdgeFill(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );
	//只对8位二值图像起作用
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}
