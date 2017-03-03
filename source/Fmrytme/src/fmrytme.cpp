// fmrytme.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "fmrytme.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "fmrytmeDoc.h"
#include "fmrytmeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeApp

BEGIN_MESSAGE_MAP(CFmrytmeApp, CWinApp)
	//{{AFX_MSG_MAP(CFmrytmeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeApp construction

CFmrytmeApp::CFmrytmeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFmrytmeApp object

CFmrytmeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeApp initialization

BOOL CFmrytmeApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifndef _AFXDLL
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Sigurd Saue"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_FMRYTMTYPE,
		RUNTIME_CLASS(CFmrytmeDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CFmrytmeView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;

	// Laster inn diverse settings
	CMainFrame::m_settings.m_nMidiDevice = GetProfileInt(CMainFrame::s_settingsHeading, CMainFrame::s_profilMidi, 1);
	CMainFrame::m_settings.m_nPeriode = GetProfileInt(CMainFrame::s_settingsHeading, CMainFrame::s_profilFrekvens, 1000);
	CString headStr;
	CString dataStr;
	for (int i=0; i<CSettings::KANALER; i++) {
		headStr.Format(L"%.5s%d", CMainFrame::s_colorsHeading, i);
		dataStr = GetProfileString(CMainFrame::s_settingsHeading, headStr);
		if (dataStr.IsEmpty()) {
			CMainFrame::m_settings.m_colFarge[i] = RGB(255,0,0);
		}
		else {
			int rValue = atoi(CT2A(dataStr));
			int gValue = atoi(CT2A(dataStr) + 5);
			int bValue = atoi(CT2A(dataStr) + 10);
			CMainFrame::m_settings.m_colFarge[i] = RGB(rValue, gValue, bValue);
		}
	}
	dataStr = GetProfileString(CMainFrame::s_settingsHeading, CMainFrame::s_profilAmpLengde);
	FMPlotInfo::m_dAmpLengde = (dataStr.IsEmpty()) ? 0.3 : atof(CT2A(dataStr));
	dataStr = GetProfileString(CMainFrame::s_settingsHeading, CMainFrame::s_profilAmpVelocity);
	FMPlotInfo::m_dAmpVelocity = (dataStr.IsEmpty()) ? 0.3 : atof(CT2A(dataStr));
	FMPlotInfo::m_nSynk = GetProfileInt(CMainFrame::s_settingsHeading, CMainFrame::s_profilSynk, FMPlotInfo::ENER);

	// Tilbake til standard prosedyre
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->LoadBarState(m_pszProfileName);
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CFmrytmeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeApp commands
