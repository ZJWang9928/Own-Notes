// DlgHitMiss.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgHitMiss.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHitMiss dialog


CDlgHitMiss::CDlgHitMiss(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHitMiss::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHitMiss)
		// NOTE: the ClassWizard will add member initialization here
	m_nStruH  = 2;
	m_nStruW  = 2;
	m_nStruXc = 1;
	m_nStruYc = 1;
	m_bStru00 = 0;
	m_bStru01 = 1;
	m_bStru02 = 0;
	m_bStru03 = 0;
	m_bStru04 = 0;
	m_bStru05 = 2;
	m_bStru06 = 1;
	m_bStru07 = 0;
	m_bStru08 = 0;
	m_bStru09 = 0;
	m_bStru10 = 0;
	m_bStru11 = 0;
	m_bStru12 = 0;
	m_bStru13 = 0;
	m_bStru14 = 0;
	m_bStru15 = 0;
	m_bStru16 = 0;
	m_bStru17 = 0;
	m_bStru18 = 0;
	m_bStru19 = 0;
	m_bStru20 = 0;
	m_bStru21 = 0;
	m_bStru22 = 0;
	m_bStru23 = 0;
	m_bStru24 = 0;
	//}}AFX_DATA_INIT
	m_nHmaskLen = 0;
	m_nMmaskLen = 0;
}


