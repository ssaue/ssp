// Time.cpp: implementation of the CTimeRef class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fmrytme.h"
#include "Time.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CMidiTime, CObject, 0)

//////////////////////////////////////////////////////////////////////
// CMidiTime
//////////////////////////////////////////////////////////////////////

CMidiTime::CMidiTime()
{
	m_nMetronom = DEFAULT_METRO;
	m_nTidsformat = DEFAULT_FORMAT;
	BeregnFaktor();
}

void CMidiTime::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << m_nMetronom << m_nTidsformat;
	}
	else {
		ar >> m_nMetronom >> m_nTidsformat;
		BeregnFaktor();
	}
}


inline void CMidiTime::BeregnFaktor(void)
{
	m_dTidsfaktor = 60000.0 / (m_nMetronom * (double) m_nTidsformat);
}

void CMidiTime::SetMetronom(int metronom)
{
	m_nMetronom = metronom;
	BeregnFaktor();
}

void CMidiTime::SetTempoEvent(long tempo)
{
	m_nMetronom = (int) 60000000 / tempo;
	BeregnFaktor();
}

void CMidiTime::SetTidsformat(int tidsformat)
{
	m_nTidsformat = tidsformat;
	BeregnFaktor();
}

//////////////////////////////////////////////////////////////////////
// CFMTime
//////////////////////////////////////////////////////////////////////

CFMTime::CFMTime()
{
	m_dDistanse = 0.0;
}
