// sspSettings.h: interface for the sspSettings class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class sspSettings  
{
public:
	static const char strProfileProcess[];
	static const char strPressureMin[];
	static const char strPressureMax[];
  static const char strPressureStandby[];
  static const char strPressureMargin[];
	static const char strPressureFactor[];
	static const char strTimerUpdate[];

	static const char strProfileMidi[];
	static const char strMidiInput[];
	static const char strMidiOutput[];
	static const char strMidiNoteonPump[];
	static const char strMidiNoteonValve[];

	static const char strProfileInput[];
  static const char strInputPort[];
  static const char strInputChannel[];
  static const char strInputAddress[];
	static const char strInputMidiMin[];
	static const char strInputMidiMax[];
	static const char strOutputPort[];
	static const char strSmsThreshold[];

public:

	// Pressure settings
	static struct Process {
		int min;
		int max;
		int margin;
    int standby;
    float factor;
		float timerUpdate;

    void ReadRegistry();
		void WriteRegistry();
	} process;

	// Resolution in milliseconds
	static struct Midi {
    int inputDevice;
    int outputDevice;
    int noteonPump;
    int noteonValve;

		void ReadRegistry();
		void WriteRegistry();
	} midi;

	// Update intervals in seconds
	static struct Input {
    int port;
    int channel;
    int address;
    int inputMin;
    int inputMax;
    int outPort;
    float smsThreshold;

    void ReadRegistry();
		void WriteRegistry();
	} input;

	static void ReadAllRegistry();
	static void WriteAllRegistry();
};
