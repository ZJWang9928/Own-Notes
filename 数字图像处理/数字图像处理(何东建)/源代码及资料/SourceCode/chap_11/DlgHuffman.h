#if !defined(AFX_DLGHUFFMAN_H__A0595170_1A55_4467_8AF1_33C61E8782CF__INCLUDED_)
#define AFX_DLGHUFFMAN_H__A0595170_1A55_4467_8AF1_33C61E8782CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHuffman.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHuffman dialog

//保存生成哈夫曼树的结点，可以动态生成
typedef struct 
{
	int Weight;//保存哈夫曼树结点的权值，在此是指频数
	int Flag;//当Flag=0时，表示该结点未加入树中，为1时表示已加入树中
	int Parent;//存放结点的父结点在结点数组中的序号
	int LChild;//存放结点的左子结点在结点数组中的序号
	int RChild;//存放结点的右子结点在结点数组中的序号
}HuffmanNode;

class CDlgHuffman : public CDialog
{
// Construction
public:
	int m_iSizeImage;
	int * m_fFreq;//各叶结点或各灰度的概率
	int m_iLeafNum;//待编码的叶结点数目

	void Huffman();
	CDlgHuffman(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHuffman)
	enum { IDD = IDD_DLG_Huffman };
	CListCtrl	m_lstTable;
	int		m_ChannelSel;
	double	m_fCodeEfficiency;
	double	m_fEntropy;
	double	m_fAvCodeLen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHuffman)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHuffman)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioGreen();
	afx_msg void OnRadioBlue();
	afx_msg void OnRadioRed();
	afx_msg void OnRadioGray();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHUFFMAN_H__A0595170_1A55_4467_8AF1_33C61E8782CF__INCLUDED_)
