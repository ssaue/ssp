// sspMidiFileWriter.cpp: implementation of the sspMidiFileWriter classes.
//
//////////////////////////////////////////////////////////////////////

#include "sspMidiFileWriter.h"

using namespace std;

sspMidiFileWriter::sspMidiFileWriter() 
: m_nTrackSize(0)
, m_nLengthPos(0) 
, m_runningStatus(0)
{}

sspMidiFileWriter::~sspMidiFileWriter() 
{
  close();
}

inline void sspMidiFileWriter::writeMSB(char* pBytes, unsigned int nAnt)
{
	reorder(pBytes, nAnt);
	m_file.write((const char*) pBytes, nAnt);
}

inline int sspMidiFileWriter::writeVarInt(unsigned int nValue)
{
	unsigned int nBuffer = nValue & 0x7F;
	while ((nValue >>= 7) > 0) {
		nBuffer <<= 8;
		nBuffer |= 0x80;
		nBuffer += (nValue & 0x7F);
	}
	int nReturn = 0;
	for (;;) {
		m_file.write((const char*) &nBuffer, 1);
		nReturn++;
		if (nBuffer & 0x80)
			nBuffer >>= 8;
		else
			break;
	}
	return nReturn;
}

int sspMidiFileWriter::open(const char* pFileName)
{
  m_file.open(pFileName, ios_base::binary | ios_base::out);
  if (m_file.fail()) 
    return ERRMSG_FILEOPEN;

  return ERRMSG_OK;
}

int sspMidiFileWriter::write(const sspMidiFileSong& song, short nType)
{
  if (song.empty())
		return ERRMSG_SONGERROR;

  if (nType > 1 || (nType == 0 && song.size() > 1))
    return ERRMSG_TYPEERROR;

	writeHeader(song, nType);
	writeInfoTrack(song, nType);

  sspMidiFileSong::const_iterator iter;
  for (iter = song.begin(); iter != song.end(); ++iter)
    writeTrack(*iter, nType, (1.0 / song.getFileTime().getTimeFactor()));

  return ERRMSG_OK;
}

void sspMidiFileWriter::close()
{
  m_file.close();
}

void sspMidiFileWriter::writeHeader(const sspMidiFileSong& song, short nType)
{
	m_file.write(s_headerName, 4);
	int nBuf4 = 6;
	writeMSB((char*) &nBuf4, 4);
	writeMSB((char*) &nType, 2);
  short nBuf2 = (nType == 0) ? 1 : (short) (song.size() + 1);
	writeMSB((char*) &nBuf2, 2);
  writeDivision(song.getFileTime());
}

void sspMidiFileWriter::writeDivision(const sspMidiFileTime& time)
{
  if (time.isSMPTE()) {
    char nByte = (char) time.getFrames();
    nByte *= -1;
    m_file.write((const char*) &nByte, 1);
    nByte = (char) time.getSubFrames();
    m_file.write((const char*) &nByte, 1);
  }
  else {
    short nBuf = (short) time.getTimeBase();
  	writeMSB((char*) &nBuf, 2);
  }
}

void sspMidiFileWriter::writeInfoTrack(const sspMidiFileSong& song, short nType)
{
  writeStartOfTrack();

  writeTitle(song.getTitle());
  writeTempo(song.getFileTime().getTempo());
  int nNum = 4, nDenom = 4;
  song.getTimeSignature(nNum, nDenom);
  writeTimesign(nNum, nDenom);

  if (nType == 1)
    writeEndOfTrack();
}

void sspMidiFileWriter::writeTrack(const sspMidiFileTrack& track, short nType, double dTimeFac)
{
  if (nType == 1) {
    writeStartOfTrack();
    writeTitle(track.getTitle());
  }

  m_runningStatus = 0;  // Reset running status
  sspMidiFileTrack::const_iterator iter;
  for (iter = track.begin(); iter != track.end(); ++iter)
    writeMidiEvent(*iter, dTimeFac);

  writeEndOfTrack();
}

