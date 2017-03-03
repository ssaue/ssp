// MidiMagiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MidiMagi.h"
#include "MidiMagiDlg.h"
#include "sspGUIMapDialog.h"
#include "sspMagiSettings.h"

#include <sspMidiFileReader.h>
#include <sspMidiFileWriter.h>
#include <map>

#undef min
#undef max

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CMidiMagiDlg dialog

CMidiMagiDlg::CMidiMagiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMidiMagiDlg::IDD, pParent)
  , m_strFilename(_T(""))
  , m_nTracks(0)
  , m_dDuration(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMidiMagiDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_FILENAME, m_strFilename);
  DDX_Text(pDX, IDC_TRACKS, m_nTracks);
  DDV_MinMaxInt(pDX, m_nTracks, 0, 1000);
  DDX_Text(pDX, IDC_CHANNEL, sspMagiSettings::midiTrack.channel);
  DDV_MinMaxByte(pDX, sspMagiSettings::midiTrack.channel, 0, 15);
  DDX_Text(pDX, IDC_NOTE, sspMagiSettings::midiTrack.noteon);
  DDV_MinMaxByte(pDX, sspMagiSettings::midiTrack.noteon, 0, 127);
  DDX_Text(pDX, IDC_STARTVELOCITY, sspMagiSettings::velocity.start);
  DDV_MinMaxInt(pDX, sspMagiSettings::velocity.start, 1, 127);
  DDX_Text(pDX, IDC_MINVELOCITY, sspMagiSettings::velocity.minimum);
  DDV_MinMaxInt(pDX, sspMagiSettings::velocity.start, 1, 127);
  DDX_Text(pDX, IDC_MAXVELOCITY, sspMagiSettings::velocity.maximum);
  DDV_MinMaxInt(pDX, sspMagiSettings::velocity.start, 1, 127);
  DDX_Text(pDX, IDC_DURATION, m_dDuration);
  DDX_Text(pDX, IDC_TIMEWINDOW, sspMagiSettings::timing.timeWindow);
  DDX_Text(pDX, IDC_TIMEOFFSET, sspMagiSettings::timing.timeOffset);
  DDX_Control(pDX, IDC_NOTEON_MAP, m_lcNoteMap);
  DDX_Text(pDX, IDC_PUMPSTART, sspMagiSettings::timing.pumpStart);
  DDX_Text(pDX, IDC_PUMPSTOP, sspMagiSettings::timing.pumpStop);
}

BEGIN_MESSAGE_MAP(CMidiMagiDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_OPENFILE, OnOpenfile)
  ON_BN_CLICKED(IDC_PROCESS, OnProcess)
  ON_NOTIFY(NM_DBLCLK, IDC_NOTEON_MAP, OnNMDblclkNoteonMap)
END_MESSAGE_MAP()


// CMidiMagiDlg message handlers

