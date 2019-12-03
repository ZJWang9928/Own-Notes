// DipDoc.cpp : implementation of the CDipDoc class
//

#include "stdafx.h"
#include "Dip.h"

#include "DipDoc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DipView.h"

//图像处理头文件
#include "PointPro.h"
#include "AreaPro.h"
#include "GeoTrans.h"
#include "FreqPro.h"
#include "MorphProc.h"
#include "ImgAnalyse.h"
#include "restore.h"


//对话框头文件
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
#include "DlgGeoTran.h"
#include "DlgGeoZoom.h"
#include "DlgGeoRota.h"
#include "DlgFouriorFilte.h"
#include "DlgMorph.h"
#include "DlgHitMiss.h"
#include "DlgFeature.h"
#include "DlgHuffman.h"
#include "DlgShannon.h"
#include "DlgStrInput.h"




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
	ON_COMMAND(ID_GEOM_MIRH, OnGeomMirh)
	ON_COMMAND(ID_GEOM_MIRV, OnGeomMirv)
	ON_COMMAND(ID_GEOM_ROTA, OnGeomRota)
	ON_COMMAND(ID_GEOM_TRAN, OnGeomTran)
	ON_COMMAND(ID_GEOM_TRPO, OnGeomTrpo)
	ON_COMMAND(ID_GEOM_ZOOM, OnGeomZoom)
	ON_UPDATE_COMMAND_UI(ID_GEOM_MIRH, OnUpdateGeomMirh)
	ON_UPDATE_COMMAND_UI(ID_GEOM_MIRV, OnUpdateGeomMirv)
	ON_UPDATE_COMMAND_UI(ID_GEOM_ROTA, OnUpdateGeomRota)
	ON_UPDATE_COMMAND_UI(ID_GEOM_TRAN, OnUpdateGeomTran)
	ON_UPDATE_COMMAND_UI(ID_GEOM_TRPO, OnUpdateGeomTrpo)
	ON_UPDATE_COMMAND_UI(ID_GEOM_ZOOM, OnUpdateGeomZoom)
	ON_COMMAND(ID_FREQ_DCT, OnFreqDct)
	ON_COMMAND(ID_FREQ_FFT, OnFreqFft)
	ON_COMMAND(ID_FREQ_FFT_FILTE, OnFreqFftFilte)
	ON_COMMAND(ID_FREQ_NFFT, OnFreqNfft)
	ON_COMMAND(ID_FREQ_WALH, OnFreqWalh)
	ON_UPDATE_COMMAND_UI(ID_FREQ_DCT, OnUpdateFreqDct)
	ON_UPDATE_COMMAND_UI(ID_FREQ_FFT_FILTE, OnUpdateFreqFftFilte)
	ON_UPDATE_COMMAND_UI(ID_FREQ_NFFT, OnUpdateFreqNfft)
	ON_UPDATE_COMMAND_UI(ID_FREQ_WALH, OnUpdateFreqWalh)
	ON_UPDATE_COMMAND_UI(ID_FREQ_FFT, OnUpdateFreqFft)
	ON_COMMAND(ID_FREQ_NDCT, OnFreqNdct)
	ON_UPDATE_COMMAND_UI(ID_FREQ_NDCT, OnUpdateFreqNdct)
	ON_COMMAND(ID_FREQ_NWALH, OnFreqNwalh)
	ON_UPDATE_COMMAND_UI(ID_FREQ_NWALH, OnUpdateFreqNwalh)
	ON_COMMAND(ID_MORPH_CLOSE, OnMorphClose)
	ON_COMMAND(ID_MORPH_DILATION, OnMorphDilation)
	ON_COMMAND(ID_MORPH_EROSION, OnMorphErosion)
	ON_COMMAND(ID_MORPH_OPEN, OnMorphOpen)
	ON_COMMAND(ID_MORPH_THINING, OnMorphThining)
	ON_UPDATE_COMMAND_UI(ID_MORPH_CLOSE, OnUpdateMorphClose)
	ON_UPDATE_COMMAND_UI(ID_MORPH_DILATION, OnUpdateMorphDilation)
	ON_UPDATE_COMMAND_UI(ID_MORPH_EROSION, OnUpdateMorphErosion)
	ON_UPDATE_COMMAND_UI(ID_MORPH_OPEN, OnUpdateMorphOpen)
	ON_UPDATE_COMMAND_UI(ID_MORPH_THINING, OnUpdateMorphThining)
	ON_COMMAND(ID_MORPH_HITMISSING, OnMorphHitmissing)
	ON_UPDATE_COMMAND_UI(ID_MORPH_HITMISSING, OnUpdateMorphHitmissing)
	ON_COMMAND(ID_ANALYSE_FEATURE, OnAnalyseFeature)
	ON_COMMAND(ID_ANALYSE_LABEL, OnAnalyseLabel)
	ON_COMMAND(ID_ANALYSE_TRACE, OnAnalyseTrace)
	ON_UPDATE_COMMAND_UI(ID_ANALYSE_FEATURE, OnUpdateAnalyseFeature)
	ON_UPDATE_COMMAND_UI(ID_ANALYSE_LABEL, OnUpdateAnalyseLabel)
	ON_UPDATE_COMMAND_UI(ID_ANALYSE_TRACE, OnUpdateAnalyseTrace)
	ON_COMMAND(ID_CODE_ARITH, OnCodeArith)
	ON_COMMAND(ID_CODE_HUFFMAN, OnCodeHuffman)
	ON_COMMAND(ID_CODE_SHANNON, OnCodeShannon)
	ON_UPDATE_COMMAND_UI(ID_CODE_ARITH, OnUpdateCodeArith)
	ON_UPDATE_COMMAND_UI(ID_CODE_HUFFMAN, OnUpdateCodeHuffman)
	ON_UPDATE_COMMAND_UI(ID_CODE_SHANNON, OnUpdateCodeShannon)
	ON_COMMAND(ID_RESTORE_BLUR, OnRestoreBlur)
	ON_COMMAND(ID_RESTORE_CONVNOISE, OnRestoreConvnoise)
	ON_COMMAND(ID_RESTORE_INVFILT, OnRestoreInvfilt)
	ON_COMMAND(ID_RESTORE_WINNER, OnRestoreWinner)
	ON_UPDATE_COMMAND_UI(ID_RESTORE_BLUR, OnUpdateRestoreBlur)
	ON_UPDATE_COMMAND_UI(ID_RESTORE_CONVNOISE, OnUpdateRestoreConvnoise)
	ON_UPDATE_COMMAND_UI(ID_RESTORE_INVFILT, OnUpdateRestoreInvfilt)
	ON_UPDATE_COMMAND_UI(ID_RESTORE_WINNER, OnUpdateRestoreWinner)
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
	m_lHeight = 0;
	m_lWidth = 0;

	m_bFft = FALSE;
	m_bDct = FALSE;
	m_bWht = FALSE;

	m_bIsLabeled = FALSE;
	m_bIsTraced = FALSE;

	//初始化Matrix<LIB>类库
	initM(MATCOM_VERSION);
}

