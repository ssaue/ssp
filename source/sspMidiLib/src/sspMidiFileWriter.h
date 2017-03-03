// sspMidiFileWriter.h: interface for the sspMidiFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPMIDIFILEWRITER_H
#define SSPMIDIFILEWRITER_H

#include "sspMidiFile.h"
#include <fstream>

class sspMidiFileWriter : public sspMidiFile
{
private:
	std::ofstream           m_file;
    std::ofstream::pos_type m_nLengthPos;

    int           m_nTrackSize;
	BYTE          m_runningStatus;

public:
  enum {ERRMSG_OK, ERRMSG_FILEOPEN, ERRMSG_SONGERROR, ERRMSG_TYPEERROR};

  sspMidiFileWriter();
  virtual ~sspMidiFileWriter();

	int open(const char* pFileName);
	int write(const sspMidiFileSong& song, short nType);
  void close();

private:
	void writeHeader(const sspMidiFileSong& song, short nType);
  void writeDivision(const sspMidiFileTime& time);
  void writeInfoTrack(const sspMidiFileSong& song, short nType);
  void writeTrack(const sspMidiFileTrack& track, short nType, double dTimeFac);

  void writeStartOfTrack();
  void writeEndOfTrack();

  void writeTitle(const std::string& str);
  void writeTempo(int nTempo);
  void writeTimesign(int nNum, int nDenom);
  void writeMidiEvent(const sspMidiEvent& event, double dTimeFac);

	void writeMSB(char* pBytes, unsigned int nAnt);
	int writeVarInt(unsigned int nValue);
};

#endif
