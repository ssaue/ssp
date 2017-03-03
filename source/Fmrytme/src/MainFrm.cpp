// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "fmrytme.h"
#include "dialog.h"
#include "fmrytmeDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_GENERELLMENY, OnGenerellmeny)
	ON_COMMAND(ID_LOOP, OnLoop)
	ON_COMMAND(ID_PLAY, OnPlay)
	ON_COMMAND(ID_STOP, OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateStop)
	ON_UPDATE_COMMAND_UI(ID_PLAY, OnUpdatePlay)
	ON_UPDATE_COMMAND_UI(ID_LOOP, OnUpdateLoop)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PLAY_DONE, OnPlayDone)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

const WCHAR CMainFrame::s_settingsHeading[] = L"Innstillinger";
const WCHAR CMainFrame::s_colorsHeading[] = L"Farge";
const WCHAR CMainFrame::s_profilMidi[] = L"MIDI";
const WCHAR CMainFrame::s_profilFrekvens[] = L"Samp.frekvens";
const WCHAR CMainFrame::s_profilAmpLengde[] = L"Amp.faktor lengde";
const WCHAR CMainFrame::s_profilAmpVelocity[] = L"Amp.faktor velocity";
const WCHAR CMainFrame::s_profilSynk[] = L"Synk.kilde";
CSettings CMainFrame::m_settings;

CMainFrame::CMainFrame()
{
	m_bDocUpdated = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndStatusBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, ID_NY_STATUSBAR) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneInfo(0, 0, 0, 0);				// Dette panelet er bare en marg
	m_wndStatusBar.SetPaneInfo(1, 0, 0, 50);			// Panel for x-koordinat
	m_wndStatusBar.SetPaneInfo(2, 0, 0, 50);			// Panel for y-koordinat
	m_wndStatusBar.SetPaneInfo(3, 0, 0, 150);			// Panel for venstre markør
	m_wndStatusBar.SetPaneInfo(4, 0, 0, 150);			// Panel for høyre markør
	m_wndStatusBar.SetPaneInfo(5, 0, SBPS_STRETCH, 50);	// Panel for differanse mellom markørene
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	if (!m_wndDialogBar.Create(this, IDD_DIALOGBAR, CBRS_TOP, ID_DIALOGBAR))
	{
		TRACE("Failed to create dialog bar\n");
		return -1;      // fail to create
	}
	m_wndDialogBar.EnableDocking(CBRS_ALIGN_TOP);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndDialogBar);
	InitDialogBar();
	m_midiPlay.Initialize(m_settings.m_nMidiDevice, GetSafeHwnd());
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnGenerellmeny() 
{
	CGenerellDialog dlg;
	dlg.m_nDeviceNr = m_settings.m_nMidiDevice;
	dlg.m_nPeriode = m_settings.m_nPeriode;
	if (dlg.DoModal() == IDOK) {
		m_settings.m_nMidiDevice = dlg.m_nDeviceNr;
		m_midiPlay.ChangeDevice(dlg.m_nDeviceNr);
		if (m_settings.m_nPeriode != dlg.m_nPeriode) {
			m_settings.m_nPeriode = dlg.m_nPeriode;
			CMDIChildWnd* pChild = (CMDIChildWnd *) GetActiveFrame();
			CFmrytmeDoc* pDoc = (CFmrytmeDoc*) pChild->GetActiveDocument();
			pDoc->UpdatePeriode();
		}
	}
}

void CMainFrame::InitDialogBar()
{
	CWnd* kontroll = m_wndDialogBar.GetDlgItem(IDC_STEMME);
	kontroll->EnableWindow(FALSE);
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) m_wndDialogBar.GetDlgItem(IDC_SPINTEMPO);
	pSpin->SetRange(MIN_TEMPO, MAX_TEMPO);
	pSpin->SetPos(DEFAULT_TEMPO);
}

void CMainFrame::UpdateSpor(int sporAnt, int sporNr)
{
	CWnd* kontroll = m_wndDialogBar.GetDlgItem(IDC_STEMME);
	kontroll->EnableWindow(TRUE);
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) m_wndDialogBar.GetDlgItem(IDC_SPINSTEMME);
	pSpin->SetRange(1,sporAnt);
	pSpin->SetPos(sporNr+1);	// Kompenserer for forskjellig indeksering
}

