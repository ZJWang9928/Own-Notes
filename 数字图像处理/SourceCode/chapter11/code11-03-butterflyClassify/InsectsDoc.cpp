// InsectsDoc.cpp : implementation of the CInsectsDoc class
//

#include "stdafx.h"
#include "Insects.h"

#include "InsectsDoc.h"
#include "InsectsView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DlgInfo.h"
#include "DibObject.h"
#include "Pretreatment.h"  //预处理类
#include "DlgLine.h"	   //图象几何变换对话框类
#include "DlgMedFilter.h"  //中值滤波对话框类
#include "DlgTempSmooth.h" //模板操作对话框类
#include "Identify.h"	   //图象识别类	
#include "DlgThreshold.h"  //阈值对话框类
#include "DlgMorth.h"	   //图象形态学对话框类
#include "MorphProc.h"	   //图象形态学类
#include "DlgGenZoom.h"	   //图象缩放对话框类
#include "DlgGenTran.h"    //图象移动对话框类 
#include "GeoTrans.h"      //图象几何变化类  
#include "DlgGenRota.h"    //图象旋转对话框类 
#include "DlgCharacter.h"  //图象特征对话框类
#include "DlgResult.h"     //结果对话框类
#include "LapDlg.h"        //图象锐化类  


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char szFilter[];
extern float antenna_factor[100];
extern float belly_factor[100];
extern int antenna_num;
extern int belly_num;
//define MAX_PATH=100;


/////////////////////////////////////////////////////////////////////////////
// CInsectsDoc

IMPLEMENT_DYNCREATE(CInsectsDoc, CDocument)

BEGIN_MESSAGE_MAP(CInsectsDoc, CDocument)
	//{{AFX_MSG_MAP(CInsectsDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_CONVERT, OnConvert)
	ON_COMMAND(ID_AUTO_LIGHT, OnAutoLight)
	ON_COMMAND(ID_BELLY_SEP, OnBellySep)
	ON_COMMAND(ID_CHARACTER, OnCharacter)
	ON_COMMAND(ID_IDENTIFY, OnIdentify)
	ON_COMMAND(ID_LAPULASI_SHARP, OnLapulasiSharp)
	ON_COMMAND(ID_LINE_LIGHT, OnLineLight)
	ON_COMMAND(ID_MIDIAN_TEMP, OnMidianTemp)
	ON_COMMAND(ID_TIDU_SHARP, OnTiduSharp)
	ON_COMMAND(ID_WING_SEP, OnWingSep)
	ON_COMMAND(ID_ANTENNA_SEP, OnAntennaSep)
	ON_COMMAND(ID_TEMP_SMOOTH, OnTempSmooth)
	ON_COMMAND(ID_THRESHOLD_SEP, OnThresholdSep)
	ON_COMMAND(ID_DELATE, OnDelate)
	ON_COMMAND(ID_CANCLE_ERSION, OnCancleErsion)
	ON_COMMAND(ID_MORPH_OPEN, OnMorphOpen)
	ON_COMMAND(ID_MORPH_CLOSE, OnMorphClose)
	ON_COMMAND(ID_GEOM_ZOOM, OnGeomZoom)
	ON_COMMAND(ID_GEOM_MIRH, OnGeomMirh)
	ON_COMMAND(ID_GEOM_TRAN, OnGeomTran)
	ON_COMMAND(ID_GEOM_MIRV, OnGeomMirv)
	ON_COMMAND(ID_GEOM_TRPO, OnGeomTrpo)
	ON_COMMAND(ID_GEOM_ROTA, OnGeomRota)
	ON_COMMAND(ID_INFO, OnInfo)
	ON_COMMAND(ID_CONTOUR_TRACE, OnContourTrace)
	ON_COMMAND(ID_EDGE_TRACE, OnEdgeTrace)
	ON_UPDATE_COMMAND_UI(ID_ANTENNA_SEP, OnUpdateAntennaSep)
	ON_UPDATE_COMMAND_UI(ID_BELLY_SEP, OnUpdateBellySep)
	ON_UPDATE_COMMAND_UI(ID_CANCLE_ERSION, OnUpdateCancleErsion)
	ON_UPDATE_COMMAND_UI(ID_CHARACTER, OnUpdateCharacter)
	ON_UPDATE_COMMAND_UI(ID_CONTOUR_TRACE, OnUpdateContourTrace)
	ON_UPDATE_COMMAND_UI(ID_DELATE, OnUpdateDelate)
	ON_UPDATE_COMMAND_UI(ID_EDGE_TRACE, OnUpdateEdgeTrace)
	ON_UPDATE_COMMAND_UI(ID_IDENTIFY, OnUpdateIdentify)
	ON_UPDATE_COMMAND_UI(ID_LAPULASI_SHARP, OnUpdateLapulasiSharp)
	ON_UPDATE_COMMAND_UI(ID_TIDU_SHARP, OnUpdateTiduSharp)
	ON_UPDATE_COMMAND_UI(ID_THRESHOLD_SEP, OnUpdateThresholdSep)
	ON_UPDATE_COMMAND_UI(ID_TEMP_SMOOTH, OnUpdateTempSmooth)
	ON_UPDATE_COMMAND_UI(ID_MIDIAN_TEMP, OnUpdateMidianTemp)
	ON_UPDATE_COMMAND_UI(ID_MORPH_CLOSE, OnUpdateMorphClose)
	ON_UPDATE_COMMAND_UI(ID_WING_SEP, OnUpdateWingSep)
	ON_UPDATE_COMMAND_UI(ID_MORPH_OPEN, OnUpdateMorphOpen)
	ON_COMMAND(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsectsDoc construction/destruction

CInsectsDoc::CInsectsDoc()
{
	// TODO: add one-time construction code here
	m_pDibObject=NULL;
	m_bImageLoaded=FALSE;
	depart_factor=3.0;

}

CInsectsDoc::~CInsectsDoc()
{
	if(m_pDibObject!=NULL)
	{
		delete m_pDibObject;
		m_pDibObject=NULL;
	}
}

BOOL CInsectsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CInsectsDoc serialization

void CInsectsDoc::Serialize(CArchive& ar)
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
// CInsectsDoc diagnostics

#ifdef _DEBUG
void CInsectsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInsectsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInsectsDoc commands


void CInsectsDoc::OnFileNew() 
{
	// TODO: Add your command handler code here
	
}

void CInsectsDoc::OnFileOpen() 
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
		if( !ReadImgToDoc() )
		{
			AfxMessageBox("无法载入图像文件！");
			return;
		}
	}
}

