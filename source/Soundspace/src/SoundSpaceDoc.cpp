// SoundSpaceDoc.cpp : implementation of the CSoundSpaceDoc class
//

#include "stdafx.h"
#include "SoundSpace.h"

#include "SoundSpaceDoc.h"
#include "sspSetting.h"
#include "ConditionalDialog.h"
#include "PlayObjectDialog.h"
#include "InputDialog.h"
#include "PlayTaskDialog.h"
#include "StartStopDialog.h"
#include "StreamDialog.h"
#include "DeviceDialog.h"
#include "ValueDialog.h"
#include "sspGUIStringDialog.h"
#include "sspPools.h"
#include "sspExecutiveManager.h"
#include "sspPlayManager.h"
#include "sspDateTimeManager.h"
#include "SSpStream.h"
#include "SettingsResetDlg.h"
#include "sspDeviceManager.h"
#include "sspInputManager.h"
#include "sspResetManager.h"
#include "sspGUIMsgList.h"
#include "sspGUIClockMsgList.h"
#include "sspGUITriggerMsgList.h"
#include "sspGUIPreferences.h"

#include <iomanip>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundSpaceDoc

IMPLEMENT_DYNCREATE(CSoundSpaceDoc, CDocument)

BEGIN_MESSAGE_MAP(CSoundSpaceDoc, CDocument)
	//{{AFX_MSG_MAP(CSoundSpaceDoc)
	ON_COMMAND(ID_STREAMS, OnStreams)
	ON_COMMAND(ID_VALUES, OnValues)
	ON_COMMAND(ID_CONDITIONALS, OnConditionals)
	ON_COMMAND(ID_EDIT_STRINGS, OnStrings)
	ON_COMMAND(ID_SOUND_OBJECTS, OnPlayObjects)
	ON_COMMAND(ID_SOUND_EVENTS, OnPlayTasks)
	ON_COMMAND(ID_TASKLIST_INITIAL, OnInitialEvents)
	ON_COMMAND(ID_TASKLIST_TRIGGERED, OnTriggerEvents)
	ON_COMMAND(ID_TASKLIST_TIME, OnClockEvents)
	ON_UPDATE_COMMAND_UI(ID_TASKLIST_INITIAL, OnUpdateInitialEvents)
	ON_UPDATE_COMMAND_UI(ID_TASKLIST_TRIGGERED, OnUpdateTriggerEvents)
	ON_UPDATE_COMMAND_UI(ID_TASKLIST_TIME, OnUpdateClockEvents)
	ON_UPDATE_COMMAND_UI(ID_SOUND_EVENTS, OnUpdatePlayTasks)
	ON_UPDATE_COMMAND_UI(ID_SOUND_OBJECTS, OnUpdatePlayObjects)
	ON_UPDATE_COMMAND_UI(ID_STREAMS, OnUpdateStreams)
	ON_UPDATE_COMMAND_UI(ID_CONDITIONALS, OnUpdateConditionals)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_RESET, OnUpdateResetSetting)
	ON_UPDATE_COMMAND_UI(ID_VALUES, OnUpdateValues)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STRINGS, OnUpdateStrings)
	ON_COMMAND(ID_SETTINGS_STARTUPSHUTDOWN, OnSettingsStartupshutdown)
	ON_COMMAND(ID_SETTINGS_RESET, OnResetSetting)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_SETTINGS_PREFERENCES, OnPreferences)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_PREFERENCES, OnUpdatePreferences)
	ON_COMMAND(ID_DEVICES_AUDIO, OnDevicesAudio)
	ON_COMMAND(ID_DEVICES_DMX, OnDevicesDmx)
	ON_COMMAND(ID_DEVICES_MIDI, OnDevicesMidi)
	ON_UPDATE_COMMAND_UI(ID_DEVICES_AUDIO, OnUpdateDevicesAudio)
	ON_UPDATE_COMMAND_UI(ID_DEVICES_MIDI, OnUpdateDevicesMidi)
	ON_UPDATE_COMMAND_UI(ID_DEVICES_DMX, OnUpdateDevicesDmx)
	ON_COMMAND(ID_EDIT_INPUTS, OnEditInputs)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INPUTS, OnUpdateEditInputs)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundSpaceDoc construction/destruction

CSoundSpaceDoc::CSoundSpaceDoc()
{
}

CSoundSpaceDoc::~CSoundSpaceDoc()
{
}

BOOL CSoundSpaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	sspPool::Instance().initContents();
	return TRUE;
}

