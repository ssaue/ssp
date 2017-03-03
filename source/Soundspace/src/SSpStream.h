// SSpStream.h: interface for the SSpStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSPSTREAM_H__1F611AA0_9384_11D3_901C_D4A2F3F5EC94__INCLUDED_)
#define AFX_SSPSTREAM_H__1F611AA0_9384_11D3_901C_D4A2F3F5EC94__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include "SSpStreamMixer.h"
#include "SSpTasks.h"
#include <list>

typedef std::list<SSpPlayTask*>	TaskList;
typedef TaskList::iterator		TaskElem;

class sspTaskScheduler
{
private:
	TaskList	m_activeTasks;
	TaskList	m_waitTasks;

  sspSynchObject	m_synch;

	unsigned int m_nMaxTasks;
	unsigned int m_nMaxWaitTasks;

	enum {NONE_AVAILABLE = -1};

public:
	sspTaskScheduler(unsigned int nBuffers, unsigned int nWaitBuffers);
	virtual ~sspTaskScheduler();

	bool loadTask(SSpPlayTask* pTask, sspMsgHandler* pStream, SSpPlayTask*& pFadeTask);
	void removeTask(SSpPlayTask* pTask);
  bool isEmpty() const {return m_activeTasks.empty() && m_waitTasks.empty();}

	SSpPlayTask*	nextTask();
	void			clearTasks();
};

class SSpStream : public sspMsgHandler  
{
protected:
	sspIntVec	m_nSubStream;
	sspValue*	m_pTimeFactor;
	int			  m_nTimeFacIndex;
	bool      m_bStopped;

  DECLARE_SERIAL( SSpStream )

public:
	SSpStream();
	SSpStream(const SSpStream&);
	virtual ~SSpStream();
	void Serialize(CArchive& ar);

  // initialize and terminate are accessed from the sequential list
  virtual bool initialize();
	virtual void terminate();

  // begin, update, end and isEmpty are accessed from the hierarchical tree
  virtual bool begin();
  virtual bool update(double dTime);
  virtual void end();
  virtual bool isEmpty();

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
  void handleMessage(const SSpMessage& msg) {}

	void setTimeFactor(int nTimeFacIndex) {m_nTimeFacIndex = nTimeFacIndex;}
	void setSubStreams(const sspIntVec& nStreams) {m_nSubStream = nStreams;}

	int getTimeFactor() const {return m_nTimeFacIndex;}
	const sspIntVec& getSubStreams() {return m_nSubStream;}

protected:
	double getVirtualTime(double dTime);
};

class SSpTaskStream : public SSpStream
{
private:
	sspSynchObject	  m_synch;
	sspTimedTaskList	m_eventList;

protected:
  DECLARE_SERIAL( SSpTaskStream )

public:
	SSpTaskStream();
	virtual ~SSpTaskStream();

	bool begin();
	bool update(double dTime);
  bool isEmpty();

	// Virtual methods
	void handleMessage(const SSpMessage& msg);

protected:
	virtual void playTask(SSpPlayTask* pTask);
};

class SSpAudioStream : public SSpTaskStream
{
private:
	SSpStreamMixer*	m_pMixer;
	sspTaskScheduler*	m_pScheduler;
	unsigned int			m_nBufCount;
	int				m_nWaitCount;
	int				m_nVolumeRange;
	int				m_nFaderValue;
	sspIntVec		m_nDSDevices;
	sspIntVec		m_nWavDevices;

	DECLARE_SERIAL( SSpAudioStream )

public:
	SSpAudioStream();
	virtual ~SSpAudioStream();
	void Serialize(CArchive& ar);

	bool initialize();
	void terminate();

  void end();
  bool isEmpty();

	void setDSDevices(const sspIntVec& nDevices) {m_nDSDevices = nDevices;}
	void setWavDevices(const sspIntVec& nDevices) {m_nWavDevices = nDevices;}

	const sspIntVec& getDSDevices() const {return m_nDSDevices;}
	const sspIntVec& getWavDevices() const {return m_nWavDevices;}

	void setPlayBuffers(unsigned int nBuf) {m_nBufCount = nBuf;}
	void setWaitBuffers(unsigned int nBuf) {m_nWaitCount = nBuf;}

	unsigned int getPlayBuffers() const {return m_nBufCount;}
	unsigned int getWaitBuffers() const {return m_nWaitCount;}

	void setFaderValue(int nValue) {m_nFaderValue = nValue;}	
	int	 getFaderValue() const {return m_nFaderValue;}
	void updateVolumeFader();

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	void handleMessage(const SSpMessage& msg);
	const SSpStreamMixer* getMixer() const {return m_pMixer;}

private:
	void playTask(SSpPlayTask* pTask);
};

#endif // !defined(AFX_SSPSTREAM_H__1F611AA0_9384_11D3_901C_D4A2F3F5EC94__INCLUDED_)
