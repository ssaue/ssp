// SSpMessage.cpp: implementation of the SSpFloat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SSpMessage.h"
#include "sspPools.h"
#include "sspSetting.h"
#include "SSpBasic.h"
#include "SSpStream.h"
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( SSpMessage, CObject, 1 )
IMPLEMENT_SERIAL( SSpStreamMsg, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// SSpMessage
//////////////////////////////////////////////////////////////////////

SSpMessage::SSpMessage()
{
	m_nType = SSP_NO_MESSAGE;
	m_strName = _T("msg");
	m_para[0].Clean();
	m_para[1].Clean();
}

SSpMessage::SSpMessage(const SSpMessage& mes) : sspIObase(mes)
{
	m_nType = mes.m_nType;
	m_para[0] = mes.m_para[0];
	m_para[1] = mes.m_para[1];
}

SSpMessage& SSpMessage::operator= (const SSpMessage& mes)
{
	sspIObase::operator=(mes);
	m_nType = mes.m_nType;
	m_para[0] = mes.m_para[0];
	m_para[1] = mes.m_para[1];
	return *this;
}

SSpMessage::~SSpMessage()
{
}

void SSpMessage::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nType << m_para[0].iVal << m_para[1].iVal;
	else
		ar >> m_nType >> m_para[0].iVal >> m_para[1].iVal;
}

void SSpMessage::printASCII(sspOutStream& outStr)
{
	switch (m_nType) {
		case SSP_LOAD_EVENT:
			if (m_para[1].iVal == USE_SEND_PARAMETER) {
				outStr << "Load current task, delay: ";
			}
			else {
				outStr << "Load task " << sspPool::Instance().tasks.GetName(m_para[1].iVal);
				outStr << ", with delay ";
			}
			outStr << sspPool::Instance().values.GetName(m_para[0].iVal);
			break;
		case SSP_SOLO:
			outStr << "Solo task, with fade time ";
			outStr << m_para[0].iVal;
			break;
		case SSP_MUTE:
			outStr << "Mute stream, with fade time ";
			outStr << m_para[0].iVal;
			break;
		case SSP_UNSOLO:
			outStr << "Unsolo task, with fade time ";
			outStr << m_para[0].iVal;
			break;
		case SSP_UNMUTE:
			outStr << "Unmute stream, with fade time ";
			outStr << m_para[0].iVal;
			break;
		case SSP_START:
			outStr << "Start stream";
			break;
		case SSP_STOP:
			outStr << "Stop stream";
			break;
		case SSP_SET_VOLUME:
			outStr << "Set volume " << sspPool::Instance().tasks.GetName(m_para[0].iVal);
			outStr << ", with fade time " << m_para[1].iVal;
			break;
		case SSP_ADJUST_VOLUME:
			outStr << "Adjust volume " << sspPool::Instance().tasks.GetName(m_para[0].iVal);
			outStr << ", with fade time " << m_para[1].iVal;
			break;
		case SSP_NO_MESSAGE:
			outStr << "No message";
			break;
		case SSP_IS_DONE:
			outStr << "Task is done";
			break;
		default:
			break;
	}
}

bool SSpMessage::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	switch (m_nType) {
		case SSP_LOAD_EVENT:
			if (m_para[0].iVal < 0 || m_para[0].iVal > (int) sspPool::Instance().values.GetSize()) {
				printError(outStr, _T("(SSpMessage - load): delay is not valid"), nErrors);
				bRet = false;
			}
			if (m_para[1].iVal != USE_SEND_PARAMETER) {
				if (m_para[1].iVal < 0 || m_para[1].iVal > (int) sspPool::Instance().tasks.GetSize()) {
					printError(outStr, _T("(SSpMessage - load): task is not valid"), nErrors);
					bRet = false;
				}
			}
			break;
		case SSP_SOLO:
		case SSP_MUTE:
		case SSP_UNSOLO:
		case SSP_UNMUTE:
			if (m_para[0].iVal < 0) {
				printError(outStr, _T("(solo/mute message): negative fade time"), nErrors);
				bRet = false;
			}
			if (m_para[0].iVal > 60) {
				printWarning(outStr, _T("(solo/mute message): fade time exceeds a minute"), nWarnings);
				bRet = false;
			}
			break;			
		case SSP_START:
		case SSP_STOP:
			break;
		case SSP_SET_VOLUME:
		case SSP_ADJUST_VOLUME:
			if (m_para[1].iVal < 0) {
				printError(outStr, _T("(volume message): negative fade time"), nErrors);
				bRet = false;
			}
			if (m_para[1].iVal > 60) {
				printWarning(outStr, _T("(volume message): fade time exceeds a minute"), nWarnings);
				bRet = false;
			}
			break;			
		case SSP_NO_MESSAGE:
		case SSP_IS_DONE:
		default:
			printError(outStr, _T("(SSpMessage): undefined message"), nErrors);
			bRet = false;
			break;
	}
	return bRet;
}

void SSpMessage::SetMsgData(int nType, SSpMsgPara para1, SSpMsgPara para2)
{
	m_nType = nType;
	m_para[0] = para1;
	m_para[1] = para2;
}



//////////////////////////////////////////////////////////////////////
// sspMsgHandler
//////////////////////////////////////////////////////////////////////

