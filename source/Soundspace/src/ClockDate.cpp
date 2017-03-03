// ClockDate.cpp: implementation of the CClockTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClockDate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( SSpClock, CObject, 1 )
IMPLEMENT_SERIAL( SSpDate, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// SSpClock
//////////////////////////////////////////////////////////////////////

SSpClock::SSpClock(int nAllSeconds)
{
  m_nSec = nAllSeconds;
  m_nHour = m_nSec / 3600;
  m_nSec -= (m_nHour * 3600);
  m_nMin = m_nSec / 60;
  m_nSec -= (m_nMin * 60);
}

SSpClock& SSpClock::operator= (const SSpClock& clock)
{
  sspClock::operator= (clock);
	return *this;
}

void SSpClock::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nHour << m_nMin << m_nSec;
	else
		ar >> m_nHour >> m_nMin >> m_nSec;
}

//////////////////////////////////////////////////////////////////////
// SSpDate
//////////////////////////////////////////////////////////////////////

SSpDate& SSpDate::operator= (const SSpDate& date)
{
  sspDate::operator= (date);
	return *this;
}

void SSpDate::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nDay << m_nMon;
	else
		ar >> m_nDay >> m_nMon;
}
