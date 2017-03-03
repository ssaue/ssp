// fmrytme.h : main header file for the FMRYTME application
//

#if !defined(AFX_FMRYTME_H__67E58404_5474_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_FMRYTME_H__67E58404_5474_11D2_97D2_00105A3B4608__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeApp:
// See fmrytme.cpp for the implementation of this class
//

class CFmrytmeApp : public CWinApp
{
public:
	CFmrytmeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmrytmeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFmrytmeApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMRYTME_H__67E58404_5474_11D2_97D2_00105A3B4608__INCLUDED_)
