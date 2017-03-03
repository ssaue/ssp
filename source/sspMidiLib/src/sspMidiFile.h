// sspMidiFile.h: interface for the sspMidiFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPMIDIFILE_H
#define SSPMIDIFILE_H

#include "sspMidiFileObjects.h"

class sspMidiFile
{
protected:
  enum {METABYTE=0XFF, TITLEBYTE=0X03, ENDBYTE=0X2F, TEMPOBYTE=0X51, TIMESIGNBYTE=0X58, STATUSBYTE=0x80};

	static const short  s_dataLength[];
	static const char*  s_headerName;
	static const char*  s_trackName;

	static void reorder(char* pBytes, int nAnt);
};

inline void sspMidiFile::reorder(char* pBytes, int nAnt)
{
	char temp;
	for (int i=0; i<(nAnt/2) ; i++) {
		temp = pBytes[i];
		pBytes[i] = pBytes[nAnt-(i+1)];
		pBytes[nAnt-(i+1)] = temp;
	}
}

#endif
