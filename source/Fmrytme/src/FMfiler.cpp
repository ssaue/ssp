// FMfiler.cpp: implementation of the CMidiFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fmrytme.h"
#include "math.h"
#include "FMfiler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Class CMidiFile
//////////////////////////////////////////////////////////////////////

const int CMidiFile::dataLengde[] = {3, 3, 3, 3, 2, 2, 3, 1};
const char* CMidiFile::headerName = "MThd";
const char* CMidiFile::trackName = "MTrk";

CMidiFile::CMidiFile()
{
	m_nType = 0;
	m_nSpor = 0;
	m_nTidsformat = 0;
	m_nRunStatus = 0;
	m_pSang = NULL;
}

CMidiFile::~CMidiFile()
{
}

BOOL CMidiFile::ReadOpen(LPCTSTR pFilnavn)
{
	if (!CFile::Open(pFilnavn,CFile::modeRead | CFile::shareDenyWrite)) {
		AfxMessageBox(IDS_IMPORTMIDI);
		return FALSE;
	}
	char tekst[5] = "";
	Read(tekst,4);
	if ((strcmp(tekst, headerName)) != 0) {
		AfxMessageBox(IDS_MIDIFIL);
		return FALSE;
	}
	Seek(4, CFile::current);
	ReadMSB(&m_nType, 2);
	if (m_nType != 0 && m_nType != 1) {
		AfxMessageBox(IDS_MIDIFILFORMAT);
		return FALSE;
	}
	if (m_nType == 0) {
		Seek(2, CFile::current);
		m_nSpor = 1;
	}
	else {
		ReadMSB(&m_nSpor, 2);
	}
	ReadMSB(&m_nTidsformat, 2);
	if (m_nTidsformat < 0) {
		AfxMessageBox(IDS_MIDITIDFORMAT);
		return FALSE;
	}	
	return TRUE;
}

BOOL CMidiFile::ReadData(void)
{
	m_pSang = new CMidiSang(m_nSpor);
	m_pSang->GetMidiTime()->SetTidsformat(m_nTidsformat);
	if (m_nType == 0)
		ReadFormat0();
	else
		ReadFormat1();
	if (m_pSang->GetSpor() > 0) {
		return TRUE;
		if (m_pSang->GetSpor() > CMidiDevice::MAX_CHANNELS) {
			AfxMessageBox(IDS_SPORANTALL);
			m_pSang->SetSpor(CMidiDevice::MAX_CHANNELS);
		}
	}
	else {
		AfxMessageBox(IDS_MIDIFILTOM);
		return FALSE;
	}
}

BOOL CMidiFile::WriteOpen(LPCTSTR pFilnavn)
{
	if (!CFile::Open(pFilnavn,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyRead)) {
		AfxMessageBox(IDS_IMPORTMIDI);
		return FALSE;
	}
	else
		return TRUE;
}

