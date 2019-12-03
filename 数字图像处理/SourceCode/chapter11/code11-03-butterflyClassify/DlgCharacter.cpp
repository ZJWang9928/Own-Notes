// DlgCharacter.cpp : implementation file
//

#include "stdafx.h"
#include "Insects.h"
#include "DlgCharacter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCharacter dialog


CDlgCharacter::CDlgCharacter(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCharacter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCharacter)
	m_nAntenna = 0.0f;
	m_nBelly = 0.0f;
	m_nSwing = 0;
	m_nEccentricity = 0.0f;
	m_nArea = 0.0f;
	m_nPerimeter = 0.0f;
	m_nCircularity = 0.0f;
	m_nFormFactor = 0.0f;
	m_Center_x = 0;
	m_Center_y = 0;
	m_Center_Square = 0.0f;
	m_Square = 0.0f;
	m_Heights = 0;
	m_Widths = 0;
	m_Indense = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgCharacter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCharacter)
	DDX_Text(pDX, IDC_ANTENNA, m_nAntenna);
	DDX_Text(pDX, IDC_BELLY, m_nBelly);
	DDX_Text(pDX, IDC_SWING, m_nSwing);
	DDX_Text(pDX, IDC_EDIT_ECCENTRICITY, m_nEccentricity);
	DDX_Text(pDX, IDC_EDIT_AREA, m_nArea);
	DDX_Text(pDX, IDC_EDIT_PERIMETER, m_nPerimeter);
	DDX_Text(pDX, IDC_EDIT_CIRCULARITY, m_nCircularity);
	DDX_Text(pDX, IDC_EDIT_FORMFACTOR, m_nFormFactor);
	DDX_Text(pDX, IDC_EDIT_CENTER_X, m_Center_x);
	DDX_Text(pDX, IDC_EDIT_CENTER_Y, m_Center_y);
	DDX_Text(pDX, IDC_EDIT_CENTER, m_Center_Square);
	DDX_Text(pDX, IDC_EDIT_SQUARE, m_Square);
	DDX_Text(pDX, IDC_EDIT_HEIGHTS, m_Heights);
	DDX_Text(pDX, IDC_EDIT_WIDTHS, m_Widths);
	DDX_Text(pDX, IDC_EDIT_CINDESE, m_Indense);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCharacter, CDialog)
	//{{AFX_MSG_MAP(CDlgCharacter)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCharacter message handlers

