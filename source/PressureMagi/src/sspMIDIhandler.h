// sspMIDIhandler.h: interface for the MIDI input classes.
//
//////////////////////////////////////////////////////////////////////

#ifndef sspMIDIhandler_H
#define sspMIDIhandler_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <sspMidiDevice.h>
#include <sspSynch.h>

class sspMIDIhandler : public sspMidiInHandler
{
protected:
  sspMidiInDevice* m_pInDevice;
  sspMidiOutDevice* m_pOutDevice;

  BYTE m_nNoteNum;

  sspMidiMessage m_inputMessage;
  sspSynchObject m_sync;

public:
	sspMIDIhandler();
	sspMIDIhandler(const sspMIDIhandler& inp);
	sspMIDIhandler& operator= (const sspMIDIhandler& inp);
	virtual ~sspMIDIhandler();

	bool initialize(int nInputDev, int nOutputDev, BYTE nNoteNum);
  bool begin();
  bool end();
	void terminate();

  BYTE getInputValue() {
    sspAutoLock lock(m_sync);
    return sspMidi::getData2(m_inputMessage);
  }

  void setOutputValue(BYTE data1, BYTE data2);
  void handleMsg(sspMidiMessage msg);
};

#endif
