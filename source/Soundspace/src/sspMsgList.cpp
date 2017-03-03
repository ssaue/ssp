// sspMsgList.cpp: implementation of the message list classes
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sspMsgList.h"
#include "sspPools.h"
#include "SSpConditionals.h"
#include "sspDateTimeManager.h"

using namespace std;

IMPLEMENT_SERIAL( sspMsgList, CObject, 1 )
IMPLEMENT_SERIAL( sspCondMsgList, CObject, 1 )
IMPLEMENT_SERIAL( sspTriggerMsgList, CObject, 1 )
IMPLEMENT_SERIAL( sspClockMsgList, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// sspMsgList
//////////////////////////////////////////////////////////////////////

sspMsgList::sspMsgList()
{
	m_strName = _T("ml");
}

sspMsgList::sspMsgList(const sspMsgList& msgs) : sspIObase(msgs)
{
  list<SSpStreamMsg*>::const_iterator iter;
  for(iter = msgs.begin(); iter != msgs.end(); ++iter) {
    add(new SSpStreamMsg(**iter));
  }
}

sspMsgList& sspMsgList::operator= (const sspMsgList& msgs)
{
	sspIObase::operator=(msgs);
  removeAll();
  list<SSpStreamMsg*>::const_iterator iter;
  for(iter = msgs.begin(); iter != msgs.end(); ++iter) {
    add(new SSpStreamMsg(**iter));
  }
	return *this;
}

sspMsgList::~sspMsgList()
{
	removeAll();
}

void sspMsgList::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
    ar << size();
    iterator iter;
    for (iter = begin(); iter != end(); ++iter) {
		  ar << (*iter);
	  }
	}
	else {
		unsigned int nSize;
		ar >> nSize;
    for (unsigned int i=0; i<nSize; i++) {
      SSpStreamMsg* pNew;
			ar >> pNew;
      add(pNew);
    }
	}
}

void sspMsgList::printASCII(sspOutStream& outStr)
{
  int i;
  iterator iter;
  for (iter = begin(), i=1; iter != end(); ++iter, ++i) {
		outStr << endl << "       " << i << ") ";
		(*iter)->printASCII(outStr);
	}
}

bool sspMsgList::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
  int i;
  iterator iter;
  for (iter = begin(), i=1; iter != end(); ++iter, ++i) {
		if (!(*iter)->verify(outStr, nErrors, nWarnings)) {
			outStr << " in message " << i;
			bRet = false;
		}
	}
	return bRet;
}

void sspMsgList::removeAll()
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter)
    delete (*iter);
  clear();
}

void sspMsgList::remove(SSpStreamMsg* pMessage)
{
  list<SSpStreamMsg*>::remove(pMessage);
  delete pMessage;
}

void sspMsgList::send(sspMsgHandler* pTask)
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter)
    (*iter)->Send(pTask);
}

void sspMsgList::send(sspMsgHandler* pStream, sspMsgHandler* pTask, int nTag)
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter)
    (*iter)->Send(pStream, pTask, nTag);
}

//////////////////////////////////////////////////////////////////////
// sspCondMsgList
//////////////////////////////////////////////////////////////////////

sspCondMsgList::sspCondMsgList()
{
	m_strName = _T("cml");
}

sspCondMsgList::sspCondMsgList(const sspCondMsgList& msgs)  : sspIObase(msgs)
{
  const_iterator iter;
  for(iter = msgs.begin(); iter != msgs.end(); ++iter) {
    add(iter->first, new sspMsgList(*iter->second));
  }
}

sspCondMsgList& sspCondMsgList::operator= (const sspCondMsgList& msgs)
{
	sspIObase::operator=(msgs);
  removeAll();
  const_iterator iter;
  for(iter = msgs.begin(); iter != msgs.end(); ++iter) {
    add(iter->first, new sspMsgList(*iter->second));
  }
	return *this;
}

sspCondMsgList::~sspCondMsgList()
{
	removeAll();
}

void sspCondMsgList::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
    ar << size();
    iterator iter;
    for (iter = begin(); iter != end(); ++iter) {
      ar << (*iter).first << (*iter).second;
	  }
	}
	else {
		unsigned int nSize;
		ar >> nSize;
    for (unsigned int i=0; i<nSize; i++) {
      int nCond;
      sspMsgList* pNew;
			ar >> nCond >> pNew;
      add(nCond, pNew);
    }
	}
}

