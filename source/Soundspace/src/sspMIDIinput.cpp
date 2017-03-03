// sspMIDIinput.cpp: implementation of the sspMIDIinput classes.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "soundspace.h"
#include "sspMIDIinput.h"
#include "SSpConditionals.h"
#include "sspPools.h"
#include "sspDeviceManager.h"
#include <sspMidi.h>
#include "sspDeviceGroups.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( sspMIDIeventInput, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// sspMIDIeventInput
//////////////////////////////////////////////////////////////////////

sspMIDIinput::sspMIDIinput(short nType) : sspInput(nType)
{
	m_nMidiDevice = -1;
}

sspMIDIinput::sspMIDIinput(const sspMIDIinput& inp) : sspInput(inp)
{
	m_nMidiDevice = inp.m_nMidiDevice;
}

sspMIDIinput& sspMIDIinput::operator= (const sspMIDIinput& inp)
{
	sspInput::operator=(inp);
	m_nMidiDevice = inp.m_nMidiDevice;
	return *this;
}

void sspMIDIinput::Serialize(CArchive& ar)
{
	sspInput::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nMidiDevice;
	else
		ar >> m_nMidiDevice;
}

void sspMIDIinput::printASCII(sspOutStream& outStr)
{
	sspInput::printASCII(outStr);
	outStr << endl << "   - m_nMidiDevice: " << m_nMidiDevice;
}

