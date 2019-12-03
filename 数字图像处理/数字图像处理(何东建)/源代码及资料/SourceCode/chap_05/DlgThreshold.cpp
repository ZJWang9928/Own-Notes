// DlgThreshold.cpp : implementation file
//

#include "stdafx.h"
#include "Dip.h"
#include "DlgThreshold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgThreshold dialog


CDlgThreshold::CDlgThreshold(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgThreshold::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgThreshold)
	m_nCurrentThreshold = 0;
	m_iLowGray = 0;
	m_iUpGray = 255;
	m_nThreshold = 0;
	m_nThresholdType = 0;
	//}}AFX_DATA_INIT
	m_pnHistogram = NULL;
	m_nThresholdAutoAdjust = 0;
	m_nThresholdDiscriminant = 0;
	m_bIsMethoed3 = FALSE;
	m_bRMouseDown = FALSE;
}


void CDlgThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgThreshold)
	DDX_Control(pDX, IDC_EDIT_CUR_TRESHOLD, m_ctrlCurEdit);
	DDX_Control(pDX, IDC_STATIC_CURTHRESH, m_ctrlCurStatic);
	DDX_Control(pDX, IDC_EDIT_CUR_TRESHSPIN, m_ctrlCurrentSpin);
	DDX_Control(pDX, IDC_EDIT_UPSPIN, m_ctrlUpGraySpin);
	DDX_Control(pDX, IDC_EDIT_LOWSPIN, m_ctrlLowGraySpin);
	DDX_Text(pDX, IDC_EDIT_CUR_TRESHOLD, m_nCurrentThreshold);
	DDV_MinMaxUInt(pDX, m_nCurrentThreshold, 0, 255);
	DDX_Text(pDX, IDC_EDIT_LowGray, m_iLowGray);
	DDV_MinMaxInt(pDX, m_iLowGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT_UpGray, m_iUpGray);
	DDX_Radio(pDX, IDC_THRESH_METHOD1, m_nThreshold);
	DDX_Radio(pDX, IDC_TRESHOLD_WHITE, m_nThresholdType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgThreshold, CDialog)
	//{{AFX_MSG_MAP(CDlgThreshold)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_THRESH_METHOD3, OnThreshMethod3)
	ON_BN_CLICKED(IDC_THRESH_METHOD1, OnThreshMethod1)
	ON_BN_CLICKED(IDC_THRESH_METHOD2, OnThreshMethod2)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(IDC_EDIT_LowGray, OnChangeEDITLowGray)
	ON_EN_CHANGE(IDC_EDIT_UpGray, OnChangeEDITUpGray)
	ON_EN_CHANGE(IDC_EDIT_CUR_TRESHOLD, OnChangeEditCurTreshold)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgThreshold message handlers

BOOL CDlgThreshold::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	// 调用默认OnInitDialog函数
	CDialog::OnInitDialog();
	
	// 获取绘制直方图的标签
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// 计算接受鼠标事件的有效区域
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	
	// 设置接受鼠标事件的有效区域
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	
	// 初始化拖动状态
	m_iIsDraging = 0;

	//设置微调控件的范围和伙伴编辑框
	m_ctrlCurrentSpin.SetRange(0,255);
	m_ctrlCurrentSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_CUR_TRESHOLD));

	m_ctrlLowGraySpin.SetRange(0,255);
	m_ctrlLowGraySpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_LowGray));
	
	m_ctrlUpGraySpin.SetRange(0,255);
	m_ctrlUpGraySpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_UpGray));

	m_ctrlCurEdit.SetReadOnly(TRUE);
	m_ctrlCurrentSpin.EnableWindow(FALSE);
	m_ctrlCurStatic.EnableWindow(FALSE);

	// 返回TRUE
	return TRUE;
}

