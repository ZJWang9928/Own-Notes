// DlgShannon.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgShannon.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon dialog


CDlgShannon::CDlgShannon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShannon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShannon)
	m_dEfficiency = 0.0;
	m_dAvgCodeLen = 0.0;
	m_dEntropy = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgShannon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShannon)
	DDX_Control(pDX, IDC_LST_Table, m_lstTable);
	DDX_Text(pDX, IDC_EDIT_CODEEF, m_dEfficiency);
	DDX_Text(pDX, IDC_EDIT_CODELEN, m_dAvgCodeLen);
	DDX_Text(pDX, IDC_EDIT_ENTROPY, m_dEntropy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShannon, CDialog)
	//{{AFX_MSG_MAP(CDlgShannon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon message handlers

BOOL CDlgShannon::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ShannonCode();

	// 返回TRUE
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShannon::ShannonCode()
{
	int i,j;
	BYTE uTempMap;
	CString str;
	//各信源符号的代码长度
	int iCodeLen;
	
	//各信源符号对应的累加概率	
	FLOAT fTemp = 0.0f,iAccP = 0.0f;

	//分配内存
	m_strCode = new CString[m_iColorNum];
	
	//采用冒泡法对待编码的信源符号按出现概率从大到小排序
	for(j=0;j<m_iColorNum-1;j++)
	{
		for(i=0;i<m_iColorNum-j-1;i++)
		{
			if( m_fFreq[i] < m_fFreq[i+1] )
			{
				fTemp = m_fFreq[i];
				m_fFreq[i] = m_fFreq[i+1];
				m_fFreq[i+1] = fTemp;

				//调整映射关系
				uTempMap = m_uMap[i];
				m_uMap[i] = m_uMap[i+1];
				m_uMap[i+1] = uTempMap;
			}
		}
	}
	
	//按照最佳编码理论逐个求出各信源符号的码字
	for(i=0;i<m_iColorNum;i++)
	{
		//按最佳编码理论计算出第i个符号的码字长度iCodeLen
		fTemp = (float)(-log(m_fFreq[i])/log(2));
		if(fTemp > (int)fTemp) iCodeLen = (int)fTemp + 1;
		else iCodeLen = (int)fTemp;
		
		//将对应的累加概率转换为长度为iCodeLen的二进制数
		j = 0;
		fTemp = iAccP;
		while(j < iCodeLen)
		{
			if( fTemp*2 >= 1.0 )
			{
				m_strCode[i] += "1";
				fTemp = fTemp*2 - 1;
			}
			else
			{
				m_strCode[i] += "0";
				fTemp = fTemp * 2;
			}
			j++;
		}
		
		//计算下一个信源符号对应的累加概率iAccP(第一个符号的累加概率为0)
		iAccP += m_fFreq[i];
	}

	// 计算平均码字长度
	for (i = 0; i < m_iColorNum; i ++)
	{
		// 累加
		m_dAvgCodeLen += m_fFreq[i] * m_strCode[i].GetLength();
		//计算图像熵
		m_dEntropy -= m_fFreq[i] * log(m_fFreq[i]) / log(2.0);
	}
	
	// 计算编码效率
	m_dEfficiency = m_dEntropy / m_dAvgCodeLen;

	//更新对话框上的文本框中的内容
	UpdateData(FALSE);

	// 输出计算结果
	
	// ListCtrl的ITEM
	LV_ITEM lvitem;
	
	// 中间变量，保存ListCtrl中添加的ITEM编号
	int		iActualItem;
	
	// 设置List控件样式
	m_lstTable.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	
	// 给List控件添加Header
	m_lstTable.InsertColumn(0, "灰度值", LVCFMT_LEFT, 60, 0);
	m_lstTable.InsertColumn(1, "出现频率", LVCFMT_LEFT, 78, 0);
	m_lstTable.InsertColumn(2, "香农弗诺编码", LVCFMT_LEFT, 110, 1);
	m_lstTable.InsertColumn(3, "码字长度", LVCFMT_LEFT, 78, 2);
	
	// 设置样式为文本
	lvitem.mask = LVIF_TEXT;
	
	for (i = 0; i < m_iColorNum; i ++)
	{
		// 添加一项
		lvitem.iItem = m_lstTable.GetItemCount();
		str.Format("%u",m_uMap[i]);
		lvitem.iSubItem = 0;
		lvitem.pszText= (LPTSTR)(LPCTSTR)str;
		iActualItem = m_lstTable.InsertItem(&lvitem);
		
		// 添加其它列
		lvitem.iItem = iActualItem;
		
		// 添加灰度值出现的频率
		lvitem.iSubItem = 1;
		str.Format("%f",m_fFreq[i]);
		lvitem.pszText = (LPTSTR)(LPCTSTR)str;
		m_lstTable.SetItem(&lvitem);
		
		// 添加香农弗诺编码
		lvitem.iSubItem = 2;
		lvitem.pszText = (LPTSTR)(LPCTSTR)m_strCode[i];
		m_lstTable.SetItem(&lvitem);
		
		// 添加码字长度
		lvitem.iSubItem = 3;
		str.Format("%u",m_strCode[i].GetLength());
		lvitem.pszText = (LPTSTR)(LPCTSTR)str;
		m_lstTable.SetItem(&lvitem);
	}
	delete []m_strCode;
}