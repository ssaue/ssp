// sspMidiDevice.h

#ifndef SSP_MIDIDEVICE_H
#define SSP_MIDIDEVICE_H

#include <windows.h>
#include <mmsystem.h>
#include "sspMidi.h"

#include <vector>
#include <string>

/****************************************************************************
** Midi Out Device classes
****************************************************************************/

class sspMidiOutDevice
{
private:
	HMIDIOUT m_MidiHandle;
	MIDIOUTCAPS m_MidiCaps;
	int m_nOpenDevice;

public:
	enum {NO_DEVICE=1111, MAX_CHANNELS=16, MAX_NOTES=32};

	sspMidiOutDevice();
	virtual ~sspMidiOutDevice();

	bool open(int deviceNr);
	bool close();
	bool isOpen() {return (m_nOpenDevice != NO_DEVICE);}
	bool reset() const;

	int  getVoices() const;
	int  getNotes() const;
	bool channelSupport(int nChannel) const {return (m_MidiCaps.wChannelMask & (0x0001 << nChannel)) != 0;}
	void sendMidi(sspMidiMessage dBytes) const {midiOutShortMsg(m_MidiHandle, dBytes);}

	static UINT getNumDevices() {return midiOutGetNumDevs();}
	static std::wstring getDeviceName(int nDevice);
};

/****************************************************************************
** Midi In Device classes
****************************************************************************/

class sspMidiInHandler
{
public:
	virtual void handleMsg(sspMidiMessage msg) = 0;
};

typedef std::vector<sspMidiInHandler*> sspMidiHandlerVec;

class sspMidiInDevice
{
private:
	HMIDIIN m_MidiHandle;
	int m_nOpenDevice;
	sspMidiHandlerVec	m_inputHandlers;

	static void CALLBACK msgCallback(HMIDIIN handle, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	void handleMsg(HMIDIIN handle, unsigned int wMsg, sspMidiMessage msg);

public:
	enum {NO_DEVICE=1111};

	sspMidiInDevice();
	virtual ~sspMidiInDevice();

	bool open(int deviceNr);
	bool close();
	bool isOpen() {return (m_nOpenDevice != NO_DEVICE);}
	bool reset() const;

	bool startRecord();
	bool stopRecord();

	void addHandler(sspMidiInHandler* pHandler);

	static UINT getNumDevices() {return midiInGetNumDevs();}
	static std::wstring getDeviceName(int nDevice);
};

#endif