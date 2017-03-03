// MidiData.h: interface for the CMidiData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDIDATA_H__716DE6A5_6925_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_MIDIDATA_H__716DE6A5_6925_11D2_97D2_00105A3B4608__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "mmsystem.h"
#include "time.h"
#include "utility.h"
#include <afxtempl.h>

#define WM_PLAY_DONE	WM_USER + 5

class CMidiUtility
{
public:
	static int GetDevices();
	static BOOL GetDeviceName(int deviceNr, CString& name);
};

class CMidiDevice
{
private:
	enum {NO_DEVICE=1111};		// An impossible value

	HMIDIOUT m_MidiHandle;
	MIDIOUTCAPS m_MidiCaps;
	int m_nOpenDevice;

public:
	enum {MAX_CHANNELS=16, MAX_NOTES=32};

	CMidiDevice();
	virtual ~CMidiDevice();

	BOOL OpenDevice(int deviceNr);
	BOOL CloseDevice();
	BOOL ResetDevice() const;
	int GetVoices() const;
	int GetNotes() const;
	BOOL ChannelSupport(int nChannel) const 
		{return (m_MidiCaps.wChannelMask & (0x0001 << nChannel));}
	void SendMidi(DWORD dBytes) const {midiOutShortMsg(m_MidiHandle, dBytes);}
};

class CMidiEvent : public CObject 
{
protected:
	union {DWORD hele; BYTE del[4];} midi;

public:
	enum {NOTEOFF=0x80, NOTEON=0x90, PROGRAM=0xC0, SYSTEM=0xF0};

	CMidiEvent();
	CMidiEvent(BYTE byte1, BYTE byte2, BYTE byte3);
	virtual ~CMidiEvent() {}

	void SetEventData(int ind, BYTE byte) {midi.del[ind] = byte;}
	void SetEventData(BYTE byte1, BYTE byte2, BYTE byte3);
	void SetKanal(int nKanal) {midi.del[0] = (midi.del[0] & 0xF0) + nKanal;}
	void ChangeProgram(BYTE program);
	BOOL IsNoteOn() const {return (GetStatus() == NOTEON) && (midi.del[2] > 0);}
	BOOL IsProgram() const {return (GetStatus() == PROGRAM);}
	BOOL IsSystemMessage() const {return (GetStatus() == SYSTEM);}
	BYTE GetStatus() const {return (midi.del[0] & 0xF0);}
	BYTE GetKanal() const {return (midi.del[0] & 0x0F);}
	BYTE GetProgram() const {return (IsProgram() ? midi.del[1] : 0);}
	BYTE GetVelocity() const {return (IsNoteOn() ? midi.del[2] : 0);}
	BYTE GetEventData(int nInd) const {return midi.del[nInd];}
	DWORD GetEventData() const {return midi.hele;}
};

class CMidiTidEvent : public CMidiEvent  
{
private:
	double m_dTid;
	double m_dTidFak;
	CTimeRef* m_pTidsRef;
	DECLARE_SERIAL(CMidiTidEvent)

public:
	CMidiTidEvent();
	CMidiTidEvent(BYTE byte1, BYTE byte2, BYTE byte3);
	virtual ~CMidiTidEvent() {}
	virtual void Serialize(CArchive& ar);

	void SetTid(double dTid) {m_dTid = dTid;}
	void SetTidsfaktor(double dTid) {m_dTidFak = dTid;}
	void SetTidsfaktor(void);
	void AddTid(double dTid) {m_dTid += dTid;}
	void DelTid(double dTid) {m_dTid = (dTid > 0.0) ? m_dTid/dTid : 1.0;}
	void SetTidsRef(CTimeRef* pTidsRef) {m_pTidsRef = pTidsRef;}
	double GetOriginalTid() const {return m_dTid;}
	double GetTid() const
		{return (m_pTidsRef != NULL) ? (m_dTidFak * m_pTidsRef->GetTid()) : m_dTid;}
};

class CMidiPlayObject : public CObject
{
public:
	virtual CMidiTime* GetMidiTime() = 0;
	virtual	double InitSpill(void) = 0;
	virtual double Spill(CMidiDevice* pDevice) = 0;
	virtual void SluttSpill() = 0;
};

class CMidiSekvens : public CMidiPlayObject
{
private:
	CTypedPtrList<CObList, CMidiTidEvent*> m_pSekvens;
	int m_nKanal;
	CString m_sTittel;
	BYTE m_nProgram;
	POSITION m_pos;
	long m_nAntall;
	CMidiTime* m_pMidiTime;
	DECLARE_SERIAL(CMidiSekvens)

public:
	CMidiSekvens();
	CMidiSekvens(int nKanal);
	virtual ~CMidiSekvens();
	virtual void Serialize(CArchive& ar);