void CSoundSpaceDoc::DeleteContents() 
{
  sspExecutiveManager::Instance().clearContents();
	sspPool::Instance().clearContents();
	sspDeviceManager::Instance().clearContents();
	sspInputManager::Instance().clearContents();
  sspResetManager::Instance().clearContents();
  sspPlayManager::Instance().clearContents();
	CDocument::DeleteContents();
}

/////////////////////////////////////////////////////////////////////////////
// CSoundSpaceDoc serialization

void CSoundSpaceDoc::Serialize(CArchive& ar)
{
	int i, nSize;

	CDocument::Serialize(ar);
	sspPool::Instance().Serialize(ar);
	sspDeviceManager::Instance().Serialize(ar);
	sspInputManager::Instance().Serialize(ar);
  sspResetManager::Instance().Serialize(ar);
  sspPlayManager::Instance().Serialize(ar);
  sspDateTimeManager::Instance().Serialize(ar);
  sspExecutiveManager::Instance().Serialize(ar);
	if (ar.IsStoring())
	{
		nSize = m_nDisplaySelection.size();
		ar << nSize;
		for (i=0; i<nSize; i++) {
			ar << m_nDisplaySelection[i];
		}
	}
	else
	{
		ar >> nSize;
		m_nDisplaySelection.resize(nSize);
		for (i=0; i<nSize; i++) {
			ar >> m_nDisplaySelection[i];
		}
	}
}

bool CSoundSpaceDoc::initialize(LPVOID hWnd)
{
	if (!sspDeviceManager::Instance().initialize(hWnd)) {
    AfxMessageBox(L"Error in initializing devices");
		return false;
	}
	if (!sspInputManager::Instance().initialize(hWnd)) {
    AfxMessageBox(L"Error in initializing inputs");
		return false;
	}
	if (!sspResetManager::Instance().initialize()) {
    AfxMessageBox(L"Error in initializing watchdog");
		return false;
	}
  if (!sspPlayManager::Instance().initialize()) {
    AfxMessageBox(L"Error in initializing play objects");
    return false;
  }
	if (!sspExecutiveManager::Instance().initialize(hWnd)) {
    AfxMessageBox(L"Error in initializing execution loop");
		return false;
	}
  return true;
}

void CSoundSpaceDoc::terminate()
{
  sspDeviceManager::Instance().terminate();
  sspInputManager::Instance().terminate();
  sspResetManager::Instance().terminate();
  sspPlayManager::Instance().terminate();
	sspExecutiveManager::Instance().terminate();
}

/////////////////////////////////////////////////////////////////////////////
// CSoundSpaceDoc diagnostics

#ifdef _DEBUG
void CSoundSpaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSoundSpaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSoundSpaceDoc commands

void CSoundSpaceDoc::OnStreams() 
{
	CStreamDialog dlg;
	dlg.m_pMain = sspPool::Instance().streams.getRoot();
	if (dlg.DoModal() == IDOK) SetModifiedFlag();
	UpdateAllViews(NULL, INITIALIZE);
}

void CSoundSpaceDoc::OnValues() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK) SetModifiedFlag();
	UpdateAllViews(NULL, INITIALIZE);
}

void CSoundSpaceDoc::OnConditionals() 
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK) SetModifiedFlag();
	UpdateAllViews(NULL, INITIALIZE);
}

void CSoundSpaceDoc::OnStrings() 
{
	sspGUIStringDialog dlg;
	if (dlg.DoModal() == IDOK) SetModifiedFlag();
	UpdateAllViews(NULL, INITIALIZE);
}

void CSoundSpaceDoc::OnPlayObjects() 
{
	CPlayObjectDialog dlg;
	if (dlg.DoModal() == IDOK) SetModifiedFlag();
	UpdateAllViews(NULL, INITIALIZE);
}

void CSoundSpaceDoc::OnEditInputs()
{
	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) SetModifiedFlag();
	UpdateAllViews(NULL, INITIALIZE);
}

void CSoundSpaceDoc::OnPlayTasks() 
{
	CPlayTaskDialog dlg;
	if (dlg.DoModal() == IDOK) SetModifiedFlag();
	UpdateAllViews(NULL, INITIALIZE);
}

void CSoundSpaceDoc::OnInitialEvents() 
{
	sspGUIMsgList dlg;
	dlg.m_list = sspPlayManager::Instance().getStartList();
  if (dlg.DoModal() == IDOK) {
    SetModifiedFlag();
		sspPlayManager::Instance().getStartList() = dlg.m_list;
  }
}

void CSoundSpaceDoc::OnTriggerEvents() 
{
	sspGUITriggerMsgList dlg;
	dlg.m_list = sspPlayManager::Instance().getTriggerList();
	if (dlg.DoModal() == IDOK) {
    SetModifiedFlag();
		sspPlayManager::Instance().getTriggerList() = dlg.m_list;
  }
}