BOOL CMidiMagiDlg::OnInitDialog()
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

	CRect ctrlRect;
	m_lcNoteMap.GetClientRect(ctrlRect);
	int nRectWidth = ctrlRect.Width() / 2;
	m_lcNoteMap.InsertColumn(0, "Pitch", LVCFMT_LEFT, nRectWidth, 0);
	m_lcNoteMap.InsertColumn(1, "DMX", LVCFMT_LEFT, nRectWidth, 1);

  char strNumeric[10];
  for (UINT i=0; i<sspMagiSettings::NOTEONS; i++) {    
    _itoa_s(sspMagiSettings::sounding[i], strNumeric, 10, 10);
		m_lcNoteMap.InsertItem(LVIF_TEXT|LVIF_STATE, i, strNumeric, 0, LVIS_SELECTED, 0, 0);
    _itoa_s(sspMagiSettings::dmx[i], strNumeric, 10, 10);
		m_lcNoteMap.SetItemText(i, 1, strNumeric);
	}
	m_lcNoteMap.SetExtendedStyle(m_lcNoteMap.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
  GetDlgItem(IDC_PROCESS)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMidiMagiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMidiMagiDlg::OnPaint() 
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
HCURSOR CMidiMagiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool SaveFile(HWND hOwner, const char* strTitle, const char* strFilter, const char* strExtension, CString& strFile)
{
	OPENFILENAME ofn;		// common dialog box structure
	char strBuf[260];
  int nLength = strFile.GetLength();
  strncpy_s(strBuf, 260, strFile, nLength);
	strBuf[nLength] = 0;

	// Initialize OPENFILENAME
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hOwner;
	ofn.lpstrFilter = strFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = strTitle;
	ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
	ofn.lpstrFile = strBuf;
  ofn.lpstrDefExt = strExtension;
	ofn.nMaxFile = sizeof(strBuf);

	// Display the Open dialog box. 

	if (::GetSaveFileName(&ofn) == TRUE) {
		strFile = ofn.lpstrFile;
		return true;
	}
	return false;
}

bool SelectFile(HWND hOwner, const char* strTitle, const char* strFilter, CString& strFile)
{
	OPENFILENAME ofn;		// common dialog box structure
	char strBuf[260];
	strBuf[0] = 0;

	// Initialize OPENFILENAME
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hOwner;
	ofn.lpstrFilter = strFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = strTitle;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrFile = strBuf;
	ofn.nMaxFile = sizeof(strBuf);

	// Display the Open dialog box. 

	if (::GetOpenFileName(&ofn) == TRUE) {
		strFile = ofn.lpstrFile;
		return true;
	}
	return false;
}

void CMidiMagiDlg::OnOpenfile()
{
  UpdateData(TRUE);
  m_song.clear();

	if (SelectFile(
			GetSafeHwnd(), 
			"Specify a MIDI file", 
			"Midi Files (*.mid)\0*.mid\0", 
			m_strFilename)) {
		sspMidiFileReader file;

    if (file.open(m_strFilename) == sspMidiFileReader::ERRMSG_OK) {
      int nFilenameOnly = m_strFilename.ReverseFind('\\') + 1;
      m_strFilename = m_strFilename.Right(m_strFilename.GetLength() - nFilenameOnly);

      if (file.read(m_song) == sspMidiFileReader::ERRMSG_OK) {
        m_nTracks = (int) m_song.size();
        m_dDuration = m_song.getDuration() / 1000.0;
        UpdateData(FALSE);
        GetDlgItem(IDC_PROCESS)->EnableWindow(TRUE);
			}
      else {
        AfxMessageBox("Invalid or empty MIDI file!");
      }
			file.close();
		}
    else {
      AfxMessageBox("Unable to open MIDI file for reading!");
    }
      }
  else {
    AfxMessageBox("Unable to open MIDI file for reading!");
  }
}

void CMidiMagiDlg::OnProcess()
{
  UpdateData(TRUE);

  // Create NoteOn map
  std::map<int, int> noteonMap;
  for (UINT i=0; i<sspMagiSettings::NOTEONS; i++) {
    if (noteonMap.find(sspMagiSettings::sounding[i]) != noteonMap.end()) {
      AfxMessageBox("There are two identical pitches in the note on map. Please check!");
      return;
    }
    noteonMap[sspMagiSettings::sounding[i] ] = sspMagiSettings::dmx[i];
  }

  m_song.mergeTracks();
  sspMidiFileTrack& track = m_song.front();
  sspMidiFileTrack  newTrack;
  sspMidiEvent      newEvent;
  newEvent.timestamp = 0;
  newEvent.message = sspMidi::createMessage(sspMidi::NOTEON, 
                                            sspMagiSettings::midiTrack.noteon, 
                                            sspMagiSettings::velocity.start);
  sspMidi::setChannel(newEvent.message, sspMagiSettings::midiTrack.channel);
  newTrack.push_back(newEvent);
  newEvent.timestamp = (sspMidiTimestamp) (1000 * sspMagiSettings::timing.pumpStart);

  int nNoteOns = 0;
  int nTotTime = 0;
  int nTimeWindow = (int) (sspMagiSettings::timing.timeWindow * 1000 + 0.5);
  int nSumVelocity = 0;

  sspMidiSequence::iterator iter;
  for (iter = track.begin(); iter != track.end(); ++iter) {
    nTotTime += iter->timestamp;
    if (nTotTime >= nTimeWindow && nNoteOns > 0) {
      int velocity = nSumVelocity / nNoteOns;
      velocity = std::max(sspMagiSettings::velocity.minimum, velocity);
      velocity = std::min(sspMagiSettings::velocity.maximum, velocity);
      sspMidi::setData2(newEvent.message, velocity);
      newTrack.push_back(newEvent);
      // Prepare the next event
      newEvent.timestamp = nTotTime;
      nNoteOns = 0;
      nTotTime = 0;
      nSumVelocity = 0;
    }

    // Map pitches
    if (sspMidi::isNoteOn(iter->message) || sspMidi::isNoteOff(iter->message)) {
      int nNote = sspMidi::getData1(iter->message);
      if (noteonMap.find(nNote) == noteonMap.end()) {
        CString error;
        error.Format("MIDI pitch %d is not in the pitch-to-DMX map. Continue?", nNote);
        int nReturn = AfxMessageBox(error, MB_OKCANCEL);
        if (nReturn == IDCANCEL) {
          resetDialog();
          return;
        }
      }
      else {
        sspMidi::setData1(iter->message, (BYTE) noteonMap[nNote]);
      }
    }

    // Calculate velocity
    if (sspMidi::isNoteOn(iter->message)) {
      nSumVelocity += sspMidi::getData2(iter->message);
      sspMidi::setData2(iter->message, (BYTE) 127);
      nNoteOns++;
    }
  }
  track.front().timestamp = (sspMidiTimestamp) (1000 * (sspMagiSettings::timing.timeOffset + sspMagiSettings::timing.pumpStart));
  sspMidi::setData2(newEvent.message, 0);
  newEvent.timestamp += nTotTime + (sspMidiTimestamp) (1000 * (sspMagiSettings::timing.timeOffset + sspMagiSettings::timing.pumpStop));
  newTrack.push_back(newEvent);
  m_song.splitTracks();
  m_song.push_back(newTrack);

//  Tempo is not maintained when using SMPTE - should be investigated further
//  m_song.getFileTime().setSMPTEBase(25,40); // Millisecond precision

  // Save the processed song to a new file
  int nCount = m_strFilename.Find('.');
  CString strSaveFile = m_strFilename.Left(nCount);
  strSaveFile += "_new.mid";
	if (SaveFile(
			GetSafeHwnd(), 
			"Specify a MIDI filename", 
			"Midi Files (*.mid)\0*.mid\0", 
      "mid",
			strSaveFile)) {

    sspMidiFileWriter file;
    if (file.open(strSaveFile) == sspMidiFileWriter::ERRMSG_OK) {
      if (file.write(m_song, 1) == sspMidiFileWriter::ERRMSG_OK) {
        AfxMessageBox("The file converted successfully!");
      }
      else {
        AfxMessageBox("Invalid or empty MIDI file!");
      }
			file.close();
		}
    else {
      AfxMessageBox("Unable to open MIDI file for writing!");
    }
      }
  else {
    AfxMessageBox("Unable to open MIDI file for writing!");
  }
  resetDialog();
}

void CMidiMagiDlg::resetDialog()
{
  m_song.clear();
  m_strFilename.Empty();
  m_dDuration = 0.0;
  m_nTracks = 0;
  UpdateData(FALSE);
  GetDlgItem(IDC_PROCESS)->EnableWindow(FALSE);
}

void CMidiMagiDlg::OnNMDblclkNoteonMap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nSelected = pNMLV->iItem;
	if (nSelected == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_lcNoteMap.SubItemHitTest(&lvhti);
		nSelected = lvhti.iItem;
	}
//	m_lcNoteMap.SetItemState(nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;

	if (nSelected < 0)
		return;

	// iSubItem == 0 does not trigger editing
  CRect rItem, rWnd;
  m_lcNoteMap.GetWindowRect(rWnd);
  m_lcNoteMap.GetSubItemRect(nSelected, pNMLV->iSubItem, LVIR_BOUNDS, rItem);
  rItem += rWnd.TopLeft();

  sspGUIMapDialog dlg;
  dlg.m_nPitch = sspMagiSettings::sounding[nSelected];
  dlg.m_nDMX = sspMagiSettings::dmx[nSelected];
	if (dlg.DoModal() == IDOK) {
		char number[10];
		sspMagiSettings::sounding[nSelected] = dlg.m_nPitch;
    _itoa_s(sspMagiSettings::sounding[nSelected],number,10,10);
		m_lcNoteMap.SetItemText(nSelected, 0, number);
		sspMagiSettings::dmx[nSelected] = dlg.m_nDMX;
    _itoa_s(sspMagiSettings::dmx[nSelected],number,10,10);
		m_lcNoteMap.SetItemText(nSelected, 1, number);
	}
}
