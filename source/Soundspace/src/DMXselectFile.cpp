// DMXselectFile.cpp: implementation of the DMXselectFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundSpace.h"
#include "DMXselectFile.h"
#include "sspSetting.h"
#include "SSpBasic.h"
#include "debug.h"
#include "sspDeviceManager.h"
#include "sspDmxDevice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class DMXselectFile
//////////////////////////////////////////////////////////////////////

DMXselectFile::DMXselectFile() : m_file()
{
	m_pIndex = NULL;
	m_pDuration = NULL;
	m_pOwner = NULL;
	m_nFrames = 0;
	m_nChannels = 0;
	m_pDevice = NULL;
	m_nRefreshRate = 0;
	m_pOwner = NULL;
}

DMXselectFile::~DMXselectFile()
{
	if (isPlaying()) end();

	DMXvector::iterator vIterator;
	for (vIterator = m_vFrames.begin(); vIterator != m_vFrames.end(); vIterator++)
		delete [] (*vIterator).dmxData;
	m_vFrames.clear();
}

bool DMXselectFile::open(LPCTSTR pFileName)
{
	using namespace std;
	m_file.open(pFileName, ios_base::in);
	if (m_file.fail()) {
		AfxMessageBox(IDS_FAILOPEN_DMXSELECTFILE);
		return false;
	}
	return true;
}

bool DMXselectFile::readData()
{
	m_file >> m_nFrames >> m_nChannels;
	if (m_file.eof() || m_file.fail() || m_nFrames < 1 || m_nChannels < 1) {
		AfxMessageBox(IDS_FAILREAD_DMXSELECTFILE);
		return false;
	}
	m_vFrames.resize(m_nFrames);
	for (int i=0; i<m_nFrames; i++) {
		sspDmxDevice::initializeData(m_vFrames[i], m_nChannels);
		for (int j=0; j<m_nChannels; j++) {
			unsigned int nValue;
			m_file >> nValue;
			m_vFrames[i].dmxData[j] = (unsigned char) nValue;
		}
		if (m_file.eof()) {
			AfxMessageBox(IDS_FAILREAD_DMXSELECTFILE);
			return false;
		}
	}
	return true;
}

void DMXselectFile::close()
{
	m_file.close();
}

void DMXselectFile::initialize(int nDevice, sspValue* pDur, sspValue* pIndex, int nRefreshRate)
{
	m_pDevice = ((sspDmxDeviceGroup*) sspDeviceManager::Instance()[SSP_DEVICE_DMX])->getDevice(nDevice);
	m_pDuration = pDur;
	m_pIndex = pIndex;
	m_nRefreshRate = nRefreshRate;
}

bool DMXselectFile::begin()
{
	if (m_pDuration != NULL)
		time(&m_startTime);
	int nIndex = (int) (m_pIndex->getValue() + 0.5);	// rounding
	if (nIndex < 0) 
		nIndex = 0;
	else if (nIndex >= m_nFrames)
		nIndex = m_nFrames-1;
	
	m_nNextTime = m_nRefreshRate;
	return (m_pDevice->sendData(&m_vFrames[nIndex]));
}

bool DMXselectFile::update()
{
	if (m_pDuration != NULL && difftime(time(NULL), m_startTime) > m_pDuration->getValue()) {
		DMXFrame frame;
		sspDmxDevice::initializeData(frame, m_nChannels);
		m_pDevice->sendData(&frame);
		m_nNextTime = 0;
		return false;
	}
	else {
		int nIndex = (int) (m_pIndex->getValue() + 0.5);	// rounding
		if (nIndex < 0) 
			nIndex = 0;
		else if (nIndex >= m_nFrames)
			nIndex = m_nFrames-1;
		m_nNextTime = m_nRefreshRate;
		return (m_pDevice->sendData(&m_vFrames[nIndex]));
	}
}
			
void DMXselectFile::end()
{
	DMXFrame frame;
	sspDmxDevice::initializeData(frame, m_nChannels);
	m_pDevice->sendData(&frame);
	m_nNextTime = 0;
	delete [] frame.dmxData;
	if (m_pOwner != NULL) {
		SSpMessage msg;
		msg.SetMsgType(SSP_IS_DONE);
		m_pOwner->handleMessage(msg);
	}
}

unsigned int DMXselectFile::getDuration() const
{
	return (m_pDuration != NULL) ? (unsigned int) m_pDuration->getValue() : 0;
}

void DMXselectFile::setOwner(sspMsgHandler* pOwner)
{
	m_pOwner = pOwner;
}