void CSoundSpaceDoc::OnClockEvents() 
{
	sspGUIClockMsgList dlg;
	dlg.m_list = sspPlayManager::Instance().getClockList();
	if (dlg.DoModal() == IDOK) {
    SetModifiedFlag();
		sspPlayManager::Instance().getClockList() = dlg.m_list;
  }
}

void CSoundSpaceDoc::OnPreferences() 
{
	sspGUIPreferences sheet(_T("Preferences"));

  sheet.m_pageAudio.m_nFrequency = sspSetting::audio.freq;
  sheet.m_pageAudio.m_nBufferLength = sspSetting::audio.bufSize;
  sheet.m_pageAudio.m_nBufferUpdate = sspSetting::audio.update;

  sheet.m_pageUpdates.m_fEvent = sspSetting::update.event;
  sheet.m_pageUpdates.m_fClock = sspSetting::update.clock;
  sheet.m_pageUpdates.m_fGUI = sspSetting::update.gui;

  sheet.m_pageResolution.m_nTimer = sspSetting::res.timer;
  sheet.m_pageResolution.m_nEnvelope = sspSetting::res.envelope;
  sheet.m_pageResolution.m_nEvent = sspSetting::res.event;

  sheet.m_pageFade.m_nFadeIn = sspSetting::fade.fadeIn;
  sheet.m_pageFade.m_nFadeOut = sspSetting::fade.fadeOut;
  sheet.m_pageFade.m_nVolume = sspSetting::fade.volume;

  if (sheet.DoModal() == IDOK) {
		sspSetting::audio.freq = sheet.m_pageAudio.m_nFrequency;
		sspSetting::audio.bufSize = sheet.m_pageAudio.m_nBufferLength;
		sspSetting::audio.update = sheet.m_pageAudio.m_nBufferUpdate;

		sspSetting::update.event = sheet.m_pageUpdates.m_fEvent;
		sspSetting::update.clock = sheet.m_pageUpdates.m_fClock;
		sspSetting::update.gui = sheet.m_pageUpdates.m_fGUI;

		sspSetting::res.timer = sheet.m_pageResolution.m_nTimer;
		sspSetting::res.envelope = sheet.m_pageResolution.m_nEnvelope;
		sspSetting::res.event = sheet.m_pageResolution.m_nEvent;

		sspSetting::fade.fadeIn = sheet.m_pageFade.m_nFadeIn;
		sspSetting::fade.fadeOut = sheet.m_pageFade.m_nFadeOut;
		sspSetting::fade.volume = sheet.m_pageFade.m_nVolume;

    sspSetting::WriteAllRegistry();
		UpdateAllViews(NULL, INITIALIZE);
	}
}

void CSoundSpaceDoc::OnSettingsStartupshutdown() 
{
	CStartStopDialog dlg;
  dlg.m_bEnableInterval = sspDateTimeManager::Instance().hasPlayInterval();
  const SSpClock& start = sspDateTimeManager::Instance().getStartTime();
  dlg.m_startTime = CTime(2000,1,1,start.hour(),start.min(), 0);
  const SSpClock& stop = sspDateTimeManager::Instance().getEndTime();
  dlg.m_stopTime = CTime(2000,1,1,stop.hour(), stop.min(),0);
  dlg.m_nStartAction = sspExecutiveManager::Instance().getStartupProcedure();
  dlg.m_emptyAction = sspExecutiveManager::Instance().getShutdownProcedure();
	if (dlg.DoModal() == IDOK) {
    sspDateTimeManager::Instance().enablePlayInterval(dlg.m_bEnableInterval == TRUE);
    sspDateTimeManager::Instance().setStartTime(SSpClock(dlg.m_startTime.GetHour(), dlg.m_startTime.GetMinute(), 0));
    sspDateTimeManager::Instance().setEndTime(SSpClock(dlg.m_stopTime.GetHour(), dlg.m_stopTime.GetMinute(), 0));
    sspExecutiveManager::Instance().setStartupProcedure((tStartupProc) dlg.m_nStartAction);
    sspExecutiveManager::Instance().setShutdownProcedure((tShutdownProc) dlg.m_emptyAction);
    SetModifiedFlag();
		UpdateAllViews(NULL, INITIALIZE);
	}
}