BOOL CMidiFile::WriteData(void)
{
	if (m_pSang == NULL || m_pSang->GetSpor() < 1) {
		AfxMessageBox(IDS_NO_SONG);
		return FALSE;
	}
	WriteHeader();
	WriteInfoTrack();
	for (int i=0; i<m_pSang->GetSpor(); i++)
		WriteTrack(i);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Private members

void CMidiFile::ReadFormat0()
{
	CMidiSekvens* pSekv = ReadSpor();
	if (pSekv == NULL) {
		AfxMessageBox(IDS_MIDISPOR);
		m_pSang->SetSpor(0);
	}
	else if (pSekv->GetAntall() == 0) {
		delete pSekv;
		m_pSang->SetSpor(0);
	}
	else {
		m_pSang->SetTittel(pSekv->GetTittel());
		CMidiSekvens* pDelSekv;
		int sporNr = 0;
		do {
			pSekv->SetHeaderInfo();			// Kanalinfo må settes før splitt
			pDelSekv = pSekv->SplittSekvens();
			pSekv->SetHeaderInfo();			// Programinfo må settes etter splitt
			m_pSang->LeggTilSekvens(pSekv, sporNr);
			sporNr++;
			pSekv = pDelSekv;
		}
		while (pSekv->GetAntall() > 0);
		delete pSekv;
		m_pSang->SetSpor(sporNr);
	}
}

void CMidiFile::ReadFormat1()
{
	BOOL bTittel = FALSE;
	int sporNr = 0;
	for (int i=0; i<m_nSpor; i++) {
		CMidiSekvens* pSekv = ReadSpor();
		if (pSekv == NULL) {
			AfxMessageBox(IDS_MIDISPOR);
			break;
		}
		else if (pSekv->GetAntall() == 0) {
			CString sTittel = pSekv->GetTittel();
			if (!bTittel && (sTittel.GetLength() > 1)) {
				m_pSang->SetTittel(sTittel);
				bTittel = TRUE;
			}
			delete pSekv;
		}
		else {
			pSekv->SetHeaderInfo();
			m_pSang->SetSekvens(pSekv, sporNr);
			sporNr++;
		}
	}
	m_pSang->SetSpor(sporNr);
}

CMidiSekvens* CMidiFile::ReadSpor()
{
	char tekst[5] = "";
	Read(tekst,4);
	if ((strcmp(tekst, trackName)) != 0)
		return NULL;
	CMidiSekvens* sekvens = new CMidiSekvens();
	Seek(4, CFile::current);
	for(;;) {
		unsigned int tid = ReadVarInt();
		BYTE buffer = 0;
		if (Read(&buffer, 1) == 0) {	// Slutten av fila, feil ved lesing av sporet!
			AfxMessageBox(IDS_MIDISLUTT);
			return sekvens;
		}
		if (buffer == METABYTE) {
			if (ReadMetaEvent(sekvens) == SLUTTBYTE) {
				return sekvens;
			}
		}
		else if (buffer == SYSEX1 || buffer == SYSEX2) {	// Leser ikke System Exclusive
			unsigned int lengde = ReadVarInt();
			Seek(lengde, CFile::current);
		}
		else {
			sekvens->LeggBak(ReadMidiEvent(tid, buffer));
		}
	}
}	

unsigned int CMidiFile::ReadVarInt()
{
	BYTE buffer = 0;
	unsigned int verdi = 0;
	do {
		Read(&buffer, 1);
		verdi = (verdi << 7) + (buffer & 0x7f);
	}
	while (buffer & 0x80);
	return verdi;
}

BYTE CMidiFile::ReadMetaEvent(CMidiSekvens* sekvens)
{
	BYTE buffer = 0;
	Read(&buffer, 1);
	switch (buffer)
	{
	case TEMPOBYTE:
		{
			Seek(1,CFile::current);
			long tempo = 0;
			ReadMSB(&tempo, 3);
			m_pSang->GetMidiTime()->SetTempoEvent(tempo);
			break;
		}
	case SLUTTBYTE:
		{
			Seek(1, CFile::current);
			break;
		}
	case TITTELBYTE:
		{
			unsigned int lengde = ReadVarInt();
			char* tekst = new char[lengde+1];
			tekst[lengde] = 0;	// Null-terminering
			Read(tekst, lengde);
			CString tittel = tekst;
			sekvens->SetTittel(tittel);
			delete[] tekst;
			break;
		}
	case TAKTARTBYTE:
		{
			Seek(1, CFile::current);
			BYTE teller = 0;
			Read(&teller, 1);
			BYTE nevner = 0;
			Read(&nevner, 1);
			m_pSang->SetTaktart(teller, (1 << nevner));
			Seek(2, CFile::current);
			break;
		}
	default:
		{
			unsigned int lengde = ReadVarInt();
			Seek(lengde, CFile::current);
			break;
		}
	}
	return buffer;
}

CMidiTidEvent* CMidiFile::ReadMidiEvent(unsigned int tid, BYTE status)
{
	CMidiTidEvent* event = new CMidiTidEvent;
	event->SetTid((double) tid);
	int i = 1;
	if (status >= STATUSBYTE)
		m_nRunStatus = status;
	else
		event->SetEventData(i++, status);
	int antall = dataLengde[(m_nRunStatus & 0x70) >> 4];
	event->SetEventData(0, m_nRunStatus);
	for (;i < antall; i++) {
		BYTE buffer = 0;
		Read(&buffer, 1);
		event->SetEventData(i, buffer);
	}
	return event;
}

UINT CMidiFile::ReadMSB(void* pBytes, UINT nAnt)
{
	UINT ret = CFile::Read(pBytes, nAnt);
	Reorder((BYTE*) pBytes, nAnt);
	return ret;
}

void CMidiFile::WriteHeader()
{
	Write(headerName, 4);
	_int32 nBuf4 = 6;
	WriteMSB(&nBuf4, 4);
	short nBuf2 = 1;
	WriteMSB(&nBuf2, 2);
	nBuf2 = (short) m_pSang->GetSpor() + 1;
	WriteMSB(&nBuf2, 2);
	nBuf2 = WRITE_FORMAT;
	WriteMSB(&nBuf2, 2);
}

void CMidiFile::WriteInfoTrack()
{
	Write(trackName, 4);
	LONGLONG lengthPos = GetPosition();
	_int32 trackLength = 0;
	Write(&trackLength, 4);
	trackLength += WriteMetaEvent(TITTELBYTE, -1);
	trackLength += WriteMetaEvent(TEMPOBYTE, -1);
	trackLength += WriteMetaEvent(TAKTARTBYTE, -1);
	trackLength += WriteMetaEvent(SLUTTBYTE, -1);
	LONGLONG endPos = GetPosition();
	Seek(lengthPos, CFile::begin);
	WriteMSB(&trackLength, 4);
	Seek(endPos, CFile::begin);
}

void CMidiFile::WriteTrack(int nTrack)
{
	CFile::Write(trackName, 4);
	LONGLONG lengthPos = GetPosition();
	_int32 trackLength = 0;
	Write(&trackLength, 4);
	trackLength += WriteMetaEvent(TITTELBYTE, nTrack);
	CMidiSekvens* pSek = m_pSang->GetSekvens(nTrack);
	m_nRunStatus = 0;
	pSek->InitPos();
	for (;;) {
		CMidiTidEvent* pEvent = pSek->GetNeste();
		if (pEvent == NULL)
			break;
		else
			trackLength += WriteMidiEvent(pEvent);
	}
	trackLength += WriteMetaEvent(SLUTTBYTE, nTrack);
	LONGLONG endPos = GetPosition();
	Seek(lengthPos, CFile::begin);
	WriteMSB(&trackLength, 4);
	Seek(endPos, CFile::begin);
}

inline int CMidiFile::WriteVarInt(unsigned int nValue)
{
	unsigned int nBuffer = nValue & 0x7F;
	while ((nValue >>= 7) > 0) {
		nBuffer <<= 8;
		nBuffer |= 0x80;
		nBuffer += (nValue & 0x7F);
	}
	int nReturn = 0;
	for (;;) {
		Write(&nBuffer, 1);
		nReturn++;
		if (nBuffer & 0x80)
			nBuffer >>= 8;
		else
			break;
	}
	return nReturn;
}

int CMidiFile::WriteMetaEvent(BYTE event, int nTrack)
{
	BYTE nByte = 0;
	switch (event)
	{
	case TEMPOBYTE:
		{
			Write(&nByte, 1);
			nByte = METABYTE;
			Write(&nByte, 1);
			Write(&event, 1);
			nByte = 3;
			Write(&nByte, 1);
			long tempo = 60000000 / m_pSang->GetMidiTime()->GetMetronom();
			WriteMSB(&tempo, 3);
			return 7;
		}
	case SLUTTBYTE:
		{
			Write(&nByte, 1);
			nByte = METABYTE;
			Write(&nByte, 1);
			Write(&event, 1);
			nByte = 0;
			Write(&nByte, 1);
			return 4;
		}
	case TITTELBYTE:
		{
			CString text;
			if (nTrack < 0)
				text = m_pSang->GetTittel();
			else
				text = m_pSang->GetSekvens(nTrack)->GetTittel();
			BYTE nLength = (BYTE) text.GetLength();
			if (nLength > 0) {
				Write(&nByte, 1);
				nByte = METABYTE;
				Write(&nByte, 1);
				Write(&event, 1);
				Write(&nLength, 1);
				Write(text, nLength);
				return nLength + 4;
			}
			else
				return 0;
		}
	case TAKTARTBYTE:
		{
			Write(&nByte, 1);
			nByte = METABYTE;
			Write(&nByte, 1);
			Write(&event, 1);
			nByte = 4;
			Write(&nByte, 1);
			nByte = (BYTE) m_pSang->GetTaktart(CMidiSang::TELLER);
			Write(&nByte, 1);
			int nNevner = m_pSang->GetTaktart(CMidiSang::NEVNER);
			for (nByte=0;nByte<16;nByte++) {
				if ((1<<nByte) == nNevner)
					break;
			}
			Write(&nByte, 1);
			nByte = 24;
			Write(&nByte, 1);
			nByte = 8;
			Write(&nByte, 1);
			return 8;
		}
	default:
		{
			return 0;
		}
	}
}

int CMidiFile::WriteMidiEvent(CMidiTidEvent* pEvent)
{
	int nLength = WriteVarInt((unsigned int) (pEvent->GetTid() * WRITE_FORMAT));
	BYTE buffer = pEvent->GetEventData(0);
	if (buffer != m_nRunStatus) {
		m_nRunStatus = buffer;
		Write(&buffer, 1);
		nLength++;
	}
	int antall = dataLengde[(m_nRunStatus & 0x70) >> 4];
	for (int i=1;i < antall; i++) {
		buffer = pEvent->GetEventData(i);
		Write(&buffer, 1);
		nLength++;
	}
	return nLength;
}

void CMidiFile::WriteMSB(void* pBytes, UINT nAnt)
{
	Reorder((BYTE*) pBytes, nAnt);
	CFile::Write(pBytes, nAnt);
}

inline void CMidiFile::Reorder(BYTE* pBytes, int nAnt)
{
	BYTE temp;
	for (int i=0; i<(nAnt/2) ; i++) {
		temp = pBytes[i];
		pBytes[i] = pBytes[nAnt-(i+1)];
		pBytes[nAnt-(i+1)] = temp;
	}
}

//////////////////////////////////////////////////////////////////////
// CTekstFile Class
//////////////////////////////////////////////////////////////////////

CTekstFile::CTekstFile() : m_fil()
{
	m_pMidiSekv = NULL;
	m_pFMSekv = NULL;
	m_dSlag = 1.0;
	m_nUnderdeling = 1;
	m_nPitsj = 60;
	m_nVelocity = 64;
	m_nAntall = 0;
}

CTekstFile::~CTekstFile()
{
}

BOOL CTekstFile::Open(LPCTSTR pFilnavn)
{
	m_fil.open(pFilnavn, std::ios::in);
	if (m_fil.fail()) {
		AfxMessageBox(IDS_FAILOPEN_TEKST);
		return FALSE;
	}
	return TRUE;
}

BOOL CTekstFile::ReadData(double dLengde, int nTaktLengde, int nKanal)
{
	m_fil >> m_dSlag >> m_nUnderdeling >> m_nPitsj >> m_nVelocity;
	if (m_fil.eof() || m_fil.fail())
		return FALSE;
	m_pMidiSekv = new CMidiSekvens(nKanal);
	BOOL bSluttNote = FALSE;
	CMidiTidEvent* pEvent = NULL;
	while (!m_fil.eof()) {
		if (m_nUnderdeling != 0)
			m_dSlag /= m_nUnderdeling;
		if (m_nVelocity == 0) {		// Dette markerer filslutt og gir en varighet av siste tone
			bSluttNote = TRUE;
			break;
		}
		if (pEvent != NULL) {
			pEvent->SetTid(dLengde * m_dSlag);
			pEvent = new CMidiTidEvent((BYTE) CMidiEvent::NOTEON+nKanal, m_nPitsj, m_nVelocity);
			pEvent->SetTid((1.0 - dLengde) * m_dSlag);
		}
		else {
			pEvent = new CMidiTidEvent((BYTE) CMidiEvent::NOTEON+nKanal, m_nPitsj, m_nVelocity);
			pEvent->SetTid(m_dSlag);
		}
		m_pMidiSekv->LeggBak(pEvent);
		pEvent = new CMidiTidEvent((BYTE) CMidiEvent::NOTEOFF+nKanal, m_nPitsj, m_nVelocity);
		m_pMidiSekv->LeggBak(pEvent);
		m_nAntall++;
		m_fil >> m_dSlag >> m_nUnderdeling >> m_nPitsj >> m_nVelocity;
	}
	if (m_nAntall > 0) {
		pEvent->SetTid(bSluttNote ? m_dSlag : 1.0);
		m_pFMSekv = new CFMSekvens();
		m_pFMSekv->LoadSekvens(m_pMidiSekv, nTaktLengde, 0.0);
		return TRUE;
	}
	else {
		delete m_pMidiSekv;
		m_pMidiSekv = NULL;
		return FALSE;
	}
}

