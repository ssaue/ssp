// sspDmxDevice.cpp

#include "stdafx.h"
#include "sspDmxDevice.h"

//////////////////////////////////////////////////////////////////////
// sspDmxDevice Class
//////////////////////////////////////////////////////////////////////

sspDmxDevice::sspDmxDevice()
{
	m_id = 0;
	m_bOpen = false;
}

sspDmxDevice::~sspDmxDevice()
{
	close();
}

bool sspDmxDevice::open(DevEntry* pDev)
{
	if (m_bOpen) return true;
	DevReturn nRet = openDevice(&m_id, pDev);
	m_bOpen = (nRet == SUCCESS);
	return m_bOpen;
}

bool sspDmxDevice::close()
{
	stopPlay();
	if (!m_bOpen) return true;
	DevReturn nRet = closeDevice(m_id);
	m_bOpen = false;
	return (nRet == SUCCESS);
}

void sspDmxDevice::initializeData(DMXFrame& frame, int nChannels)
{
	frame.numberChannels = nChannels;
	if (nChannels > 0) {
		frame.dmxData = new unsigned char[nChannels];
		for (int i=0; i<nChannels; i++)
			frame.dmxData[i] = 0;
	}
	else
		frame.dmxData = NULL;
	frame.startCode = 0;
	frame.timeCode = -1;
}

//////////////////////////////////////////////////////////////////////
// sspDmxDeviceGroup Class
//////////////////////////////////////////////////////////////////////

sspDmxDeviceGroup::sspDmxDeviceGroup()
{
	m_strName = _T("DMX");
}

sspDmxDeviceGroup::~sspDmxDeviceGroup()
{
	terminate();
	clearInfo();
}

unsigned int sspDmxDeviceGroup::getAllDevices(void)
{
	if (m_info.empty()) {
		DevEntry* pInfo = new DevEntry;
		DevReturn nRet = getFirstDevice(pInfo);
		while (nRet == SUCCESS) {
			m_info.push_back(pInfo);
			pInfo = new DevEntry;
			nRet = getNextDevice(pInfo);
		}
		delete pInfo;
	}
	return m_info.size();
}

sspString sspDmxDeviceGroup::getDeviceName(int nDevice)
{
	if (nDevice < (int) m_info.size()) {
		return m_info[nDevice]->deviceName;
	}
	else
		return _T("");
}

bool sspDmxDeviceGroup::initializeImpl(LPVOID hWnd)
{
	m_pDevices.reserve(m_nDevices.size());
	for (unsigned int i=0; i<m_nDevices.size(); ++i) {
		sspDmxDevice* pDev = new sspDmxDevice;
		if (m_nDevices[i] < (int) m_info.size() && pDev->open(m_info[m_nDevices[i]]))
			m_pDevices.push_back(pDev);
		else
			return false;
	}
	return true;
}

bool sspDmxDeviceGroup::beginImpl()
{
	for (unsigned int i=0; i<m_pDevices.size(); ++i) {
		if (!m_pDevices[i]->startPlay())
			return false;
	}
	return true;
}

bool sspDmxDeviceGroup::endImpl()
{
	for (unsigned int i=0; i<m_pDevices.size(); ++i) {
		if (!m_pDevices[i]->stopPlay())
			return false;
	}
	return true;
}

bool sspDmxDeviceGroup::terminateImpl()
{
	for (unsigned int i=0; i<m_pDevices.size(); ++i)
		delete m_pDevices[i];
	m_pDevices.clear();
	return true;
}
	
void sspDmxDeviceGroup::clearInfo()
{
	for (unsigned int i=0; i<m_info.size(); i++) {
		delete m_info[i];
	}
	m_info.clear();
}

sspDmxDevice* sspDmxDeviceGroup::getDevice(int nSubsetIndex)
{
	return (nSubsetIndex < (int) m_pDevices.size()) ? m_pDevices[nSubsetIndex] : NULL;
}