bool sspMIDIinput::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
	if (!sspInput::verify(outStr, nErrors, nWarnings))
		bRet = false;
	if (m_nMidiDevice < 0 || m_nMidiDevice >= (int) sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT]->getSubsetSize()) {
		printError(outStr, _T("(sspMIDIinput): m_nMidiDevice is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool sspMIDIinput::initialize(LPVOID pWnd)
{
	if (m_nMidiDevice < 0)
		return false;
	sspMidiInDevice* pDevice = 
		((sspMidiInDeviceGroup*) sspDeviceManager::Instance()[SSP_DEVICE_MIDIIN])->getDevice(m_nMidiDevice);
	pDevice->addHandler(this);
	return sspInput::initialize(pWnd);
}


//////////////////////////////////////////////////////////////////////
// sspMIDIeventInput
//////////////////////////////////////////////////////////////////////

sspMIDIeventInput::sspMIDIeventInput() : sspMIDIinput(SSP_MIDIIN_EVENT)
{
	m_nConditional = -1;
	m_nMsgType = SSP_MI_ANY_MSG;
	m_nMsgChannel[0] = -1;
	m_nMsgChannel[1] = -1;
	m_nMsgData1[0] = -1;
	m_nMsgData1[1] = -1;
	m_nMsgData2[0] = -1;
	m_nMsgData2[1] = -1;
	m_bEvent = false;
}

sspMIDIeventInput::sspMIDIeventInput(const sspMIDIeventInput& inp) : sspMIDIinput(inp)
{
	m_nConditional = inp.m_nConditional;
	m_nMsgType = inp.m_nMsgType;
	m_nMsgChannel[0] = inp.m_nMsgChannel[0];
	m_nMsgChannel[1] = inp.m_nMsgChannel[1];
	m_nMsgData1[0] = inp.m_nMsgData1[0];
	m_nMsgData1[1] = inp.m_nMsgData1[1];
	m_nMsgData2[0] = inp.m_nMsgData2[0];
	m_nMsgData2[1] = inp.m_nMsgData2[1];
	m_bEvent = false;
}

sspMIDIeventInput& sspMIDIeventInput::operator= (const sspMIDIeventInput& inp)
{
	sspMIDIinput::operator=(inp);
	m_nConditional = inp.m_nConditional;
	m_nMsgType = inp.m_nMsgType;
	m_nMsgChannel[0] = inp.m_nMsgChannel[0];
	m_nMsgChannel[1] = inp.m_nMsgChannel[1];
	m_nMsgData1[0] = inp.m_nMsgData1[0];
	m_nMsgData1[1] = inp.m_nMsgData1[1];
	m_nMsgData2[0] = inp.m_nMsgData2[0];
	m_nMsgData2[1] = inp.m_nMsgData2[1];
	return *this;
}

sspMIDIeventInput::~sspMIDIeventInput()
{
	end();
}

void sspMIDIeventInput::Serialize(CArchive& ar)
{
	sspMIDIinput::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nConditional << m_nMsgType << m_nMsgChannel[0] << m_nMsgChannel[1];
		ar << m_nMsgData1[0] << m_nMsgData1[1] << m_nMsgData2[0] << m_nMsgData2[1];
	}
	else {
		ar >> m_nConditional >> m_nMsgType >> m_nMsgChannel[0] >> m_nMsgChannel[1];
		ar >> m_nMsgData1[0] >> m_nMsgData1[1] >> m_nMsgData2[0] >> m_nMsgData2[1];
	}
}

void sspMIDIeventInput::printASCII(sspOutStream& outStr)
{
	outStr << endl << ": sspMIDIeventInput " << m_strName;
	sspMIDIinput::printASCII(outStr);
	outStr << endl << "   - m_nConditional: " << sspPool::Instance().conditionals.GetName(m_nConditional);
	outStr << endl << "   - m_nMsgType: " << m_nMsgType;
	outStr << endl << "   - m_nMsgChannel: " << m_nMsgChannel[0] << ", " << m_nMsgChannel[1];
	outStr << endl << "   - m_nMsgData1: " << m_nMsgData1[0] << ", " << m_nMsgData1[1];
	outStr << endl << "   - m_nMsgData2: " << m_nMsgData2[0] << ", " << m_nMsgData2[1];
}

bool sspMIDIeventInput::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
	if (!sspMIDIinput::verify(outStr, nErrors, nWarnings))
		bRet = false;
	if (m_nMsgChannel[0] >= 0 && m_nMsgChannel[0] < m_nMsgChannel[1]) {
		printError(outStr, _T("(sspMIDIeventInput): m_nMsgChannel range is not valid"), nErrors);
		bRet = false;
	}
	if (m_nMsgData1[0] >= 0 && m_nMsgData1[0] < m_nMsgData1[1]) {
		printError(outStr, _T("(sspMIDIeventInput): m_nMsgData1 range is not valid"), nErrors);
		bRet = false;
	}
	if (m_nMsgData2[0] >= 0 && m_nMsgData2[0] < m_nMsgData2[1]) {
		printError(outStr, _T("(sspMIDIeventInput): m_nMsgData2 range is not valid"), nErrors);
		bRet = false;
	}
	if (m_nConditional < 0 || m_nConditional >= (int) sspPool::Instance().conditionals.GetSize()) {
		printError(outStr, _T("(sspMIDIeventInput): m_nConditional is not valid"), nErrors);
		bRet = false;
	}
	else if (sspPool::Instance().conditionals.GetConditional(m_nConditional)->getType() != SSP_COND_BOOLEAN) {
		printError(outStr, _T("(sspMIDIeventInput): m_nConditional must be of basic type"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool sspMIDIeventInput::initialize(LPVOID pWnd)
{
	m_pConditional = dynamic_cast<SSpBoolean*> (sspPool::Instance().conditionals.GetConditional(m_nConditional));
	if (!m_pConditional) return false;
	return sspMIDIinput::initialize(pWnd);
}

bool sspMIDIeventInput::begin()
{
	m_pConditional->setValue(true);
	m_bEvent = false;
	return true;
}

bool sspMIDIeventInput::update()
{
	if (!sspInput::update())
		return false;

	bool bRet = m_bEvent ^ m_pConditional->isTrue();
	m_pConditional->setValue(m_bEvent);
	m_bEvent = false;
	return bRet;
}

void sspMIDIeventInput::handleMsg(sspMidiMessage msg)
{
	switch (m_nMsgType) {
		case SSP_MI_ANY_MSG:
			if (m_nMsgChannel[0] < 0) {
				m_bEvent = true;
			}
      else if (!sspMidi::isSystemMessage(msg)) {	// channel is specified
        BYTE nChannel = sspMidi::getChannel(msg);
				if (nChannel >= m_nMsgChannel[0] && nChannel <= m_nMsgChannel[1])
					m_bEvent = true;
			}
			break;
		case SSP_MI_CONTROL_MSG:
      if (sspMidi::isControl(msg)) {
				BYTE nChannel = sspMidi::getChannel(msg);
				if (m_nMsgChannel[0] < 0 || (nChannel >= m_nMsgChannel[0] && nChannel <= m_nMsgChannel[1])) {
          BYTE nData1 = sspMidi::getData1(msg);
					if (m_nMsgData1[0] < 0 || (nData1 >= m_nMsgData1[0] && nData1 <= m_nMsgData1[1])) {
            BYTE nData2 = sspMidi::getData2(msg);
						if (m_nMsgData2[0] < 0 || (nData1 >= m_nMsgData2[0] && nData1 <= m_nMsgData2[1])) {
							m_bEvent = true;
						}
					}
				}
			}
			break;
		case SSP_MI_NOTEON_MSG:
      if (sspMidi::isNoteOn(msg)) {
				BYTE nChannel = sspMidi::getChannel(msg);
				if (m_nMsgChannel[0] < 0 || (nChannel >= m_nMsgChannel[0] && nChannel <= m_nMsgChannel[1])) {
          BYTE nData1 = sspMidi::getData1(msg);
					if (m_nMsgData1[0] < 0 || (nData1 >= m_nMsgData1[0] && nData1 <= m_nMsgData1[1])) {
						m_bEvent = true;
					}
				}
			}
			break;
		default:
			break;
	}
}
