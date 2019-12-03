// InsectsView.cpp : implementation of the CInsectsView class
//

#include "stdafx.h"
#include "Insects.h"

#include "InsectsDoc.h"
#include "InsectsView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "Pretreatment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsectsView

IMPLEMENT_DYNCREATE(CInsectsView, CScrollView)

BEGIN_MESSAGE_MAP(CInsectsView, CScrollView)
	//{{AFX_MSG_MAP(CInsectsView)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsectsView construction/destruction

CInsectsView::CInsectsView()
{
	// TODO: add construction code here

}

CInsectsView::~CInsectsView()
{
}

BOOL CInsectsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CInsectsView drawing

void CInsectsView::OnDraw(CDC* pDC)
{
	CInsectsDoc* pDoc = GetDocument();
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

void CInsectsView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CInsectsView printing

BOOL CInsectsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CInsectsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CInsectsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CInsectsView diagnostics

#ifdef _DEBUG
void CInsectsView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CInsectsView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CInsectsDoc* CInsectsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInsectsDoc)));
	return (CInsectsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

void CInsectsView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu Contextmenu;
	Contextmenu.LoadMenu (IDR_CONTEXTMENU);
	CMenu *pContextmenu=Contextmenu.GetSubMenu (0);
	pContextmenu->TrackPopupMenu (TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	Contextmenu.DestroyMenu ();
}
