// sspMIDIInput.h: interface for the MIDI input classes.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPMIDIINPUT_H
#define SSPMIDIINPUT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspInput.h"
#include <sspMidiDevice.h>

class SSpBoolean;

class sspMIDIinput : public sspInput, public sspMidiInHandler
{
protected:
	int	m_nMidiDevice;

public:
	sspMIDIinput(short nType);
	sspMIDIinput(const sspMIDIinput& inp);
	sspMIDIinput& operator= (const sspMIDIinput& inp);
	virtual ~sspMIDIinput() {}
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	bool initialize(LPVOID pWnd);
	bool end() {return true;}
	bool terminate() {return true;}

	void setMidiDevice(int nDevice) {m_nMidiDevice = nDevice;}
	int	 getMidiDevice() const {return m_nMidiDevice;}
};

// sspMIDIeventInput
// Checks if a specified event has occured since the previous update

class sspMIDIeventInput : public sspMIDIinput
{
public:
	enum {SSP_MI_ANY_MSG, SSP_MI_CONTROL_MSG, SSP_MI_NOTEON_MSG, SSP_MI_TYPES};

protected:
    DECLARE_SERIAL(sspMIDIeventInput)

	int	m_nMsgType;
	int	m_nMsgChannel[2];	// Ranges delimiting the event, m_n...[0] = -1 indicates no limit
	int m_nMsgData1[2];
	int m_nMsgData2[2];

	bool		m_bEvent;
	int			m_nConditional;
	SSpBoolean*	m_pConditional;

public:
	sspMIDIeventInput();
	sspMIDIeventInput(const sspMIDIeventInput& inp);
	sspMIDIeventInput& operator= (const sspMIDIeventInput& inp);
	virtual ~sspMIDIeventInput();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	bool initialize(LPVOID pWnd);
	bool begin();
	bool update();

	void handleMsg(sspMidiMessage msg);

	void setMsgType(int nType) {m_nMsgType = nType < SSP_MI_TYPES ? nType : 0;}
	void setMsgChannel(int nLow, int nHigh) {m_nMsgChannel[0] = nLow; m_nMsgChannel[1] = nHigh;}
	void setMsgData1(int nLow, int nHigh) {m_nMsgData1[0] = nLow; m_nMsgData1[1] = nHigh;}
	void setMsgData2(int nLow, int nHigh) {m_nMsgData2[0] = nLow; m_nMsgData2[1] = nHigh;}
	void setConditional(int nCond) {m_nConditional = nCond;}

	int getMsgType() const {return m_nMsgType;}
	void getMsgChannel(int& nLow, int& nHigh) const {nLow = m_nMsgChannel[0]; nHigh = m_nMsgChannel[1];}
	void getMsgData1(int& nLow, int& nHigh) const {nLow = m_nMsgData1[0]; nHigh = m_nMsgData1[1];}
	void getMsgData2(int& nLow, int& nHigh) const {nLow = m_nMsgData2[0]; nHigh = m_nMsgData2[1];}
	int getConditional() const {return m_nConditional;}
};

#endif
