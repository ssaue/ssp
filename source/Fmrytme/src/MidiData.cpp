// MidiData.cpp: implementation of the CMidiData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fmrytme.h"
#include "MidiData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CMidiSang, CObject, 0)
IMPLEMENT_SERIAL(CMidiSekvens, CObject, 0)
IMPLEMENT_SERIAL(CMidiTidEvent, CObject, 0)

//////////////////////////////////////////////////////////////////////
// CMidiUtility Class
//////////////////////////////////////////////////////////////////////

int CMidiUtility::GetDevices()
{
	return (int) midiOutGetNumDevs();
}

BOOL CMidiUtility::GetDeviceName(int deviceNr, CString& name)
{
	MIDIOUTCAPS midiDevice;
	if (midiOutGetDevCaps(deviceNr, (LPMIDIOUTCAPS) &midiDevice, sizeof(MIDIOUTCAPS)) == MMSYSERR_NOERROR) {
		name = midiDevice.szPname;
		return TRUE;
	}
	else
		return FALSE;
}
			
//////////////////////////////////////////////////////////////////////
// CMidiDevice Class
//////////////////////////////////////////////////////////////////////

CMidiDevice::CMidiDevice()
{
	m_nOpenDevice = NO_DEVICE;
}

CMidiDevice::~CMidiDevice()
{
	CloseDevice();
}

BOOL CMidiDevice::OpenDevice(int deviceNr)
{
	if (m_nOpenDevice == deviceNr)			// Device already open
		return TRUE;
	else if (m_nOpenDevice != NO_DEVICE)	// Another device is open, close this first
		CloseDevice();
	MMRESULT res = midiOutOpen((LPHMIDIOUT) &m_MidiHandle, deviceNr, NULL, 0L, CALLBACK_NULL);
	if (res == MMSYSERR_NOERROR ) {
		m_nOpenDevice = deviceNr;
		midiOutGetDevCaps(deviceNr, (LPMIDIOUTCAPS) &m_MidiCaps, sizeof(MIDIOUTCAPS));
		return TRUE;
	}
	else {
		m_nOpenDevice = NO_DEVICE;
		AfxMessageBox(IDS_MIDIOPEN_FAIL);
		return FALSE;
	}
}

BOOL CMidiDevice::CloseDevice()
{
	if (m_nOpenDevice == NO_DEVICE)
		return TRUE;		// Enheten er allerede lukket
	midiOutReset(m_MidiHandle);
	MMRESULT res = midiOutClose(m_MidiHandle);
	if (res == MMSYSERR_NOERROR ) {
		m_nOpenDevice = NO_DEVICE;
		return TRUE;
	}
	else {
		AfxMessageBox(IDS_MIDICLOSE_FAIL);
		return FALSE;
	}
}

BOOL CMidiDevice::ResetDevice() const
{
	if (midiOutReset(m_MidiHandle) == 0)
		return TRUE;
	else
		return FALSE;
}


int CMidiDevice::GetVoices() const
{
	if (m_MidiCaps.wTechnology == MOD_MIDIPORT)
		return MAX_CHANNELS;
	else
		return m_MidiCaps.wVoices;
}

int CMidiDevice::GetNotes() const
{
	if (m_MidiCaps.wTechnology == MOD_MIDIPORT)
		return MAX_NOTES;
	else
		return m_MidiCaps.wNotes;
}

//////////////////////////////////////////////////////////////////////
// CMidiEvent Class
//////////////////////////////////////////////////////////////////////

CMidiEvent::CMidiEvent()
{
	midi.del[0] = 0x80;		// Legger inn en default MIDI-beskjed (note av)
	midi.del[1] = 64;
	midi.del[2] = 0;
	midi.del[3] = 0;
}

CMidiEvent::CMidiEvent(BYTE byte1, BYTE byte2, BYTE byte3)
{
	midi.del[0] = byte1;
	midi.del[1] = byte2;
	midi.del[2] = byte3;
	midi.del[3] = 0;
}

void CMidiEvent::SetEventData(BYTE byte1, BYTE byte2, BYTE byte3)
{
	midi.del[0] = byte1;
	midi.del[1] = byte2;
	midi.del[2] = byte3;
}

