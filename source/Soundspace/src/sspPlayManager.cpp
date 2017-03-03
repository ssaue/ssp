//////////////////////////////////////////
// sspPlayManager.h

#include "stdafx.h"
#include "sspPlayManager.h"
#include "SSpStream.h"
#include "sspPools.h"
#include "sspDeviceManager.h"
#include "sspInputManager.h"
#include "sspDateTimeManager.h"
#include "sspSetting.h"

using namespace std;

sspPlayManager* sspPlayManager::m_pInstance = NULL;
bool sspPlayManager::m_bDestroyed = false;

sspPlayManager::sspPlayManager(void)
: m_update(sspTimeConvert::secs2time(1.0f)) // Check play thread every second
{
	m_bPlaying = false;
}

sspPlayManager::sspPlayManager(const sspPlayManager& pm)
{
}

sspPlayManager& sspPlayManager::operator=(const sspPlayManager&)
{
	return *this;
}

sspPlayManager::~sspPlayManager(void)
{
	m_pInstance = NULL;
	m_bDestroyed = true;
	clearContents();
}

sspPlayManager& sspPlayManager::Instance()
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

void sspPlayManager::Serialize(CArchive& ar)
{
 	m_listStart.Serialize(ar);
	m_listTrigger.Serialize(ar);
	m_listClock.Serialize(ar);
}

void sspPlayManager::printASCII(sspOutStream& outStr)
{
	outStr << endl << "sspPlayManager";
	if (m_listStart.size() > 0) {
		outStr << endl << "- list of initial events: ";
		m_listStart.printASCII(outStr);
		outStr << endl;
	}
	if (m_listTrigger.size() > 0) {
		outStr << endl << "- list of triggered events: ";
		m_listTrigger.printASCII(outStr);
		outStr << endl;
	}
	if (m_listClock.size() > 0) {
		outStr << endl << "- list of time events: ";
		m_listClock.printASCII(outStr);
		outStr << endl;
	}
}

bool sspPlayManager::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
	if (!m_listStart.verify(outStr, nErrors, nWarnings))
		bRet = false;
	if (!m_listTrigger.verify(outStr, nErrors, nWarnings))
		bRet = false;
	if (!m_listClock.verify(outStr, nErrors, nWarnings))
		bRet = false;
	return bRet;
}

bool sspPlayManager::initialize()
{
	if (!sspPool::Instance().initialize()) {
		return false;
	}
  m_scheduler.initialize(sspSetting::res.timer);
  m_pRootStream = sspPool::Instance().streams.getRoot();
	m_pRootStream->initialize();
	return true;
}

bool sspPlayManager::start()
{
	if (!m_bPlaying) {
		sspDeviceManager::Instance().start();
		sspInputManager::Instance().start();
		sspInputManager::Instance().update();
    m_previous.getCurrentTime();
    m_startRun.getCurrentTime();
    m_pRootStream->begin();
	  m_listTrigger.reset();
	  m_listClock.reset();
	  m_listStart.send(NULL);
    m_scheduler.start();
    m_update.initialize();
		m_bPlaying = true;
	}
  return m_bPlaying;
}

bool sspPlayManager::update()
{
	if (m_bPlaying) {
    if (sspDateTimeManager::Instance().isUpdated()) {
      m_listClock.testAndSend();
      m_listTrigger.testAndSend();
      adjustVolume();
    }
    else if (sspInputManager::Instance().isUpdated())  {
      m_listTrigger.testAndSend();
      adjustVolume();
    }

    sspTime newTime;
    m_pRootStream->update(sspTimeConvert::time2secs(newTime - m_previous));
    m_previous = newTime;

    return !m_scheduler.isEmpty() || !m_pRootStream->isEmpty();
	}
  return false;
}

void sspPlayManager::stop()
{
	if (m_bPlaying) {
    // Important: Must stop the streams before stopping the scheduler!
    // Otherwise sequence objects may continue
    m_pRootStream->end();
    m_scheduler.stop();
		sspInputManager::Instance().stop();
		sspDeviceManager::Instance().stop();
		m_bPlaying = false;
	}
}

void sspPlayManager::terminate()
{
  stop();
  m_scheduler.terminate();
  sspPool::Instance().terminate();
}

void sspPlayManager::clearContents()
{
	terminate();
  m_listStart.removeAll();
  m_listClock.removeAll();
  m_listTrigger.removeAll();
}

bool sspPlayManager::verifyRunning()
{
  return (m_bPlaying && m_update.update()) ? m_scheduler.verifyRunning(1) : true;
}


void sspPlayManager::adjustVolume()
{
	for (unsigned int i=0; i<sspPool::Instance().streams.GetSize(); i++) {
		SSpAudioStream* pStr = dynamic_cast<SSpAudioStream*> (sspPool::Instance().streams.GetStream(i));
		if (pStr != NULL && pStr->getFaderValue() >= 0) pStr->updateVolumeFader();
  }
}

void sspPlayManager::create()
{
	static sspPlayManager theInstance;
	m_pInstance = &theInstance;
}

void sspPlayManager::onDeadReference()
{
	throw std::runtime_error("Dead reference to play manager detected");
}
