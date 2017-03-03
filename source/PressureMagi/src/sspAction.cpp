//////////////////////////////////////////
// sspAction.cpp

#include "stdafx.h"
#include "sspAction.h"
#include "sspSettings.h"

#include "resource.h"
#include "PressureMagiDlg.h"

#include <sspTimer.h>
#include <stdexcept>

sspAction* sspAction::m_pInstance = NULL;
bool sspAction::m_bDestroyed = false;

namespace {
  enum parabits {
    PRESSURE_BELOW_THRESHOLD
  };
}

sspAction::sspAction(void)
: m_pTimer(NULL)
, m_bRunning(false)
, m_fConvertFac(1.0f)
, m_fMeasuredMilliamp(0.0f)
, m_nMeasuredMidi(0)
, m_nInputMidi(0)
, m_nOutputMidi(0)
, m_nOutputValve(VALVE_UNDEFINED)
, m_pParent(NULL)
{
}

sspAction::sspAction(const sspAction&)
{
}

sspAction& sspAction::operator=(const sspAction&)
{
	return *this;
}


sspAction::~sspAction(void)
{
	terminate();
	m_pInstance = NULL;
	m_bDestroyed = true;
}

sspAction& sspAction::Instance()
{
	if (!m_pInstance) {
		if (m_bDestroyed) {
			onDeadReference();
		}
		else {
			create();
		}
	}
	return *m_pInstance;
}

sspAction::tErrorCode sspAction::initialize(CPressureMagiDlg* pParent)
{
  // Using 100 msecs as default resolution (unless update interval is even smaller)
  sspTimerType nMsecs = std::min(sspTimerType(100), sspTimeConvert::secs2timer(sspSettings::process.timerUpdate));

  delete m_pTimer;
  m_pTimer = new sspPeriodicTimer(nMsecs);
  if (!m_pTimer->isInitialized()) 
    return ERROR_TIMER;

  if (!m_midi.initialize(sspSettings::midi.inputDevice, sspSettings::midi.outputDevice,
                         static_cast<BYTE>(sspSettings::midi.noteonPump)))
    return ERROR_MIDI;

  m_input.setPort(sspSettings::input.port);
  m_input.setChannel(sspSettings::input.channel);
  m_input.setAddress(sspSettings::input.address);
  if (!m_input.initialize())
    return ERROR_INPUT;

  m_output.setPort(sspSettings::input.outPort);
  if (!m_output.initialize())
    return ERROR_OUTPUT;

  m_pParent = pParent;
  m_fConvertFac = ((sspSettings::input.inputMax - sspSettings::input.inputMin) / (14.0f - 4.0f));
  
  return OK;
}

sspAction::tErrorCode sspAction::start()
{
  if (!m_midi.begin())
    return ERROR_MIDI;
  if (!m_pTimer || !m_pTimer->run(this))
    return ERROR_TIMER;
  return OK;
}

sspAction::tErrorCode sspAction::stop()
{
  m_midi.end();
  if (m_pTimer) m_pTimer->stop();
  return OK;
}

sspAction::tErrorCode sspAction::terminate()
{
  stop();
  m_midi.terminate();
  while (m_pTimer && m_pTimer->isPlaying()) {
    Sleep(100);
  }
  delete m_pTimer;
  m_pTimer = NULL;
  return OK;
}

bool sspAction::begin(sspTimerType& nMSecs)
{
  // Initialize valve and pump
  m_nOutputMidi = 0;
  m_midi.setOutputValue(sspSettings::midi.noteonPump, m_nOutputMidi);
  m_nOutputValve = VALVE_CLOSED;
  m_midi.setOutputValue(sspSettings::midi.noteonValve, m_nOutputValve);

  m_nMeasuredMidi = sspSettings::input.inputMin;

  nMSecs = sspTimeConvert::secs2timer(sspSettings::process.timerUpdate);
  m_bRunning = true;
  return m_bRunning;
}

inline int sspAction::getMidiPressure(float fmA)
{
  if (fmA <= 4.0f)
    return sspSettings::input.inputMin;
  else if (fmA < 14.0f) {
    return sspSettings::input.inputMin + static_cast<int>((fmA - 4.0f) * m_fConvertFac);
  }
  else {
    return sspSettings::input.inputMax;
  }  
}

bool sspAction::update(sspTimerType& nMSecs)
{
  // First read the desired pressure
  m_nInputMidi = m_midi.getInputValue();

  // Default output
  int nOutMidi = m_nInputMidi;
  int nOutValve = VALVE_CLOSED;

  float fmA = 0.0f;
  if (m_input.getValue(fmA)) {
    m_fMeasuredMilliamp = fmA;
    m_output.setBit(PRESSURE_BELOW_THRESHOLD, m_fMeasuredMilliamp < sspSettings::input.smsThreshold);
    if (m_fMeasuredMilliamp > 14.0) 
      nOutValve = VALVE_OPEN;
    m_nMeasuredMidi = getMidiPressure(m_fMeasuredMilliamp);
  }

  if (m_nInputMidi == 0) {
    // Reach rest pressure !!!
    nOutValve = (m_nMeasuredMidi > sspSettings::process.standby) ? VALVE_OPEN : VALVE_CLOSED;
  }
  else if (m_nInputMidi > 0) {

    int nDelta = m_nMeasuredMidi - m_nInputMidi;
    if (nDelta > sspSettings::process.margin) {
      nOutMidi -= static_cast<int>(sspSettings::process.factor * nDelta + 0.5f);
      nOutValve = VALVE_OPEN;
    }
    else if (nDelta < -sspSettings::process.margin) {
      nOutMidi -= static_cast<int>(sspSettings::process.factor * nDelta - 0.5f);
    }

    nOutMidi = std::max(sspSettings::process.min, nOutMidi);
    nOutMidi = std::min(sspSettings::process.max, nOutMidi);
  }

  if (nOutMidi != m_nOutputMidi) {
    m_nOutputMidi = nOutMidi;
    m_midi.setOutputValue(sspSettings::midi.noteonPump, m_nOutputMidi);
  }
  if (nOutValve != m_nOutputValve) {
    m_nOutputValve = nOutValve;
    m_midi.setOutputValue(sspSettings::midi.noteonValve, m_nOutputValve);
  }
  if (m_pParent) m_pParent->updateMonitor();
 
  return true;
}

void sspAction::end()
{
  m_bRunning = false;
}

void sspAction::create()
{
	static sspAction theInstance;
	m_pInstance = &theInstance;
}

void sspAction::onDeadReference()
{
  throw std::runtime_error("Dead reference to action object detected");
}
