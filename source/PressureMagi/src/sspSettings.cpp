// sspSettings.cpp: implementation of the sspSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sspSettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////
// Static declarations
/////////////////////////////////

const char sspSettings::strProfileProcess[] = "Process";
const char sspSettings::strPressureMin[] = "pressure min";
const char sspSettings::strPressureMax[] = "pressure max";
const char sspSettings::strPressureMargin[] = "pressure margin";
const char sspSettings::strPressureStandby[] = "standby";
const char sspSettings::strPressureFactor[] = "factor";
const char sspSettings::strTimerUpdate[] = "timer update";

const char sspSettings::strProfileMidi[] = "Midi";
const char sspSettings::strMidiInput[] = "input device";
const char sspSettings::strMidiOutput[] = "output device";
const char sspSettings::strMidiNoteonPump[] = "noteon pump";
const char sspSettings::strMidiNoteonValve[] = "noteon valve";

const char sspSettings::strProfileInput[] = "Input";
const char sspSettings::strInputPort[] = "port";
const char sspSettings::strInputChannel[] = "channel";
const char sspSettings::strInputAddress[] = "address";
const char sspSettings::strInputMidiMin[] = "input-Midi min";
const char sspSettings::strInputMidiMax[] = "input-MIDI max";
const char sspSettings::strOutputPort[] = "output port";
const char sspSettings::strSmsThreshold[] = "sms threshold";

sspSettings::Process  sspSettings::process = {10, 127, 2, 25, 1.0f, 1.0f};
sspSettings::Midi     sspSettings::midi = {0, 0, 11, 12};
sspSettings::Input	  sspSettings::input = {1, 1, 1, 0, 127, 0x378, 6.0f};

void sspSettings::Process::ReadRegistry()
{
	min = AfxGetApp()->GetProfileInt(strProfileProcess, strPressureMin, 10);
	max = AfxGetApp()->GetProfileInt(strProfileProcess, strPressureMax, 127);
	margin = AfxGetApp()->GetProfileInt(strProfileProcess, strPressureMargin, 2);
	standby = AfxGetApp()->GetProfileInt(strProfileProcess, strPressureStandby, 25);

	CString dataStr;
	dataStr = AfxGetApp()->GetProfileString(strProfileProcess, strPressureFactor);
	sscanf(dataStr, "%f", &factor);
	dataStr = AfxGetApp()->GetProfileString(strProfileProcess, strTimerUpdate);
	sscanf(dataStr, "%f", &timerUpdate);
}

void sspSettings::Process::WriteRegistry()
{
	AfxGetApp()->WriteProfileInt(strProfileProcess, strPressureMin, min);
	AfxGetApp()->WriteProfileInt(strProfileProcess, strPressureMax, max);
	AfxGetApp()->WriteProfileInt(strProfileProcess, strPressureMargin, margin);
	AfxGetApp()->WriteProfileInt(strProfileProcess, strPressureStandby, standby);

	CString dataStr;
	dataStr.Format("%.2f", factor);
	AfxGetApp()->WriteProfileString(strProfileProcess, strPressureFactor, dataStr);
	dataStr.Format("%.2f", timerUpdate);
	AfxGetApp()->WriteProfileString(strProfileProcess, strTimerUpdate, dataStr);
}

void sspSettings::Midi::ReadRegistry()
{
	inputDevice = AfxGetApp()->GetProfileInt(strProfileMidi, strMidiInput, 0);
	outputDevice = AfxGetApp()->GetProfileInt(strProfileMidi, strMidiOutput, 0);
  noteonPump = AfxGetApp()->GetProfileInt(strProfileMidi, strMidiNoteonPump, 11);
  noteonValve = AfxGetApp()->GetProfileInt(strProfileMidi, strMidiNoteonValve, 12);
}

void sspSettings::Midi::WriteRegistry()
{
	AfxGetApp()->WriteProfileInt(strProfileMidi, strMidiInput, inputDevice);
	AfxGetApp()->WriteProfileInt(strProfileMidi, strMidiOutput, outputDevice);
	AfxGetApp()->WriteProfileInt(strProfileMidi, strMidiNoteonPump, noteonPump);
	AfxGetApp()->WriteProfileInt(strProfileMidi, strMidiNoteonValve, noteonValve);
}

void sspSettings::Input::ReadRegistry()
{
	port = AfxGetApp()->GetProfileInt(strProfileInput, strInputPort, 1);
	channel = AfxGetApp()->GetProfileInt(strProfileInput, strInputChannel, 0);
  address = AfxGetApp()->GetProfileInt(strProfileInput, strInputAddress, 0);
	inputMin = AfxGetApp()->GetProfileInt(strProfileInput, strInputMidiMin, 0);
	inputMax = AfxGetApp()->GetProfileInt(strProfileInput, strInputMidiMax, 127);
	outPort = AfxGetApp()->GetProfileInt(strProfileInput, strOutputPort, 0x378);

	CString dataStr;
	dataStr = AfxGetApp()->GetProfileString(strProfileInput, strSmsThreshold);
	sscanf(dataStr, "%f", &smsThreshold);
}

void sspSettings::Input::WriteRegistry()
{
	AfxGetApp()->WriteProfileInt(strProfileInput, strInputPort, port);
	AfxGetApp()->WriteProfileInt(strProfileInput, strInputChannel, channel);
	AfxGetApp()->WriteProfileInt(strProfileInput, strInputAddress, address);
	AfxGetApp()->WriteProfileInt(strProfileInput, strInputMidiMin, inputMin);
	AfxGetApp()->WriteProfileInt(strProfileInput, strInputMidiMax, inputMax);

	CString dataStr;
	dataStr.Format("%.2f", smsThreshold);
	AfxGetApp()->WriteProfileString(strProfileInput, strSmsThreshold, dataStr);
}

void sspSettings::ReadAllRegistry()
{
	process.ReadRegistry();
	midi.ReadRegistry();
	input.ReadRegistry();
}

void sspSettings::WriteAllRegistry()
{
	process.WriteRegistry();
	midi.WriteRegistry();
	input.WriteRegistry();
}
