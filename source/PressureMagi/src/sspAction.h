//////////////////////////////////////////
// sspAction.h

#ifndef SSP_ACTION_H
#define SSP_ACTION_H

#include "sspMIDIhandler.h"
#include "sspICPinput.h"
#include "sspParallelOutput.h"

#include <sspTimerInterface.h>

class sspTimer;
class CPressureMagiDlg;

// Class sspAction is implemented as a Singleton pattern
class sspAction : public sspTimerObject
{
public:

  enum tErrorCode {
    OK,
    ERROR_TIMER,
    ERROR_MIDI,
    ERROR_INPUT,
    ERROR_OUTPUT
  };

	static sspAction& Instance();

	tErrorCode initialize(CPressureMagiDlg* pParent);
	tErrorCode start();
	tErrorCode stop();
	tErrorCode terminate();

  // Virtual methods from sspTimerObject
	bool begin(sspTimerType& nMSecs);
	bool update(sspTimerType& nMSecs);
	void end();

  bool isRunning() const {return m_bRunning;}

  // Access methods
  float getMeasuredMilliAmpere() const {return m_fMeasuredMilliamp;}
  int getMeasuredMidi() const {return m_nMeasuredMidi;}
  int getInputMidi() const {return m_nInputMidi;}
  int getOutputMidi() const {return m_nOutputMidi;}
  bool isValveClosed() const {return (m_nOutputValve == VALVE_CLOSED);}

private:
	sspAction();
	sspAction(const sspAction&);
	sspAction& operator=(const sspAction&);
	~sspAction();

  static sspAction* m_pInstance;
	static bool m_bDestroyed;
	static void create();
	static void onDeadReference();

  sspMIDIhandler    m_midi;
  sspICPinput       m_input;
  sspParallelOutput m_output;

  sspTimer*       m_pTimer;
  bool            m_bRunning;

  int getMidiPressure(float fmA);
  float m_fConvertFac;

  enum {VALVE_CLOSED = 0, VALVE_OPEN = 127, VALVE_UNDEFINED = 64};

  float m_fMeasuredMilliamp;
  int   m_nMeasuredMidi;
  int   m_nInputMidi;
  int   m_nOutputMidi;
  int  m_nOutputValve;

  CPressureMagiDlg* m_pParent;
};

#endif