// PressureMagiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PressureMagi.h"
#include "PressureMagiDlg.h"
#include "sspAction.h"
#include "sspSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_PLAY_AUTOMATIC	WM_USER + 5

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPressureMagiDlg dialog




CPressureMagiDlg::CPressureMagiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPressureMagiDlg::IDD, pParent)
  , m_bAutoStart(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPressureMagiDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_MIDIINPUT_COMBO, m_cbInputDevice);
  DDX_Control(pDX, IDC_MIDIOUTPUT_COMBO, m_cbOutputDevice);
  DDX_CBIndex(pDX, IDC_MIDIINPUT_COMBO, sspSettings::midi.inputDevice);
  DDX_CBIndex(pDX, IDC_MIDIOUTPUT_COMBO, sspSettings::midi.outputDevice);
  DDX_Text(pDX, IDC_EDIT_NOTEON_PUMP, sspSettings::midi.noteonPump);
  DDV_MinMaxInt(pDX, sspSettings::midi.noteonPump, 0, 127);
  DDX_Text(pDX, IDC_EDIT_NOTEON_VALVE, sspSettings::midi.noteonValve);
  DDV_MinMaxInt(pDX, sspSettings::midi.noteonValve, 0, 127);
  DDX_Text(pDX, IDC_EDIT_INPUT_PORT, sspSettings::input.port);
  DDV_MinMaxInt(pDX, sspSettings::input.port, 1, 3);
  DDX_Text(pDX, IDC_EDIT_INPUT_ADDRESS, sspSettings::input.address);
  DDV_MinMaxInt(pDX, sspSettings::input.address, 0, 255);
  DDX_Text(pDX, IDC_EDIT_INPUT_CHANNEL, sspSettings::input.channel);
  DDV_MinMaxInt(pDX, sspSettings::input.channel, 0, 7);
  DDX_Text(pDX, IDC_EDIT_INPUT_MIN, sspSettings::input.inputMin);
  DDV_MinMaxInt(pDX, sspSettings::input.inputMin, 0, 127);
  DDX_Text(pDX, IDC_EDIT_INPUT_MAX, sspSettings::input.inputMax);
  DDV_MinMaxInt(pDX, sspSettings::input.inputMax, 0, 127);
  DDX_Text(pDX, IDC_EDIT_SMS_LEVEL, sspSettings::input.smsThreshold);
  DDX_Text(pDX, IDC_EDIT_PROCESS_MIN, sspSettings::process.min);
  DDV_MinMaxInt(pDX, sspSettings::process.min, 0, 127);
  DDX_Text(pDX, IDC_EDIT_PROCESS_MAX, sspSettings::process.max);
  DDV_MinMaxInt(pDX, sspSettings::process.max, 0, 127);
  DDX_Text(pDX, IDC_EDIT_PROCESS_TOLERANCE, sspSettings::process.margin);
  DDV_MinMaxInt(pDX, sspSettings::process.margin, 0, 127);
  DDX_Text(pDX, IDC_EDIT_PROCESS_STANDBY, sspSettings::process.standby);
  DDV_MinMaxInt(pDX, sspSettings::process.standby, 0, 127);
  DDX_Text(pDX, IDC_EDIT_COMPENSATION_FACTOR, sspSettings::process.factor);
  DDX_Text(pDX, IDC_EDIT_UPDATE_INTERVAL, sspSettings::process.timerUpdate);
  DDX_Control(pDX, IDC_INPUT_VELOCITY, m_editInputVelocity);
  DDX_Control(pDX, IDC_READ_VELOCITY, m_editReadVelocity);
  DDX_Control(pDX, IDC_OUTPUT_VELOCITY, m_editOutputVelocity);
  DDX_Control(pDX, IDC_MEASURED_CURRENT, m_editMeasuredCurrent);
  DDX_Control(pDX, IDC_OUTPUT_VALVE, m_btnOutputValve);
}