void CDlgThreshold::OnPaint() 
{
	// 字符串
	CString str;
	
	// 循环变量
	LONG i;
	
	// 最大计数
	LONG lMaxCount = 0;
	
	// 设备上下文
	CPaintDC dc(this);
	
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,300);
	
	// 创建画笔对象
	CPen* pPenRed = new CPen;
	
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	
	// 创建画笔对象
	CPen* pPenGreen = new CPen;
	
	// 绿色画笔
	pPenGreen->CreatePen(PS_SOLID,1,RGB(0,255,0));
	
	// 创建画笔对象
	CPen* pPenBrow = new CPen;
	
	// 褐色画笔
	pPenBrow->CreatePen(PS_DOT,1,RGB(128,64,64));

	// 选中当前黑色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// 绘制坐标轴
	pDC->MoveTo(10,6);
	
	// 垂直轴
	pDC->LineTo(10,276);
	
	// 水平轴
	pDC->LineTo(320,276);
	
	// 写X轴刻度值
	str.Format("0");
	pDC->TextOut(10, 279, str);
	str.Format("50");
	pDC->TextOut(60, 279, str);
	str.Format("100");
	pDC->TextOut(110, 279, str);
	str.Format("150");
	pDC->TextOut(160, 279, str);
	str.Format("200");
	pDC->TextOut(210, 279, str);
	str.Format("255");
	pDC->TextOut(265, 279, str);
	
	// 绘制X轴刻度
	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10的倍数
			pDC->MoveTo(i + 10, 276);
			pDC->LineTo(i + 10, 280);
		}
		else
		{
			// 10的倍数
			pDC->MoveTo(i + 10, 276);
			pDC->LineTo(i + 10, 278);
		}
	}
	
	// 绘制X轴箭头
	pDC->MoveTo(315,271);
	pDC->LineTo(320,276);
	pDC->LineTo(315,281);
	
	// 绘制X轴箭头
	pDC->MoveTo(10,6);
	pDC->LineTo(5,11);
	pDC->MoveTo(10,6);
	pDC->LineTo(15,11);
	
	// 计算最大计数值
	for (i = m_iLowGray; i <= m_iUpGray; i ++)
	{
		if ( m_pnHistogram[ i ] > lMaxCount )
		{
			// 更新最大值
			lMaxCount = m_pnHistogram[ i ];
		}
	}
	
	// 输出最大计数值
	pDC->MoveTo(10, 21);
	pDC->LineTo(14, 21);
	str.Format("%d", lMaxCount);
	pDC->TextOut(11, 22, str);
	
	// 更改成褐色画笔
	pDC->SelectObject(pPenBrow);	
	
	// 绘制窗口上下限
	pDC->MoveTo(m_iLowGray + 10, 21);
	pDC->LineTo(m_iLowGray + 10, 276);
	
	pDC->MoveTo(m_iUpGray + 10, 21);
	pDC->LineTo(m_iUpGray + 10, 276);
	
	// 更改画笔
	pDC->SelectObject(pPenBlue);	
	
	// 判断是否有计数
	if (lMaxCount > 0)
	{
		// 绘制直方图
		for (i = m_iLowGray; i <= m_iUpGray; i ++)
		{
			pDC->MoveTo(i + 10, 276);
			pDC->LineTo(i + 10, 277 - (int) (m_pnHistogram[ i ] * 256 / lMaxCount));
		}
	}
	
	// 更改成黄色画笔
	pDC->SelectObject(pPenGreen);	
	
	// 绘制窗口上下限
	pDC->MoveTo(m_nCurrentThreshold + 10, 21);
	pDC->LineTo(m_nCurrentThreshold + 10, 276);
	
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
	delete pPenBrow;
}

