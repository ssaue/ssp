// sspDeviceManager.cpp

#include "stdafx.h"
#include "sspDeviceManager.h"
#include "sspDeviceGroups.h"
#include "sspDirectSoundDevice.h"
#include "sspWavDevice.h"
#include "sspDmxDevice.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// sspDeviceManager Class
//////////////////////////////////////////////////////////////////////

sspDeviceManager* sspDeviceManager::m_pInstance = NULL;
bool sspDeviceManager::m_bDestroyed = false;

sspDeviceManager::sspDeviceManager()
{
	m_pGroups.resize(SSP_DEVICE_COUNT);
	m_pGroups[SSP_DEVICE_MIDIIN] = new sspMidiInDeviceGroup;
	m_pGroups[SSP_DEVICE_MIDIOUT] = new sspMidiOutDeviceGroup;
	m_pGroups[SSP_DEVICE_DIRECTSOUND] = new sspDSDeviceGroup;
	m_pGroups[SSP_DEVICE_WAV] = new sspWavOutDeviceGroup;
	m_pGroups[SSP_DEVICE_DMX] = new sspDmxDeviceGroup;
}

sspDeviceManager::sspDeviceManager(const sspDeviceManager&)
{
}

sspDeviceManager& sspDeviceManager::operator=(const sspDeviceManager&)
{
	return *this;
}

sspDeviceManager::~sspDeviceManager()
{
	m_pInstance = NULL;
	m_bDestroyed = true;
	clearContents();
	for (unsigned int i=0; i<m_pGroups.size(); ++i)
		delete m_pGroups[i];
	m_pGroups.clear();
}

sspDeviceManager& sspDeviceManager::Instance()
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

void sspDeviceManager::Serialize(CArchive& ar)
{
	m_pGroups[SSP_DEVICE_MIDIIN]->Serialize(ar);
	m_pGroups[SSP_DEVICE_MIDIOUT]->Serialize(ar);
	m_pGroups[SSP_DEVICE_DIRECTSOUND]->Serialize(ar);
	m_pGroups[SSP_DEVICE_WAV]->Serialize(ar);
	m_pGroups[SSP_DEVICE_DMX]->Serialize(ar);
}

void sspDeviceManager::printASCII(sspOutStream& outStr)
{
	outStr << endl << "sspDeviceManager";
	for (unsigned int i=0; i<m_pGroups.size(); ++i)
		m_pGroups[i]->printASCII(outStr);
}

bool sspDeviceManager::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_pGroups.size(); ++i) {
		if (!m_pGroups[i]->verify(outStr, nErrors, nWarnings)) {
			bRet = false;
		}
	}
	return bRet;
}

bool sspDeviceManager::initialize(LPVOID hWnd)
{
	for (unsigned int i=0; i<m_pGroups.size(); ++i) {
		if (!m_pGroups[i]->initialize(hWnd)) {
			CString txt;
			txt.Format(_T("sspDeviceManager::Error in initializing device group %d"), i);
			AfxMessageBox(txt);
			return false;
		}
	}
	return true;
}

bool sspDeviceManager::start()
{
	for (unsigned int i=0; i<m_pGroups.size(); ++i) {
		if (!m_pGroups[i]->begin())
			return false;
	}
	return true;
}

bool sspDeviceManager::stop()
{
	for (unsigned int i=0; i<m_pGroups.size(); ++i) {
		if (!m_pGroups[i]->end())
			return false;
	}
	return true;
}

bool sspDeviceManager::terminate()
{
	for (unsigned int i=0; i<m_pGroups.size(); ++i) {
		if (!m_pGroups[i]->terminate())
			return false;
	}
	return true;
}

void sspDeviceManager::clearContents()
{
	terminate();
}

void sspDeviceManager::create()
{
	static sspDeviceManager theInstance;
	m_pInstance = &theInstance;
}

void sspDeviceManager::onDeadReference()
{
	throw std::runtime_error("Dead reference to device pool detected");
}
