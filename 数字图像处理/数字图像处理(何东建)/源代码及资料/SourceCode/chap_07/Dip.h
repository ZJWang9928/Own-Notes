// Dip.h : main header file for the DIP application
//

#if !defined(AFX_DIP_H__44D26BAE_D84D_40C8_8DFC_69BA0BC89566__INCLUDED_)
#define AFX_DIP_H__44D26BAE_D84D_40C8_8DFC_69BA0BC89566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "mainfrm.h"


/////////////////////////////////////////////////////////////////////////////
// CDipApp:
// See Dip.cpp for the implementation of this class
//

class CDipApp : public CWinApp
{
public:
	int NumberOfOpenDocuments() const;
	CDipApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDipApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDipApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnChannelCombine();
	afx_msg void OnUpdateChannelCombine(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

///////////////// Helper Functions //////////////////
CDipApp* GetApp();
CMainFrame* GetMainFrame();

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIP_H__44D26BAE_D84D_40C8_8DFC_69BA0BC89566__INCLUDED_)
