//////////////////////////////////////
// sspTime.cpp : implementation file
//////////////////////////////////////

#include "sspTime.h"
#include "sspTimeResources.h"
//#include <stdio.h>

#ifdef WIN32
  #include <windows.h>
  #define snprintf _snprintf
  #undef min // macros incompatible with STL !
  #undef max
#else
  //#include <sys/time.h>
  //#include <stdio.h>
#endif

#define AP_DELTA_EPOCH_IN_USEC   11644473600000000;   // time diff between MicroSoft and Unix
#define AP_DELTA_EPOCH_IN_100NS  116444736000000000;


//////////////////////////////////////////////////////////////////////
// sspTimeConvert Class
//////////////////////////////////////////////////////////////////////

const double sspTimeConvert::s_us2s = 1.0 / SEC2USEC;
const double sspTimeConvert::s_us2ms = 1.0 / MSEC2USEC;

//////////////////////////////////////////////////////////////////////
// sspTime Class
//////////////////////////////////////////////////////////////////////

sspTime::sspTime()
{
  sspTimeResources::Instance().initialize();    // Improves resolution on timeGetTime()
  update();
}

sspTimeType sspTime::getCurrentTime()
{
  update();
  return m_time;
}

#ifdef WIN32
  sspTimeType sspTime::s_baseTime = 0;
  sspTimeType sspTime::s_baseCounter = 0;
  double sspTime::s_dFreqFac = 1.0;
  bool sspTime::s_bInitialized = false;
#endif

void sspTime::update()
{
#ifdef WIN32
  LARGE_INTEGER timeNow, perfFreq;
  if (!QueryPerformanceCounter(&timeNow) || !s_bInitialized) 
  {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    sspTimeType time = ft.dwHighDateTime;
    time = (time << 32) | ft.dwLowDateTime; // time is now in 100 nano sec resolution. 1.1.1601 epoch
    time -= AP_DELTA_EPOCH_IN_100NS;       
    time /= 10;                             // time is now in microseconds. 1.1.1970 epoch (unix time)    
    s_baseTime = m_time = time;
    s_baseCounter = timeNow.QuadPart;
    QueryPerformanceFrequency(&perfFreq);
    s_dFreqFac = 1e6 / double(perfFreq.QuadPart);   // time per tick (in microseconds)
    s_bInitialized = true;
  }
  else 
  {
    m_time = s_baseTime + (sspTimeType) ((timeNow.QuadPart - s_baseCounter) * s_dFreqFac);
  }
#else
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  m_time = sspTimeConvert::timeval2time(tv);
#endif
}

//////////////////////////////////////////////////////////////////////
// sspDate Class
//////////////////////////////////////////////////////////////////////

int	sspDate::m_nMonths[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

sspDate& sspDate::operator= (const sspDate& date)
{
	m_nDay = date.m_nDay;
	m_nMon = date.m_nMon;
	return *this;
}

const char* sspDate::dateAsString(char *buffer, int nMaxSize) const
{
  _snprintf_s(buffer, nMaxSize, _TRUNCATE, "%02d/%02d", m_nDay, m_nMon);
  return buffer;
}

const wchar_t* sspDate::dateAsString(wchar_t *buffer, int nMaxSize) const
{
	_snwprintf_s(buffer, nMaxSize, _TRUNCATE, L"%02d/%02d", m_nDay, m_nMon);
	return buffer;
}

//////////////////////////////////////////////////////////////////////
// sspClock Class
//////////////////////////////////////////////////////////////////////

sspClock& sspClock::operator= (const sspClock& clock)
{
	m_nHour = clock.m_nHour;
	m_nMin = clock.m_nMin;
	m_nSec = clock.m_nSec;
	return *this;
}

const char* sspClock::clockAsString(char *buffer, int nMaxSize) const
{
  _snprintf_s(buffer, nMaxSize, _TRUNCATE, "%02d:%02d:%02d", m_nHour, m_nMin, m_nSec);
  return buffer;
}


const wchar_t* sspClock::clockAsString(wchar_t *buffer, int nMaxSize) const
{
	_snwprintf_s(buffer, nMaxSize, _TRUNCATE, L"%02d:%02d:%02d", m_nHour, m_nMin, m_nSec);
	return buffer;
}

//////////////////////////////////////////////////////////////////////
// sspLocalTime Class
//////////////////////////////////////////////////////////////////////

const char sspLocalTime::s_dayNames[WEEKDAYS][10] = {"Sunday", "Monday", "Tuesday", 
	"Wednesday", "Thursday", "Friday", "Saturday"};

sspLocalTime::sspLocalTime() 
{
  localTime();
}

sspTime& sspLocalTime::getCurrentTime()
{
  sspTime::getCurrentTime();
  localTime();
  return *this;
}

void sspLocalTime::localTime()
{
  struct tm conv;
  time_t unixTime = (time_t) sspTimeConvert::time2secs(m_time);
  localtime_s(&conv, &unixTime);  

  m_nSec   = conv.tm_sec;
  m_nMin   = conv.tm_min;
  m_nHour  = conv.tm_hour;
  m_nDay   = conv.tm_mday;
  m_nMon   = conv.tm_mon + 1;
  m_nYear  = conv.tm_year + 1900;
  m_nDayOfWeek = conv.tm_wday;
  m_nDayOfYear = conv.tm_yday;
}
