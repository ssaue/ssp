///////////////////////////////////////////
// sspMidiScheduler.cpp : implementation file
///////////////////////////////////////////

#include "StdAfx.h"
#include "sspMidiScheduler.h"
#include "SSpMessage.h"
#include "SSpBasic.h"
#include "sspMidiDevice.h"
#include "sspDeviceManager.h"
#include "sspDeviceGroups.h"

//////////////////////////////////////////////////////////////////////
// sspMidiScheduler Class
//////////////////////////////////////////////////////////////////////

sspMidiScheduler::sspMidiScheduler()
{
	m_pTimeFactor = NULL;
	m_pDevice = NULL;
	m_pOwner = NULL;
	m_dDuration = 0.0;
}

sspMidiScheduler::~sspMidiScheduler()
{
	m_sequence.clear();
}

void sspMidiScheduler::setSequence(const sspMidiSequence& sequence)
{
  m_sequence = sequence;
  setDuration();
}

void sspMidiScheduler::swapSequence(sspMidiSequence& sequence)
{
  std::swap(m_sequence, sequence);
  setDuration();
}

void sspMidiScheduler::setTimeFactor(sspValue* pFactor)
{
	m_pTimeFactor = pFactor;
}

void sspMidiScheduler::setMidiDevice(int nDevice)
{
	m_pDevice = ((sspMidiOutDeviceGroup*) sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT])->getDevice(nDevice);
}

void sspMidiScheduler::setOwner(sspMsgHandler* pOwner)
{
	m_pOwner = pOwner;
}

unsigned int sspMidiScheduler::getDuration() const
{
	if (m_pTimeFactor == NULL)
		return (unsigned int) m_dDuration;
	else
		return (unsigned int) (m_pTimeFactor->getValue() * m_dDuration);
}

bool sspMidiScheduler::begin()
{
	m_iterator = m_sequence.begin();
	if (m_iterator == m_sequence.end())
		return false;
	else {
    m_nNextTime = (int) (m_pTimeFactor ? (m_pTimeFactor->getValue() * m_iterator->timestamp) : m_iterator->timestamp);
		return true;
	}
}

bool sspMidiScheduler::update()
{
	m_pDevice->sendMidi(m_iterator->message);
	m_iterator++;
	if (m_iterator != m_sequence.end()) {
    m_nNextTime = (int) (m_pTimeFactor ? (m_pTimeFactor->getValue() * m_iterator->timestamp) : m_iterator->timestamp);
		return true;
	}
	else {
		return false;
	}
}

void sspMidiScheduler::end(void)
{
	if (m_pOwner != NULL) {
		SSpMessage msg;
		msg.SetMsgType(SSP_IS_DONE);
		m_pOwner->handleMessage(msg);
	}
}

void sspMidiScheduler::setDuration()
{
  m_dDuration = 0.0;
  sspMidiSequence::const_iterator iter;
  for (iter = m_sequence.begin(); iter != m_sequence.end(); ++iter)
    m_dDuration += iter->timestamp;
}
