///////////////////////////////////////////
// Scheduler.cpp : implementation file
///////////////////////////////////////////

#include "StdAfx.h"
#include "sspScheduler.h"
#include <sspTimer.h>

#include "debug.h"

//////////////////////////////////////////////////////////////////////
// sspScheduleObject Class
//////////////////////////////////////////////////////////////////////

sspScheduleObject::sspScheduleObject()
{
	m_bPlaying = false;
	m_nNextTime = 0;
}

//////////////////////////////////////////////////////////////////////
// sspScheduler Class
//////////////////////////////////////////////////////////////////////

sspScheduler::sspScheduler()
{
  m_pTimer = NULL;
	m_bCountDown = true;
}

sspScheduler::~sspScheduler()
{
  terminate();
}

bool sspScheduler::addObject(sspScheduleObject* pObj)
{
	sspAutoLock autolock(m_synch);
  if (!pObj->begin() || pObj->isPlaying()) {
		DOUT(_T("sspScheduler addObject failed\n"));
		return false;
  }
	else if (m_list.empty()) {
		m_list.push_front(pObj);
		m_bCountDown = pObj->m_nNextTime > SSP_DEFAULT_TIME;
		if (m_bCountDown)
			pObj->m_nNextTime -= SSP_DEFAULT_TIME;
	}
	else {
		tElem iter;
		for (iter = m_list.begin(); iter != m_list.end(); ++iter) {
			if (pObj->m_nNextTime >= (*iter)->m_nNextTime) {
				pObj->m_nNextTime -= (*iter)->m_nNextTime;
			}
			else {
				(*iter)->m_nNextTime -= pObj->m_nNextTime;
				m_list.insert(iter, pObj);
				pObj->setPlaying();
				break;
			}
		}
		if (iter == m_list.end()) {
			m_list.push_back(pObj);
			pObj->setPlaying();
		}
	}
	return true;
}

bool sspScheduler::verifyRunning(int nWaitSecs)
{
  bool bRet = (nWaitSecs >= 0) ? m_event.timedWait(nWaitSecs * 1000) : m_event.wait();

	sspAutoLock autolock(m_synch);
  m_event.reset();
  return bRet;
}

void sspScheduler::clear()
{
	sspAutoLock autolock(m_synch);
	m_list.clear();
}

bool sspScheduler::initialize(unsigned int nResolution)
{
  delete m_pTimer;
  m_pTimer = new sspOneShotTimer(nResolution);
  return m_pTimer->isInitialized();
}

bool sspScheduler::start()
{
  return m_pTimer && m_pTimer->run(this);
}

void sspScheduler::stop()
{
  if (m_pTimer) m_pTimer->stop();
  clear();
}

void sspScheduler::terminate()
{
  stop();
  delete m_pTimer;
  m_pTimer = NULL;
}

bool sspScheduler::begin(sspTimerType& nMSecs)
{
	sspAutoLock autolock(m_synch);
	if (m_list.empty()) {
		m_bCountDown = true;
		nMSecs = SSP_DEFAULT_TIME;
	}
	else if (m_list.front()->m_nNextTime > SSP_DEFAULT_TIME) {
		m_list.front()->m_nNextTime -= SSP_DEFAULT_TIME;
		m_bCountDown = true;
		nMSecs = SSP_DEFAULT_TIME;
	}
	else {
		m_bCountDown = false;
		nMSecs = m_list.front()->m_nNextTime;
	}
  m_event.set();
  return true;
}

bool sspScheduler::update(sspTimerType& nMSecs)
{
	sspAutoLock autolock(m_synch);
	if (!m_bCountDown) {
		sspScheduleObject* pObj = m_list.front();
		m_list.pop_front();
		if (pObj->update()) {
			tElem iter;
			for (iter = m_list.begin(); iter != m_list.end(); ++iter) {
				if (pObj->m_nNextTime >= (*iter)->m_nNextTime) {
					pObj->m_nNextTime -= (*iter)->m_nNextTime;
				}
				else {
					(*iter)->m_nNextTime -= pObj->m_nNextTime;
					m_list.insert(iter, pObj);
					pObj->setPlaying();
					break;
				}
			}
			if (iter == m_list.end()) {
				m_list.push_back(pObj);
				pObj->setPlaying();
			}
		}
		else {
			pObj->setPlaying(false);
			pObj->end();
		}
	}
	if (m_list.empty()) {
		m_bCountDown = true;
		nMSecs = SSP_DEFAULT_TIME;
	}
	else if (m_list.front()->m_nNextTime > SSP_DEFAULT_TIME) {
		m_list.front()->m_nNextTime -= SSP_DEFAULT_TIME;
		m_bCountDown = true;
		nMSecs = SSP_DEFAULT_TIME;
	}
	else {
		m_bCountDown = false;
		nMSecs = m_list.front()->m_nNextTime;
	}
  m_event.set();
//  DOUT1 ("sspScheduler update msecs %d\n", nMSecs);
  return true;
}

void sspScheduler::end()
{
	sspAutoLock autolock(m_synch);
	for (tElem iter = m_list.begin(); iter != m_list.end(); ++iter) {
		(*iter)->setPlaying(false);
		(*iter)->end();
	}
}
