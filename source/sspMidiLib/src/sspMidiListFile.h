// sspMidiListFile.h: interface for the sspMidiFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPMIDILISTFILE_H
#define SSPMIDILISTFILE_H

#include "sspMidi.h"
#include <fstream>

class sspMidiListFile  
{
private:
	std::ifstream m_file;

public:
  sspMidiListFile() {}
  virtual ~sspMidiListFile() {}

	bool open(const wchar_t* pFileName);
	bool read(sspMidiSequence& seq);
	void close();
};

#endif
