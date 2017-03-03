// sspDScontrolFile.cpp: implementation of the sspDScontrolFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspDScontrolFile.h"
#include "sspPlayManager.h"
#include "sspDSchannelControl.h"
#include "sspDirectSoundService.h"
#include "SSpStreamMixer.h"
#include "sspSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <algorithm>

using namespace std;

//////////////////////////////////////////////////////////////////////
// sspDScontrolPoint
//////////////////////////////////////////////////////////////////////

sspDScontrolPoint::sspDScontrolPoint(float fTime, size_t size) : m_fTime(fTime)
{
	m_values.resize(size, 0.0f);
}

//////////////////////////////////////////////////////////////////////
// sspDScontrolFile
//////////////////////////////////////////////////////////////////////

sspDScontrolFile::sspDScontrolFile() : m_file()
{
	m_dDuration = 0.0;
	m_pOwner = NULL;
}

sspDScontrolFile::~sspDScontrolFile()
{
	for (m_vIterator = m_vElement.begin(); m_vIterator != m_vElement.end(); m_vIterator++)
		delete *m_vIterator;
	m_vElement.clear();
}

bool sspDScontrolFile::open(LPCTSTR pFileName)
{
	m_file.open(pFileName, ios_base::in);
	if (m_file.fail()) {
		AfxMessageBox(IDS_FAILOPEN_DSCONTROL);
		return false;
	}
	return true;
}

bool sspDScontrolFile::readData()
{
	float fTime = 0.0f;
	size_t nSize = 0;
	sspDScontrolPoint* pPoint = NULL;

	m_file >> fTime >> nSize;
	if (m_file.eof() || m_file.fail()) {
		AfxMessageBox(IDS_FAILREAD_DSCONTROL);
		return false;
	}
	int nCount = 0;
	m_dDuration = 0.0;
	while (!m_file.eof()) {
		pPoint = new sspDScontrolPoint(fTime, nSize);
		for (size_t i=0; i<nSize; ++i) {
			m_file >> pPoint->m_values[i];
		}
		if (m_file.eof()) break;
		nCount += nSize;
		m_dDuration += fTime;
		m_vElement.push_back(pPoint);
		if (!m_file.eof())
			m_file >> fTime >> nSize;
	}
	return (nCount > 0);
}

void sspDScontrolFile::close()
{
	m_file.close();
}

void sspDScontrolFile::initialize(sspDSbuffer* pBuf)
{
	m_pBuffer = pBuf;
	m_pControl = pBuf->getDSControl();
}

bool sspDScontrolFile::begin()
{
	m_vIterator = m_vElement.begin();
	m_nNextTime = 0;	// Do TimeStep immediately
	return (m_vIterator != m_vElement.end());
}

bool sspDScontrolFile::update()
{
	if (m_vIterator == m_vElement.end() || !m_pBuffer->isLoaded()) {
		m_nNextTime = 0;
		return false;
	}

	sspDScontrolPoint* pPoint = *m_vIterator;
	for (size_t i=0, j=0; i<m_pControl.size(), j<pPoint->m_values.size(); ++i, ++j) {
		bool bLoad = false;
		float fVol;
		if (m_pControl[i]->getMode() == sspDSchannelControl::SSP_STEREO) {
			float fLeft = pPoint->m_values[j];
			++j;
			float fRight = (j < pPoint->m_values.size()) ? pPoint->m_values[j] : fLeft;
			fVol = max(fLeft,fRight);
			bLoad = m_pControl[i]->setPanRamp(fRight - fLeft, pPoint->m_fTime);
		}
		else {
			fVol = pPoint->m_values[j];
		}
		if (m_pControl[i]->setVolumeRamp(fVol, pPoint->m_fTime) || bLoad) {
			sspPlayManager::Instance().getScheduler().addObject(m_pControl[i]);
		}
	}

	m_nNextTime = (int) (1000 * pPoint->m_fTime);
	m_vIterator++;
	return true;
}
			
void sspDScontrolFile::end()
{
	m_nNextTime = 0;
	m_vIterator = m_vElement.end();
	if (m_pOwner != NULL) {
		SSpMessage msg;
		msg.SetMsgType(SSP_IS_DONE);
		m_pOwner->handleMessage(msg);
	}
}

unsigned int sspDScontrolFile::getDuration() const
{
	return (unsigned int) m_dDuration;
}

void sspDScontrolFile::setOwner(sspMsgHandler* pOwner)
{
	m_pOwner = pOwner;
}

