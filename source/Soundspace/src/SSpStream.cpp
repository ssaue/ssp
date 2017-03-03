// SSpStream.cpp: implementation of the SSpStream class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SSpStream.h"
#include "sspSetting.h"
#include "sspPools.h"
#include "debug.h"
#include "sspDeviceManager.h"
#include "sspDirectSoundService.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( SSpStream, CObject, 1 )
IMPLEMENT_SERIAL( SSpTaskStream, SSpStream, 1 )
IMPLEMENT_SERIAL( SSpAudioStream, SSpTaskStream, 1 )

//////////////////////////////////////////////////////////////////////
// sspTaskScheduler
//////////////////////////////////////////////////////////////////////

sspTaskScheduler::sspTaskScheduler(unsigned int nBuffers, unsigned int nWaitBuffers)
: m_nMaxTasks(nBuffers), m_nMaxWaitTasks(nWaitBuffers)
{
}

sspTaskScheduler::~sspTaskScheduler()
{
	clearTasks();
}

bool sspTaskScheduler::loadTask(SSpPlayTask* pTask, sspMsgHandler* pStream, SSpPlayTask*& pFadeTask)
{
	sspAutoLock autolock(m_synch);

  pFadeTask = NULL;
	if (m_activeTasks.size() < m_nMaxTasks) {
		m_activeTasks.push_back(pTask);
		return true;
	}
	else {
		switch (pTask->getPriority()) {
		case SSpPlayTask::CANCEL:
			pTask->execute(false, pStream, NULL);
			return false;
		case SSpPlayTask::WAIT:
			if (m_waitTasks.size() < m_nMaxWaitTasks)
				m_waitTasks.push_back(pTask);
			else
        pTask->execute(false, pStream, pTask->getBuffer());
			return false;
		case SSpPlayTask::LOAD:
			for (TaskElem i = m_activeTasks.begin(); i != m_activeTasks.end(); ++i) {
				if ((*i)->getPriority() < pTask->getPriority()) {
					pFadeTask = *i;
					m_activeTasks.remove(pFadeTask);
					m_activeTasks.push_back(pTask);
					return true;
				}
			}
			// If no space is found
			pTask->execute(false, pStream, pTask->getBuffer());
			return false;
		case SSpPlayTask::LOAD_ALWAYS:
			for (TaskElem i = m_activeTasks.begin(); i != m_activeTasks.end(); ++i) {
				if (*i != pTask) {  // A task cannot stop itself
					pFadeTask = *i;
					m_activeTasks.remove(pFadeTask);
					m_activeTasks.push_back(pTask);
					return true;
				}
			}
			// If no space is found
			pTask->execute(false, pStream, pTask->getBuffer());
			return false;
		default:
			return false;
		}
	}
}

void sspTaskScheduler::removeTask(SSpPlayTask* pTask)
{
	sspAutoLock autolock(m_synch);
	m_activeTasks.remove(pTask);
}

SSpPlayTask* sspTaskScheduler::nextTask()
{
	sspAutoLock autolock(m_synch);
	if (m_activeTasks.size() < m_nMaxTasks && !m_waitTasks.empty()) {
		SSpPlayTask* pTask = m_waitTasks.front();
		m_waitTasks.pop_front();
		if (pTask != NULL) {
			m_activeTasks.push_back(pTask);
			return pTask;
		}
	}
	return NULL;
}

void sspTaskScheduler::clearTasks()
{
	sspAutoLock autolock(m_synch);
	for (TaskElem i = m_activeTasks.begin(); i != m_activeTasks.end(); ++i)
		(*i)->abort();
	m_activeTasks.clear();
	m_waitTasks.clear();
}

//////////////////////////////////////////////////////////////////////
// SSpStream
//////////////////////////////////////////////////////////////////////


SSpStream::SSpStream() : sspMsgHandler(SSP_CLASS_STREAM, 0)
//Later is should be SSpStream::SSpStream(int nType) : sspMsgHandler(SSP_CLASS_STREAM, nType) ????
{
	m_bStopped = false;
	m_pTimeFactor = NULL;
	m_nTimeFacIndex = 1;
	m_nType = SSP_STREAM_TIME;	// Hack for now
}

SSpStream::SSpStream(const SSpStream& in) : sspMsgHandler(SSP_CLASS_STREAM, 0)
{
	m_bStopped = false;
	m_pTimeFactor = NULL;
  m_nSubStream = in.m_nSubStream;
  m_nTimeFacIndex = in.m_nTimeFacIndex;
	m_nType = SSP_STREAM_TIME;	// Hack for now
}