sspMsgHandler::sspMsgHandler(short nClass, short nType) : sspObject(nClass, nType)
{
	m_pOwner = NULL;
}

sspMsgHandler::~sspMsgHandler()
{
}

//////////////////////////////////////////////////////////////////////
// SSpStreamMsg
//////////////////////////////////////////////////////////////////////

SSpStreamMsg::SSpStreamMsg()
{
	m_nStream = USE_SEND_PARAMETER;
	m_strName = _T("strmsg");
}

SSpStreamMsg::SSpStreamMsg(const SSpStreamMsg& msg) : sspIObase(msg)
{
	m_nStream = msg.m_nStream;
	m_msg = msg.m_msg;
}

SSpStreamMsg& SSpStreamMsg::operator= (const SSpStreamMsg& msg)
{
	sspIObase::operator=(msg);
	m_nStream = msg.m_nStream;
	m_msg = msg.m_msg;
	return *this;
}

SSpStreamMsg::~SSpStreamMsg()
{
}

void SSpStreamMsg::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_msg.Serialize(ar);
	if (ar.IsStoring())
		ar << m_nStream;
	else
		ar >> m_nStream;
}

void SSpStreamMsg::printASCII(sspOutStream& outStr)
{
	if (m_nStream == USE_SEND_PARAMETER) {
    outStr << "message to current stream: ";
	}
	else {
    outStr << "message to " << sspPool::Instance().streams.GetName(m_nStream) << ": ";
	}
	m_msg.printASCII(outStr);
}

bool SSpStreamMsg::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nStream != USE_SEND_PARAMETER) {
		if (m_nStream < 0 || m_nStream > (int) sspPool::Instance().streams.GetSize()) {
			printError(outStr, _T("(SSpStreamMsg): stream is not valid"), nErrors);
			bRet = false;
		}
		if (m_nStream == 0) {
			printWarning(outStr, _T("(SSpStreamMsg): message to application stream"), nWarnings);
			bRet = false;
		}
		if (!m_msg.verify(outStr, nErrors, nWarnings)) {
			bRet = false;
		}
	}
	return bRet;
}

void SSpStreamMsg::SetMsgData(int nStream, const SSpMessage& msg)
{
	m_nStream = nStream;
	m_msg = msg;
}

void SSpStreamMsg::Send(sspMsgHandler* pTask)
{
	if (m_nStream == USE_SEND_PARAMETER)
		return;		// Illegal message: should use the next implementation of send()

	SSpMessage newMsg = m_msg;
	switch (m_msg.GetMsgType()) {
	case SSP_NO_MESSAGE:
		return;
	case SSP_LOAD_EVENT:
		{
			float fTime = sspPool::Instance().values.getValue(m_msg.GetMsgPara(0).iVal)->getValue();
			newMsg.GetMsgPara(0).Clean();
			newMsg.GetMsgPara(0).iVal = (int)(fTime);
			if (m_msg.GetMsgPara(1).iVal == USE_SEND_PARAMETER)
				newMsg.GetMsgPara(1).pVal = pTask;
			else
				newMsg.GetMsgPara(1).pVal = sspPool::Instance().tasks.GetPlayTask(m_msg.GetMsgPara(1).iVal);
			break;
		}
	default:
		break;
	}
	sspPool::Instance().streams.GetStream(m_nStream)->handleMessage(newMsg);
}

void SSpStreamMsg::Send(sspMsgHandler* pStream, sspMsgHandler* pTask, int nTag)
{
	SSpMessage newMsg = m_msg;
	switch (m_msg.GetMsgType()) {
	case SSP_NO_MESSAGE:
		return;
	case SSP_IS_DONE:
		newMsg.GetMsgPara(0).Clean();
		newMsg.GetMsgPara(0).iVal = nTag;
		break;
	case SSP_LOAD_EVENT:
		{
			float fTime = sspPool::Instance().values.getValue(m_msg.GetMsgPara(0).iVal)->getValue();
			newMsg.GetMsgPara(0).Clean();
			newMsg.GetMsgPara(0).iVal = (int)(fTime);
			if (m_msg.GetMsgPara(1).iVal == USE_SEND_PARAMETER)
				newMsg.GetMsgPara(1).pVal = pTask;
			else
				newMsg.GetMsgPara(1).pVal = sspPool::Instance().tasks.GetPlayTask(m_msg.GetMsgPara(1).iVal);
			break;
		}
	case SSP_SOLO:
	case SSP_UNSOLO:
		if (m_msg.GetMsgPara(1).iVal == USE_SEND_PARAMETER) {
			newMsg.GetMsgPara(1).Clean();
			newMsg.GetMsgPara(1).uiVal = nTag;
		}
		break;
	case SSP_SET_VOLUME:
	case SSP_ADJUST_VOLUME:
		newMsg.GetMsgPara(0).fVal = sspPool::Instance().values.getValue(m_msg.GetMsgPara(0).iVal)->getValue();
		break;
	default:
		break;
	}
	if (m_nStream == USE_SEND_PARAMETER)
		pStream->handleMessage(newMsg);
	else
		sspPool::Instance().streams.GetStream(m_nStream)->handleMessage(newMsg);
}
