// SSpTasks.cpp: implementation of the SSpPlayTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SSpTasks.h"
#include "sspPools.h"
#include "SSpBasic.h"
#include "sspDirectSoundService.h"
using namespace std;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( SSpPlayTask, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// sspPlayObject
//////////////////////////////////////////////////////////////////////

sspPlayObject::sspPlayObject(int nType) 
: sspMsgHandler(SSP_CLASS_PLAYOBJECT, nType)
, m_nPlayCount(0)
{
	for (int i=0; i<SSP_TYPES; i++)
		m_nResourceUsage[i] = 0;
}

sspPlayObject::~sspPlayObject()
{
}

bool sspPlayObject::update()
{
  m_nPlayCount--;
  return false;
}

void sspPlayObject::end()
{
  m_nPlayCount--;
}

void sspPlayObject::handleMessage(const SSpMessage& msg)
{
	if (msg.GetMsgType() == SSP_IS_DONE && isPlaying() && !update()) {
	  SSpMessage msg;
	  msg.SetMsgType(SSP_IS_DONE);
	  m_pOwner->handleMessage(msg);
	}
}

//////////////////////////////////////////////////////////////////////
// SSpPlayTask
//////////////////////////////////////////////////////////////////////

SSpPlayTask::SSpPlayTask() 
: sspMsgHandler(SSP_CLASS_PLAYOBJECT, 0)
,	m_nPriority(CANCEL)
, m_nCondIndex(1)     // default conditional true
, m_nPlayObject(-1)
, m_nVolumeFactor(1)  // default value 1
,	m_bPlaying(false)
, m_pBuffer(NULL)
{
	for (int i=0; i<SSP_TYPES; i++)
		m_nResourceUsage[i] = 0;
}

SSpPlayTask::SSpPlayTask(const SSpPlayTask& task)
: sspMsgHandler(task)
,	m_nPriority(task.m_nPriority)
, m_nCondIndex(task.m_nCondIndex)     // default conditional true
, m_nPlayObject(task.m_nPlayObject)
, m_nVolumeFactor(task.m_nVolumeFactor)  // default value 1
,	m_bPlaying(false)
, m_pBuffer(NULL)
{
	for (int i=0; i<SSP_TYPES; i++)
		m_nResourceUsage[i] = task.m_nResourceUsage[i];
  for (int i=0; i<TASK_EVENT_TYPES; i++) {
    m_messages[i] = task.m_messages[i];
  }
}

SSpPlayTask& SSpPlayTask::operator= (const SSpPlayTask& task)
{
	sspMsgHandler::operator=(task);
	m_nPriority = task.m_nPriority;
	m_nCondIndex = task.m_nCondIndex;
	m_nPlayObject = task.m_nPlayObject;
	m_nVolumeFactor = task.m_nVolumeFactor;
	m_pBuffer = NULL;
	m_bPlaying = false;
	for (int i=0; i<SSP_TYPES; i++)
		m_nResourceUsage[i] = task.m_nResourceUsage[i];
  for (int i=0; i<TASK_EVENT_TYPES; i++) {
    m_messages[i] = task.m_messages[i];
  }
	return *this;
}

SSpPlayTask::~SSpPlayTask()
{
  for (int i=0; i<TASK_EVENT_TYPES; i++) {
    m_messages[i].removeAll();
  }
}

void SSpPlayTask::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nCondIndex << m_nPlayObject << m_nPriority << m_nVolumeFactor;
	}
	else {
		ar >> m_nCondIndex >> m_nPlayObject >> m_nPriority >> m_nVolumeFactor;
	}
	for (int i=0; i<TASK_EVENT_TYPES; i++)
		m_messages[i].Serialize(ar);
}

void SSpPlayTask::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpPlayTask " << m_strName;
	outStr << endl << "   - m_nPlayObject: " << sspPool::Instance().objects.GetName(m_nPlayObject);
	outStr << endl << "   - m_nCondIndex: " << sspPool::Instance().conditionals.GetName(m_nCondIndex);
	switch (m_nPriority) {
		case CANCEL:
			outStr << " , m_nPriority: Cancel";
			break;
		case WAIT:
			outStr << " , m_nPriority: Wait";
			break;
		case LOAD:
			outStr << " , m_nPriority: Load now";
			break;
		case LOAD_ALWAYS:
			outStr << " , m_nPriority: Play always";
			break;
		default:
			outStr << " , m_nPriority: unknown";
			break;
	}
	outStr << endl << "   - m_nVolumeFactor: " << sspPool::Instance().values.GetName(m_nVolumeFactor);
	outStr << endl << "   - m_nTask enter messages";
	m_messages[EVENT_ENTER].printASCII(outStr);
	outStr << endl << "   - m_nTask start messages";
	m_messages[EVENT_START].printASCII(outStr);
	outStr << endl << "   - m_nTask end messages";
	m_messages[EVENT_END].printASCII(outStr);
	outStr << endl << "   - m_nTask exit messages";
	m_messages[EVENT_EXIT].printASCII(outStr);
}

