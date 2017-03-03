// sspMidiFileReader.h: interface for the sspMidiFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPMIDIFILEREADER_H
#define SSPMIDIFILEREADER_H

#include "sspMidiFile.h"
#include <fstream>

class sspMidiFileReader : public sspMidiFile
{
private:
	std::ifstream m_file;
	BYTE          m_runningStatus;

public:
  enum {ERRMSG_OK, ERRMSG_FILEOPEN, ERRMSG_MIDIFILE, ERRMSG_FILEFORMAT, ERRMSG_MIDITRACK, ERRMSG_FILEEND};

  sspMidiFileReader();
  virtual ~sspMidiFileReader();

	int open(const wchar_t* pFileName);
	int read(sspMidiFileSong& song);
  void close();

private:
	int readHeader(sspMidiFileSong& song, int& nTracks);
	int readFormat0(sspMidiFileSong& song);
	int readFormat1(sspMidiFileSong& song, int nTracks);
	int readTrack(sspMidiFileSong& song);

	BYTE readMetaEvent(sspMidiFileSong& song, sspMidiFileTrack& track);
	sspMidiEvent readMidiEvent(sspMidiTimestamp trueTime, const BYTE status);

	void readMSB(char* pBytes, unsigned int nAnt);
	unsigned int readVarInt();
};

#endif
