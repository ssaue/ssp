// sspMagiSettings.cpp: implementation of the sspMagiSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sspMagiSettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////
// Static declarations
/////////////////////////////////

const char sspMagiSettings::strProfileMap[] = "Mapping";
const char sspMagiSettings::strSounding[] = "sounding pitch";
const char sspMagiSettings::strDMX[] = "dmx channel";

const char sspMagiSettings::strProfileFilter[] = "Velocity filter";
const char sspMagiSettings::strTimeWindow[] = "time window";
const char sspMagiSettings::strTimeOffset[] = "time offset";
const char sspMagiSettings::strPumpStart[] = "pump start";
const char sspMagiSettings::strPumpStop[] = "pump stop";
const char sspMagiSettings::strStartVelocity[] = "start velocity";
const char sspMagiSettings::strMinVelocity[] = "min velocity";
const char sspMagiSettings::strMaxVelocity[] = "max velocity";
const char sspMagiSettings::strChannel[] = "MIDI channel";
const char sspMagiSettings::strNoteon[] = "note on";


sspMagiSettings::Timing	    sspMagiSettings::timing = {0.25, 0.0, 10.0, 1.0};
sspMagiSettings::Velocity	  sspMagiSettings::velocity = {64, 1, 127};
sspMagiSettings::MidiTrack  sspMagiSettings::midiTrack = {1, 1};

int sspMagiSettings::sounding[] = {34, 37, 41, 42, 44, 46, 48, 49, 51, 53};
int sspMagiSettings::dmx[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

void sspMagiSettings::Timing::readRegistry()
{
	CString dataStr;
	dataStr = AfxGetApp()->GetProfileString(strProfileFilter, strTimeWindow);
	sscanf_s(dataStr, "%f", &timeWindow);
	dataStr = AfxGetApp()->GetProfileString(strProfileFilter, strTimeOffset);
	sscanf_s(dataStr, "%f", &timeOffset);
	dataStr = AfxGetApp()->GetProfileString(strProfileFilter, strPumpStart);
	sscanf_s(dataStr, "%f", &pumpStart);
	dataStr = AfxGetApp()->GetProfileString(strProfileFilter, strPumpStop);
	sscanf_s(dataStr, "%f", &pumpStop);
}

void sspMagiSettings::Timing::writeRegistry()
{
	CString dataStr;
	dataStr.Format("%.2f", timeWindow);
	AfxGetApp()->WriteProfileString(strProfileFilter, strTimeWindow, dataStr);
	dataStr.Format("%.2f", timeOffset);
	AfxGetApp()->WriteProfileString(strProfileFilter, strTimeOffset, dataStr);
	dataStr.Format("%.2f", pumpStart);
	AfxGetApp()->WriteProfileString(strProfileFilter, strPumpStart, dataStr);
	dataStr.Format("%.2f", pumpStop);
	AfxGetApp()->WriteProfileString(strProfileFilter, strPumpStop, dataStr);
}


void sspMagiSettings::Velocity::readRegistry()
{
	start = AfxGetApp()->GetProfileInt(strProfileFilter, strStartVelocity, 64);
	minimum = AfxGetApp()->GetProfileInt(strProfileFilter, strMinVelocity, 1);
	maximum = AfxGetApp()->GetProfileInt(strProfileFilter, strMaxVelocity, 127);
}

void sspMagiSettings::Velocity::writeRegistry()
{
	AfxGetApp()->WriteProfileInt(strProfileFilter, strStartVelocity, start);
	AfxGetApp()->WriteProfileInt(strProfileFilter, strMinVelocity, minimum);
	AfxGetApp()->WriteProfileInt(strProfileFilter, strMaxVelocity, maximum);
}

void sspMagiSettings::MidiTrack::readRegistry()
{
	channel = AfxGetApp()->GetProfileInt(strProfileFilter, strChannel, 1);
	noteon = AfxGetApp()->GetProfileInt(strProfileFilter, strNoteon, 1);
}

void sspMagiSettings::MidiTrack::writeRegistry()
{
	AfxGetApp()->WriteProfileInt(strProfileFilter, strChannel, channel);
	AfxGetApp()->WriteProfileInt(strProfileFilter, strNoteon, noteon);
}

void sspMagiSettings::readMapRegistry()
{
	CString dataStr;
	dataStr = AfxGetApp()->GetProfileString(strProfileMap, strSounding);
	sscanf_s(dataStr, "%d %d %d %d %d %d %d %d %d %d", &sounding[0], &sounding[1], &sounding[2], &sounding[3],
		&sounding[4], &sounding[5], &sounding[6], &sounding[7], &sounding[8], &sounding[9]);
	dataStr = AfxGetApp()->GetProfileString(strProfileMap, strDMX);
	sscanf_s(dataStr, "%d %d %d %d %d %d %d %d %d %d", &dmx[0], &dmx[1], &dmx[2], &dmx[3],
    &dmx[4], &dmx[5], &dmx[6], &dmx[7], &dmx[8], &dmx[9]);
}

void sspMagiSettings::writeMapRegistry()
{
	CString dataStr;
	dataStr.Format("%d %d %d %d %d %d %d %d %d %d", sounding[0], sounding[1], sounding[2],
		sounding[3], sounding[4], sounding[5], sounding[6], sounding[7], sounding[8], sounding[9]);
	AfxGetApp()->WriteProfileString(strProfileMap, strSounding, dataStr);
	dataStr.Format("%d %d %d %d %d %d %d %d %d %d", dmx[0], dmx[1], dmx[2],
		dmx[3], dmx[4], dmx[5], dmx[6], dmx[7], dmx[8], dmx[9]);
	AfxGetApp()->WriteProfileString(strProfileMap, strDMX, dataStr);
}

void sspMagiSettings::readAllRegistry()
{
	timing.readRegistry();
	velocity.readRegistry();
	midiTrack.readRegistry();
	readMapRegistry();
}

void sspMagiSettings::writeAllRegistry()
{
	timing.writeRegistry();
	velocity.writeRegistry();
	midiTrack.writeRegistry();
	writeMapRegistry();
}
