// sspMidiFileReader.cpp: implementation of the sspMidiFileReader classes.
//
//////////////////////////////////////////////////////////////////////

#include "sspMidiFileReader.h"

using namespace std;

sspMidiFileReader::sspMidiFileReader()
: m_runningStatus(0)
{}

sspMidiFileReader::~sspMidiFileReader()
{
  close();
}

inline void sspMidiFileReader::readMSB(char* pBytes, unsigned int nAnt)
{
	m_file.read(pBytes, nAnt);
	reorder(pBytes, nAnt);
}

inline unsigned int sspMidiFileReader::readVarInt()
{
	BYTE buffer = 0;
	unsigned int nVal = 0;
	do {
		m_file.read((char*) &buffer, 1);
		nVal = (nVal << 7) + (buffer & 0x7f);
	}
	while (buffer & 0x80);
	return nVal;
}

int sspMidiFileReader::open(const wchar_t* pFileName)
{
	m_file.open(pFileName, ios_base::binary | ios_base::in);
  if (m_file.fail()) 
    return ERRMSG_FILEOPEN;

  // Verify that it is a MIDI file
	char text[5] = "";
  m_file.read(text, 4);
	if ((strcmp(text, s_headerName)) != 0)
    return ERRMSG_MIDIFILE;

  return ERRMSG_OK;
}

int sspMidiFileReader::read(sspMidiFileSong& song)
{
  int nTracks = 0;
  int nErr = ERRMSG_OK;
  int nType = readHeader(song, nTracks);
  switch (nType) {
    case 0: nErr = readFormat0(song); break;
    case 1: nErr = readFormat1(song, nTracks); break;
    default: nErr = ERRMSG_FILEFORMAT; break;
  }
  return nErr;
}

void sspMidiFileReader::close()
{
  m_file.close();
}

int sspMidiFileReader::readHeader(sspMidiFileSong& song, int& nTracks)
{
  int nType = 0;
  m_file.seekg(8, ios_base::beg);
	readMSB((char*) &nType, 2);
	readMSB((char*) &nTracks, 2);

  // Read time format (MIDI timebase or SMPTE)
  union tBase {short nBase; char nBytes[2];} base;
  m_file.read((char*) &base.nBase, 2);
  if (base.nBytes[0] >= 0) {  // MIDI timebase
    reorder((char*) &base.nBase, 2);
    song.getFileTime().setTimeBase(base.nBase);
  }
  else {
    song.getFileTime().setSMPTEBase(-base.nBytes[0], base.nBytes[1]);
  }

  return nType;
}

int sspMidiFileReader::readFormat0(sspMidiFileSong& song)
{
  return readTrack(song);
}

int sspMidiFileReader::readFormat1(sspMidiFileSong& song, int nTracks)
{
	for (int i=0; i<nTracks; ++i) {
    int nErr = readTrack(song);
    if (nErr != ERRMSG_OK)
      return nErr;
	}
  return ERRMSG_OK;
}

int sspMidiFileReader::readTrack(sspMidiFileSong& song)
{
	char text[5] = "";
  m_file.read(text, 4);
	if ((strcmp(text, s_trackName)) != 0)
    return ERRMSG_MIDITRACK;
  m_file.seekg(4, ios_base::cur);

  m_runningStatus = 0;  // Reset running status

  sspMidiFileTrack track;
	for(;;) {
		unsigned int nTime = readVarInt();
		BYTE buffer = 0;
    m_file.read((char*)&buffer, 1);

    if (m_file.fail()) 
      return ERRMSG_FILEEND;
		else if (buffer == METABYTE) {
      if (readMetaEvent(song, track) == ENDBYTE) {
        if (track.empty()) {
          if (song.empty() && !track.getTitle().empty())
            song.setTitle(track.getTitle());
        }
        else {
          song.push_back(track);
        }
        return ERRMSG_OK;
      }
		}
    else if (buffer == sspMidi::SYSEX || buffer == sspMidi::EO_SYSEX) {	// System Exclusive is neglected
			unsigned int nLength = readVarInt();
      m_file.seekg(nLength, ios_base::cur);
		}
		else {
      track.push_back(readMidiEvent((sspMidiTimestamp) (nTime * song.getFileTime().getTimeFactor() + 0.5), buffer));
		}
	}
}

BYTE sspMidiFileReader::readMetaEvent(sspMidiFileSong& song, sspMidiFileTrack& track)
{
	BYTE metaEvent = 0, buffer = 0;
  m_file.read((char*) &metaEvent, 1);
  switch (metaEvent) {
	case TEMPOBYTE:
		{
			long tempo = 0;
      m_file.seekg(1, ios_base::cur);
			readMSB((char*) &tempo, 3);
      song.getFileTime().setTempo(tempo);
			break;
		}
	case ENDBYTE:
		{
      m_file.seekg(1, ios_base::cur);
			break;
		}
	case TITLEBYTE:
		{
			unsigned int nLength = readVarInt();
			char* txt = new char[nLength+1];
			m_file.read(txt, nLength);
			txt[nLength] = 0;
      track.setTitle(txt);
			delete[] txt;
			break;
		}
	case TIMESIGNBYTE:
		{
      BYTE nNum = 0, nDenom = 0;
      m_file.seekg(1, ios_base::cur);
      m_file.read((char*) &nNum, 1);
      m_file.read((char*) &nDenom, 1);
      m_file.seekg(2, ios_base::cur);
      song.setTimeSignature(nNum, (1 << nDenom));
			break;
		}
	default:
		{
			unsigned int nLength = readVarInt();
      m_file.seekg(nLength, ios_base::cur);
			break;
		}
	}
	return metaEvent;
}

sspMidiEvent sspMidiFileReader::readMidiEvent(sspMidiTimestamp trueTime, const BYTE status)
{
  BYTE buffer = status;
  sspMidiEvent event = {0,0};

  event.timestamp = trueTime;
  bool bNewStatus = (status >= STATUSBYTE);
  if (bNewStatus) m_runningStatus = status;
	short nCount = s_dataLength[m_runningStatus - STATUSBYTE];

  sspMidi::setStatus(event.message, m_runningStatus);
  if (nCount > 1) {
    if (bNewStatus) m_file.read((char*) &buffer, 1);
    sspMidi::setData1(event.message, buffer);
  }
  if (nCount > 2) {
    m_file.read((char*) &buffer, 1);
    sspMidi::setData2(event.message, buffer);
  }

	return event;
}