void CDlgThreshold::OnThreshMethod3() 
{
	m_bIsMethoed3 = TRUE;

	// 保存变动（控件中数值保存到成员变量中）
	UpdateData(TRUE);

	m_ctrlCurEdit.SetReadOnly(FALSE);
	m_ctrlCurrentSpin.EnableWindow(TRUE);
	m_ctrlCurStatic.EnableWindow(TRUE);

	UpdateData(FALSE);

	// 重绘直方图
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgThreshold::OnThreshMethod1() 
{
	m_bIsMethoed3 = FALSE;

	// 保存变动（控件中数值保存到成员变量中）
	UpdateData(TRUE);

	m_nCurrentThreshold = m_nThresholdAutoAdjust;
	
	// 保存变动（成员变量中数值保存到控件中）
	UpdateData(FALSE);

	m_ctrlCurEdit.SetReadOnly(TRUE);
	m_ctrlCurrentSpin.EnableWindow(FALSE);
	m_ctrlCurStatic.EnableWindow(FALSE);

	// 重绘直方图
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgThreshold::OnThreshMethod2() 
{
	m_bIsMethoed3 = FALSE;

	// 保存变动（控件中数值保存到成员变量中）
	UpdateData(TRUE);

	m_nCurrentThreshold = m_nThresholdDiscriminant;
	
	// 保存变动（成员变量中数值保存到控件中）
	UpdateData(FALSE);

	m_ctrlCurEdit.SetReadOnly(TRUE);
	m_ctrlCurrentSpin.EnableWindow(FALSE);
	m_ctrlCurStatic.EnableWindow(FALSE);

	// 重绘直方图
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgThreshold::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	// 判断是否在接受鼠标事件的有效区域中
	if(m_MouseRect.PtInRect(point))
	{
		if(	m_bIsMethoed3 && !m_bRMouseDown )
		{
			::SetCursor(::LoadCursor(NULL,IDC_CROSS));
			m_nCurrentThreshold = point.x-20;
			UpdateData(FALSE);
			InvalidateRect(m_MouseRect, TRUE);
		}
		
			// 判断是否正在拖动
		if (m_iIsDraging != 0)
		{
			// 判断正在拖动上限还是下限
			if (m_iIsDraging == 1)
			{
				// 判断是否下限<上限
				if (point.x - m_MouseRect.left < m_iUpGray)
				{
					// 更改下限
					m_iLowGray = point.x - m_MouseRect.left;
				}
				else
				{
					// 下限拖过上限，设置为上限-1
					m_iLowGray = m_iUpGray - 1;
					
					// 重设鼠标位置
					point.x = m_MouseRect.left + m_iUpGray - 1;
				}
			}
			else
			{
				// 正在拖动上限
				
				// 判断是否上限>下限
				if (point.x - m_MouseRect.left > m_iLowGray)
				{
					// 更改下限
					m_iUpGray = point.x - m_MouseRect.left;
				}
				else
				{
					// 下限拖过上限，设置为下限＋1
					m_iUpGray = m_iLowGray + 1;
					
					// 重设鼠标位置
					point.x = m_MouseRect.left + m_iLowGray + 1;
				}
			}
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			
			// 更新
			UpdateData(FALSE);
			
			// 重绘直方图
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_iLowGray) || point.x == (m_MouseRect.left + m_iUpGray))
		{
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// 默认鼠标移动处理事件
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgThreshold::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 当用户单击鼠标左键开始拖动
	
	// 判断是否在接受鼠标事件的有效区域中
	if(m_MouseRect.PtInRect(point))
	{
		m_bRMouseDown = FALSE;
		if(	m_bIsMethoed3 && !m_bRMouseDown )
		{
			::SetCursor(::LoadCursor(NULL,IDC_CROSS));
			m_nCurrentThreshold = point.x-20;
			UpdateData(FALSE);
			InvalidateRect(m_MouseRect, TRUE);
		}
		if (point.x == (m_MouseRect.left + m_iLowGray))
		{
			
			// 设置拖动状态1，拖动下限
			m_iIsDraging = 1;
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_iUpGray))
		{
			
			// 设置拖动状态为2，拖动上限
			m_iIsDraging = 2;
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// 默认单击鼠标左键处理事件
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgThreshold::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// 当用户释放鼠标左键停止拖动
	if (m_iIsDraging != 0)
	{
		// 重置拖动状态
		m_iIsDraging = 0;
	}
	
	// 默认释放鼠标左键处理事件
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgThreshold::OnChangeEDITLowGray() 
{
	// 保存变动（控件中数值保存到成员变量中）
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_iLowGray > m_iUpGray)
	{
		// 互换
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		
		// 更新（成员变量中数值保存到控件中）
		UpdateData(FALSE);
	}
	
	// 重绘直方图
	InvalidateRect(m_MouseRect, TRUE);
	
}

void CDlgThreshold::OnChangeEDITUpGray() 
{
	// 保存变动（控件中数值保存到成员变量中）
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_iLowGray > m_iUpGray)
	{
		// 互换
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		
		// 更新（成员变量中数值保存到控件中）
		UpdateData(FALSE);
	}
	
	// 重绘直方图
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgThreshold::OnChangeEditCurTreshold() 
{
	// 保存变动（控件中数值保存到成员变量中）
	UpdateData(TRUE);

	// 更新（成员变量中数值保存到控件中）
	UpdateData(FALSE);
	
	// 重绘直方图
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgThreshold::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// 判断是否在接受鼠标事件的有效区域中
	if(m_MouseRect.PtInRect(point))
	{
		m_bRMouseDown = TRUE;
	}
	CDialog::OnRButtonDown(nFlags, point);
}
