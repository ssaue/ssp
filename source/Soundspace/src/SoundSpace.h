// SoundSpace.h : main header file for the SOUNDSPACE application
//

#if !defined(AFX_SOUNDSPACE_H__ED028788_8AC6_11D3_901C_F7D8A2DF1632__INCLUDED_)
#define AFX_SOUNDSPACE_H__ED028788_8AC6_11D3_901C_F7D8A2DF1632__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSoundSpaceApp:
// See SoundSpace.cpp for the implementation of this class
//

class CSoundSpaceApp : public CWinApp
{
public:
	BOOL m_bCommandLine;	// Checking if there is a command line argument

	CSoundSpaceApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundSpaceApp)
	public:
	virtual BOOL InitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSoundSpaceApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDSPACE_H__ED028788_8AC6_11D3_901C_F7D8A2DF1632__INCLUDED_)
