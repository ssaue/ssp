// sspMidiDevice.cpp

#include "sspMidiDevice.h"

//////////////////////////////////////////////////////////////////////
// sspMidiOutDevice Class
//////////////////////////////////////////////////////////////////////

sspMidiOutDevice::sspMidiOutDevice()
{
	m_nOpenDevice = NO_DEVICE;
}

sspMidiOutDevice::~sspMidiOutDevice()
{
	close();
}

bool sspMidiOutDevice::open(int deviceNr)
{
	if (m_nOpenDevice == deviceNr)			// Device already open
		return true;
	else if (m_nOpenDevice != NO_DEVICE)	// Another device is open, close this first
		close();
	MMRESULT res = midiOutOpen((LPHMIDIOUT) &m_MidiHandle, deviceNr, NULL, 0L, CALLBACK_NULL);
	if (res == MMSYSERR_NOERROR ) {
		m_nOpenDevice = deviceNr;
		midiOutGetDevCaps(deviceNr, (LPMIDIOUTCAPS) &m_MidiCaps, sizeof(MIDIOUTCAPS));
		return true;
	}
	else {
		m_nOpenDevice = NO_DEVICE;
		return false;
	}
}

bool sspMidiOutDevice::close()
{
	if (m_nOpenDevice == NO_DEVICE)
		return true;		// Enheten er allerede lukket
	midiOutReset(m_MidiHandle);
	MMRESULT res = midiOutClose(m_MidiHandle);
	if (res == MMSYSERR_NOERROR ) {
		m_nOpenDevice = NO_DEVICE;
		return true;
	}
  else
    return false;
}

bool sspMidiOutDevice::reset() const
{
	if (midiOutReset(m_MidiHandle) == 0)
		return true;
	else
		return false;
}

int sspMidiOutDevice::getVoices() const
{
	if (m_MidiCaps.wTechnology == MOD_MIDIPORT)
		return MAX_CHANNELS;
	else
		return m_MidiCaps.wVoices;
}

int sspMidiOutDevice::getNotes() const
{
	if (m_MidiCaps.wTechnology == MOD_MIDIPORT)
		return MAX_NOTES;
	else
		return m_MidiCaps.wNotes;
}

std::wstring	sspMidiOutDevice::getDeviceName(int nDevice)
{
	MIDIOUTCAPS midiDevice;
	if (midiOutGetDevCaps(nDevice, (LPMIDIOUTCAPS) &midiDevice, sizeof(MIDIOUTCAPS)) == MMSYSERR_NOERROR)
		return midiDevice.szPname;
	else
		return L"";
}

//////////////////////////////////////////////////////////////////////
// sspMidiInDevice Class
//////////////////////////////////////////////////////////////////////

sspMidiInDevice::sspMidiInDevice()
{
	m_nOpenDevice = NO_DEVICE;
}

sspMidiInDevice::~sspMidiInDevice()
{
	close();
	m_inputHandlers.clear();
}

bool sspMidiInDevice::open(int deviceNr)
{
	if (m_nOpenDevice == deviceNr)			// Device already open
		return true;
	else if (m_nOpenDevice != NO_DEVICE)	// Another device is open, close this first
		close();
	MMRESULT res = midiInOpen((LPHMIDIIN) &m_MidiHandle, deviceNr, (DWORD_PTR)msgCallback,  (DWORD_PTR)this, CALLBACK_FUNCTION);
	if (res == MMSYSERR_NOERROR ) {
		m_nOpenDevice = deviceNr;
		return true;
	}
	else {
		m_nOpenDevice = NO_DEVICE;
		return false;
	}
}

bool sspMidiInDevice::close()
{
	if (m_nOpenDevice == NO_DEVICE)
		return true;		// Enheten er allerede lukket
	midiInReset(m_MidiHandle);
	MMRESULT res = midiInClose(m_MidiHandle);
	if (res == MMSYSERR_NOERROR ) {
		m_nOpenDevice = NO_DEVICE;
		return true;
	}
  else
    return false;
}

bool sspMidiInDevice::reset() const
{
	if (midiInReset(m_MidiHandle) == 0)
		return true;
	else
		return false;
}

bool sspMidiInDevice::startRecord()
{
	if (midiInStart(m_MidiHandle) == 0)
		return true;
	else
		return false;
}

bool sspMidiInDevice::stopRecord()
{
	if (reset() && midiInStop(m_MidiHandle) == 0)
		return true;
	return false;	
}

void sspMidiInDevice::addHandler(sspMidiInHandler* pHandler)
{
	if (pHandler != NULL)
		m_inputHandlers.push_back(pHandler);
}

void CALLBACK sspMidiInDevice::msgCallback(HMIDIIN handle, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	((sspMidiInDevice*) UIntToPtr(dwInstance))->handleMsg(handle, wMsg, static_cast<sspMidiMessage>(dwParam1));
}

void sspMidiInDevice::handleMsg(HMIDIIN handle, UINT wMsg, sspMidiMessage msg)
{
	if (handle == m_MidiHandle && wMsg == MIM_DATA) {
		for (size_t i=0; i<m_inputHandlers.size(); ++i) {
			m_inputHandlers[i]->handleMsg(msg);
		}
	}
}

std::wstring sspMidiInDevice::getDeviceName(int nDevice)
{
	MIDIINCAPS midiDevice;
	if (midiInGetDevCaps(nDevice, (LPMIDIINCAPS) &midiDevice, sizeof(MIDIINCAPS)) == MMSYSERR_NOERROR)
		return midiDevice.szPname;
	else
		return L"";
}

