// DlgHuffman.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgHuffman.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHuffman dialog


CDlgHuffman::CDlgHuffman(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHuffman::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHuffman)
	m_ChannelSel = 0;
	m_fCodeEfficiency = 0.0f;
	m_fEntropy = 0.0f;
	m_fAvCodeLen = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgHuffman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHuffman)
	DDX_Control(pDX, IDC_LST_Table, m_lstTable);
	DDX_Radio(pDX, IDC_RADIO_GRAY, m_ChannelSel);
	DDX_Text(pDX, IDC_EDIT_CODEEF, m_fCodeEfficiency);
	DDX_Text(pDX, IDC_EDIT_ENTROPY, m_fEntropy);
	DDX_Text(pDX, IDC_EDIT_CODELEN, m_fAvCodeLen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHuffman, CDialog)
	//{{AFX_MSG_MAP(CDlgHuffman)
	ON_BN_CLICKED(IDC_RADIO_GREEN, OnRadioGreen)
	ON_BN_CLICKED(IDC_RADIO_BLUE, OnRadioBlue)
	ON_BN_CLICKED(IDC_RADIO_RED, OnRadioRed)
	ON_BN_CLICKED(IDC_RADIO_GRAY, OnRadioGray)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHuffman message handlers

void CDlgHuffman::Huffman()
{
	CString str;
	int i,j,x1,x2,c,p;
	int m1,m2;
	LV_ITEM lvitem;				//ListCtrl的ITEM
	int iActualItem;			//中间变量，保存ListCtrl中添加的ITEM编号
	HuffmanNode * huffNode;	//存放生成哈夫曼树的结点
	CString * strCode;		//指向各字符的哈夫曼编码指针
	int *iMap;					//位置映射指针
	int iNumNode = 0;			//频数不为零的结点数(或灰度数)

	//统计直方图数据中频数不为零的灰度数
	for(i=0;i<m_iLeafNum;i++)
	{
		if(m_fFreq[i+m_ChannelSel*m_iLeafNum] != 0) iNumNode++;
	}

	//分配内存，对于具有n个叶结点的哈夫曼树而言，它具有2n-1个结点
	huffNode = new HuffmanNode [iNumNode*2-1];
	strCode = new CString [iNumNode];
	iMap = new int[iNumNode];
	//初始化huffNode中的各结点
	for(i=0;i<iNumNode*2-1;i++)
	{
		huffNode[i].Weight = 0;
		huffNode[i].Parent = 0;
		huffNode[i].Flag = 0;
		huffNode[i].LChild = -1;
		huffNode[i].RChild = -1;
	}

	//输入iNumNode个叶结点的权值或者说频数,iMap中存放的是频数不为零的灰度值
	j = 0;
	for(i=0;i<m_iLeafNum;i++)
	{
		if(m_fFreq[i+m_ChannelSel*m_iLeafNum] != 0)
		{
			huffNode[j].Weight = m_fFreq[i+m_ChannelSel*m_iLeafNum];
			iMap[j] = i;
			j++;
		}
	}

	//构造哈夫曼树,对iNumNode个叶结点，需构造iNumNode-1次
	for(i=0;i<iNumNode-1;i++)
	{
		m1=m2=m_iSizeImage;//比较排序中用于选出两个最小子树的变量
		x1=x2=0;//用于确定最小的两个子树在结点数组中的序号
		for(j=0;j<iNumNode+i;j++)
		{
			//对标记Flag为0的结点比较排序，x1与m1对应最小的子树，x2与m2次之
			if(huffNode[j].Weight<m1 && huffNode[j].Flag ==0 )
			{
				m2=m1;
				x2=x1;
				m1=huffNode[j].Weight;
				x1=j;
			}
			else if(huffNode[j].Weight<m2 && huffNode[j].Flag==0)
			{
				m2=huffNode[j].Weight;
				x2=j;
			}
		}
	
		//将找出的两棵最小子树合并为一棵子树
		huffNode[x1].Parent = iNumNode+i;
		huffNode[x2].Parent = iNumNode+i;
		//Flag为1，说明该结点已经成为其他结点的子结点
		huffNode[x1].Flag = 1;
		huffNode[x2].Flag = 1;
		huffNode[iNumNode+i].Weight = huffNode[x1].Weight+huffNode[x2].Weight;
		//概率值或权值较小的子树作为右子树
		huffNode[iNumNode+i].LChild = x2;
		huffNode[iNumNode+i].RChild = x1;
	}

	//求每个叶结点的哈夫曼编码
	for(i=0;i<iNumNode;i++)
	{
		c=i;
		p=huffNode[c].Parent;
		//由叶结点上溯到树根,即直到当前点的父结点是根结点
		while(p != 0)
		{
			//如果是左子树，则代码的高一位添加0，否则添加1
			if(huffNode[p].LChild == c) strCode[i]="0"+strCode[i];
			else	strCode[i]="1"+strCode[i];
			c=p;
			p=huffNode[c].Parent;
		}
	}

	//计算平均码字长度和图像熵
	int CodeLen=0;
	m_fEntropy = 0.0;
	for (i = 0; i < iNumNode; i ++)
	{
		 CodeLen+= huffNode[i].Weight * strCode[i].GetLength();
		// 计算图像熵
		m_fEntropy -= ((double)huffNode[i].Weight/m_iSizeImage) * log((double)huffNode[i].Weight/m_iSizeImage) / log(2.0);
	}
	//计算平均码字长度
	m_fAvCodeLen = (double)CodeLen/m_iSizeImage;
	// 计算编码效率
	m_fCodeEfficiency = m_fEntropy / m_fAvCodeLen;
	//更新对话框上的文本框中的内容
	UpdateData(FALSE);
	//输出哈夫曼编码结果
	lvitem.mask = LVIF_TEXT;	// 设置样式为文本
	for (i = 0; i < iNumNode; i ++)
	{
		if(huffNode[i].Weight > 0)
		{
			// 添加一项
			lvitem.iItem = m_lstTable.GetItemCount();
			str.Format("%u",iMap[i]);
			lvitem.iSubItem = 0;
			lvitem.pszText= (LPTSTR)(LPCTSTR)str;
			iActualItem = m_lstTable.InsertItem(&lvitem);
			
			// 添加其它列
			lvitem.iItem = iActualItem;
			
			// 添加灰度值出现的频率
			lvitem.iSubItem = 1;
			str.Format("%f",(float)huffNode[i].Weight/m_iSizeImage);
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstTable.SetItem(&lvitem);
			
			// 添加哈夫曼编码
			lvitem.iSubItem = 2;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strCode[i];
			m_lstTable.SetItem(&lvitem);
			
			// 添加码字长度
			lvitem.iSubItem = 3;
			str.Format("%u",strCode[i].GetLength());
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstTable.SetItem(&lvitem);
		}
	}
	delete[] huffNode;
	delete[] strCode;
	delete[] iMap;
}

BOOL CDlgHuffman::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 设置List控件样式
	m_lstTable.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	// 给List控件添加Header
	m_lstTable.InsertColumn(0, "灰度值", LVCFMT_LEFT, 60, 0);
	m_lstTable.InsertColumn(1, "出现频率", LVCFMT_LEFT, 78, 0);
	m_lstTable.InsertColumn(2, "哈夫曼编码", LVCFMT_LEFT, 110, 1);
	m_lstTable.InsertColumn(3, "码字长度", LVCFMT_LEFT, 78, 2);

	Huffman();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHuffman::OnRadioGreen() 
{
	m_lstTable.DeleteAllItems();
	UpdateData();

	Huffman();	
}

void CDlgHuffman::OnRadioBlue() 
{
	m_lstTable.DeleteAllItems();
	UpdateData();

	Huffman();	
}

void CDlgHuffman::OnRadioRed() 
{
	m_lstTable.DeleteAllItems();
	UpdateData();

	Huffman();	
	
}

void CDlgHuffman::OnRadioGray() 
{
	m_lstTable.DeleteAllItems();
	UpdateData();

	Huffman();	
}

