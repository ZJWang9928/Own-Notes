// Insects.h : main header file for the INSECTS application
//

#if !defined(AFX_IMGPRO_H__F726E4F2_3D8A_49AC_8E33_144E6719539D__INCLUDED_)
#define AFX_IMGPRO_H__F726E4F2_3D8A_49AC_8E33_144E6719539D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CInsectsApp:
// See Insects.cpp for the implementation of this class
//

class CInsectsApp : public CWinApp
{
public:
	CInsectsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsectsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CInsectsApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGPRO_H__F726E4F2_3D8A_49AC_8E33_144E6719539D__INCLUDED_)