void CMainFrame::UpdateTempo(int tempo)
{
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) m_wndDialogBar.GetDlgItem(IDC_SPINTEMPO);
	pSpin->SetPos(tempo);
}

int CMainFrame::GetSpor()
{
	CWnd* editCntr = m_wndDialogBar.GetDlgItem(IDC_STEMME);
	CString tekst;
	editCntr->GetWindowText(tekst);
	return (atoi(CT2A(tekst)) - 1);	// Sporene indekseres fra 0
}

int CMainFrame::GetTempo()
{
	CWnd* editCntr = m_wndDialogBar.GetDlgItem(IDC_TEMPO);
	CString tekst;
	editCntr->GetWindowText(tekst);
	return atoi(CT2A(tekst));
}

void CMainFrame::OnLoop() 
{
	m_midiPlay.SetLoop(!m_midiPlay.IsLooping());
}

void CMainFrame::OnPlay() 
{
	CWnd* kontroll = m_wndDialogBar.GetDlgItem(IDC_STEMME);
	kontroll->EnableWindow(FALSE);
	m_midiPlay.Spill();
}

void CMainFrame::OnStop() 
{
	m_midiPlay.Stopp();	
}

void CMainFrame::OnUpdateStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_midiPlay.IsPlaying());
}

void CMainFrame::OnUpdatePlay(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_midiPlay.IsPlaying());
}

void CMainFrame::OnUpdateLoop(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_midiPlay.IsLooping());
}

LRESULT CMainFrame::OnPlayDone(WPARAM wParam, LPARAM lParam)
{
	m_midiPlay.SpillSlutt();
	CWnd* kontroll = m_wndDialogBar.GetDlgItem(IDC_STEMME);
	kontroll->EnableWindow(TRUE);
	if (m_bDocUpdated) {
		CMDIChildWnd* pChild = (CMDIChildWnd *) GetActiveFrame();
		CFmrytmeDoc* pDoc = (CFmrytmeDoc*) pChild->GetActiveDocument();
		pDoc->UpdateFrame();
		m_bDocUpdated = FALSE;
	}
	return 0;
}

void CMainFrame::SetDocUpdated()
{
	m_bDocUpdated = TRUE;
}

void CMainFrame::OnViewStatusBar() 
{
	m_wndStatusBar.ShowWindow((m_wndStatusBar.GetStyle() & WS_VISIBLE) == 0);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewStatusBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_wndStatusBar.GetStyle() & WS_VISIBLE) != 0);
}

void CMainFrame::OnDestroy() 
{
	AfxGetApp()->WriteProfileInt(s_settingsHeading, s_profilMidi, m_settings.m_nMidiDevice);
	AfxGetApp()->WriteProfileInt(s_settingsHeading, s_profilFrekvens, m_settings.m_nPeriode);
	CString headStr;
	CString dataStr;
	for (int i=0; i<CSettings::KANALER; i++) {
		headStr.Format(L"%.5s%d", s_colorsHeading, i);
		dataStr.Format(L"%04d %04d %04d", 
			GetRValue(m_settings.m_colFarge[i]), 
			GetGValue(m_settings.m_colFarge[i]),
			GetBValue(m_settings.m_colFarge[i]));
		AfxGetApp()->WriteProfileString(s_settingsHeading, headStr, dataStr);
	}
	dataStr.Format(L"%.2f", FMPlotInfo::m_dAmpLengde);
	AfxGetApp()->WriteProfileString(s_settingsHeading, s_profilAmpLengde, dataStr);
	dataStr.Format(L"%.2f", FMPlotInfo::m_dAmpVelocity);
	AfxGetApp()->WriteProfileString(s_settingsHeading, s_profilAmpVelocity, dataStr);
	AfxGetApp()->WriteProfileInt(s_settingsHeading, s_profilSynk, FMPlotInfo::m_nSynk);
	SaveBarState(AfxGetApp()->m_pszProfileName);
	CMDIFrameWnd::OnDestroy();
}
