// ImgProView.cpp : implementation of the CImgProView class
//


#include "stdafx.h"
#include "ImgPro.h"

#include "ImgProDoc.h"
#include "ImgProView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "Pretreatment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CImgProView

IMPLEMENT_DYNCREATE(CImgProView, CScrollView)

BEGIN_MESSAGE_MAP(CImgProView, CScrollView)
	//{{AFX_MSG_MAP(CImgProView)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_Salient_Display, OnSalientDisplay)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgProView construction/destruction

CImgProView::CImgProView()
{
	// TODO: add construction code here

}

CImgProView::~CImgProView()
{

}

BOOL CImgProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImgProView drawing

void CImgProView::OnDraw(CDC* pDC)
{
	CImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if( !pDoc->m_bImageLoaded)
	{
		pDoc->ReadImgToDoc();
	}
	
	//滚动窗口
	CSize sizeTotal;
	sizeTotal.cx = pDoc->m_pDibObject->GetWidth();
	sizeTotal.cy = pDoc->m_pDibObject->GetHeight();
	SetScrollSizes (MM_TEXT, sizeTotal);
	
	//获取客户区尺寸
	OnPrepareDC(pDC);
	CRect Rect;
	GetClientRect( &Rect );
	
	//获取图像宽度及高度
	int nImageWidth, nImageHeight;
	nImageWidth = pDoc->m_pDibObject->GetWidth();
	nImageHeight = pDoc->m_pDibObject->GetHeight();
	
	//当图像实际尺寸小于窗口尺寸时，将图像放在客户区中间
	int nX, nY;
	if( nImageWidth < Rect.Width() )
		nX = (Rect.Width() - nImageWidth ) / 2;
	else
		nX = 0;
	
	if( nImageHeight < Rect.Height() )
		nY = (Rect.Height() - nImageHeight ) / 2;
	else	
		nY = 0;
	
	pDoc->m_pDibObject->Draw(pDC, nX, nY);
}

void CImgProView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CImgProView printing

BOOL CImgProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImgProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImgProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImgProView diagnostics

#ifdef _DEBUG
void CImgProView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImgProView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImgProDoc* CImgProView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImgProDoc)));
	return (CImgProDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

void CImgProView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu Contextmenu;
	Contextmenu.LoadMenu (IDR_CONTEXTMENU);
	CMenu *pContextmenu=Contextmenu.GetSubMenu (0);
	pContextmenu->TrackPopupMenu (TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	Contextmenu.DestroyMenu ();
}

void CImgProView::OnSalientDisplay() 
{
    CImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( !pDoc->m_bImageLoaded)
	{
		pDoc->ReadImgToDoc();
	}
	//滚动窗口
	CSize sizeTotal;
	sizeTotal.cx = pDoc->m_pDibObject->GetWidth();
	sizeTotal.cy = pDoc->m_pDibObject->GetHeight();
	SetScrollSizes (MM_TEXT, sizeTotal);
	
	//获取客户区尺寸

	CRect Rect,Rect1;
	GetClientRect( &Rect );
	//获取图像宽度及高度
	int nImageWidth, nImageHeight;
	nImageWidth = pDoc->m_pDibObject->GetWidth();
	nImageHeight = pDoc->m_pDibObject->GetHeight();
	//LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);
	//int cxDIB = (int) ::DIBWidth(lpDIB);         
	//int cyDIB = (int) ::DIBHeight(lpDIB);     
	int i,j;
	#define corner(ROW,COL) corner[nImageWidth*(ROW)+(COL)]
     
	CDC *dc=GetDC();
	// 创建红色画笔对象
	CPen* pPenRed = new CPen;
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	dc->SelectObject(pPenRed);
	FILE *fp;
	fp=fopen("Corner.txt","w");
	for(i = 0; i < nImageHeight; i++)
	{
		for(j = 0; j < nImageWidth; j++)
		{
				fprintf(fp,"%d ",pDoc->corner(i,j));
				if(pDoc->corner(i,j))
				{
					//在角点处划十字叉以标注
					dc->MoveTo ((Rect.Width()-nImageWidth)/2+j-2,(Rect.Height()-nImageHeight)/2+i);
					dc->LineTo ((Rect.Width()-nImageWidth)/2+j+3,(Rect.Height()-nImageHeight)/2+i);
					dc->MoveTo ((Rect.Width()-nImageWidth)/2+j,(Rect.Height()-nImageHeight)/2+i-2);
					dc->LineTo ((Rect.Width()-nImageWidth)/2+j,(Rect.Height()-nImageHeight)/2+i+3);
				}
		}
		fprintf(fp,"\n");
	}
		
}