void sspMidiFileWriter::writeStartOfTrack()
{
  m_file.write(s_trackName, 4);
  m_nLengthPos = m_file.tellp();
	m_nTrackSize = 0;
	m_file.write((const char*) &m_nTrackSize, 4);
}

void sspMidiFileWriter::writeEndOfTrack()
{
	BYTE nByte = 0;
    m_file.write((const char*) &nByte, 1);
    nByte = METABYTE;
    m_file.write((const char*) &nByte, 1);
    nByte = ENDBYTE;
    m_file.write((const char*) &nByte, 1);
    nByte = 0;
    m_file.write((const char*) &nByte, 1);
    m_nTrackSize += 4;

    // Insert the true track size
    ofstream::pos_type endPos = m_file.tellp();
    m_file.seekp(m_nLengthPos, ios_base::beg);
    writeMSB((char*) &m_nTrackSize, 4);
    m_file.seekp(endPos, ios_base::beg);
}

void sspMidiFileWriter::writeTitle(const std::string& str)
{
  if (str.empty()) return;

	BYTE nByte = 0;
  m_file.write((const char*) &nByte, 1);
	nByte = METABYTE;
  m_file.write((const char*) &nByte, 1);
	nByte = TITLEBYTE;
  m_file.write((const char*) &nByte, 1);
  nByte = (BYTE) str.size();
  m_file.write((const char*) &nByte, 1);
  m_file.write(str.c_str(), nByte);
  m_nTrackSize += (nByte + 4);
}

void sspMidiFileWriter::writeTempo(int nTempo)
{
	BYTE nByte = 0;
  m_file.write((const char*) &nByte, 1);
	nByte = METABYTE;
  m_file.write((const char*) &nByte, 1);
	nByte = TEMPOBYTE;
  m_file.write((const char*) &nByte, 1);
  nByte = 3;
  m_file.write((const char*) &nByte, 1);
  if (nTempo <= 0) nTempo = sspMidiFileTime::DEFAULT_TEMPO;
  writeMSB((char*) &nTempo, 3);
  m_nTrackSize += 7;
}

void sspMidiFileWriter::writeTimesign(int nNum, int nDenom)
{
	BYTE nByte = 0;
  m_file.write((const char*) &nByte, 1);
	nByte = METABYTE;
  m_file.write((const char*) &nByte, 1);
	nByte = TIMESIGNBYTE;
  m_file.write((const char*) &nByte, 1);
	nByte = 4;
  m_file.write((const char*) &nByte, 1);
	nByte = nNum;
  m_file.write((const char*) &nByte, 1);
	for (nByte=0;nByte<16;nByte++) {
		if ((1<<nByte) >= nDenom) break;
	}
  m_file.write((const char*) &nByte, 1);
	nByte = 24;
  m_file.write((const char*) &nByte, 1);
	nByte = 8;
  m_file.write((const char*) &nByte, 1);
  m_nTrackSize += 8;
}

void sspMidiFileWriter::writeMidiEvent(const sspMidiEvent& event, double dTimeFac)
{
  m_nTrackSize += writeVarInt((unsigned int) (event.timestamp * dTimeFac + 0.5));
  BYTE nBuf = sspMidi::getStatus(event.message);
	if (nBuf != m_runningStatus) {
		m_runningStatus = nBuf;
		m_file.write((const char*) &nBuf, 1);
		m_nTrackSize++;
	}
  int nCount = s_dataLength[m_runningStatus - STATUSBYTE];
  if (nCount > 1) {
    nBuf = sspMidi::getData1(event.message);
		m_file.write((const char*) &nBuf, 1);
		m_nTrackSize++;
	}
  if (nCount > 2) {
    nBuf = sspMidi::getData2(event.message);
		m_file.write((const char*) &nBuf, 1);
		m_nTrackSize++;
	}
}