BOOL CInsectsDoc::ReadImgToDoc()
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

void CInsectsDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	static int nIndex = 1;

	CFileDialog DialogSaveAs( FALSE, NULL, m_pDibObject->GetImageName(),
		                      OFN_HIDEREADONLY, szFilter );

	DialogSaveAs.m_ofn.nFilterIndex = (DWORD) nIndex;

	if( DialogSaveAs.DoModal() == IDOK )
	{
		CMainFrame *pMainFrame = ( CMainFrame *)AfxGetMainWnd();
		CChildFrame *pChildFrame = ( CChildFrame *)pMainFrame->MDIGetActive();
		CInsectsView *pImgProView = ( CInsectsView * )pChildFrame->GetActiveView();
		
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
				pImgProView->InvalidateRect( NULL, FALSE );
				pImgProView->UpdateWindow();
			}
		}
	}
}

/*===========将彩色图象转换成256色的灰度图象========*/
void CInsectsDoc::OnConvert() 
{
	//获取应用程序类指针
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *mixed = new CDibObject(m_pDibObject);
	
	//创建预处理CPretreatment类对象
	CPretreatment Pre(m_pDibObject);
	
	// 更改光标形状
	BeginWaitCursor();
		
	if (Pre.MakeGray(mixed))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = mixed;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("转换后的灰度图象");

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 50, 
			pChild->m_nHeight + 50, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("转换错误！");
	
	// 恢复光标
	EndWaitCursor();	
}

/*========自动调整图象的亮度=========*/
void CInsectsDoc::OnAutoLight() 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPretreatment Pre( m_pDibObject );

	int *pHistogram = Pre.GetHistogram();
	
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
	// 调用AutoLight()函数进行线性变换
	Pre.AutoLight(nGrayMin,0,nGrayMax,255);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
	
}

