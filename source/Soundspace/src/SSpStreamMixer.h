// SSpStreamMixer.h: interface for the SSpStreamMixer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSPSTREAMMIXER_H__BBCCD300_9D9F_11D3_901C_85196F094B1C__INCLUDED_)
#define AFX_SSPSTREAMMIXER_H__BBCCD300_9D9F_11D3_901C_85196F094B1C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspWavControlObject.h"

class SSpPlayTask;
class sspDSbuffer;

typedef std::list<sspDSbuffer*>	sspDSbufferList;
typedef sspDSbufferList::iterator	sspDSbufferElem;

class SSpStreamMixer  
{
public:
	enum {ABSOLUTE_REF, RELATIVE_REF};

private:
	sspMsgHandler*		m_pStream;
	sspWavControlObject	m_masterFader;
	sspSynchObject	m_synch;

	sspIntVec			  m_nChannels;	// Counted as mono
	sspDSbufferList	m_pBuffers;
	unsigned int		m_nMaxBuffers;

public:
	SSpStreamMixer(sspMsgHandler* pStream, unsigned int nMaxBuf, const sspIntVec& nChans);
	virtual ~SSpStreamMixer();

	void createMasterFader(const sspIntVec& nChans);	// Counted as mono

	bool begin(SSpPlayTask* pTask, float fTime = 0.0f);	// Allowing fade in
	void end();
	void terminate();

	bool masterVolume(float fVolume, float fTime = 0.0, int nRef = ABSOLUTE_REF);
	bool masterFadeOut(float fTime);
	bool masterFadeIn(float fTime);

	// Figure out how to address the correct buffer later
	bool bufferVolume(sspDSbuffer* pBuf, float fVolume, float fTime = 0.0f, int nRef = ABSOLUTE_REF);
	bool bufferFadeOut(sspDSbuffer* pBuf, float fTime);
	bool bufferFadeIn(sspDSbuffer* pBuf, float fTime);
	bool bufferSolo(sspDSbuffer* pBuf, float fTime);
	bool bufferUnSolo(sspDSbuffer* pBuf, float fTime);
};

#endif // !defined(AFX_SSPSTREAMMIXER_H__BBCCD300_9D9F_11D3_901C_85196F094B1C__INCLUDED_)