BEGIN_MESSAGE_MAP(CPressureMagiDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
  ON_BN_CLICKED(IDC_BUTTON_STOP, OnStop)
	ON_MESSAGE(WM_PLAY_AUTOMATIC, OnPlayAutomatic)
END_MESSAGE_MAP()


// CPressureMagiDlg message handlers

BOOL CPressureMagiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CString txt;
	txt.Format("0x%x", sspSettings::input.outPort);	// Hex notation
	GetDlgItem(IDC_EDIT_OUTPUT_PORT)->SetWindowText(txt);

	m_cbInputDevice.ResetContent();
  for (unsigned int i=0; i<sspMidiInDevice::getNumDevices(); ++i) {
		m_cbInputDevice.InsertString(i, sspMidiInDevice::getDeviceName(i).c_str());
  }
  m_cbInputDevice.SetCurSel(sspSettings::midi.inputDevice);

	m_cbOutputDevice.ResetContent();
  for (unsigned int i=0; i<sspMidiOutDevice::getNumDevices(); ++i) {
		m_cbOutputDevice.InsertString(i, sspMidiOutDevice::getDeviceName(i).c_str());
  }
  m_cbOutputDevice.SetCurSel(sspSettings::midi.outputDevice);

  sspAction::tErrorCode nError = sspAction::Instance().initialize(this);
  if (nError != sspAction::OK) {
    CString txt = "ERROR: Failed to initialize ";
    switch (nError) {
      case sspAction::ERROR_INPUT:
        txt += "input";
        break;
      case sspAction::ERROR_MIDI:
        txt += "MIDI";
        break;
      case sspAction::ERROR_TIMER:
        txt += "timer";
        break;
      case sspAction::ERROR_OUTPUT:
        txt += "output";
        break;
      default:
        break;
    }
    AfxMessageBox(txt, MB_OK);
  }
  enableEdits(true);

	if (m_bAutoStart) {
  	this->PostMessage(WM_PLAY_AUTOMATIC, 0, 0);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPressureMagiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPressureMagiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPressureMagiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPressureMagiDlg::OnStart()
{
  UpdateData(TRUE);

	// Reading LPT port as hex number
	char buffer[10];
	GetDlgItem(IDC_EDIT_SMS_LEVEL)->GetWindowText(buffer, 10);
	char* stop;
  sspSettings::input.outPort = (unsigned short) strtoul(buffer, &stop, 0);

  sspAction::tErrorCode nError = sspAction::Instance().start();
  if (nError != sspAction::OK) {
    CString txt = "ERROR: Failed to start ";
    switch (nError) {
      case sspAction::ERROR_INPUT:
        txt += "input";
        break;
      case sspAction::ERROR_MIDI:
        txt += "MIDI";
        break;
      case sspAction::ERROR_TIMER:
        txt += "timer";
        break;
      case sspAction::ERROR_OUTPUT:
        txt += "output";
        break;
      default:
        break;
    }
    AfxMessageBox(txt, MB_OK);
  }
  else {
    enableEdits(false);
  }
}

void CPressureMagiDlg::OnStop()
{
  if (sspAction::Instance().stop() == sspAction::OK) {
    enableEdits(true);
  }
}

void CPressureMagiDlg::OnOK()
{
  sspAction::Instance().terminate();
  CDialog::OnOK();
}

void CPressureMagiDlg::OnCancel()
{
  sspAction::Instance().terminate();
  CDialog::OnCancel();
}

void CPressureMagiDlg::enableEdits(bool bEnable)
{
  CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
  if (!pWnd) return;
  pWnd->EnableWindow(bEnable);

  GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(!bEnable);
  GetDlgItem(IDC_MIDIINPUT_COMBO)->EnableWindow(bEnable);
  GetDlgItem(IDC_MIDIOUTPUT_COMBO)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_NOTEON_PUMP)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_NOTEON_VALVE)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_INPUT_PORT)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_INPUT_ADDRESS)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_INPUT_CHANNEL)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_INPUT_MIN)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_INPUT_MAX)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_OUTPUT_PORT)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_SMS_LEVEL)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_PROCESS_MIN)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_PROCESS_MAX)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_PROCESS_TOLERANCE)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_PROCESS_STANDBY)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_COMPENSATION_FACTOR)->EnableWindow(bEnable);
  GetDlgItem(IDC_EDIT_UPDATE_INTERVAL)->EnableWindow(bEnable);
}

void CPressureMagiDlg::updateMonitor()
{
  CString txt;
	txt.Format("%d", sspAction::Instance().getInputMidi());
  m_editInputVelocity.SetWindowText(txt);
	txt.Format("%d", sspAction::Instance().getMeasuredMidi());
  m_editReadVelocity.SetWindowText(txt);
	txt.Format("%d", sspAction::Instance().getOutputMidi());
  m_editOutputVelocity.SetWindowText(txt);
	txt.Format("%.2f", sspAction::Instance().getMeasuredMilliAmpere());
  m_editMeasuredCurrent.SetWindowText(txt);
  m_btnOutputValve.SetCheck(sspAction::Instance().isValveClosed() ? BST_CHECKED : BST_UNCHECKED);
}

LRESULT CPressureMagiDlg::OnPlayAutomatic(WPARAM wParam, LPARAM lParam)
{
  OnStart();
	return 0L;
}
