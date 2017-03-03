// SSpTasks.h: interface for the SSpPlayTask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSPEVENTS_H__99543960_9202_11D3_901C_F98F7F995B9F__INCLUDED_)
#define AFX_SSPEVENTS_H__99543960_9202_11D3_901C_F98F7F995B9F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SSpMessage.h"
#include "sspMsgList.h"

enum SSP_RESOURCE_TYPES {SSP_MIDI, SSP_DIRECTSOUND, SSP_WAV, SSP_TYPES};

class sspDSbuffer;

class sspPlayObject : public sspMsgHandler  
{
protected:
	int 	m_nPlayCount;
	short	m_nResourceUsage[SSP_TYPES];

public:
	sspPlayObject(int nType);
	virtual ~sspPlayObject();

	virtual bool initialize() = 0;
	virtual void terminate() {}		// Compatibility with SonBase

	virtual bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner) = 0;
	virtual bool update();
	virtual void end();

	virtual int	 estimateDuration() = 0;
	virtual bool isPlaying() {return (m_nPlayCount > 0);}

	void	handleMessage(const SSpMessage& msg);
	short	getResourceUsage(int nType) {return nType < SSP_TYPES ? m_nResourceUsage[nType] : 0;}
};

class SSpPlayTask : public sspMsgHandler  
{
public:
	enum {CANCEL, WAIT, LOAD, LOAD_ALWAYS};
	enum {EVENT_ENTER, EVENT_START, EVENT_END, EVENT_EXIT, TASK_EVENT_TYPES};

protected:
	int		m_nPriority;
	int		m_nCondIndex;
	int		m_nPlayObject;
	int		m_nVolumeFactor;
	bool	m_bPlaying;
	short	m_nResourceUsage[SSP_TYPES];

	sspDSbuffer*	m_pBuffer;

//	SSpConditionalMessageList	m_messageList[TASK_EVENT_TYPES];
  sspCondMsgList m_messages[TASK_EVENT_TYPES];

    DECLARE_SERIAL( SSpPlayTask )

public:
	SSpPlayTask();
	SSpPlayTask(const SSpPlayTask& task);
	SSpPlayTask& operator= (const SSpPlayTask& task);
	virtual ~SSpPlayTask();
	void Serialize(CArchive& ar);

	void setPriority(int nPriority) {m_nPriority = nPriority;}
	void setObjectIndex(int nObject) {m_nPlayObject = nObject;}
	void setPlayCondition(int nCondition) {m_nCondIndex = nCondition;}
	void setVolumeFactor(int nFactor) {m_nVolumeFactor = nFactor;}
  void setMsgList(int nType, int nCondition, sspMsgList* pMsgs);

	bool initialize();
	bool execute(bool bReady, sspMsgHandler* pOwner, sspDSbuffer* pBuf);
	void abort();

	sspDSbuffer* getBuffer() const {return m_pBuffer;}
	void clearBuffer() {m_pBuffer = NULL;}

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	void handleMessage(const SSpMessage& msg);

	unsigned int getDuration();
	int	getPriority() const  {return m_nPriority;}
	int getObjectIndex() const {return m_nPlayObject;}
	int getPlayCondition() const {return m_nCondIndex;}
	int getVolumeFactor() const {return m_nVolumeFactor;}
	short getResourceUsage(int nType) const {return (nType < SSP_TYPES) ? m_nResourceUsage[nType] : 0;}
  sspCondMsgList* getMsgList(int nType) {return &m_messages[nType];}

	int& priority() {return m_nPriority;}
	bool isPlaying() {return m_bPlaying;}
};

struct sspTimedTask
{
	unsigned int			m_nTime;
	SSpPlayTask*	m_pTask;

	sspTimedTask(unsigned int nTime, SSpPlayTask* pTask);
	virtual ~sspTimedTask();
};

class sspTimedTaskList : public std::list<sspTimedTask*>
{
private:
	double	m_dTimeCounter;

public:
	sspTimedTaskList();
	virtual ~sspTimedTaskList();

	void reset();
	void loadTask(SSpPlayTask* pTask, unsigned int nTime = 0);	// default: play immediately

	SSpPlayTask* getFirstTask(double dTimeStep);
	SSpPlayTask* getNextTask();	
};

#endif // !defined(AFX_SSPEVENTS_H__99543960_9202_11D3_901C_F98F7F995B9F__INCLUDED_)
