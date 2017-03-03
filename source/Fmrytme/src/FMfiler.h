// FMfiler.h: interface for the CMidiFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMFILER_H__BEA86C43_838A_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_FMFILER_H__BEA86C43_838A_11D2_97D2_00105A3B4608__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "mididata.h"
#include "FMData.h"
#include <fstream>

class CMidiFile : public CFile  
{
private:
	enum {METABYTE=0XFF, TITTELBYTE=0X03, SLUTTBYTE=0X2f, TEMPOBYTE=0X51, TAKTARTBYTE=0X58};
	enum {SYSEX1=0XF0, SYSEX2=0XF7};
	enum {STATUSBYTE=0x80, WRITE_FORMAT=384};
	static const int dataLengde[];
	static const char* headerName;
	static const char* trackName;
	short m_nType;
	short m_nSpor;
	short m_nTidsformat;
	BYTE  m_nRunStatus;
	CMidiSang* m_pSang;

public:
	CMidiFile();
	virtual ~CMidiFile();

	BOOL ReadOpen(LPCTSTR pFilnavn);
	BOOL ReadData();
	CMidiSang* GetSang() {return m_pSang;}

	BOOL WriteOpen(LPCTSTR pFilnavn);
	BOOL WriteData();
	void LoadSong(CMidiSang* pSang) {m_pSang = pSang;}

private:
	void ReadFormat0();
	void ReadFormat1();
	CMidiSekvens* ReadSpor();
	unsigned int ReadVarInt();
	BYTE ReadMetaEvent(CMidiSekvens* sekvens);
	CMidiTidEvent* ReadMidiEvent(unsigned int tid, BYTE status);
	UINT ReadMSB(void* pBytes, UINT nAnt);

	void WriteHeader();
	void WriteInfoTrack();
	void WriteTrack(int nTrack);
	int WriteVarInt(unsigned int nValue);
	int WriteMetaEvent(BYTE event, int nTrack);
	int WriteMidiEvent(CMidiTidEvent* pEvent);
	void WriteMSB(void* pBytes, UINT nAnt);

	void Reorder(BYTE* pBytes, int nAnt);
};

class CTekstFile
{
private:
	std::ifstream m_fil;
	CMidiSekvens* m_pMidiSekv;
	CFMSekvens* m_pFMSekv;
	double m_dSlag;
	int m_nUnderdeling;
	int m_nPitsj;
	int m_nVelocity;
	int m_nAntall;

public:
	CTekstFile();
	virtual ~CTekstFile();
	BOOL Open(LPCTSTR pFilnavn);
	BOOL ReadData(double dLengde, int nTaktLengde, int nKanal);
	CMidiSekvens* GetMidiSekvens(void) const {return m_pMidiSekv;}
	CFMSekvens* GetFMSekvens(void) const {return m_pFMSekv;}
	void Close() {m_fil.close();}
};

#endif // !defined(AFX_FMFILER_H__BEA86C43_838A_11D2_97D2_00105A3B4608__INCLUDED_)