void CDlgHitMiss::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHitMiss)
	DDX_Control(pDX, IDC_STRU00, m_ctrlStru00);
	DDX_Control(pDX, IDC_STRU01, m_ctrlStru01);
	DDX_Control(pDX, IDC_STRU02, m_ctrlStru02);
	DDX_Control(pDX, IDC_STRU03, m_ctrlStru03);
	DDX_Control(pDX, IDC_STRU04, m_ctrlStru04);
	DDX_Control(pDX, IDC_STRU05, m_ctrlStru05);
	DDX_Control(pDX, IDC_STRU06, m_ctrlStru06);
	DDX_Control(pDX, IDC_STRU07, m_ctrlStru07);
	DDX_Control(pDX, IDC_STRU08, m_ctrlStru08);
	DDX_Control(pDX, IDC_STRU09, m_ctrlStru09);
	DDX_Control(pDX, IDC_STRU10, m_ctrlStru10);
	DDX_Control(pDX, IDC_STRU11, m_ctrlStru11);
	DDX_Control(pDX, IDC_STRU12, m_ctrlStru12);
	DDX_Control(pDX, IDC_STRU13, m_ctrlStru13);
	DDX_Control(pDX, IDC_STRU14, m_ctrlStru14);
	DDX_Control(pDX, IDC_STRU15, m_ctrlStru15);
	DDX_Control(pDX, IDC_STRU16, m_ctrlStru16);
	DDX_Control(pDX, IDC_STRU17, m_ctrlStru17);
	DDX_Control(pDX, IDC_STRU18, m_ctrlStru18);
	DDX_Control(pDX, IDC_STRU19, m_ctrlStru19);
	DDX_Control(pDX, IDC_STRU20, m_ctrlStru20);
	DDX_Control(pDX, IDC_STRU21, m_ctrlStru21);
	DDX_Control(pDX, IDC_STRU22, m_ctrlStru22);
	DDX_Control(pDX, IDC_STRU23, m_ctrlStru23);
	DDX_Control(pDX, IDC_STRU24, m_ctrlStru24);
	DDX_Control(pDX, IDC_EDIT_STRU_YC_SPIN, m_ctrlStruYCSpin);
	DDX_Control(pDX, IDC_EDIT_STRU_XC_SPIN, m_ctrlStruXCSpin);
	DDX_Control(pDX, IDC_EDIT_STRU_W_SPIN, m_ctrlStruWSpin);
	DDX_Control(pDX, IDC_EDIT_STRU_H_SPIN, m_ctrlStruHSpin);
	DDX_Text(pDX, IDC_EDIT_STRU_H, m_nStruH);
	DDV_MinMaxInt(pDX, m_nStruH, 1, 5);
	DDX_Text(pDX, IDC_EDIT_STRU_W, m_nStruW);
	DDV_MinMaxInt(pDX, m_nStruW, 1, 5);
	DDX_Text(pDX, IDC_EDIT_STRU_XC, m_nStruXc);
	DDV_MinMaxInt(pDX, m_nStruXc, 0, 4);
	DDX_Text(pDX, IDC_EDIT_STRU_YC, m_nStruYc);
	DDV_MinMaxInt(pDX, m_nStruYc, 0, 4);
	DDX_Check(pDX, IDC_STRU00, m_bStru00);
	DDX_Check(pDX, IDC_STRU01, m_bStru01);
	DDX_Check(pDX, IDC_STRU02, m_bStru02);
	DDX_Check(pDX, IDC_STRU03, m_bStru03);
	DDX_Check(pDX, IDC_STRU04, m_bStru04);
	DDX_Check(pDX, IDC_STRU05, m_bStru05);
	DDX_Check(pDX, IDC_STRU06, m_bStru06);
	DDX_Check(pDX, IDC_STRU07, m_bStru07);
	DDX_Check(pDX, IDC_STRU08, m_bStru08);
	DDX_Check(pDX, IDC_STRU09, m_bStru09);
	DDX_Check(pDX, IDC_STRU10, m_bStru10);
	DDX_Check(pDX, IDC_STRU11, m_bStru11);
	DDX_Check(pDX, IDC_STRU12, m_bStru12);
	DDX_Check(pDX, IDC_STRU13, m_bStru13);
	DDX_Check(pDX, IDC_STRU14, m_bStru14);
	DDX_Check(pDX, IDC_STRU15, m_bStru15);
	DDX_Check(pDX, IDC_STRU16, m_bStru16);
	DDX_Check(pDX, IDC_STRU17, m_bStru17);
	DDX_Check(pDX, IDC_STRU18, m_bStru18);
	DDX_Check(pDX, IDC_STRU19, m_bStru19);
	DDX_Check(pDX, IDC_STRU20, m_bStru20);
	DDX_Check(pDX, IDC_STRU21, m_bStru21);
	DDX_Check(pDX, IDC_STRU22, m_bStru22);
	DDX_Check(pDX, IDC_STRU23, m_bStru23);
	DDX_Check(pDX, IDC_STRU24, m_bStru24);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHitMiss, CDialog)
	//{{AFX_MSG_MAP(CDlgHitMiss)
	ON_EN_KILLFOCUS(IDC_EDIT_STRU_H, OnKillfocusEditStruH)
	ON_EN_KILLFOCUS(IDC_EDIT_STRU_W, OnKillfocusEditStruW)
	ON_EN_KILLFOCUS(IDC_EDIT_STRU_XC, OnKillfocusEditStruXc)
	ON_EN_KILLFOCUS(IDC_EDIT_STRU_YC, OnKillfocusEditStruYc)
	ON_EN_CHANGE(IDC_EDIT_STRU_H, OnChangeEditStruH)
	ON_EN_CHANGE(IDC_EDIT_STRU_W, OnChangeEditStruW)
	ON_EN_CHANGE(IDC_EDIT_STRU_XC, OnChangeEditStruXc)
	ON_EN_CHANGE(IDC_EDIT_STRU_YC, OnChangeEditStruYc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHitMiss message handlers
void CDlgHitMiss::UpdateEdit()
{
	BOOL	bEnable = TRUE;
	
	//循环变量
	int		i;
	int		j;
	
	//设置文本框及其微调控件的状态
	(CEdit *) GetDlgItem(IDC_EDIT_STRU_H)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_STRU_H_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_STRU_W)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_STRU_W_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_STRU_XC)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_STRU_XC_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_STRU_YC)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_STRU_YC_SPIN)->EnableWindow(bEnable);
	
	//IDC_STRU00等ID其实是一个整数，它的数值定义在Resource.h中定义。
	
	int nMid = m_nStruYc * 5 + m_nStruXc;

	//设置模板元素文本框Enable状态
	for (i = IDC_STRU00; i <= IDC_STRU24; i++)
	{
		//设置检查框为可用状态
		(CButton *) GetDlgItem(i)->EnableWindow(bEnable);
	}
	
	//显示应该可用的结构元素检查框
	for (i = 0; i < m_nStruH; i++)
	{
		for (j = 0; j < m_nStruW; j++)
		{
			//设置检查框为可用状态
			(CButton *) GetDlgItem(IDC_STRU00 + i * 5 + j)->ShowWindow(SW_SHOW);
		}
	}

	//隐藏不可用状态的检查框（前m_nStruH行的后几列）
	for (i = 0; i < m_nStruH; i++)
	{
		for (j = m_nStruW; j < 5; j++)
		{
			//设置为不可用
			(CButton *) GetDlgItem(IDC_STRU00 + i * 5 + j)->ShowWindow(SW_HIDE);
		}
	}
	
	//隐藏应该不可见的模板元素文本框（后几行）
	for (i = m_nStruH; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			//设置为不可用
			(CButton *) GetDlgItem(IDC_STRU00 + i * 5 + j)->ShowWindow(SW_HIDE);
		}
	}

}

