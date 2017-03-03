// MidiMagi.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CMidiMagiApp:
// See MidiMagi.cpp for the implementation of this class
//

class CMidiMagiApp : public CWinApp
{
public:
	CMidiMagiApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMidiMagiApp theApp;