inline void CMidiEvent::ChangeProgram(BYTE program)
{
	if (GetStatus() == PROGRAM)
		midi.del[1] = program;
}


//////////////////////////////////////////////////////////////////////
// CMidiTidEvent Class
//////////////////////////////////////////////////////////////////////

CMidiTidEvent::CMidiTidEvent()
{
	m_dTidFak = m_dTid = 0.0;
	m_pTidsRef = NULL;
}

CMidiTidEvent::CMidiTidEvent(BYTE byte1, BYTE byte2, BYTE byte3) : CMidiEvent(byte1, byte2, byte3)
{
	m_dTidFak = m_dTid = 0.0;
	m_pTidsRef = NULL;
}

void CMidiTidEvent::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << m_dTid << midi.hele;
	}
	else {
		ar >> m_dTid >> midi.hele;
	}
}

void CMidiTidEvent::SetTidsfaktor()
{
	double dTid = (m_pTidsRef != NULL) ? m_pTidsRef->GetTid() : 1.0;
	m_dTidFak = (dTid > 0.0) ? m_dTid/dTid : 1.0;
}

//////////////////////////////////////////////////////////////////////
// CMidiSekvens Class
//////////////////////////////////////////////////////////////////////

CMidiSekvens::CMidiSekvens()
{
	m_nKanal = 0;
	m_sTittel = "";
	m_pos = NULL;
	m_nAntall = 0;
	m_nProgram = 0;
	m_pMidiTime = NULL;
}

CMidiSekvens::CMidiSekvens(int nKanal)
{
	m_nKanal = nKanal;
	m_sTittel = "";
	m_pos = NULL;
	m_nAntall = 0;
	m_nProgram = 0;
	m_pMidiTime = NULL;
}

CMidiSekvens::~CMidiSekvens()
{
	while (m_pSekvens.GetHeadPosition())
		delete m_pSekvens.RemoveHead();
}

void CMidiSekvens::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << m_nAntall << m_nKanal << m_nProgram << m_sTittel;
	}
	else {
		ar >> m_nAntall >> m_nKanal >> m_nProgram >> m_sTittel;
	}
	m_pSekvens.Serialize(ar);
}


void CMidiSekvens::SetHeaderInfo()
{
	InitPos();
	m_nKanal = 0;
	CMidiTidEvent* pEvent;
	while (m_pos != NULL) {
		pEvent = GetNeste();
		if (!(pEvent->IsSystemMessage()))
			m_nKanal = pEvent->GetKanal();
	}
	pEvent = FinnEvent(CMidiEvent::PROGRAM);
	if (pEvent != NULL)
		m_nProgram = pEvent->GetProgram();
}

void CMidiSekvens::SetSingleChannel()
{
	InitPos();
	while (m_pos != NULL) {
		CMidiTidEvent* pEvent = GetNeste();
		if (!(pEvent->IsSystemMessage()))
			pEvent->SetKanal(m_nKanal);
	}
}

void CMidiSekvens::SetSingleProgram()
{
	FilterEvent(CMidiEvent::PROGRAM);
	CMidiTidEvent* pEvent = new CMidiTidEvent((CMidiEvent::PROGRAM + m_nKanal), m_nProgram, 0);
	LeggForan(pEvent);
}

CMidiTidEvent* CMidiSekvens::FjernEvent(void)
{
	CMidiTidEvent* pEvent = m_pSekvens.GetAt(m_pos);
	m_pSekvens.RemoveAt(m_pos);
	m_nAntall--;
	return pEvent;
}

CMidiTidEvent* CMidiSekvens::FinnEvent(BYTE type)
{
	for (InitPos(); m_pos != NULL;) {
		CMidiTidEvent* event = GetNeste();
		if (event->GetStatus() == type)
			return event;
	}
	return NULL;
}

void CMidiSekvens::FilterEvent(BYTE type)
{
	POSITION pos1, pos2;
	for (InitPos(); (pos1 = m_pos) != NULL;) {
		if (GetNeste()->GetStatus() == type) {
			pos2 = m_pos;
			m_pos = pos1;
			delete FjernEvent();
			m_pos = pos2;
		}
	}
}