BOOL CDlgHitMiss::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//设置微调控件的范围和伙伴编辑框

	m_ctrlStruWSpin.SetRange(1, 5);
	m_ctrlStruWSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_STRU_W));
	
	m_ctrlStruHSpin.SetRange(1, 5);
	m_ctrlStruHSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_STRU_H));
	
	m_ctrlStruXCSpin.SetRange(0, 4);
	m_ctrlStruXCSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_STRU_XC));

	m_ctrlStruYCSpin.SetRange(0, 4);
	m_ctrlStruYCSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_STRU_YC));
	
	UpdateEdit();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHitMiss::OnOK() 
{
	// 获取用户设置（更新）
	UpdateData(TRUE);
	
	// 判断设置是否有效
	if ((m_nStruW < 0) || (m_nStruXc > m_nStruW - 1) ||
		(m_nStruH < 0) || (m_nStruYc > m_nStruH - 1))
	{
		// 提示用户参数设置错误
		MessageBox("中心元素参数设置错误！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	int Mask[200];

	m_nHmaskLen = 0;

	int i, j, k = 0;

	//根据对检查框的选择状态给结构元素赋值（记录的相应点的X，Y坐标）
	for(i = 0; i < m_nStruH; i++)
	{
		for(j = 0; j < m_nStruW; j++)
		{
			//IDC_STRU00等ID其实是一个整数，它的数值定义在Resource.h中定义。
			
			//获取相应检查框控件的指针
			CButton *pCheckBox = (CButton *) GetDlgItem(IDC_STRU00 + i * 5 + j);
			//检查框的状态
			if(pCheckBox->GetCheck() == 1)
			{
				//结构元素计数（以点数为计数单位）
				m_nHmaskLen++;
				Mask[k] = j - m_nStruXc;
				k++;
				Mask[k] = m_nStruYc - i;
				k++;
			}
		}
	}

	//创建记录结构元素的数组
	m_npHmask = new int [2 * m_nHmaskLen];

	//将统计出的结构元素拷贝到m_npMask指向的数组中
	memcpy(m_npHmask, Mask, 2 * m_nHmaskLen * sizeof(int));
	
	m_nMmaskLen = 0;

	k = 0;

	//根据对检查框的选择状态给结构元素赋值（记录的相应点的X，Y坐标）
	for(i = 0; i < m_nStruH; i++)
	{
		for(j = 0; j < m_nStruW; j++)
		{
			//IDC_STRU00等ID其实是一个整数，它的数值定义在Resource.h中定义。
			
			//获取相应检查框控件的指针
			CButton *pCheckBox = (CButton *) GetDlgItem(IDC_STRU00 + i * 5 + j);
			//检查框的状态
			if(pCheckBox->GetCheck() == 0)
			{
				//结构元素计数（以点数为计数单位）
				m_nMmaskLen++;
				Mask[k] = j - m_nStruXc;
				k++;
				Mask[k] = m_nStruYc - i;
				k++;
			}
		}
	}

	//创建记录结构元素的数组
	m_npMmask = new int [2 * m_nMmaskLen];

	//将统计出的结构元素拷贝到m_npMask指向的数组中
	memcpy(m_npMmask, Mask, 2 * m_nMmaskLen * sizeof(int));

	CDialog::OnOK();
}

void CDlgHitMiss::OnKillfocusEditStruH() 
{
	// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();
}

void CDlgHitMiss::OnKillfocusEditStruW() 
{
	// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();
}

void CDlgHitMiss::OnKillfocusEditStruXc() 
{
	// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();
}

void CDlgHitMiss::OnKillfocusEditStruYc() 
{
	// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();
}

void CDlgHitMiss::OnChangeEditStruH() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();

}

void CDlgHitMiss::OnChangeEditStruW() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();
}

void CDlgHitMiss::OnChangeEditStruXc() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();
}

void CDlgHitMiss::OnChangeEditStruYc() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
}
