// PressureMagi.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PressureMagi.h"
#include "PressureMagiDlg.h"
#include "sspSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPressureMagiApp

BEGIN_MESSAGE_MAP(CPressureMagiApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPressureMagiApp construction

CPressureMagiApp::CPressureMagiApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPressureMagiApp object

CPressureMagiApp theApp;


// CPressureMagiApp initialization

BOOL CPressureMagiApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
  SetRegistryKey(_T("Acoulogos"));
  LoadStdProfileSettings(0);
  sspSettings::ReadAllRegistry();

	CPressureMagiDlg dlg;
	m_pMainWnd = &dlg;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_nShellCommand != CCommandLineInfo::FileNew)
		dlg.m_bAutoStart = true;

  INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
    sspSettings::WriteAllRegistry();
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