CMidiSekvens* CMidiSekvens::SplittSekvens(void)
{
	CMidiSekvens* nySekvens = new CMidiSekvens();
	POSITION pos1, pos2;
	double tid1 = 0.0, tid2 = 0.0;
	for (InitPos(); (pos1 = m_pos) != NULL;) {
		CMidiTidEvent* event = GetNeste();
		if (event->GetKanal() == m_nKanal) {
			event->AddTid(tid1);
			tid1 = 0.0;
			tid2 += event->GetTid();
		}
		else {
			event->AddTid(tid2);
			tid1 += event->GetTid();
			tid2 = 0.0;
			pos2 = m_pos;
			m_pos = pos1;
			nySekvens->LeggBak(FjernEvent());
			m_pos = pos2;
		}
	}
	return nySekvens;
}

void CMidiSekvens::NormaliserTid()		// Setter fjerdedel som tidsenhet 1.0
{
	int tidsenhet = m_pMidiTime->GetTidsformat();
	for (InitPos(); m_pos != NULL;) {
		CMidiTidEvent* pEvent = GetNeste();
		pEvent->DelTid(tidsenhet);		// Dividerer alle MIDI-events med tidsformatet
	}
}

double CMidiSekvens::InitSpill(void)
{
	InitPos();
	if (m_pos == NULL)
		return -1.0;
	else
		return GetEvent()->GetTid();
}

double CMidiSekvens::Spill(CMidiDevice* pDevice)
{
	pDevice->SendMidi(GetNeste()->GetEventData());
	if (m_pos == NULL)
		return -1.0;
	else
		return GetEvent()->GetTid();
}

//////////////////////////////////////////////////////////////////////
// CMidiSang Class
//////////////////////////////////////////////////////////////////////

CMidiSang::CMidiSang()
{
	m_sTittel = "";
	m_nSpor = 0;
	m_nTaktart[TELLER] = DEFAULT_TAKTART1;
	m_nTaktart[NEVNER] = DEFAULT_TAKTART2;
}

CMidiSang::CMidiSang(int spor)
{
	m_sTittel = "";
	m_nSpor = spor;
	m_pSang.SetSize(spor);
	m_nTaktart[TELLER] = DEFAULT_TAKTART1;
	m_nTaktart[NEVNER] = DEFAULT_TAKTART2;
}

CMidiSang::~CMidiSang()
{
	for (int i=0; i<m_nSpor; i++) {
		delete m_pSang[i];
	}
	m_pSang.RemoveAll();
	while (m_utList.GetHeadPosition()) {
		delete m_utList.RemoveHead();
	}
}

void CMidiSang::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << m_nSpor << m_nTaktart[0] << m_nTaktart[1] << m_sTittel;
	}
	else {
		ar >> m_nSpor >> m_nTaktart[0] >> m_nTaktart[1] >> m_sTittel;
	}
	m_cMidiTime.Serialize(ar);
	m_pSang.Serialize(ar);
	if (ar.IsLoading()) {
		for (int i=0; i < m_nSpor; i++)
			m_pSang[i]->SetMidiTime(&m_cMidiTime);
	}
}

void CMidiSang::SetSpor(int spor)
{
	for (int i=spor; i<m_nSpor; i++) {
			delete m_pSang[i];
	}
	m_pSang.SetSize(spor);
	m_nSpor = spor;
	m_pSang.FreeExtra();
}

void CMidiSang::SetTaktart(int teller, int nevner)
{
	m_nTaktart[TELLER] = teller;
	m_nTaktart[NEVNER] = nevner;
}

void CMidiSang::SetSekvens(CMidiSekvens* pSekv, int nInd)
{
	m_pSang[nInd] = pSekv;
	pSekv->SetMidiTime(&m_cMidiTime);
}

void CMidiSang::LeggTilSekvens(CMidiSekvens* pSekv, int nInd)
{
	m_pSang.SetAtGrow(nInd, pSekv);
	pSekv->SetMidiTime(&m_cMidiTime);
	m_pSang.FreeExtra();
	m_nSpor = (nInd >= m_nSpor) ? nInd + 1 : m_nSpor;
}