/*===============对图象腹部进行分割=================*/
void CInsectsDoc::OnBellySep() 
{
	//获取应用程序类指针
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *belly = new CDibObject(m_pDibObject);
	
	//创建预处理CIdentify类对象
	CIdentify Ide(belly);
	
	// 更改光标形状
	BeginWaitCursor();
		
	if (Ide.BellySep())
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = belly;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("腹部分割");

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 50, 
			pChild->m_nHeight + 50, SWP_NOZORDER  );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("分割出现错误！");
	
	// 恢复光标
	EndWaitCursor();
}

/*=============获取图象的重要信息：触角，腹部和翅膀================*/
void CInsectsDoc::OnCharacter() 
{
	//创建对话框
	CDlgCharacter dlgPara;

	CDibObject *pDib0 = new CDibObject(m_pDibObject);
	CDibObject *pDib1 = new CDibObject(m_pDibObject);
	CDibObject *pDib2 = new CDibObject(m_pDibObject);	
	CDibObject *pDib3 = new CDibObject(m_pDibObject);
	CDibObject *pDib4 = new CDibObject(m_pDibObject);
	CDibObject *pDib5 = new CDibObject(m_pDibObject);
	CDibObject *pDib6 = new CDibObject(m_pDibObject);
	CDibObject *pDib7 = new CDibObject(m_pDibObject);
	//创建预处理CIdentify类对象
	CIdentify Ide0(pDib0);
	CIdentify Ide1(pDib1);
	CIdentify Ide2(pDib2);
	CIdentify Ide3(pDib3);
	CIdentify Ide4(pDib4);
	CIdentify Ide5(pDib5);
	CIdentify Ide6(pDib6);
	CIdentify Ide7(pDib7);

	//调用相应的分割函数
	Ide0.AntennaSep();
	Ide1.BellySep();
	Ide2.SwingSep();
	Ide5.EdgeTrace();

	//获取其触角形状因子
	dlgPara.m_nAntenna=Ide0.GetAntennaPara();
	//获取其腹部粗细因子
	dlgPara.m_nBelly=Ide1.GetBellyPara();
	//获取其翅膀对数
	dlgPara.m_nSwing=Ide2.GetSwingPara();
	//获取其偏心率
	dlgPara.m_nEccentricity=Ide3.GetEccentricity();
	//获取图象长轴
	dlgPara.m_Widths=m_pDibObject->GetHeight()>m_pDibObject->GetWidth()?m_pDibObject->GetHeight():m_pDibObject->GetWidth();
	//获取图象短抽
	dlgPara.m_Heights=m_pDibObject->GetHeight()>m_pDibObject->GetWidth()?m_pDibObject->GetWidth():m_pDibObject->GetHeight();
	//获取其面积
	dlgPara.m_nArea=Ide4.GetArea();
	//获取其周长
	dlgPara.m_nPerimeter=(float)Ide5.GetPeriByChainCode();
	//获取图象矩形度
	dlgPara.m_Square=Ide4.GetArea()/(m_pDibObject->GetWidth()*m_pDibObject->GetHeight());
	//获取其圆形度
	dlgPara.m_nCircularity=(float)(4*Ide4.GetArea()/(3.1416*m_pDibObject->GetWidth()*m_pDibObject->GetWidth()));
	//获取其形状参数
	dlgPara.m_nFormFactor=(float)((Ide5.GetPeriByChainCode()*Ide5.GetPeriByChainCode())/(4*3.1416*Ide4.GetArea()));	
	//获取图象的致密度
	dlgPara.m_Indense=(float)((Ide5.GetPeriByChainCode()*Ide5.GetPeriByChainCode())/Ide4.GetArea());
	//获取图象质心
	dlgPara.m_Center_x=(Ide6.GetCentroid()).x;
	dlgPara.m_Center_y=(Ide6.GetCentroid()).y;
	//获取图象二阶中心矩
	dlgPara.m_Center_Square=Ide7.GetSquare();
	//显示对话框
	if(dlgPara.DoModal() != IDOK)
	{
		return;
	}
	
}

