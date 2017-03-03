// sspMIDIhandler.cpp: implementation of the sspMIDIhandler classes.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "soundspace.h"
#include "sspMIDIhandler.h"
//#include "SSpConditionals.h"
//#include "sspPools.h"
//#include "sspDeviceManager.h"
//#include <sspMidi.h>
//#include "sspDeviceGroups.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// sspMIDIeventInput
//////////////////////////////////////////////////////////////////////

sspMIDIhandler::sspMIDIhandler()
: m_pInDevice(NULL)
, m_pOutDevice(NULL)
, m_nNoteNum(0)
, m_inputMessage(0)
{
}

sspMIDIhandler::sspMIDIhandler(const sspMIDIhandler& inp)
{
}

sspMIDIhandler& sspMIDIhandler::operator= (const sspMIDIhandler& inp)
{
	return *this;
}

sspMIDIhandler::~sspMIDIhandler()
{
  terminate();
}

bool sspMIDIhandler::initialize(int nInputDev, int nOutputDev, BYTE nNoteNum)
{
	sspMidiInDevice* pInDev = new sspMidiInDevice;
  if (pInDev->open(nInputDev)) {
    m_pInDevice = pInDev;
    m_pInDevice->addHandler(this);
  }
  else {
    delete pInDev;
    m_pInDevice = NULL;
    return false;
  }

	sspMidiOutDevice* pOutDev = new sspMidiOutDevice;
  if (pOutDev->open(nOutputDev)) {
    m_pOutDevice = pOutDev;
  }
  else {
    delete pOutDev;
    m_pOutDevice = NULL;
    return false;
  }

  m_nNoteNum = nNoteNum;

  return true;
}

bool sspMIDIhandler::begin()
{
  return (m_pInDevice && m_pInDevice->startRecord());
}

bool sspMIDIhandler::end() 
{
  sspAutoLock lock(m_sync);
  return (m_pInDevice && m_pInDevice->stopRecord());
}

void sspMIDIhandler::terminate() 
{
  delete m_pInDevice;
  m_pInDevice = NULL;
  delete m_pOutDevice;
  m_pOutDevice = NULL;
}

void sspMIDIhandler::setOutputValue(BYTE data1, BYTE data2)
{
  if (m_pOutDevice != NULL) {
    sspAutoLock lock(m_sync);
    int nStatus = sspMidi::getStatus(m_inputMessage);
    m_pOutDevice->sendMidi(sspMidi::createMessage(nStatus ? nStatus : sspMidi::NOTEON, data1, data2));
  }
}

void sspMIDIhandler::handleMsg(sspMidiMessage msg)
{
  sspAutoLock lock(m_sync);
  if (sspMidi::getData1(msg) == m_nNoteNum) {
    if (sspMidi::getType(msg) == sspMidi::NOTEON)
      m_inputMessage = msg;
    else if (sspMidi::getType(msg) == sspMidi::NOTEOFF)
      sspMidi::setData2(m_inputMessage, 0);
  }
  else if (m_pOutDevice != NULL) {
    m_pOutDevice->sendMidi(msg);
  }
}