void sspCondMsgList::printASCII(sspOutStream& outStr)
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
    outStr << endl << "     " << "* Condition: " << sspPool::Instance().conditionals.GetName((*iter).first);
	  (*iter).second->printASCII(outStr);
  }
}

bool sspCondMsgList::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	int i;
	bool bRet = true;

  iterator iter;
  for (iter = begin(), i=1; iter != end(); ++iter, ++i) {
		if ((*iter).first < 0 || (*iter).first > (int) sspPool::Instance().conditionals.GetSize()) {
			printError(outStr, _T("(sspCondMsgList): a conditional index is not valid"), nErrors);
			bRet = false;
		}
		if (!(*iter).second->verify(outStr, nErrors, nWarnings)) {
			outStr << endl << "   (sspCondMsgList): error in messagelist " << i;
			bRet = false;
		}
  }
	return bRet;
}

void sspCondMsgList::add(int nConditional, sspMsgList* pMessage)
{
  (*this)[nConditional] = pMessage;
}

void sspCondMsgList::removeAll()
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter)
    delete (*iter).second;
  clear();
}

bool sspCondMsgList::remove(int nConditional)
{
  iterator iter = find(nConditional);
  if (iter != end()) {
    delete (*iter).second;
    erase(iter);
    return true;
  }
  return false;
}

void sspCondMsgList::send(sspMsgHandler* pTask)
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
		if (sspPool::Instance().conditionals.GetConditional((*iter).first)->isTrue())
			(*iter).second->send(pTask);
	}
}

void sspCondMsgList::send(sspMsgHandler* pStream, sspMsgHandler* pTask, int nTag)
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
		if (sspPool::Instance().conditionals.GetConditional((*iter).first)->isTrue())
			(*iter).second->send(pStream, pTask, nTag);
	}
}

//////////////////////////////////////////////////////////////////////
// sspTriggerMsgList
//////////////////////////////////////////////////////////////////////

sspTriggerMsgList::sspTriggerMsgList()
{
	m_strName = _T("tml");
}

sspTriggerMsgList::sspTriggerMsgList(const sspTriggerMsgList& msgs) : sspIObase(msgs)
{
  const_iterator iter;
  for(iter = msgs.begin(); iter != msgs.end(); ++iter) {
    add(new SSpTrigger(*iter->first), new sspCondMsgList(*iter->second));
  }
}

sspTriggerMsgList& sspTriggerMsgList::operator= (const sspTriggerMsgList& msgs)
{
	sspIObase::operator=(msgs);
  removeAll();
  const_iterator iter;
  for(iter = msgs.begin(); iter != msgs.end(); ++iter) {
    add(new SSpTrigger(*iter->first), new sspCondMsgList(*iter->second));
  }
	return *this;
}

sspTriggerMsgList::~sspTriggerMsgList()
{
	removeAll();
}

void sspTriggerMsgList::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
    ar << size();
    iterator iter;
    for (iter = begin(); iter != end(); ++iter) {
      ar << (*iter).first << (*iter).second;
	  }
	}
	else {
		unsigned int nSize;
		ar >> nSize;
    for (unsigned int i=0; i<nSize; i++) {
      SSpTrigger* pNewTrigger;
      sspCondMsgList* pNewList;
			ar >> pNewTrigger >> pNewList;
      add(pNewTrigger, pNewList);
    }
	}
}

void sspTriggerMsgList::printASCII(sspOutStream& outStr)
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
	  (*iter).first->printASCII(outStr);
	  (*iter).second->printASCII(outStr);
  }
}

bool sspTriggerMsgList::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	int i;
	bool bRet = true;

  iterator iter;
  for (iter = begin(), i=1; iter != end(); ++iter, ++i) {
		if (!(*iter).first->verify(outStr, nErrors, nWarnings)) {
			outStr << endl << "   (sspTriggerMsgList): error in trigger " << i;
			bRet = false;
		}
		if (!(*iter).second->verify(outStr, nErrors, nWarnings)) {
			outStr << endl << "   (sspTriggerMsgList): error in messagelist " << i;
			bRet = false;
		}
  }
	return bRet;
}

void sspTriggerMsgList::add(SSpTrigger* pTrigger, sspCondMsgList* pMessage)
{
  push_back(make_pair(pTrigger, pMessage));
}