CDipDoc::~CDipDoc()
{
	if(m_pDibObject != NULL)
	{
		delete m_pDibObject;
		m_pDibObject = NULL;
	}

	//结束Matrix<LIB>类库调用
	exitM();	

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
	PointOperation.InteEqualize();

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

void CDipDoc::OnGeomMirh() 
{
	//创建点处理CImageGeoTrans类对象
	CGeoTrans GeoTrans( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	//调用Mirror()函数进行垂直镜像
	GeoTrans.Mirror(TRUE);

	// 设置脏标记
	//	SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnGeomMirv() 
{
	//创建点处理CImageGeoTrans类对象
	CGeoTrans GeoTrans( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	//调用Mirror()函数进行水平镜像
	GeoTrans.Mirror(FALSE);

	// 设置脏标记
	//	SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnGeomRota() 
{
	//旋转角度
	int nRotateAngle;
	int nIntorType;
	BOOL bIntorType;
	
	//创建对话框
	CDlgGeoRota dlgPara;
	
	//初始化变量值
	dlgPara.m_iRotateAngle = 90;
	dlgPara.m_nIntorType = 0;
	
	//显示对话框，提示用户设定旋转角度
	if(dlgPara.DoModal() != IDOK)
	{
		//返回
		return;
	}
	
	//获取用户设定的角度
	nRotateAngle = dlgPara.m_iRotateAngle;
	nIntorType = dlgPara.m_nIntorType;
	
	//删除对话框
	delete dlgPara;	

	if(nIntorType == 0)
	{
		bIntorType = FALSE;
	}
	else
	{
		bIntorType = TRUE;
	}

	//创建点处理CImageGeoTrans类对象
	CGeoTrans GeoTrans( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	//调用Mirror()函数进行垂直镜像
	GeoTrans.Rotate(nRotateAngle, bIntorType);

	// 设置脏标记
	//	SetModifiedFlag(TRUE);
	
	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnGeomTran() 
{
	// 创建对话框
	CDlgGeoTran dlgPara;
	
	// 初始化变量值
	dlgPara.m_XOffset = 0;
	dlgPara.m_YOffset = 0;
	dlgPara.m_nTransType = 0;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	LONG lXOffset = dlgPara.m_XOffset;
	LONG lYOffset = dlgPara.m_YOffset;
	int nTransType = dlgPara.m_nTransType;
	// 删除对话框
	delete dlgPara;	
	
	//创建点处理CImageGeoTrans类对象
	CGeoTrans GeoTrans( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	if(nTransType == 0)
	{
		// 调用TranslationPixel()函数进行平移
		GeoTrans.TranslationPixel(lXOffset, lYOffset);
	}
	else if(nTransType == 1)
	{
		// 调用TranslationLine()函数进行平移
		GeoTrans.TranslationLine(lXOffset, lYOffset);
	}

	// 设置脏标记
	//	SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnGeomTrpo() 
{
	//创建点处理CImageGeoTrans类对象
	CGeoTrans GeoTrans( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	//调用Mirror()函数进行垂直镜像
	GeoTrans.Transpose();

	// 设置脏标记
	//	SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

void CDipDoc::OnGeomZoom() 
{
// 缩放比率
	float fXZoomRatio;
	float fYZoomRatio;
	int nIntorType;
	BOOL bIntorType;
	
	// 创建对话框
	CDlgGeoZoom dlgPara;
	
	// 初始化变量值
	dlgPara.m_XZoom = 4.0;
	dlgPara.m_YZoom = 4.0;
	dlgPara.m_nIntorType = 0;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	fXZoomRatio = dlgPara.m_XZoom;
	fYZoomRatio = dlgPara.m_YZoom;
	nIntorType = dlgPara.m_nIntorType;
	
	// 删除对话框
	delete dlgPara;	
	
	if(nIntorType == 0)
	{
		bIntorType = FALSE;
	}
	else
	{
		bIntorType = TRUE;
	}
	
	//创建点处理CImageGeoTrans类对象
	CGeoTrans GeoTrans( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();
	
	//调用Mirror()函数进行垂直镜像
	GeoTrans.ZoomImage(fXZoomRatio, fYZoomRatio, bIntorType);
	
	// 设置脏标记
	//	SetModifiedFlag(TRUE);
	
	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

//以下六个命令更新消息映射，保证图像几何变换只对8位以上的图像进行操作。
void CDipDoc::OnUpdateGeomMirh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateGeomMirv(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateGeomRota(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateGeomTran(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateGeomTrpo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnUpdateGeomZoom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CDipDoc::OnFreqDct() 
{
	//创建点处理CImagePro类对象
	CFreqPro FreqPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	m_matTransed = FreqPro.DCT2() ;

	if(!isempty(m_matTransed)) m_bDct = TRUE;
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnFreqNdct() 
{
	//创建点处理CImagePro类对象
	CFreqPro FreqPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	if(FreqPro.NDCT2(m_matTransed)) m_bDct = FALSE;
	
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnFreqFft() 
{
	//创建点处理CImagePro类对象
	CFreqPro FreqPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	m_matTransed = FreqPro.FFT2() ;

	if(!isempty(m_matTransed)) m_bFft = TRUE;
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnFreqNfft() 
{
	//创建点处理CImagePro类对象
	CFreqPro FreqPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	if(FreqPro.NFFT2(m_matTransed)) m_bFft = FALSE;
	
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnFreqFftFilte() 
{
	// 创建对话框
	CDlgFouriorFilte dlgPara;
	
	// 初始化变量值
	dlgPara.m_nFitleParaA = 0.0;
	dlgPara.m_nFilteType = 0;
	
	// 显示对话框，提示用户设定滤波参数
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的滤波参数
	double a = dlgPara.m_nFitleParaA;
	int nType = dlgPara.m_nFilteType;

	// 删除对话框
	delete dlgPara;	
	
	if(nType == 0)
	{
		//低通滤波
		Mm matSize = size(m_matTransed);
		Mm matTemp = mabs(m_matTransed);
		Mm matVector = reshape(m_matTransed, matSize.r(1,1)*matSize.r(1,2) , 1);

		Mm matMax = mmax(matVector);

		matTemp = matTemp / matMax;
		Mm matL = matTemp >= (double)a;
		m_matTransed = times(m_matTransed, matL);
	}
	else if(nType == 1)
	{
		//高通滤波
		Mm matSize = size(m_matTransed);
		Mm matTemp = mabs(m_matTransed);
		Mm matVector = reshape(m_matTransed, matSize.r(1,1)*matSize.r(1,2) , 1);

		Mm matMax = mmax(matVector);

		matTemp = matTemp / matMax;
		Mm matL = matTemp <= (double)a;
		m_matTransed = times(m_matTransed, matL);
	}

	//创建点处理CImagePro类对象
	CFreqPro FreqPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	if(FreqPro.NFFT2(m_matTransed)) m_bFft = FALSE;
	
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);

}

void CDipDoc::OnFreqWalh() 
{
	//创建点处理CImagePro类对象
	CFreqPro FreqPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	m_matTransed = FreqPro.WHT2() ;

	if(!isempty(m_matTransed)) m_bWht = TRUE;
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnFreqNwalh() 
{
	//创建点处理CImagePro类对象
	CFreqPro FreqPro( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	if(FreqPro.NWHT2(m_matTransed)) m_bWht = FALSE;
	
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
}

//以下七个命令更新消息映射，保证频域变换只对8位图像进行操作。
void CDipDoc::OnUpdateFreqDct(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8);
}

void CDipDoc::OnUpdateFreqNdct(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8 && m_bDct);
}

void CDipDoc::OnUpdateFreqFft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8);
}

void CDipDoc::OnUpdateFreqNfft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8 && m_bFft);
}

void CDipDoc::OnUpdateFreqFftFilte(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8 && m_bFft);
}

void CDipDoc::OnUpdateFreqWalh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8);
}

void CDipDoc::OnUpdateFreqNwalh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8 && m_bWht);
}

void CDipDoc::OnMorphClose() 
{
	//结构元素数组指针（存储的是各结构元素相对于结构中心的坐标）
	int *nMask;

	//结构元素的大小（以结构元素中的点数为单位）
	int nMaskLen = 0;


	// 创建对话框
	CDlgMorph dlgPara;
	
	// 显示对话框，提示用户设定腐蚀方向
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	nMask = dlgPara.m_npMask;
	nMaskLen = dlgPara.m_nMaskLen;

	// 删除对话框
	delete dlgPara;	

	//创建形态学处理CMorphPro类对象
	CMorphPro MorphProcesses( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	MorphProcesses.Closing(nMask, nMaskLen);
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);	
}

void CDipDoc::OnMorphDilation() 
{
	//结构元素数组指针（存储的是各结构元素相对于结构中心的坐标）
	int *nMask;

	//结构元素的大小（以结构元素中的点数为单位）
	int nMaskLen = 0;


	// 创建对话框
	CDlgMorph dlgPara;
	
	// 显示对话框，提示用户设定腐蚀方向
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	nMask = dlgPara.m_npMask;
	nMaskLen = dlgPara.m_nMaskLen;

	// 删除对话框
	delete dlgPara;	

	//创建形态学处理CMorphPro类对象
	CMorphPro MorphProcesses( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	MorphProcesses.Dilation(nMask, nMaskLen);
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);	
}

void CDipDoc::OnMorphErosion() 
{
	//结构元素数组指针（存储的是各结构元素相对于结构中心的坐标）
	int *nMask;

	//结构元素的大小（以结构元素中的点数为单位）
	int nMaskLen = 0;


	// 创建对话框
	CDlgMorph dlgPara;
	
	// 显示对话框，提示用户设定腐蚀方向
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	nMask = dlgPara.m_npMask;
	nMaskLen = dlgPara.m_nMaskLen;

	// 删除对话框
	delete dlgPara;	

	//创建形态学处理CMorphPro类对象
	CMorphPro MorphProcesses( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	MorphProcesses.Erosion(nMask, nMaskLen);
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);	
}

void CDipDoc::OnMorphOpen() 
{
	//结构元素数组指针（存储的是各结构元素相对于结构中心的坐标）
	int *nMask;

	//结构元素的大小（以结构元素中的点数为单位）
	int nMaskLen = 0;


	// 创建对话框
	CDlgMorph dlgPara;
	
	// 显示对话框，提示用户设定腐蚀方向
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	nMask = dlgPara.m_npMask;
	nMaskLen = dlgPara.m_nMaskLen;

	// 删除对话框
	delete dlgPara;	

	//创建形态学处理CMorphPro类对象
	CMorphPro MorphProcesses( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	MorphProcesses.Opening(nMask, nMaskLen);
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);	
}

void CDipDoc::OnMorphHitmissing() 
{
	//结构元素数组指针（存储的是各结构元素相对于结构中心的坐标）
	int *nHmask, *nMmask;

	//结构元素的大小（以结构元素中的点数为单位）
	int nHmaskLen = 0, nMmaskLen = 0;


	// 创建对话框
	CDlgHitMiss dlgPara;
	
	// 显示对话框，提示用户设定腐蚀方向
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	nHmask = dlgPara.m_npHmask;
	nHmaskLen = dlgPara.m_nHmaskLen;
	nMmask = dlgPara.m_npMmask;
	nMmaskLen = dlgPara.m_nMmaskLen;


	/*CString s;
	s.Format("nHmaskLen=%d\nnMmaskLen=%d\n",nHmaskLen,nMmaskLen);
	AfxMessageBox(s);
	for(int i = 0; i < 2 * nHmaskLen; i = i + 2)
	{
		s.Format("Num=%d\n(%d, %d)",i/2,nHmask[i],nHmask[i+1]);
		AfxMessageBox(s);
	}
	for(i = 0; i < 2 * nMmaskLen; i = i + 2)
	{
		s.Format("Num=%d\n(%d, %d)",i/2,nMmask[i],nMmask[i+1]);
		AfxMessageBox(s);
	}*/

	// 删除对话框
	delete dlgPara;	

	//创建形态学处理CMorphPro类对象
	CMorphPro MorphProcesses( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	MorphProcesses.HitMissTran(nHmask, nHmaskLen, nMmask, nMmaskLen);
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);	
}

void CDipDoc::OnMorphThining() 
{
	//创建形态学处理CMorphPro类对象
	CMorphPro MorphProcesses( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();
	
	MorphProcesses.ThiningDIB(m_pDibObject);
	//恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);	
}

void CDipDoc::OnUpdateMorphClose(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnUpdateMorphDilation(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnUpdateMorphErosion(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnUpdateMorphOpen(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnUpdateMorphThining(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnUpdateMorphHitmissing(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnAnalyseFeature() 
{
	//判断当前是否有图像对象
	if(m_pDibObject == NULL ) return;

	CImgAnalyse ImgAnalyse(m_pDibObject);

	m_bIsTraced = FALSE;
	m_bIsLabeled = FALSE;

	if(ImgAnalyse.GetEdgePointNum()<1)
	{
		AfxMessageBox("在特征分析前没有跟踪轮廓或跟踪不正确!\n\n请重新进行轮廓跟踪!!!");
		return;
	}

	CDlgFeature dlgFeature;
	dlgFeature.DoModal();
}

void CDipDoc::OnAnalyseLabel() 
{
	if(m_pDibObject==NULL)return;

	CImgAnalyse ImgAnalyse(m_pDibObject);
	ImgAnalyse.LabelArea();
	
	m_bIsLabeled = TRUE;

	//更新视图
	UpdateAllViews(NULL);	
}

void CDipDoc::OnAnalyseTrace() 
{
	//判断当前是否有图像对象
	if(m_pDibObject == NULL ) return;

	//边界跟踪
	CImgAnalyse ImgAnalyse(m_pDibObject);
	ImgAnalyse.EdgeTrace();

	m_bIsTraced = TRUE;
	m_bIsLabeled = FALSE;

	//更新视图
	UpdateAllViews(NULL);	
}

void CDipDoc::OnUpdateAnalyseFeature(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsTraced);
}

void CDipDoc::OnUpdateAnalyseLabel(CCmdUI* pCmdUI) 
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation( m_pDibObject );	
	pCmdUI->Enable(PointOperation.IsBlackandWhite256());
}

void CDipDoc::OnUpdateAnalyseTrace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsLabeled);
} 

void CDipDoc::OnCodeArith() 
{
	CDlgStrInput InputBox;
	if(InputBox.DoModal() == IDOK)
	{
		CString strTemp,strNew,strCodeL,strCodeR;
		int i,j,s;
		BYTE *uCodeNum;
		FLOAT *fFreq,*fInterval;

		strTemp = InputBox.m_strInput;

		//分配内存
		uCodeNum = new BYTE[strTemp.GetLength()];

		s = 0;
		i = 0;
		while( strTemp.GetLength() > 0 )
		{
			strNew += strTemp.GetAt(0);

			//各个符号的数目
			s = strTemp.Remove(strTemp.GetAt(0));
			uCodeNum[i] = (BYTE)s;
			//统计字符串中的符号数
			i++;
		}
		//分配内存
		fFreq = new FLOAT[i];
		//各符号的初始区间的左端
		fInterval = new FLOAT[i];

		//按照字符串中出现的字符依次计算字符串的所处的间隔
		float fStart = 0.0f,fEnd = 0.0f,fStartB = 0.0f,fLeft = 0.0f,fRight = 0.0f,fLen = 0.0f;

		strTemp = InputBox.m_strInput;
		//统计各个符号的频率
		for(j=0;j<i;j++)
		{
			fFreq[j] = (float)uCodeNum[j]/strTemp.GetLength();
		}
		
		//确定各符号的初始区间的左端
		fInterval[0] = 0.0f;
		i = 1;
		while(i < strNew.GetLength())
		{
			fInterval[i] = fInterval[i-1] + fFreq[i-1];
			i++;
		}
		
		//前一间隔的起始位置
		fStartB = 0.0f;
		fLen = fFreq[0];

		for(i=1;i<strTemp.GetLength();i++)
		{
			//确定字符串中各字符在新字符串中的位置
			j = strNew.Find(strTemp.GetAt(i));

			//当前编码符号的初始区间的左端和右端
			fLeft = fInterval[j];
			fRight = fLeft + fFreq[j];

			//新区间的起始位置
			fStart = fStartB +fLeft*fLen;
			fEnd = fStartB + fRight*fLen;

			//更新前一间隔的起始位置和间隔长度
			fStartB = fStart;
			fLen = fEnd - fStart;
		}

		//对最后的新间隔(即fStart,fEnd)转换为二进制
		FLOAT fTemp1,fTemp2;
		fTemp1 = fStart;
		fTemp2 = fEnd;
		while(((fTemp1*2 >= 1.0) && (fTemp2*2 >= 1.0)) || ((fTemp1*2 < 1.0) && (fTemp2*2 < 1.0)))
		{
			//如果两个数都被完全转换，则退出循环
			if((fTemp1 == 0.0) && (fTemp2 == 0.0)) break;

			//将fStart转换为二进制
			if( fTemp1*2 >= 1.0 )
			{
				strCodeL += "1";
				fTemp1 = fTemp1*2 - 1;
			}
			else
			{
				strCodeL += "0";
				fTemp1 = fTemp1 * 2;
			}
		
			//将fEnd转换为二进制
			if( fTemp2*2 >= 1.0 )
			{
				strCodeR += "1";
				fTemp2 = fTemp2*2 - 1;
			}
			else
			{
				strCodeR += "0";
				fTemp2 = fTemp2 * 2;
			}
		}

		//最后取两者二进制的中间值
		if( fTemp2*2 >= 1.0 )
		{
			strCodeL += "1";
		}
		
		//输出编码结果
		AfxMessageBox(strCodeL);

		delete [] uCodeNum;
		delete [] fInterval;
		delete [] fFreq;
	}
}

void CDipDoc::OnCodeHuffman() 
{
	int imgSize;
	imgSize = m_pDibObject->GetWidth()*m_pDibObject->GetHeight();
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation(m_pDibObject );

	int *pHistogram = PointOperation.GetHistogram();

	//生成一个对话框CHistDlg类的实例
	CDlgHuffman HuffmanDlg;

	//将绘制直方图的数据传递给CHistDlg对话框类的公有成员变量m_pnHistogram
	if( pHistogram != NULL )
	{
		//设置直方图数据指针
		HuffmanDlg.m_fFreq = pHistogram;
		HuffmanDlg.m_iSizeImage = imgSize;
		if(m_pDibObject->GetNumBits() >= 8)
			HuffmanDlg.m_iLeafNum=256;
		else
			HuffmanDlg.m_iLeafNum=m_pDibObject->GetNumBits();
	}
	//显示对话框
	if ( HuffmanDlg.DoModal() != IDOK)
		return;

	delete [] pHistogram;	
}

void CDipDoc::OnCodeShannon() 
{
/*	int i,j,imgSize;
	FLOAT *fFreq;
	BYTE *uMap;
	int *pHist;

	//在点处理CPointPro类中创建直方图数据，并定义指向该数据的指针
	CPointPro PointOperation(m_pDibObject);
	pHist = PointOperation.GetHistogram();

	//图像大小
	imgSize = m_pDibObject->GetWidth()*m_pDibObject->GetHeight();

	//统计频率不为0的灰度数
	j = 0;
	for(i=0;i<256;i++)
	{
		if(pHist[i] > 0) j++;
	}

	// 分配内存
	fFreq = new FLOAT[j];
	uMap = new BYTE[j];

	//计算图像中亮度数据的各灰度的频率
	j = 0;
	for(i=0;i<256;i++)
	{
		if(pHist[i] > 0)
		{
			fFreq[j] = (float)pHist[i]/imgSize;
		
			//记住频率不为0的灰度的映射关系
			uMap[j] = i;
			j++;
		}
	}

	//创建香农编码对话框实例
	CDlgShannon Shannon;



	//传递灰度频率数据、灰度级数给对话框
	Shannon.m_fFreq = fFreq;
	Shannon.m_uMap = uMap;
	Shannon.m_iColorNum = j;

	//显示对话框
	Shannon.DoModal();

	//释放内存
	delete[] fFreq;
	delete[] uMap;
*/	
	//***************
	//把上面的代码替换为下面的代码可以对指定的信源符号进行香农编码
	FLOAT fFreq[8];
	BYTE uMap[8];

	fFreq[0] = 0.4f;
	fFreq[1] = 0.18f;
	fFreq[2] = 0.10f;
	fFreq[3] = 0.10f;
	fFreq[4] = 0.07f;
	fFreq[5] = 0.06f;
	fFreq[6] = 0.05f;
	fFreq[7] = 0.04f;
	
	for(int i=0;i<8;i++) uMap[i] = i;
	CDlgShannon Shannon;
	Shannon.m_fFreq = fFreq;
	Shannon.m_iColorNum = 8;
	Shannon.m_uMap = uMap;
	Shannon.DoModal();
	/****************/
}

void CDipDoc::OnUpdateCodeArith(CCmdUI* pCmdUI) 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation( m_pDibObject );

	//判断是否为256色灰度图像
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnUpdateCodeHuffman(CCmdUI* pCmdUI) 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation( m_pDibObject );

	//判断是否为256色灰度图像
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnUpdateCodeShannon(CCmdUI* pCmdUI) 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation( m_pDibObject );

	//判断是否为256色灰度图像
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CDipDoc::OnRestoreBlur() 
{
	//判断当前是否载入图像
	if( m_pDibObject == NULL ) return;
	//定义一个图像复原对象
	CRestore Restore( m_pDibObject );
	//调用卷积模糊函数
	Restore.ConvBlur();

	//更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnRestoreConvnoise() 
{
	//判断当前是否载入图像
	if( m_pDibObject == NULL ) return;
	//定义一个图像复原对象
	CRestore Restore( m_pDibObject );
	//调用卷积器噪音模糊函数
	Restore.ConvNoiseBlur();

	//更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnRestoreInvfilt() 
{
	//判断当前是否载入图像
	if( m_pDibObject == NULL ) return;
	
	//定义一个图像复原对象
	CRestore Restore( m_pDibObject );
	//调用逆滤波复原函数
	Restore.InvFilter();
	
	//更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnRestoreWinner() 
{
	//判断当前是否载入图像
	if( m_pDibObject == NULL ) return;
	
	//定义一个图像复原对象
	CRestore Restore( m_pDibObject );
	//调用维纳滤波复原函数
	Restore.Wiener();

	//更新视图
	UpdateAllViews(NULL);
}

void CDipDoc::OnUpdateRestoreBlur(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8);
}

void CDipDoc::OnUpdateRestoreConvnoise(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8);
}

void CDipDoc::OnUpdateRestoreInvfilt(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8);
}

void CDipDoc::OnUpdateRestoreWinner(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() == 8);
}
