///////////////////////////////
// sspTime.h : header file
///////////////////////////////

#ifndef SSPTIME_H
#define SSPTIME_H

#include <time.h>

// These macroes cause trouble
#undef min
#undef max

// 
// Basic time units
//

typedef long long sspTimeType;      // absolute: the number of microseconds since epoch
typedef unsigned int sspTimerType;  // relative: a length in milliseconds 

class sspTimeConvert
{
public:
  enum {SEC2MSEC = 1000, MSEC2USEC=1000, USEC2NSEC=1000, SEC2USEC = 1000000, MSEC2NSEC=1000000, SEC2NSEC=1000000000};
  enum {HOUR2SEC = 3600, DAY2HOUR=24};

private:
  static const double s_us2s;
  static const double s_us2ms;
 
public:
  static double time2secs(sspTimeType time) {return time * s_us2s;}
  static double time2msecs(sspTimeType time) {return time * s_us2ms;}

  static sspTimeType days2time(double days) {return (sspTimeType) hours2time(days * DAY2HOUR);}
  static sspTimeType hours2time(double hours) {return (sspTimeType) secs2time(hours * HOUR2SEC);}

  static sspTimeType secs2time(double secs) {return (sspTimeType) (secs * SEC2USEC);}
  static sspTimeType msecs2time(double msecs) {return (sspTimeType) (msecs * MSEC2USEC);}

  static sspTimerType secs2timer(double secs) {return (sspTimerType) (secs * SEC2MSEC);}
  static sspTimerType msecs2timer(double msecs) {return (sspTimerType) (msecs + 0.5);}

#if !defined(WIN32)
  static void time2timeval(sspTimeType usecs, struct timeval& time)
  {
	  time.tv_sec = (long) (usecs / SEC2USEC);
	  time.tv_usec = (long) (usecs % SEC2USEC);
  }

  static sspTimeType timeval2time(struct timeval& time)
  {
    return (sspTimeType) (tv.tv_sec * SEC2USEC + tv.tv_usec);
  }

  static void timer2timespec(sspTimerType msecs, timespec_t& time)
  {
	  time.tv_sec = (time_t) (msecs / SEC2MSEC);
	  time.tv_nsec = (long) ((msecs % SEC2MSEC) * MSEC2NSEC);
  }

  static sspTimerType timespec2timer(timespec_t& time)
  {
    return (sspTimerType) (tv.tv_sec * SEC2MSEC + tv.tv_nsec / MSEC2NSEC);
  }
#endif
};

class sspTime
{
public:
  sspTime();
	sspTime(sspTimeType time) : m_time(time) {}
  sspTime(const sspTime& time) : m_time(time.m_time) {}
  sspTime& operator= (const sspTime& time) {m_time = time.m_time; return *this;}
  virtual ~sspTime() {}

  sspTimeType getCurrentTime();
  sspTimeType getTime() const { return m_time; };

  // Operations involving two sspTime objects
  sspTimeType operator- (const sspTime& time) const
  {
      return m_time - time.getTime();
  };
  bool operator> (const sspTime& time) const
  {
      return m_time > time.getTime();
  };
  bool operator< (const sspTime& time) const
  {
      return m_time < time.getTime();
  };

  // Operations involving a sspTimeType value
  sspTime& operator-= (const sspTimeType& time)
  {
      m_time -= time;
      return *this;
  };
  sspTime& operator+= (const sspTimeType& time)
  {
      m_time += time;
      return *this;
  };
  sspTime operator- (const sspTimeType& time) const
  {
    return sspTime(m_time-time);
  };
  sspTime operator+ (const sspTimeType& time) const
  {
    return sspTime(m_time+time);
  };

protected:
  void update();
  sspTimeType  m_time;

private:
#ifdef WIN32
  static sspTimeType s_baseTime, s_baseCounter;
  static double s_dFreqFac;
  static bool s_bInitialized;
#endif
};

// The sspTimeUpdate is a utility class that handles time updates
class sspTimeUpdate
{
private:
  sspTime     m_next;
	sspTimeType m_nInterval;

public:
  sspTimeUpdate() : m_nInterval(0) {}
  sspTimeUpdate(sspTimeType nInterval) : m_nInterval(nInterval) {}

  void setInterval(sspTimeType nInterval) {m_nInterval = nInterval;}

  void initialize(bool bIncrement = true)
  {
    m_next.getCurrentTime();
    if (bIncrement) m_next += m_nInterval;
  }

  bool update()
  {
    bool bUpdate = (sspTime() > m_next);
    if (bUpdate) m_next += m_nInterval;
    return bUpdate;
  }
};