SSpStream::~SSpStream()
{
	m_nSubStream.clear();
}

void SSpStream::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nTimeFacIndex << m_nSubStream.size();
		for (unsigned int i=0; i<m_nSubStream.size(); i++)
			ar << m_nSubStream[i];
	}
	else {
		unsigned int nSize;
		ar >> m_nTimeFacIndex >> nSize;
		m_nSubStream.resize(nSize);
		for (unsigned int i=0; i<nSize; i++)
			ar >> m_nSubStream[i];
	}
}

void SSpStream::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpStream " << m_strName;
	outStr << endl << "   - m_nTimeFacIndex: " << sspPool::Instance().values.GetName(m_nTimeFacIndex);
	for (unsigned int i=0; i<m_nSubStream.size(); i++) {
		outStr << endl << "   - substream " << i << ": " << sspPool::Instance().streams.GetName(m_nSubStream[i]);
	}
}

bool SSpStream::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
	for (unsigned int i=0; i<m_nSubStream.size(); ++i) {
		if (m_nSubStream[i] == m_nIndex || m_nSubStream[i] <= 0 
			|| m_nSubStream[i] > (int) sspPool::Instance().streams.GetSize()) {
			printError(outStr, _T("(SSpStream): a substream index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nTimeFacIndex < 0 || m_nTimeFacIndex >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpStream): m_nTimeFacIndex is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpStream::initialize()
{
	m_bStopped = false;
	m_pTimeFactor = sspPool::Instance().values.getValue(m_nTimeFacIndex);
  return true;
}

void SSpStream::terminate()
{
}

bool SSpStream::begin()
{
	m_bStopped = false;
	for (unsigned int i=0; i<m_nSubStream.size(); i++)
		sspPool::Instance().streams.GetStream(m_nSubStream[i])->begin();
  return true;
}

bool SSpStream::update(double dTime)
{
	for (unsigned int i=0; i<m_nSubStream.size(); i++)
		sspPool::Instance().streams.GetStream(m_nSubStream[i])->update(dTime);
  return true;
}

void SSpStream::end()
{
  m_bStopped = true;
	for (unsigned int i=0; i<m_nSubStream.size(); i++)
		sspPool::Instance().streams.GetStream(m_nSubStream[i])->end();
}

bool SSpStream::isEmpty()
{
  for (unsigned int i=0; i<m_nSubStream.size(); i++) {
    if (!sspPool::Instance().streams.GetStream(m_nSubStream[i])->isEmpty())
      return false;
  }
  return true;
}

inline double SSpStream::getVirtualTime(double dTime)
{
	return (m_pTimeFactor != NULL) ? dTime * m_pTimeFactor->getValue() : dTime;
}

//////////////////////////////////////////////////////////////////////
// SSpTaskStream
//////////////////////////////////////////////////////////////////////

SSpTaskStream::SSpTaskStream()
{
	m_bStopped = FALSE;
	m_nType = SSP_STREAM_TASK;	// Hack for now
}

SSpTaskStream::~SSpTaskStream()
{
}

inline void SSpTaskStream::playTask(SSpPlayTask* pTask)
{
	pTask->execute(true, this, NULL);
}

bool SSpTaskStream::begin()
{
	sspAutoLock autolock(m_synch);
  m_eventList.reset();
  return SSpStream::begin();
}

bool SSpTaskStream::update(double dTime)
{
	sspAutoLock autolock(m_synch);
	if (m_bStopped) return false;
	dTime = getVirtualTime(dTime);
	SSpPlayTask* pTask = m_eventList.getFirstTask(dTime);
	while (pTask != NULL) {
		playTask(pTask);
		pTask = m_eventList.getNextTask();
	}
  return SSpStream::update(dTime);
}

bool SSpTaskStream::isEmpty()
{
	sspAutoLock autolock(m_synch);
  if (!m_eventList.empty())
    return false;
  return SSpStream::isEmpty();;
}

void SSpTaskStream::handleMessage(const SSpMessage& msg)
{
	SSpStream::handleMessage(msg);
	sspAutoLock autolock(m_synch);
	switch (msg.GetMsgType()) {
	case SSP_LOAD_EVENT: {
		m_eventList.loadTask((SSpPlayTask*) msg.GetMsgPara(1).pVal, msg.GetMsgPara(0).iVal);
		break;						 }
	case SSP_SOLO:
  case SSP_MUTE:
		m_bStopped = true;
		break;
	case SSP_UNSOLO:
  case SSP_UNMUTE:
		m_bStopped = false;
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// SSpAudioStream
//////////////////////////////////////////////////////////////////////

SSpAudioStream::SSpAudioStream()
{
	m_pMixer = NULL;
	m_pScheduler = NULL;
	m_nBufCount = 1;
	m_nWaitCount = 0;
	m_nFaderValue = -1;
	m_nType = SSP_STREAM_AUDIO;	// Hack for now
}

SSpAudioStream::~SSpAudioStream()
{
	delete m_pMixer;
	delete m_pScheduler;
}

void SSpAudioStream::Serialize(CArchive& ar)
{
	SSpTaskStream::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nDSDevices.size() << m_nWavDevices.size() << m_nBufCount << m_nWaitCount << m_nFaderValue;
		for (size_t i=0; i<m_nDSDevices.size(); i++)
			ar << m_nDSDevices[i];
		for (size_t i=0; i<m_nWavDevices.size(); i++)
			ar << m_nWavDevices[i];
	}
	else {
		unsigned int nDSsize, nWavsize;

// The next two lines should be enable when updating older files (comment out the succeeding one)
//		bool bStereo;	// temporary hack
//		ar >> bStereo >> nDSsize >> nWavsize >> m_nBufCount >> m_nWaitCount >> m_nFaderValue;
		ar >> nDSsize >> nWavsize >> m_nBufCount >> m_nWaitCount >> m_nFaderValue;
		m_nDSDevices.resize(nDSsize);
		for (size_t i=0; i<nDSsize; i++)
			ar >> m_nDSDevices[i];
		m_nWavDevices.resize(nDSsize);
		for (size_t i=0; i<nWavsize; i++)
			ar >> m_nWavDevices[i];
	}
}

void SSpAudioStream::printASCII(sspOutStream& outStr)
{
	SSpStream::printASCII(outStr);
	outStr << " , m_nBufCount: " << m_nBufCount;
	outStr << " , m_nWaitCount: " << m_nWaitCount;
	outStr << endl << "   - m_nFaderValue " << sspPool::Instance().values.GetName(m_nFaderValue);
	for (unsigned int i=0; i<m_nDSDevices.size(); i++) {
		outStr << endl << "   - DS device " << i << ": " << sspDeviceManager::Instance()[SSP_DEVICE_DIRECTSOUND]->getSubsetName(m_nDSDevices[i]);
	}
	for (unsigned int i=0; i<m_nWavDevices.size(); i++) {
		outStr << endl << "   - Wav device " << i << ": " << sspDeviceManager::Instance()[SSP_DEVICE_WAV]->getSubsetName(m_nWavDevices[i]);
	}
}

bool SSpAudioStream::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
	if (!SSpStream::verify(outStr, nErrors, nWarnings))
		bRet = false;
	for (unsigned int i=0; i<m_nDSDevices.size(); ++i) {
		if (m_nDSDevices[i] < 0 || m_nDSDevices[i] >=  2 * (int) sspDeviceManager::Instance()[SSP_DEVICE_DIRECTSOUND]->getSubsetSize()) {
			printError(outStr, _T("(SSpAudioStream): a DS device is not valid"), nErrors);
			bRet = false;
		}
	}
	for (unsigned int i=0; i<m_nWavDevices.size(); ++i) {
		if (m_nWavDevices[i] < 0 || m_nWavDevices[i] >= 2 * (int) sspDeviceManager::Instance()[SSP_DEVICE_WAV]->getSubsetSize()) {
			printError(outStr, _T("(SSpAudioStream): a Wav device is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nFaderValue >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpAudioStream): m_nFaderValue is not valid"), nErrors);
		bRet = false;
	}
	if (m_nBufCount < 1) {
		printError(outStr, _T("(SSpAudioStream): m_nBufCount is not valid"), nErrors);
		bRet = false;
	}
	if (m_nWaitCount < 0) {
		printError(outStr, _T("(SSpAudioStream): m_nWaitCount is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpAudioStream::initialize()
{
	SSpTaskStream::initialize();
	delete m_pMixer;
	m_pMixer = new SSpStreamMixer(this, m_nBufCount, m_nDSDevices);
	m_pMixer->createMasterFader(m_nWavDevices);
	delete m_pScheduler;
	m_pScheduler = new sspTaskScheduler(m_nBufCount, m_nWaitCount);
	srand( (unsigned)time( NULL ) );
	return true;
}

void SSpAudioStream::terminate()
{
  if (m_pMixer) m_pMixer->terminate();
	SSpTaskStream::terminate();
}

void SSpAudioStream::end()
{
  m_pScheduler->clearTasks();
  SSpTaskStream::end();
}

bool SSpAudioStream::isEmpty()
{
  if (!m_pScheduler->isEmpty())
    return false;
  return SSpTaskStream::isEmpty();;
}

void SSpAudioStream::updateVolumeFader()
{
	if (m_nFaderValue >= 0)
		m_pMixer->masterVolume(sspPool::Instance().values.getValue(m_nFaderValue)->getValue());
}

void SSpAudioStream::handleMessage(const SSpMessage& msg)
{
	SSpTaskStream::handleMessage(msg);
	switch (msg.GetMsgType()) {
	case SSP_IS_DONE: {
		m_pMixer->end();
		SSpPlayTask* pTask = sspPool::Instance().tasks.GetPlayTask(msg.GetMsgPara(0).iVal);
		DOUT1(_T("SSpAudioStream - done task %s\n"), pTask->getName().c_str());
    if (pTask->isPlaying()) return;   // The task has been reassigned
		m_pScheduler->removeTask(pTask);
		if (m_bStopped) break;
		pTask = m_pScheduler->nextTask();
		while (pTask != NULL) {
			DOUT1(_T("SSpAudioStream - next task %s\n"), pTask->getName().c_str());
			if (m_pMixer->begin(pTask))
				break;
			else
				pTask = m_pScheduler->nextTask();
		}
		break;
					  }
	case SSP_SOLO: {
		SSpPlayTask* pTask = sspPool::Instance().tasks.GetPlayTask(msg.GetMsgPara(1).uiVal);
		if (pTask) m_pMixer->bufferSolo(pTask->getBuffer(), (float) msg.GetMsgPara(0).iVal);
		break;
				   }
	case SSP_UNSOLO: {
		SSpPlayTask* pTask = sspPool::Instance().tasks.GetPlayTask(msg.GetMsgPara(1).uiVal);
    if (pTask) m_pMixer->bufferUnSolo(pTask->getBuffer(), (float) msg.GetMsgPara(0).iVal);
		for (unsigned int i=0; i<m_nBufCount; i++) {
			SSpPlayTask* pTask = m_pScheduler->nextTask();
			while (pTask != NULL) {
				if (m_pMixer->begin(pTask))
					break;
				else
					pTask = m_pScheduler->nextTask();
			}
		}
		break;
					 }
	case SSP_MUTE:
		m_pMixer->bufferSolo(NULL, (float) msg.GetMsgPara(0).iVal);
		break;
	case SSP_UNMUTE:
		m_pMixer->bufferUnSolo(NULL, (float) msg.GetMsgPara(0).iVal);
		for (unsigned int i=0; i<m_nBufCount; i++) {
			SSpPlayTask* pTask = m_pScheduler->nextTask();
			while (pTask != NULL) {
				if (m_pMixer->begin(pTask))
					break;
				else
					pTask = m_pScheduler->nextTask();
			}
		}
		break;
	case SSP_SET_VOLUME:
		m_pMixer->masterVolume(msg.GetMsgPara(0).fVal, (float) msg.GetMsgPara(1).iVal);
		break;
	case SSP_ADJUST_VOLUME:
		m_pMixer->masterVolume(msg.GetMsgPara(0).fVal, (float) msg.GetMsgPara(1).iVal, SSpStreamMixer::RELATIVE_REF);
		break;
	default:
		break;
	}
}

inline void SSpAudioStream::playTask(SSpPlayTask* pTask)
{
	SSpPlayTask* pFadeTask = NULL;
	DOUT1(_T("\nSSpAudioStream - enter task %s\n"), pTask->getName().c_str());
	if (m_pScheduler->loadTask(pTask, this, pFadeTask)) {
		DOUT(_T("SSpAudioStream - play task\n"));
		if (pFadeTask == NULL) {
			m_pMixer->begin(pTask);
		}
		else {
      pFadeTask->getBuffer()->setMaxDuration(sspSetting::fade.fadeOut);
			m_pMixer->bufferFadeOut(pFadeTask->getBuffer(), (float) sspSetting::fade.fadeOut);
			m_pMixer->begin(pTask, (float) sspSetting::fade.fadeIn);
		}
	}
}
