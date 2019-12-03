
class CCombineDlg : public CDialog
{
public:
	CString m_strTitle;
	CCombineDlg(CWnd* pParent = NULL);   // standard constructor
	
	CDibObject* GetRedImage() const { return m_pRedImage; };
	CDibObject* GetGreenImage() const { return m_pGreenImage; };
	CDibObject* GetBlueImage() const { return m_pBlueImage; };
	
	
	//{{AFX_DATA(CCombineDlg)
	enum { IDD = IDD_CHANNEL_COMBINE };
	CComboBox	m_ctrlRedChannel;
	CComboBox	m_ctrlGreenChannel;
	CComboBox	m_ctrlBlueChannel;
	//}}AFX_DATA
	
	
	//{{AFX_VIRTUAL(CCombineDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
protected:
	
	//{{AFX_MSG(CCombineDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
		CDibObject* m_pRedImage;
	CDibObject* m_pGreenImage;
	CDibObject* m_pBlueImage;
};