// The sspDate class is only concerned with days and months within a year
class sspDate
{
protected:
	static int m_nMonths[];

  int m_nDay;   
  int m_nMon;     

public:
  sspDate() : m_nDay(1), m_nMon(1) {}
	sspDate(int nDay, int nMonth) : m_nDay(nDay), m_nMon(nMonth) {}
  sspDate(const sspDate& date) : m_nDay(date.m_nDay), m_nMon(date.m_nMon) {}
	sspDate& operator= (const sspDate& date);
  virtual ~sspDate() {}

 	void setDay(int nDay) {m_nDay = nDay;}
	void setMonth(int nMonth) {m_nMon = nMonth;}

  int day() const   {return m_nDay;}
  int month() const {return m_nMon;}

  int daysInYear() const 
  {
	  int nDay = m_nDay;
	  for (int i=0; i<m_nMon-1; ++i)
		  nDay += m_nMonths[i];
	  return nDay;
  }

  int operator- (const sspDate& date) const
  {
    return daysInYear() - date.daysInYear();
  }
  bool operator> (const sspDate& date) const
  {
    return daysInYear() > date.daysInYear();
  }
  bool operator< (const sspDate& date) const
  {
    return daysInYear() < date.daysInYear();
  }
  bool operator>= (const sspDate& date) const
  {
    return daysInYear() >= date.daysInYear();
  }
  bool operator<= (const sspDate& date) const
  {
    return daysInYear() <= date.daysInYear();
  }
  bool operator== (const sspDate& date) const
  {
    return m_nDay == date.m_nDay && m_nMon == date.m_nMon;
  }
	bool isValid() const 
  {
    return m_nMon >= 1 && m_nMon <= 12 && m_nDay >= 1 && m_nDay <= m_nMonths[m_nMon-1];
  }

	const char* dateAsString(char *buffer, int nMaxSize) const;
	const wchar_t* dateAsString(wchar_t *buffer, int nMaxSize) const;
};

class sspClock
{
protected:
  int m_nSec;     
  int m_nMin;     
  int m_nHour;    

public:
  sspClock() : m_nHour(0), m_nMin(0), m_nSec(0) {}
	sspClock(int nHour, int nMin, int nSec) : m_nHour(nHour), m_nMin(nMin), m_nSec(nSec) {}
  sspClock(const sspClock& clock) : m_nHour(clock.m_nHour), m_nMin(clock.m_nMin), m_nSec(clock.m_nSec) {}
	sspClock& operator= (const sspClock& clock);
  virtual ~sspClock() {}

	void setHour(int nHour) {m_nHour = nHour;}
	void setMinute(int nMin) {m_nMin = nMin;}
	void setSecond(int nSec) {m_nSec = nSec;}

  int sec() const   {return m_nSec; };
  int min() const   {return m_nMin; };
  int hour() const  {return m_nHour;};

  int secondsInDay() const 
  {
    return m_nSec + 60 * (m_nMin + 60 * m_nHour);
  }

  int operator- (const sspClock& clock) const
  {
    return secondsInDay() - clock.secondsInDay();
  }
  bool operator> (const sspClock& clock) const
  {
    return secondsInDay() > clock.secondsInDay();
  }
  bool operator< (const sspClock& clock) const
  {
    return secondsInDay() < clock.secondsInDay();
  }
  bool operator>= (const sspClock& clock) const
  {
    return secondsInDay() >= clock.secondsInDay();
  }
  bool operator<= (const sspClock& clock) const
  {
    return secondsInDay() <= clock.secondsInDay();
  }
  bool operator== (const sspClock& clock) const
  {
    return m_nHour == clock.m_nHour && m_nMin == clock.m_nMin && m_nSec == clock.m_nSec;
  }
	bool isValid() const
  {
    return m_nHour < 24 && m_nMin < 60 && m_nSec < 60;
  }

  const char* clockAsString(char *buffer, int nMaxSize) const;
	const wchar_t* clockAsString(wchar_t *buffer, int nMaxSize) const;
};

class sspLocalTime : public sspTime, public sspDate, public sspClock
{
public:
  enum {SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, WEEKDAYS};
	static const char s_dayNames[WEEKDAYS][10];

  sspLocalTime();
  virtual ~sspLocalTime() {}

  sspTime& getCurrentTime();

  int year() const  { return m_nYear; };
	int dayOfWeek() const {return m_nDayOfWeek;}
  int dayOfYear() const {return m_nDayOfYear;}

protected:
  void localTime();

private:
  int m_nYear;   
	int m_nDayOfWeek;
  int m_nDayOfYear;
};

#endif
