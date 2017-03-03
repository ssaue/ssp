// SSpStreamMixer.cpp: implementation of the SSpStreamMixer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "debug.h"
#include "SSpStreamMixer.h"
#include "SSpTasks.h"
#include "sspPlayManager.h"
#include "sspDSchannelControl.h"
#include "sspDirectSoundService.h"
#include "sspPools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// SSpStreamMixer
//////////////////////////////////////////////////////////////////////

SSpStreamMixer::SSpStreamMixer(sspMsgHandler* pStream, unsigned int nMaxBuf, const sspIntVec& nChans)
: m_pStream(pStream), m_nChannels(nChans), m_nMaxBuffers(nMaxBuf)
{
}

SSpStreamMixer::~SSpStreamMixer()
{
}

void SSpStreamMixer::createMasterFader(const sspIntVec& nChans)
{
	m_masterFader.create(nChans);
}

bool SSpStreamMixer::begin(SSpPlayTask* pTask, float fTime)
{
	// Does not check on number of channels - allowing crossfades
	sspAutoLock autolock(m_synch);
	sspDSbuffer* pBuf = new sspDSbuffer(
			m_nChannels, 
			sspPool::Instance().values.getValue(pTask->getVolumeFactor())->getValue(),
			fTime);
	bool bExecuteOK = pTask->execute(true, m_pStream, pBuf);
	if (bExecuteOK && pTask->getResourceUsage(SSP_WAV) > 0) {
		DOUT1(_T("SSpStreamMixer begin %s successfully\n"), pTask->getName().c_str());
		m_pBuffers.push_back(pBuf);
	}
	else {
		DOUT1(_T("SSpStreamMixer begin %s failed\n"), pTask->getName().c_str());
		pTask->clearBuffer();
		delete pBuf;
	}
	return bExecuteOK;
}

void SSpStreamMixer::end()
{
	sspAutoLock autolock(m_synch);
	DOUT1(_T("SSpStreamMixer end - current size: %d\n"), m_pBuffers.size());
	for (sspDSbufferElem i = m_pBuffers.begin(); i != m_pBuffers.end(); ++i) {
		if (!(*i)->isPlaying()) {
			delete (*i);
			m_pBuffers.erase(i);
			break;
		}
	}
}

void SSpStreamMixer::terminate()
{
	sspAutoLock autolock(m_synch);
	for (sspDSbufferElem i = m_pBuffers.begin(); i != m_pBuffers.end(); ++i)
		delete *i;
	m_pBuffers.clear();
	m_masterFader.end();
}

bool SSpStreamMixer::masterVolume(float fVolume, float fTime, int nRef)
{
	if (nRef == RELATIVE_REF) 
		fVolume += m_masterFader.getVolume();
	if (fVolume < 0.0) 
		fVolume = 0.0;
	else if (fVolume > 1.0)	
		fVolume = 1.0;
	if (m_masterFader.setVolumeRamp(fVolume, fTime)) {
		sspPlayManager::Instance().getScheduler().addObject(&m_masterFader);
	}
	return true;
}

bool SSpStreamMixer::masterFadeIn(float fTime)
{
	if (m_masterFader.setFadeRamp(true, fTime)) {
		sspPlayManager::Instance().getScheduler().addObject(&m_masterFader);
	}
	return true;
}

bool SSpStreamMixer::masterFadeOut(float fTime)
{
	if (m_masterFader.setFadeRamp(false, fTime)) {
		sspPlayManager::Instance().getScheduler().addObject(&m_masterFader);
	}
	return true;
}

bool SSpStreamMixer::bufferVolume(sspDSbuffer* pBuf, float fVolume, float fTime, int nRef)
{
	if (pBuf == NULL) return false;
	if (nRef == RELATIVE_REF) 
		fVolume += m_masterFader.getVolume();
	if (fVolume < 0.0) 
		fVolume = 0.0;
	else if (fVolume > 1.0)	
		fVolume = 1.0;
	sspDSControlList pDSClist = pBuf->getDSControl();
	for (size_t i=0; i<pDSClist.size(); ++i) {
		if (pDSClist[i]->setVolumeRamp(fVolume, fTime)) {
			sspPlayManager::Instance().getScheduler().addObject(pDSClist[i]);
		}
	}
	return true;
}

bool SSpStreamMixer::bufferFadeIn(sspDSbuffer* pBuf, float fTime)
{
	if (pBuf == NULL) return false;
	sspDSControlList pDSClist = pBuf->getDSControl();
	for (size_t i=0; i<pDSClist.size(); ++i) {
		if (pDSClist[i]->setFadeRamp(true, fTime)) {
			sspPlayManager::Instance().getScheduler().addObject(pDSClist[i]);
		}
	}
	return true;
}

bool SSpStreamMixer::bufferFadeOut(sspDSbuffer* pBuf, float fTime)
{
	if (pBuf == NULL) return false;
	sspDSControlList pDSClist = pBuf->getDSControl();
	for (size_t i=0; i<pDSClist.size(); ++i) {
		if (pDSClist[i]->setFadeRamp(false, fTime)) {
			sspPlayManager::Instance().getScheduler().addObject(pDSClist[i]);
		}
	}
	return true;
}

bool SSpStreamMixer::bufferSolo(sspDSbuffer* pBuf, float fTime)
{
	for (sspDSbufferElem i = m_pBuffers.begin(); i != m_pBuffers.end(); ++i) {
		if (*i != pBuf)	bufferFadeOut(*i, fTime);
	}
	if (pBuf != NULL) bufferFadeIn(pBuf, fTime);

	return true;
}

bool SSpStreamMixer::bufferUnSolo(sspDSbuffer* pBuf, float fTime)
{
	for (sspDSbufferElem i = m_pBuffers.begin(); i != m_pBuffers.end(); ++i) {
		if (*i != pBuf)	bufferFadeIn(*i, fTime);
	}
	return true;
}
