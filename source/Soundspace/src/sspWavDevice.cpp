// sspWavDevice.cpp

#include "stdafx.h"
#include "resource.h"
#include "sspWavDevice.h"

sspWavOutDeviceGroup::sspWavOutDeviceGroup()
{
	m_strName = _T("MME wav");
}

sspWavOutDeviceGroup::~sspWavOutDeviceGroup()
{
	terminate();
}

sspString sspWavOutDeviceGroup::getDeviceName(int nDevice)
{
	WAVEOUTCAPS waveDevice;
	if (waveOutGetDevCaps(nDevice, (LPWAVEOUTCAPS) &waveDevice, sizeof(WAVEOUTCAPS)) == MMSYSERR_NOERROR)
		return waveDevice.szPname;
	else
		return _T("");
}