	void SetTittel(const CString& tittel) {m_sTittel = tittel;}
	void SetHeaderInfo();
	void SetKanal(int kanal) {m_nKanal = kanal; SetSingleChannel();}
	void SetKanal() {SetSingleChannel();}
	void SetProgram(BYTE program) {m_nProgram = program; SetSingleProgram();}
	void SetProgram() {SetSingleProgram();}
	void SetMidiTime(CMidiTime* pMidiTime) {m_pMidiTime = pMidiTime;}
	const CString& GetTittel() const {return m_sTittel;}
	int GetKanal() const {return m_nKanal;}
	long GetAntall() const {return m_nAntall;}
	BYTE GetProgram() const {return m_nProgram;}

	void LeggForan(CMidiTidEvent* event) {m_pos = m_pSekvens.AddHead(event); m_nAntall++;}
	void LeggBak(CMidiTidEvent* event) {m_pos = m_pSekvens.AddTail(event); m_nAntall++;}
	CMidiTidEvent* FjernEvent(void);
	void InitPos(void) {m_pos = m_pSekvens.GetHeadPosition();}
	void SetPos(CMidiTidEvent* event) {m_pos = m_pSekvens.Find(event);}
	CMidiTidEvent* GetEvent(void) {return (m_pos != NULL) ? m_pSekvens.GetAt(m_pos) : NULL;}
	CMidiTidEvent* GetNeste(void) {return (m_pos != NULL) ? m_pSekvens.GetNext(m_pos) : NULL;}
	CMidiTidEvent* FinnEvent(BYTE type);
	void FilterEvent(BYTE type);
	void SetSingleChannel();
	void SetSingleProgram();
	CMidiSekvens* SplittSekvens(void);
	void NormaliserTid();

	CMidiTime* GetMidiTime() {return m_pMidiTime;}
	double InitSpill(void);
	double Spill(CMidiDevice* pDevice);
	void SluttSpill() {}
};

class CMidiSang : public CMidiPlayObject
{
public:
	enum {TELLER, NEVNER};
	enum {DEFAULT_TAKTART1 = 4, DEFAULT_TAKTART2 = 4};

private:
	CTypedPtrArray<CObArray, CMidiSekvens*> m_pSang;
	CString m_sTittel;
	int m_nSpor;
	int m_nTaktart[2];
	CMidiTime m_cMidiTime;
	UTFloatIntList m_utList;
	DECLARE_SERIAL(CMidiSang)

public:
	CMidiSang();
	CMidiSang(int spor);
	virtual ~CMidiSang();
	virtual void Serialize(CArchive& ar);

	void SetTittel(const CString& tittel) {m_sTittel = tittel;}
	void SetSpor(int spor);
	void SetTaktart(int teller, int nevner);
	void SetSekvens(CMidiSekvens* pSekv, int nInd);
	void LeggTilSekvens(CMidiSekvens* pSekv, int nInd);
	void FjernSekvens(int nInd);
	void SetMetronom(int metronom) {m_cMidiTime.SetMetronom(metronom);}

	const CString& GetTittel() const {return m_sTittel;}
	int GetSpor() const {return m_nSpor;}
	int GetTaktart(int telnev) const {return m_nTaktart[telnev];}
	CMidiTime* GetMidiTime() {return &m_cMidiTime;}
	CMidiSekvens* GetSekvens(int nInd) const {return m_pSang[nInd];}
	void NormaliserTid() ;

	double InitSpill(void);
	double Spill(CMidiDevice* pDevice);
	void SluttSpill(void);
};

class CMidiPlay  
{
private:
	enum {MIN_TIME = 1, MIN_RES = 0};
	enum {DONE_OK, DONE_FAIL};
	CMidiDevice m_device;
	HWND m_winHandle;
	CMidiPlayObject* m_pSekvens;
	volatile BOOL m_bLoop;
	volatile BOOL m_bPlaying;
	volatile BOOL m_bStopped;
	volatile WORD m_nMinTime, m_nMaxTime;
	volatile long m_nTimeBuffer;

	friend void CALLBACK TimeCallback(WORD wID, WORD msg, DWORD dwUser, DWORD dw1, DWORD dw2);
	void SpillNeste();
	WORD NyTid(void);
	WORD NyTid(double tid);
	static volatile WORD m_nTimerID;

public:
	CMidiPlay();
	virtual ~CMidiPlay() {}

	void Initialize(int DeviceNr, HWND handle);
	void ChangeDevice(int DeviceNr) {m_device.OpenDevice(DeviceNr);}

	void SetSekvens (CMidiPlayObject* pSekvens) {m_pSekvens = pSekvens;}
	void SetLoop (BOOL bLoop) {m_bLoop = bLoop;}
	BOOL IsPlaying() {return m_bPlaying;}
	BOOL IsLooping() {return m_bLoop;}
	void Spill();
	void SpillSlutt();
	void Stopp();
};

#endif // !defined(AFX_MIDIDATA_H__716DE6A5_6925_11D2_97D2_00105A3B4608__INCLUDED_)
