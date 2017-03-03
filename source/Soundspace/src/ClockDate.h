// ClockDate.h: interface for the clock and date classes.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLOCKDATE_H__9576A663_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_)
#define AFX_CLOCKDATE_H__9576A663_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <sspTime.h>

// Thin classes adding serialization and some input/output

class SSpClock : public CObject, public sspClock
{
private:
	DECLARE_SERIAL( SSpClock )
  SSpClock() {}

public:
  SSpClock(int nAllSeconds);
  SSpClock(int nHour, int nMin, int nSec) : sspClock(nHour, nMin, nSec) {}
  SSpClock(const SSpClock& clock) : sspClock(clock) {}
	SSpClock& operator= (const SSpClock& clock);
  virtual ~SSpClock() {}
	void Serialize(CArchive& ar);
};

class SSpDate : public CObject, public sspDate
{
private:
	DECLARE_SERIAL( SSpDate )
  SSpDate() {}

public:

  SSpDate(int nDay, int nMonth) : sspDate(nDay, nMonth) {}
  SSpDate(const SSpDate& date) : sspDate(date) {}
	SSpDate& operator= (const SSpDate& date);
  virtual ~SSpDate() {}
	void Serialize(CArchive& ar);
};

#endif // !defined(AFX_CLOCKDATE_H__9576A663_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_)
