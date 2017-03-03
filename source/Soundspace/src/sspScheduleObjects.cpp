///////////////////////////////////////////
// sspScheduleObjects.cpp : implementation file
///////////////////////////////////////////

#include "StdAfx.h"
#include "sspScheduleObjects.h"
#include "sspSetting.h"

//////////////////////////////////////////////////////////////////////
// sspSilenceObject Class
//////////////////////////////////////////////////////////////////////

sspSilenceObject::	sspSilenceObject(sspValue* pValue)
{
	m_pValue = pValue;
	m_fValue = 0.0f;
	m_pOwner = NULL;
}

sspSilenceObject::~sspSilenceObject()
{
}

bool sspSilenceObject::begin(void)
{
	if (m_pValue != NULL)
		m_nNextTime = (int) (1000 * m_pValue->getValue());
	else
		m_nNextTime = (int) (1000 * m_fValue);
	return true;
}

bool sspSilenceObject::update(void)
{
	m_nNextTime = 0;
	return false;
}

void sspSilenceObject::end(void)
{
	if (m_pOwner != NULL) {
		SSpMessage msg;
		msg.SetMsgType(SSP_IS_DONE);
		m_pOwner->handleMessage(msg);
	}
}

void sspSilenceObject::setOwner(sspMsgHandler* pOwner)
{
	m_pOwner = pOwner;
}