void sspTriggerMsgList::removeAll()
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
    delete (*iter).first;
    delete (*iter).second;
  }
  clear();
}

sspTriggerMsgList::iterator sspTriggerMsgList::find(SSpTrigger* pTrigger)
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
    if ((*iter).first == pTrigger)
      break;
  }
  return iter;
}

bool sspTriggerMsgList::remove(SSpTrigger* pTrigger)
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
    if ((*iter).first == pTrigger) {
      delete (*iter).first;
      delete (*iter).second;
      erase(iter);
      return true;
    }
  }
  return false;
}

void sspTriggerMsgList::reset()
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
    (*iter).first->reset();
  }
}		

void sspTriggerMsgList::testAndSend()
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
		if ((*iter).first->isTrue())
			(*iter).second->send(NULL);
	}
}

//////////////////////////////////////////////////////////////////////
// sspClockMsgList
//////////////////////////////////////////////////////////////////////

sspClockMsgList::sspClockMsgList()
{
	m_strName = _T("clml");
  m_pos = end();
}

sspClockMsgList::sspClockMsgList(const sspClockMsgList& msgs) : sspIObase(msgs)
{
  const_iterator iter;
  for(iter = msgs.begin(); iter != msgs.end(); ++iter) {
    add(iter->first, new sspCondMsgList(*iter->second));
  }
  m_pos = end();
}

sspClockMsgList& sspClockMsgList::operator= (const sspClockMsgList& msgs)
{
	sspIObase::operator=(msgs);
	removeAll();
  const_iterator iter;
  for(iter = msgs.begin(); iter != msgs.end(); ++iter) {
    add(iter->first, new sspCondMsgList(*iter->second));
  }
  m_pos = end();
	return *this;
}

sspClockMsgList::~sspClockMsgList()
{
	removeAll();
}

void sspClockMsgList::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
    ar << size();
    iterator iter;
    for (iter = begin(); iter != end(); ++iter) {
      SSpClock clock = (*iter).first;
      clock.Serialize(ar);
      ar << (*iter).second;
	  }
	}
	else {
		unsigned int nSize;
		ar >> nSize;
    for (unsigned int i=0; i<nSize; i++) {
      SSpClock clock;
      clock.Serialize(ar);
      sspCondMsgList* pNewList;
			ar >> pNewList;
      add(clock, pNewList);
    }
	}
}

void sspClockMsgList::printASCII(sspOutStream& outStr)
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
  	outStr << endl << "  - clock: " << (*iter).first.hour() << ":" << (*iter).first.min() << ":" << (*iter).first.sec();
	  (*iter).second->printASCII(outStr);
  }
}

bool sspClockMsgList::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	int i;
	bool bRet = true;

  iterator iter;
  for (iter = begin(), i=1; iter != end(); ++iter, ++i) {
	  if (!(*iter).first.isValid()) {
			outStr << endl << "   (sspClockMsgList): error in clock " << i;
		  bRet = false;
	  }
		if (!(*iter).second->verify(outStr, nErrors, nWarnings)) {
			outStr << endl << "   (sspClockMsgList): error in messagelist " << i;
			bRet = false;
		}
  }
	return bRet;
}

void sspClockMsgList::add(const SSpClock& clock, sspCondMsgList* pMessage)
{
  remove(clock);
  (*this)[clock] = pMessage;
}

void sspClockMsgList::removeAll()
{
  iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
    delete (*iter).second;
  }
  clear();
  m_pos = end();
}

bool sspClockMsgList::remove(const SSpClock& clock)
{
  iterator iter = find(clock);
  if (iter != end()) {
    if (iter == m_pos) m_pos = end();
    delete (*iter).second;
    erase(iter);
    return true;
  }
  return false;
}

void sspClockMsgList::reset()
{
  sspClock now = (sspClock) sspDateTimeManager::Instance().current();
  for (m_pos = begin(); m_pos != end(); ++m_pos) {
  	// Moves to the point in the list corresponding to the current time
    if (now < (*m_pos).first)
      break;
  }
}

void sspClockMsgList::testAndSend()
{
  sspClock now = (sspClock) sspDateTimeManager::Instance().current();
  if (!empty() && m_pos == end() && now < begin()->first)
    m_pos = begin();
  for (;m_pos != end(); ++m_pos) {
    if (now > (*m_pos).first)
      (*m_pos).second->send(NULL);
    else
      break;
  }
}
