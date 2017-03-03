// SSpMessage.h: definition and interface for the message handlers
//
/////////////////////////////////////////////////////////////////////

#ifndef SSP_MESSAGE_H
#define SSP_MESSAGE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SSpObject.h"

union SSpMsgPara {
	float			fVal;
	int				iVal;
	unsigned int	uiVal;
	void*			pVal;

	void Clean() {iVal = 0;}
};

class SSpMessage : public sspIObase
{
private:
	int	m_nType;
	SSpMsgPara	m_para[2];	// Message parameters (two allowed)

    DECLARE_SERIAL( SSpMessage )

public:
	SSpMessage();
	SSpMessage(const SSpMessage& msg);
	SSpMessage& operator= (const SSpMessage& msg);
	virtual ~SSpMessage();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	void SetMsgData(int nType, SSpMsgPara para1, SSpMsgPara para2);
	void SetMsgType(int nType) {m_nType = nType;}
	void SetMsgParameter(int nIndex, const SSpMsgPara& para) {m_para[nIndex] = para;}

	int GetMsgType() const {return m_nType;}
	SSpMsgPara& GetMsgPara(int nIndex) {return m_para[nIndex];}
	SSpMsgPara GetMsgPara(int nIndex) const {return m_para[nIndex];}
};

class sspMsgHandler : public sspObject
{
protected:
	sspMsgHandler* m_pOwner;

public:
	sspMsgHandler(short nClass, short nType);
	virtual ~sspMsgHandler();

	void setOwner(sspMsgHandler* pOwner) {m_pOwner = pOwner;}
	virtual void handleMessage(const SSpMessage& msg) = 0;
};

class SSpStreamMsg : public sspIObase
{
private:
	int			m_nStream;	// The recipient of this message (index to a SSpMessageHandler)
	SSpMessage	m_msg;

    DECLARE_SERIAL( SSpStreamMsg )

public:
	SSpStreamMsg();
	SSpStreamMsg(const SSpStreamMsg& mes);
	SSpStreamMsg& operator= (const SSpStreamMsg& mes);
	virtual ~SSpStreamMsg();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	void SetMsgData(int nStream, const SSpMessage& msg);
	void SetStream(int nStream) {m_nStream = nStream;}
	void SetMsg(const SSpMessage& msg) {m_msg = msg;}

	int GetStream() const {return m_nStream;}
	SSpMessage& GetMessage() {return m_msg;}
	SSpMessage GetMessage() const {return m_msg;}
	SSpMsgPara& GetMsgPara(int nIndex) {return m_msg.GetMsgPara(nIndex);}
	SSpMsgPara GetMsgPara(int nIndex) const {return m_msg.GetMsgPara(nIndex);}

	void Send(sspMsgHandler* pTask);	
	void Send(sspMsgHandler* pStream, sspMsgHandler* pTask, int nTag);	
};

#endif