// sspMsgList.h: interface for the message list classes
//
/////////////////////////////////////////////////////////////

#ifndef SSP_MSGLIST_H
#define SSP_MSGLIST_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SSpMessage.h"
#include "ClockDate.h"

#include <list>
#include <map>

class SSpTrigger;

class sspMsgList : public sspIObase, public std::list<SSpStreamMsg*>
{
protected:
  DECLARE_SERIAL( sspMsgList )

public:
	sspMsgList();
	sspMsgList(const sspMsgList& msgs);
	sspMsgList& operator= (const sspMsgList& msgs);
	virtual ~sspMsgList();

	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

  void add(SSpStreamMsg* pMessage) {push_back(pMessage);}
  void remove(SSpStreamMsg* pMessage);
	void removeAll();

	void send(sspMsgHandler* pTask);	
	void send(sspMsgHandler* pStream, sspMsgHandler* pTask, int nTag);	
};

class sspCondMsgList : public sspIObase, public std::map<int, sspMsgList*>
{
protected:
  DECLARE_SERIAL( sspCondMsgList )

public:
	sspCondMsgList();
	sspCondMsgList(const sspCondMsgList& msgs);
	sspCondMsgList& operator= (const sspCondMsgList& msgs);
	virtual ~sspCondMsgList();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	void add(int nConditional, sspMsgList* pMessage);
	bool remove(int nCondition);
	void removeAll();

	void send(sspMsgHandler* pTask);	
	void send(sspMsgHandler* pStream, sspMsgHandler* pTask, int nTag);	
};

class sspTriggerMsgList : public sspIObase, public std::list<std::pair<SSpTrigger*, sspCondMsgList*> >
{
private:
  DECLARE_SERIAL( sspTriggerMsgList )

public:
	sspTriggerMsgList();
	sspTriggerMsgList(const sspTriggerMsgList& msgs);
	sspTriggerMsgList& operator= (const sspTriggerMsgList& msgs);
	virtual ~sspTriggerMsgList();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	void add(SSpTrigger* pTrigger, sspCondMsgList* pMessage);
	bool remove(SSpTrigger* pTrigger);
  void removeAll();

  sspTriggerMsgList::iterator find(SSpTrigger* pTrigger);

  void reset();
	void testAndSend();
};

class sspClockMsgList : public sspIObase, public std::map<SSpClock, sspCondMsgList*>
{
private:
  DECLARE_SERIAL( sspClockMsgList )
  std::map<SSpClock, sspCondMsgList*>::iterator m_pos;

public:
	sspClockMsgList();
	sspClockMsgList(const sspClockMsgList& msgs);
	sspClockMsgList& operator= (const sspClockMsgList& msgs);
	virtual ~sspClockMsgList();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	void add(const SSpClock& clock, sspCondMsgList* pMessage);
	bool remove(const SSpClock& clock);
  void removeAll();

	void reset();
	void testAndSend();
};

#endif