void CMidiSang::FjernSekvens(int nInd)
{
	CMidiSekvens* pGml = m_pSang[nInd];
	if (pGml == NULL)
		return;			// Allerede slettet
	m_pSang.RemoveAt(nInd);
	delete pGml;
	m_pSang.FreeExtra();
	m_nSpor--;
}

void CMidiSang::NormaliserTid()
{
	for (int i=0; i<m_nSpor; i++)
		m_pSang[i]->NormaliserTid();
	m_cMidiTime.SetTidsformat(1);		//Bruker fjerdedel som enhet
}

double CMidiSang::InitSpill(void)
{
	for (int i = 0; i < m_nSpor; i++) {
		double tid = (m_pSang[i])->InitSpill();
		if (tid < 0.0) continue;
		UTFloatInt* pNyFI = new UTFloatInt(i, tid);
		POSITION pos = m_utList.GetHeadPosition();
		BOOL bPlassert = FALSE;
		while (pos != NULL) {
			UTFloatInt* pGmlFI = m_utList.GetAt(pos);
			if (pNyFI->m_dVerdi > pGmlFI->m_dVerdi) {
				pNyFI->m_dVerdi -= pGmlFI->m_dVerdi;
			}
			else {
				m_utList.InsertBefore(pos, pNyFI);
				pGmlFI->m_dVerdi -= pNyFI->m_dVerdi;
				bPlassert = TRUE;
				break;
			}
			m_utList.GetNext(pos);
		}
		if (!bPlassert) {
			m_utList.AddTail(pNyFI);
		}
	}
	if (m_utList.IsEmpty())
		return -1.0;
	else
		return m_utList.GetHead()->m_dVerdi;
}

double CMidiSang::Spill(CMidiDevice* pDevice)
{
	if (m_utList.IsEmpty()) return -1.0;
	UTFloatInt* pNyFI = m_utList.GetHead();
	pNyFI->m_dVerdi = (m_pSang[pNyFI->m_nIndeks])->Spill(pDevice);
	m_utList.RemoveHead();
	if (pNyFI->m_dVerdi < 0.0) {
		delete pNyFI;
	}
	else {
		POSITION pos = m_utList.GetHeadPosition();
		BOOL bPlassert = FALSE;
		while (pos != NULL) {
			UTFloatInt* pGmlFI = m_utList.GetAt(pos);
			if (pNyFI->m_dVerdi > pGmlFI->m_dVerdi) {
				pNyFI->m_dVerdi -= pGmlFI->m_dVerdi;
			}
			else {
				m_utList.InsertBefore(pos, pNyFI);
				pGmlFI->m_dVerdi -= pNyFI->m_dVerdi;
				bPlassert = TRUE;
				break;
			}
			m_utList.GetNext(pos);
		}
		if (!bPlassert) {
			m_utList.AddTail(pNyFI);
		}
	}
	if (m_utList.IsEmpty())
		return -1.0;
	else
		return m_utList.GetHead()->m_dVerdi;
}

void CMidiSang::SluttSpill(void)
{
	while (m_utList.GetHeadPosition()) {
		delete m_utList.RemoveHead();
	}
}

//////////////////////////////////////////////////////////////////////
// CMidiPlay Class
//////////////////////////////////////////////////////////////////////

volatile WORD CMidiPlay::m_nTimerID = 0;	// Initialiserer global variabel

CMidiPlay::CMidiPlay()
{
	m_bLoop = m_bPlaying = m_bStopped = FALSE;
	m_pSekvens = NULL;
	m_nMaxTime = m_nMinTime = 0;
	m_nTimeBuffer = 0;
}

void CMidiPlay::Initialize(int DeviceNr, HWND handle)
{
	TIMECAPS timeCaps;

	if (!m_device.OpenDevice(DeviceNr))
		return;
	if (timeGetDevCaps(&timeCaps, sizeof(TIMECAPS)) != 0)
	{
		AfxMessageBox(IDS_TIMEOPEN_FAIL);
		return;
	}
	m_winHandle = handle;
	m_nMaxTime = timeCaps.wPeriodMax;
	m_nMinTime = max(timeCaps.wPeriodMin, MIN_TIME);
}