/*==============对图象进行识别======================*/
void CInsectsDoc::OnIdentify() 
{
	//创建对话框
	CDlgResult dlgPara;
	double weight=0.0;

	CDibObject *pDib1 = new CDibObject(m_pDibObject);
	CDibObject *pDib2 = new CDibObject(m_pDibObject);	
	CDibObject *pDib3 = new CDibObject(m_pDibObject);
	CDibObject *pDib4 = new CDibObject(m_pDibObject);
	CDibObject *pDib5 = new CDibObject(m_pDibObject);
	CDibObject *pDib6 = new CDibObject(m_pDibObject);

	//创建预处理CIdentify类对象
	CIdentify Ide1(pDib1);
	CIdentify Ide2(pDib2);
	CIdentify Ide3(pDib3);
	CIdentify Ide4(pDib4);
	CIdentify Ide5(pDib5);
	CIdentify Ide6(pDib6);

	Ide1.AntennaSep();
	Ide2.BellySep();
	Ide3.SwingSep();
	Ide6.EdgeTrace();

	dlgPara.m_nAntenna=Ide1.GetAntennaPara();
	dlgPara.m_nBelly=Ide2.GetBellyPara();
	dlgPara.m_nSwing=Ide3.GetSwingPara();
	dlgPara.m_nEccentricity=Ide4.GetEccentricity();
	dlgPara.m_nArea=Ide5.GetArea();
	dlgPara.m_nSquare=(float)(Ide5.GetArea()/(m_pDibObject->GetWidth()*m_pDibObject->GetHeight()));
	
	if(Ide1.GetAntennaPara()>0.5)
		weight=weight+0.7;

	if(Ide2.GetBellyPara()<0.057)
		weight=weight+0.1;

	if(Ide5.GetArea()>88866)
		weight=weight+0.1;

	if(dlgPara.m_nSquare>=0.47)
		weight=weight+0.05;

	if(dlgPara.m_nEccentricity>=3.3)
		weight=weight+0.05;
	CString str;
	str.Format("%f",weight);
	dlgPara.m_nWeight=str;
    
	if(weight>=0.705)
		dlgPara.m_Result="根据特征参数提取和测量，由于W>=0.705,\n\n所以可以判定该鳞翅目昆虫为蝴蝶(Butterfly)";
	else
		dlgPara.m_Result="根据特征参数提取和测量，由于W<0.705,\n\n所以可以判定该鳞翅目昆虫为蛾子（Moth）";
	//显示对话框
	if(dlgPara.DoModal() != IDOK)
	{
		return;
	}
	 	
}

/*============对图象进行拉普拉斯锐化=================*/
void CInsectsDoc::OnLapulasiSharp() 
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
	CPretreatment Pre( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	Pre.LapSharping(aValue, fTempC, nTempW, nTempH, nTempXc, nTempYc);
	
	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);	
}