void CSoundSpaceDoc::OnResetSetting() 
{
	CSettingsResetDlg dlg;
  dlg.m_bEnable = sspResetManager::Instance().hasWatchdog();
  dlg.m_nWatchdogType = sspResetManager::Instance().getWathdog();
  dlg.m_nTimeout = sspResetManager::Instance().getTimeOut();
  dlg.m_nTimeUnit = sspResetManager::Instance().getTimeUnit();
  dlg.m_nInterval = sspResetManager::Instance().getResetInterval();
  dlg.m_nConditionIndex = sspResetManager::Instance().getResetCondition();
  const SSpClock& time = sspResetManager::Instance().getTimeOfDay();
  dlg.m_timeOfDay = CTime(2000,1,1,time.hour(),time.min(), 0);
	if (dlg.DoModal() == IDOK) {
    sspResetManager::Instance().enableWatchdog(dlg.m_bEnable == TRUE);
    sspResetManager::Instance().setWathdog(dlg.m_nWatchdogType);
    sspResetManager::Instance().setTimeOut(dlg.m_nTimeout);
    sspResetManager::Instance().setTimeUnit((sspResetManager::tTimeUnit) dlg.m_nTimeUnit);
    sspResetManager::Instance().setResetInterval(dlg.m_nInterval);
    sspResetManager::Instance().setResetCondition(dlg.m_nConditionIndex);
    sspResetManager::Instance().setTimeOfDay(SSpClock(dlg.m_timeOfDay.GetHour(), dlg.m_timeOfDay.GetMinute(), 0));
    SetModifiedFlag();
  }
}

void CSoundSpaceDoc::OnUpdateEditInputs(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning());
}

void CSoundSpaceDoc::OnUpdateClockEvents(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning() && sspPool::Instance().tasks.IsLoaded());
}

void CSoundSpaceDoc::OnUpdateInitialEvents(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning() && sspPool::Instance().tasks.IsLoaded());
}

void CSoundSpaceDoc::OnUpdatePlayTasks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning() && sspPool::Instance().objects.IsLoaded());
}

void CSoundSpaceDoc::OnUpdatePlayObjects(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning() && sspPool::Instance().values.IsLoaded() && sspPool::Instance().conditionals.IsLoaded());
}

void CSoundSpaceDoc::OnUpdateTriggerEvents(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning() && sspPool::Instance().tasks.IsLoaded());
}

void CSoundSpaceDoc::OnUpdateStreams(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning() && sspPool::Instance().values.IsLoaded());
}

void CSoundSpaceDoc::OnUpdatePreferences(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning());
}

void CSoundSpaceDoc::OnUpdateConditionals(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning());
}

void CSoundSpaceDoc::OnUpdateStrings(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning());
}

void CSoundSpaceDoc::OnUpdateResetSetting(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning());
}

void CSoundSpaceDoc::OnUpdateValues(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning());
}

void CSoundSpaceDoc::OnDevicesAudio()
{
	CDeviceAudioDialog dlg;
	if (dlg.DoModal() == IDOK) {
		UpdateAllViews(NULL, INITIALIZE);
	}
}

void CSoundSpaceDoc::OnDevicesMidi()
{
	CDeviceMidiDialog dlg;
	if (dlg.DoModal() == IDOK) {
		UpdateAllViews(NULL, INITIALIZE);
	}
}

void CSoundSpaceDoc::OnDevicesDmx()
{
	CDeviceDmxDialog dlg;
	if (dlg.DoModal() == IDOK) {
		UpdateAllViews(NULL, INITIALIZE);
	}
}

void CSoundSpaceDoc::OnUpdateDevicesAudio(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning());
}

void CSoundSpaceDoc::OnUpdateDevicesMidi(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning());
}

void CSoundSpaceDoc::OnUpdateDevicesDmx(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!sspExecutiveManager::Instance().isRunning());
}

bool CSoundSpaceDoc::exportAscii(const CString& strFile)
{
  sspOutStream file(strFile);
  if (!file) return false;

	file << "Document: " << (LPCTSTR)GetTitle() << setprecision(6) << endl;
	sspPool::Instance().printASCII(file);
	file << endl;
	sspDeviceManager::Instance().printASCII(file);
	file << endl;
  sspPlayManager::Instance().printASCII(file);

  return true;
}

bool CSoundSpaceDoc::exportErrors(const CString& strFile, int& nErrors, int& nWarnings)
{
	sspOutStream file(strFile);
  if (!file) return false;

	nErrors = 0; nWarnings = 0;
	file << "Configuration errors in document: " << (LPCTSTR)GetTitle() << endl;
	if (!sspPool::Instance().verify(file, nErrors, nWarnings))
    return false;
	if (!sspDeviceManager::Instance().verify(file, nErrors, nWarnings))
    return false;
	if (!sspPlayManager::Instance().verify(file, nErrors, nWarnings))
    return false;

  return true;
}
