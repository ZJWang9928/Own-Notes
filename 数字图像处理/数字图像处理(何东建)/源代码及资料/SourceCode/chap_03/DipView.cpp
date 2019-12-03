// DipView.cpp : implementation of the CDipView class
//

#include "stdafx.h"
#include "Dip.h"

#include "DipDoc.h"
#include "DipView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDipView

IMPLEMENT_DYNCREATE(CDipView, CScrollView)

BEGIN_MESSAGE_MAP(CDipView, CScrollView)
	//{{AFX_MSG_MAP(CDipView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDipView construction/destruction

CDipView::CDipView()
{
	// TODO: add construction code here

}

CDipView::~CDipView()
{
}

BOOL CDipView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDipView drawing

void CDipView::OnDraw(CDC* pDC)
{
	CDipDoc* pDoc = GetDocument();
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

void CDipView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CDipView printing

BOOL CDipView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDipView diagnostics

#ifdef _DEBUG
void CDipView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDipView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDipDoc* CDipView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDipDoc)));
	return (CDipDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDipView message handlers