bool SSpPlayTask::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nPlayObject < 0 || m_nPlayObject > (int) sspPool::Instance().objects.GetSize()) {
		printError(outStr, _T("(SSpPlayTask): m_nPlayObject is not valid"), nErrors);
		bRet = false;
	}
	if (m_nCondIndex < 0 || m_nCondIndex > (int) sspPool::Instance().conditionals.GetSize()) {
		printError(outStr, _T("(SSpPlayTask): m_nCondIndex is not valid"), nErrors);
		bRet = false;
	}
	if (m_nVolumeFactor < 0 || m_nVolumeFactor > (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpPlayTask): m_nVolumeFactor is not valid"), nErrors);
		bRet = false;
	}
	if (m_nPriority < 0 || m_nPriority > LOAD_ALWAYS) {
		printError(outStr, _T("(SSpPlayTask): m_nPriority is not valid"), nErrors);
		bRet = false;
	}
	for (int i=0; i<TASK_EVENT_TYPES; ++i) {
		if (!m_messages[i].verify(outStr, nErrors, nWarnings)) {
			outStr << endl << "  (SSpPlayTask): error in messagelist " << i;
			bRet = false;
		}
	}
	if (m_messages[EVENT_START].size() != m_messages[EVENT_END].size()) {
		printWarning(outStr, _T("(SSpPlayTask): start and end messages do not match"), nWarnings);
		bRet = false;
	}
	if (m_messages[EVENT_ENTER].size() == 0 && m_messages[EVENT_EXIT].size() == 0) {
		printWarning(outStr, _T("(SSpPlayTask): no enter or exit messages"), nWarnings);
		bRet = false;
	}
	return bRet;
}

void SSpPlayTask::setMsgList(int nType, int nCondition, sspMsgList* pMsgs)
{
  m_messages[nType].add(nCondition, pMsgs);
}

bool SSpPlayTask::initialize()
{
	m_bPlaying = false;
	for (int j=0; j<SSP_TYPES; j++)
		m_nResourceUsage[j] = sspPool::Instance().objects.GetPlayObject(m_nPlayObject)->getResourceUsage(j);
	return true;
}

bool SSpPlayTask::execute(bool bReady, sspMsgHandler* pOwner, sspDSbuffer* pBuf)
{
	m_pBuffer = pBuf;
	m_pOwner = pOwner;
	m_messages[EVENT_ENTER].send(m_pOwner, this, m_nIndex);
	m_bPlaying = bReady 
				&& sspPool::Instance().conditionals.GetConditional(m_nCondIndex)->isTrue()
				&& sspPool::Instance().objects.GetPlayObject(m_nPlayObject)->begin(pBuf, this);
  if (m_bPlaying) {
		m_messages[EVENT_START].send(m_pOwner, this, m_nIndex);
  }
  else {
    SSpMessage msg;
		msg.SetMsgType(SSP_IS_DONE);
		msg.GetMsgPara(0).iVal = m_nIndex;
		m_pOwner->handleMessage(msg);
    m_messages[EVENT_EXIT].send(m_pOwner, this, m_nIndex);
  }
	return m_bPlaying;
}

void SSpPlayTask::abort()
{
	if (m_bPlaying) {
		sspPool::Instance().objects.GetPlayObject(m_nPlayObject)->end();
		m_bPlaying = FALSE;
		m_messages[EVENT_END].send(m_pOwner, this, m_nIndex);
		m_messages[EVENT_EXIT].send(m_pOwner, this, m_nIndex);
	}
}

unsigned int SSpPlayTask::getDuration()
{
	return sspPool::Instance().objects.GetPlayObject(m_nPlayObject)->estimateDuration();
}

void SSpPlayTask::handleMessage(const SSpMessage& msg)
{
	if (msg.GetMsgType() == SSP_IS_DONE) {
		if (m_pOwner == NULL) return;
    if (m_pBuffer && !m_pBuffer->isPlaying()) {
      m_bPlaying = false;
      clearBuffer();
    }

		SSpMessage newMsg = msg;
		newMsg.GetMsgPara(0).iVal = m_nIndex;
		m_pOwner->handleMessage(newMsg);
		m_messages[EVENT_END].send(m_pOwner, this, m_nIndex);
		m_messages[EVENT_EXIT].send(m_pOwner, this, m_nIndex);
	}
}

//////////////////////////////////////////////////////////////////////
// sspTimedTask
//////////////////////////////////////////////////////////////////////

sspTimedTask::sspTimedTask(unsigned int nTime, SSpPlayTask* pTask)
{
	m_nTime = nTime;
	m_pTask = pTask;
}

sspTimedTask::~sspTimedTask()
{
	// The tasks are removed elsewhere
}

//////////////////////////////////////////////////////////////////////
// sspTimedTaskList
//////////////////////////////////////////////////////////////////////

sspTimedTaskList::sspTimedTaskList()
{
	m_dTimeCounter = 0.0;
}

sspTimedTaskList::~sspTimedTaskList()
{
  reset();
}

void sspTimedTaskList::reset()
{
  iterator iter;
  for (iter=begin(); iter != end(); ++iter)
    delete (*iter);
  clear();
  m_dTimeCounter = 0.0f;
}

void sspTimedTaskList::loadTask(SSpPlayTask* pTask, unsigned int nTime)
{
	sspTimedTask* pTE = new sspTimedTask(nTime, pTask);

  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
		if (pTE->m_nTime > (*iter)->m_nTime) {
			pTE->m_nTime -= (*iter)->m_nTime;
		}
		else {
			(*iter)->m_nTime -= pTE->m_nTime;
      insert(iter, pTE);
			break;
		}
  }
  if (iter == end()) {
    push_back(pTE);
  }
}

SSpPlayTask* sspTimedTaskList::getFirstTask(double dTimeStep)
{
	m_dTimeCounter += dTimeStep;
  return getNextTask();
}

SSpPlayTask* sspTimedTaskList::getNextTask()
{
  if (empty()) {
		m_dTimeCounter = 0.0f;
    return NULL;
  }
	if (m_dTimeCounter > front()->m_nTime) {
		m_dTimeCounter -= front()->m_nTime;
		SSpPlayTask* pTask = front()->m_pTask;
    delete front();
    pop_front();
		return pTask;
	}
	else {
		return NULL;
	}
}