/*==========对图象的亮度进行线性变化==================*/
void CInsectsDoc::OnLineLight() 
{
	// 创建对话框
	CDlgLine dlg;
	
	// 线性变换的斜率
	float f1;	
	// 线性变换的截距
	float f2;

	// 初始化变量值
	dlg.m_f1= 1.0;
	dlg.m_f2 = 0.0;
	
	if (dlg.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	f1 = dlg.m_f1;
	f2 = dlg.m_f2;

	// 删除对话框
	delete dlg;	
	
	//创建点处理CPretreatment类对象
	CPretreatment Pre( m_pDibObject );
	
	// 更改光标形状
	BeginWaitCursor();
	// 调用LinerTrans()函数进行线性变换
	Pre.LinerTran(f1, f2);

	// 更新视图
	UpdateAllViews(NULL);

	// 恢复光标
	EndWaitCursor();
	
}

/*========中值滤波消除噪音=============*/
void CInsectsDoc::OnMidianTemp() 
{	// 窗口高度
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
	CDlgMedFilter dlg;
	
	// 初始化对话框变量值
	dlg.m_nWindowSize = 0;
	dlg.m_iTempH  = 3;
	dlg.m_iTempW  = 3;
	dlg.m_iTempMX = 1;
	dlg.m_iTempMY = 1;
	dlg.m_nSortType = 0;
	
	// 显示对话框，提示用户设定平移量
	if (dlg.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	nTempH  = dlg.m_iTempH;
	nTempW  = dlg.m_iTempW;
	nTempXc = dlg.m_iTempMX;
	nTempYc = dlg.m_iTempMY;
	nSortType  = dlg.m_nSortType;

	
	// 删除对话框
	delete dlg;	
	
	//创建点处理CPretreatment类对象
	CPretreatment Pre( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	Pre.MedianFilter(nSortType, nTempW, nTempH, nTempXc, nTempYc);

	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);	

}

/*========梯度锐化=============*/
void CInsectsDoc::OnTiduSharp() 
{
	//创建点处理CPointPro类对象
	CPretreatment Pre( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	Pre.GradSharping(10);

	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);	
}

/*========对图象的翅膀进行分割=============*/
void CInsectsDoc::OnWingSep() 
{
	//获取应用程序类指针
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *swing = new CDibObject(m_pDibObject);
	
	//创建预处理CIdentify类对象
	CIdentify Ide(swing);
	
	// 更改光标形状
	BeginWaitCursor();
		
	if (Ide.SwingSep())
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = swing;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("翅膀分割");

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 50, 
			pChild->m_nHeight + 50, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("分割出现错误！");
	
	// 恢复光标
	EndWaitCursor();		
}

void CInsectsDoc::OnAntennaSep() 
{
	//获取应用程序类指针
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *antenna = new CDibObject(m_pDibObject);
	
	//创建预处理CIdentify类对象
	CIdentify Ide(antenna);
	
	// 更改光标形状
	BeginWaitCursor();
		
	if (Ide.AntennaSep())
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = antenna;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("触角分割");

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("分割出现错误！");
	
	// 恢复光标
	EndWaitCursor();
}

void CInsectsDoc::OnTempSmooth() 
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
	float nTemp[25];

	// 创建对话框
	CDlgTempSmooth dlg;
	
	// 给模板数组赋初值（为平均模板）
	nTemp[0] = 1.0;
	nTemp[1] = 1.0;
	nTemp[2] = 1.0;
	nTemp[3] = 1.0;
	nTemp[4] = 1.0;
	nTemp[5] = 1.0;
	nTemp[6] = 1.0;
	nTemp[7] = 1.0;
	nTemp[8] = 1.0;
	nTemp[9] = 1.0;
	nTemp[10] = 1.0;
	nTemp[11] = 1.0;
	nTemp[12] = 1.0;
	nTemp[13] = 1.0;
	nTemp[14] = 1.0;
	nTemp[15] = 1.0;
	nTemp[16] = 1.0;
	nTemp[17] = 1.0;
	nTemp[18] = 1.0;
	nTemp[19] = 1.0;
	nTemp[20] = 1.0;
	nTemp[21] = 1.0;
	nTemp[22] = 1.0;
	nTemp[23] = 1.0;
	nTemp[24] = 1.0;
	
	// 初始化对话框变量值
	dlg.m_intType = 0;
	dlg.m_iTempH  = 3;
	dlg.m_iTempW  = 3;
	dlg.m_iTempMX = 1;
	dlg.m_iTempMY = 1;
	dlg.m_fTempC  = 9.0;
	dlg.m_fpArray = nTemp;
	
	// 显示对话框，提示用户设定平移量
	if (dlg.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	nTempH  = dlg.m_iTempH;
	nTempW  = dlg.m_iTempW;
	nTempXc = dlg.m_iTempMX;
	nTempYc = dlg.m_iTempMY;
	fTempC  = dlg.m_fTempC;
	
	// 删除对话框
	delete dlg;	
	
	//创建点处理CPretreatment类对象
	CPretreatment Pre( m_pDibObject );
	// 更改光标形状
	BeginWaitCursor();

	Pre.TemplateSmooth(nTemp, fTempC, nTempW, nTempH, nTempXc, nTempYc);

	// 恢复光标
	EndWaitCursor();
	
	// 更新视图
	UpdateAllViews(NULL);
	
}


void CInsectsDoc::OnThresholdSep() 
{
	//判断当前是否有图像对象
	if( m_pDibObject == NULL ) return;
	
	//在点处理CPretreatment类中创建用来绘制直方图的数据
	CPretreatment Pre( m_pDibObject );
	
	CDlgThreshold Dlg;
	
	// 更改光标形状
	BeginWaitCursor();

	Dlg.m_nThresholdJustAnalyze = Pre.ThresholdJustAnlyze();
	Dlg.m_ThresholdAuto = Pre.ThresholdAuto();
	Dlg.m_nCurrentThreshold = Dlg.m_ThresholdAuto;
	
	//显示对话框
	if (Dlg.DoModal() != IDOK)
		return;


	// 调用Threshold()函数进行阈值化操作
	  Pre.Threshold(Dlg.m_nCurrentThreshold, Dlg.m_nThresholdType, TRUE);

	m_bBinary = TRUE;

	// 更新视图
	UpdateAllViews(NULL);	
	// 恢复光标
	EndWaitCursor();	
}

void CInsectsDoc::OnDelate() 
{
	//结构元素数组指针（存储的是各结构元素相对于结构中心的坐标）
	int *nMask;

	//结构元素的大小（以结构元素中的点数为单位）
	int nMaskLen = 0;


	// 创建对话框
	CDlgMorth dlgPara;
	
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

void CInsectsDoc::OnCancleErsion() 
{
		//结构元素数组指针（存储的是各结构元素相对于结构中心的坐标）
	int *nMask;

	//结构元素的大小（以结构元素中的点数为单位）
	int nMaskLen = 0;


	// 创建对话框
	CDlgMorth dlgPara;
	
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

void CInsectsDoc::OnMorphOpen() 
{
//结构元素数组指针（存储的是各结构元素相对于结构中心的坐标）
	int *nMask;

	//结构元素的大小（以结构元素中的点数为单位）
	int nMaskLen = 0;


	// 创建对话框
	CDlgMorth dlgPara;
	
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

void CInsectsDoc::OnMorphClose() 
{
    
	//结构元素数组指针（存储的是各结构元素相对于结构中心的坐标）
	int *nMask;

	//结构元素的大小（以结构元素中的点数为单位）
	int nMaskLen = 0;


	// 创建对话框
	CDlgMorth dlgPara;
	
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

void CInsectsDoc::OnGeomZoom() 
{
    // 缩放比率
	float fXZoomRatio;
	float fYZoomRatio;
	
	// 创建对话框
	CDlgGenZoom dlgPara;
	
	// 初始化变量值
	dlgPara.m_XZoom = 1.0;
	dlgPara.m_YZoom = 1.0;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	fXZoomRatio = dlgPara.m_XZoom;
	fYZoomRatio = dlgPara.m_YZoom;
	
	// 删除对话框
	delete dlgPara;	
	
	//创建点处理CImageGeoTrans类对象
	CGeoTrans GeoTrans( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();
	
	//调用ZoomImage()函数进行图象缩放
	GeoTrans.ZoomImage(fXZoomRatio, fYZoomRatio);
	
	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();	
}

void CInsectsDoc::OnGeomMirh() 
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

void CInsectsDoc::OnGeomTran() 
{
// 创建对话框
	CDlgGenTran dlgPara;
	
	// 初始化变量值
	dlgPara.m_XOffset = 0;
	dlgPara.m_YOffset = 0;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	LONG lXOffset = dlgPara.m_XOffset;
	LONG lYOffset = dlgPara.m_YOffset;
	// 删除对话框
	delete dlgPara;	
	
	//创建点处理CImageGeoTrans类对象
	CGeoTrans GeoTrans( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	GeoTrans.TranslationPixel(lXOffset, lYOffset);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();	
}

void CInsectsDoc::OnGeomMirv() 
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

void CInsectsDoc::OnGeomTrpo() 
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

void CInsectsDoc::OnGeomRota() 
{
	//旋转角度
	int nRotateAngle;
	
	//创建对话框
	CDlgGenRota dlgPara;
	
	//初始化变量值
	dlgPara.m_iRotateAngle = 90;
	
	//显示对话框，提示用户设定旋转角度
	if(dlgPara.DoModal() != IDOK)
	{
		//返回
		return;
	}
	
	//获取用户设定的角度
	nRotateAngle = dlgPara.m_iRotateAngle;
	
	//删除对话框
	delete dlgPara;	

	//创建点处理CImageGeoTrans类对象
	CGeoTrans GeoTrans( m_pDibObject );

	// 更改光标形状
	BeginWaitCursor();

	//调用Rotate()函数进行图象旋转
	GeoTrans.Rotate(nRotateAngle);

	// 更新视图
	UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();	
}

void CInsectsDoc::OnInfo() 
{
	CDlgInfo dlg;

	dlg.m_nHeight=m_pDibObject->GetHeight();
	dlg.m_nWidth=m_pDibObject->GetWidth();
	dlg.m_nSize=(m_pDibObject->GetWidth()*m_pDibObject->GetHeight())/1024;
	dlg.m_nBits=m_pDibObject->GetNumBits();

	//显示对话框，提示用户设定旋转角度
	if(dlg.DoModal() != IDOK)
	{
		//返回
		return;
	}
}

void CInsectsDoc::OnThin() 
{	
}

void CInsectsDoc::OnAnteenaFactor() 
{
	CDibObject *pDib = new CDibObject(m_pDibObject);
	//创建预处理CIdentify类对象
	CIdentify Ide(pDib);

	//调用相应的分割函数
	Ide.AntennaSep();
	CString str;
	str.Format("%f",Ide.GetAntennaFactor());
    AfxMessageBox(str);
	antenna_factor[antenna_num++]=Ide.GetAntennaFactor();
}

void CInsectsDoc::OnBellyFactor() 
{
	CDibObject *pDib = new CDibObject(m_pDibObject);
	//创建预处理CIdentify类对象
	CIdentify Ide(pDib);

	//调用相应的分割函数
	Ide.BellySep();
	CString str;
	str.Format("%f",Ide.GetBellyFactor());
    AfxMessageBox(str);
	belly_factor[belly_num++]=Ide.GetBellyFactor();
}

void CInsectsDoc::OnAnteenaAvgFactor() 
{
	float total=0.0,avg_factor=0.0;
	for(int i=0;i<antenna_num;i++)
		total+=antenna_factor[i];
	avg_factor=total/antenna_num;
	
	CString str;
	str.Format("%f",avg_factor);
	AfxMessageBox(str);
	
}

void CInsectsDoc::OnBellyAvgFactor() 
{
	float total=0.0;
	for(int i=0;i<belly_num;i++)
		total+=belly_factor[i];
	depart_factor=total/belly_num;
	
	CString str;
	str.Format("%f",depart_factor);
	AfxMessageBox(str);
	
}

void CInsectsDoc::OnContourTrace() 
{


	//获取应用程序类指针
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *counter_trace = new CDibObject(m_pDibObject);
	//创建预处理CIdentify类对象
	CIdentify Ide(counter_trace);

	// 更改光标形状
	BeginWaitCursor();
		
	if (Ide.ContourTrace())
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = counter_trace;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("轮廓提取");

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("轮廓提取错误！");
	
	// 恢复光标
	EndWaitCursor();

}

void CInsectsDoc::OnEdgeTrace() 
{

	//获取应用程序类指针
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *edge_trace = new CDibObject(m_pDibObject);
	//创建预处理CIdentify类对象
	CIdentify Ide(edge_trace);

	// 更改光标形状
	BeginWaitCursor();
		
	if (Ide.EdgeTrace())
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = edge_trace;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("边界跟踪");

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 50, 
			pChild->m_nHeight + 50, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("边界跟踪错误！");
	
	// 恢复光标
	EndWaitCursor();
	
}

void CInsectsDoc::OnUpdateAntennaSep(CCmdUI* pCmdUI) 
{
  
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());
	
}

void CInsectsDoc::OnUpdateBellySep(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());
	
}

void CInsectsDoc::OnUpdateCancleErsion(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateCharacter(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateContourTrace(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateDelate(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateEdgeTrace(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateIdentify(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateLapulasiSharp(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsGrayImage());	
}

void CInsectsDoc::OnUpdateTiduSharp(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsGrayImage());		
}

void CInsectsDoc::OnUpdateThresholdSep(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsGrayImage());		
}

void CInsectsDoc::OnUpdateTempSmooth(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsGrayImage());		
}

void CInsectsDoc::OnUpdateMidianTemp(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsGrayImage());		
}

void CInsectsDoc::OnUpdateMorphClose(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
	
}

void CInsectsDoc::OnUpdateWingSep(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateMorphOpen(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}



void CInsectsDoc::OnHelp() 
{
	//帮助文件
	::ShellExecute(0,"open",".\\help.chm","","",SW_SHOWNORMAL);	
}
