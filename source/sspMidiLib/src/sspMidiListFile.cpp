// sspMidiListFile.cpp: implementation of the sspMidiFile classes.
//
//////////////////////////////////////////////////////////////////////

#include "sspMidiListFile.h"

using namespace std;

bool sspMidiListFile::open(const wchar_t* pFileName)
{
	m_file.open(pFileName);
  return !m_file.fail();
}

bool sspMidiListFile::read(sspMidiSequence& seq)
{
	int nCount;
	m_file >> nCount;
	if (m_file.eof() || m_file.fail()) {
		return false;
	}
	double dTime;
  int nStatus, nData1, nData2;

  seq.clear();
	for (;nCount > 0; nCount--) {
		m_file >> dTime >> nStatus >> nData1 >> nData2;
		if (m_file.eof()) break;

    sspMidiEvent event;
    event.message = sspMidi::createMessage(nStatus, nData1, nData2);
    event.timestamp = (sspMidiTimestamp) dTime;
    seq.push_back(event);
	}
  return (!seq.empty() && nCount == 0);
}

void sspMidiListFile::close()
{
	m_file.close();
}
