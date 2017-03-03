// sspDeviceGroups.cpp

#include "stdafx.h"
#include "resource.h"
#include "sspDeviceGroups.h"
#include <sspMidiDevice.h>

//////////////////////////////////////////////////////////////////////
// sspMidiOutDeviceGroup Class
//////////////////////////////////////////////////////////////////////

sspMidiOutDeviceGroup::sspMidiOutDeviceGroup()
{
	m_strName = _T("MidiOut");
}

sspMidiOutDeviceGroup::~sspMidiOutDeviceGroup()
{
	terminate();
}

unsigned int sspMidiOutDeviceGroup::getAllDevices() 
{
  return sspMidiOutDevice::getNumDevices();
}

unsigned int sspMidiOutDeviceGroup::getDeviceCount()
{
  return sspMidiOutDevice::getNumDevices();
}

sspString sspMidiOutDeviceGroup::getDeviceName(int nDevice)
{
  return sspMidiOutDevice::getDeviceName(nDevice);
}

bool sspMidiOutDeviceGroup::initializeImpl(LPVOID hWnd)
{
	m_pDevices.reserve(m_nDevices.size());
	for (unsigned int i=0; i<m_nDevices.size(); ++i) {
		sspMidiOutDevice* pDev = new sspMidiOutDevice;
		if (pDev->open(m_nDevices[i]))
			m_pDevices.push_back(pDev);
		else
			return false;
	}
	return true;
}

bool sspMidiOutDeviceGroup::terminateImpl()
{
	for (unsigned int i=0; i<m_pDevices.size(); ++i)
		delete m_pDevices[i];
	m_pDevices.clear();
	return true;
}
	
sspMidiOutDevice* sspMidiOutDeviceGroup::getDevice(int nSubsetIndex)
{
	return (nSubsetIndex < (int) m_nDevices.size()) ? m_pDevices[nSubsetIndex] : NULL;
}

//////////////////////////////////////////////////////////////////////
// sspMidiInDeviceGroup Class
//////////////////////////////////////////////////////////////////////

sspMidiInDeviceGroup::sspMidiInDeviceGroup()
{
	m_strName = _T("MidiIn");
}

sspMidiInDeviceGroup::~sspMidiInDeviceGroup()
{
	terminate();
}

unsigned int sspMidiInDeviceGroup::getAllDevices() 
{
  return sspMidiInDevice::getNumDevices();
}

unsigned int sspMidiInDeviceGroup::getDeviceCount()
{
  return sspMidiInDevice::getNumDevices();
}

sspString sspMidiInDeviceGroup::getDeviceName(int nDevice)
{
  return sspMidiInDevice::getDeviceName(nDevice);
}

bool sspMidiInDeviceGroup::initializeImpl(LPVOID hWnd)
{
	m_pDevices.reserve(m_nDevices.size());
	for (unsigned int i=0; i<m_nDevices.size(); ++i) {
		sspMidiInDevice* pDev = new sspMidiInDevice;
		if (pDev->open(m_nDevices[i]))
			m_pDevices.push_back(pDev);
		else
			return false;
	}
	return true;
}

bool sspMidiInDeviceGroup::beginImpl()
{
	for (unsigned int i=0; i<m_pDevices.size(); ++i) {
		if (!m_pDevices[i]->startRecord())
			return false;
	}
	return true;
}

bool sspMidiInDeviceGroup::endImpl()
{
	for (unsigned int i=0; i<m_pDevices.size(); ++i) {
		if (!m_pDevices[i]->stopRecord())
			return false;
	}
	return true;
}

bool sspMidiInDeviceGroup::terminateImpl()
{
	for (unsigned int i=0; i<m_pDevices.size(); ++i)
		delete m_pDevices[i];
	m_pDevices.clear();
	return true;
}
	
sspMidiInDevice* sspMidiInDeviceGroup::getDevice(int nSubsetIndex)
{
	return (nSubsetIndex < (int) m_nDevices.size()) ? m_pDevices[nSubsetIndex] : NULL;
}