void CMidiPlay::Spill()
{
	if ((m_pSekvens == NULL) || (m_nMaxTime == 0))
		return;
	m_nTimeBuffer = 0;
	double tid = m_pSekvens->InitSpill() * m_pSekvens->GetMidiTime()->GetTid();
	if (tid < 0.0) return;
	timeBeginPeriod(m_nMinTime);
	m_bPlaying = TRUE;
	m_bStopped = FALSE;
	while (tid < m_nMinTime) {
		tid = m_pSekvens->Spill(&m_device) * m_pSekvens->GetMidiTime()->GetTid();
		if (tid < 0.0) {
			SpillSlutt();
			return;
		}
	}
	m_nTimerID = timeSetEvent(NyTid(tid), MIN_RES, (LPTIMECALLBACK) TimeCallback, (DWORD) this, TIME_ONESHOT);
	if (!m_nTimerID)
	{
		SpillSlutt();
		AfxMessageBox(IDS_TIMEOPEN_FAIL);
		return;
	}
}

void CMidiPlay::SpillNeste()
{
	if (m_bStopped)
	{
		::PostMessage(m_winHandle, WM_PLAY_DONE, DONE_OK, 0);
		return;
	}
	if (m_nTimeBuffer > m_nMinTime)
	{
		m_nTimerID = timeSetEvent(NyTid(), MIN_RES, (LPTIMECALLBACK) TimeCallback, (DWORD) this, TIME_ONESHOT);
		if (!m_nTimerID)
			::PostMessage(m_winHandle, WM_PLAY_DONE, DONE_FAIL, 0);
		return;
	}
	double tid;
	do {
		tid = m_pSekvens->Spill(&m_device) * m_pSekvens->GetMidiTime()->GetTid();
		if (tid < 0.0) {
			if (m_bLoop) {
				tid = m_pSekvens->InitSpill() * m_pSekvens->GetMidiTime()->GetTid();
				if (tid < 0.0) {
					::PostMessage(m_winHandle, WM_PLAY_DONE, DONE_OK, 0);
					return;
				}
			}
			else {
				::PostMessage(m_winHandle, WM_PLAY_DONE, DONE_OK, 0);
				return;
			}
		}
	}
	while (tid < m_nMinTime);
	m_nTimerID = timeSetEvent(NyTid(tid), MIN_RES, (LPTIMECALLBACK) TimeCallback, (DWORD) this, TIME_ONESHOT);
	if (!m_nTimerID)
		::PostMessage(m_winHandle, WM_PLAY_DONE, DONE_FAIL, 0);
	return;
}

void CMidiPlay::SpillSlutt()
{
	if (m_nTimerID) {
		timeKillEvent(m_nTimerID);
		m_nTimerID = 0;
	}
	timeEndPeriod(m_nMinTime);
	m_pSekvens->SluttSpill();
	m_bPlaying = FALSE;
}

void CMidiPlay::Stopp()
{
	m_bStopped = TRUE;
	m_device.ResetDevice();
}

inline WORD CMidiPlay::NyTid(double tid)
{
	WORD wTid = (WORD) tid;
	if (wTid > m_nMaxTime) {
		m_nTimeBuffer = wTid - m_nMaxTime;
		return m_nMaxTime;
	}
	else
		return wTid;
}

inline WORD CMidiPlay::NyTid()
{
	if (m_nTimeBuffer > m_nMaxTime) {
		m_nTimeBuffer -= m_nMaxTime;
		return m_nMaxTime;
	}
	else {
		WORD tid = (WORD) m_nTimeBuffer;
		m_nTimeBuffer = 0;
		return tid;
	}
}

void CALLBACK TimeCallback(WORD wID, WORD msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	if (wID != CMidiPlay::m_nTimerID)
		return;
	CMidiPlay::m_nTimerID = 0;
	((CMidiPlay*) dwUser)->SpillNeste();
}

