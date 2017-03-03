// SSpConditionals.h: interface for the SSpBoolean class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSPCONDITIONALS_H__9576A666_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_)
#define AFX_SSPCONDITIONALS_H__9576A666_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include "SSpBasic.h"
#include "ClockDate.h"

class SSpBoolean : public sspConditional  
{
private:
	bool m_bValue;
    DECLARE_SERIAL( SSpBoolean )

public:
	SSpBoolean();
	SSpBoolean(const SSpBoolean& cond);
	SSpBoolean& operator= (const SSpBoolean& cond);
	virtual ~SSpBoolean();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings) {return true;}

	// Accessors
	void setValue(bool bVal) {m_bValue = bVal;}
};

class SSpValueRelation : public sspConditional  
{
private:
	int		m_nTestIndex;
	int		m_nThresholdIndex;
	int		m_nRelation;
    DECLARE_SERIAL( SSpValueRelation )

public:
	enum {EQUAL, UNEQUAL, SMALLER, LARGER, SMALL_EQ, LARGE_EQ};
	SSpValueRelation();
	SSpValueRelation(const SSpValueRelation& cond);
	SSpValueRelation& operator= (const SSpValueRelation& cond);
	virtual ~SSpValueRelation();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setTestValue(int nTestInd) {m_nTestIndex = nTestInd;}
	void setThresholdValue(int nThresholdInd) {m_nThresholdIndex = nThresholdInd;}
	void setRelation(int nRelation) {m_nRelation = nRelation;}

	int	getTestValue() const {return m_nTestIndex;}
	int	getThresholdValue() const {return m_nThresholdIndex;}
	int getRelation() const {return m_nRelation;}
};

class SSpInRange : public sspConditional  
{
private:
	int		m_nRangeMin;
	int		m_nRangeMax;
	int		m_nValueIndex;
    DECLARE_SERIAL( SSpInRange )

public:
	SSpInRange();
	SSpInRange(const SSpInRange& cond);
	SSpInRange& operator= (const SSpInRange& cond);
	virtual ~SSpInRange();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setRangeMin(int nRange) {m_nRangeMin = nRange;}
	void setRangeMax(int nRange) {m_nRangeMax = nRange;}
	void setTestValue(int nTestVal) {m_nValueIndex = nTestVal;}

	int	 getTestValue() const {return m_nValueIndex;}
	int	 getRangeMin() const {return m_nRangeMin;}
	int	 getRangeMax() const {return m_nRangeMax;}
};

class SSpTimeInterval : public sspConditional  
{
private:
	SSpClock	m_clockStart;
	SSpClock	m_clockEnd;
  DECLARE_SERIAL( SSpTimeInterval )

public:
	SSpTimeInterval();
	SSpTimeInterval(const SSpTimeInterval& cond);
	SSpTimeInterval& operator= (const SSpTimeInterval& cond);
	virtual ~SSpTimeInterval();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setStartClock(const SSpClock& clock) {m_clockStart = clock;}
	void setEndClock(const SSpClock& clock) {m_clockEnd = clock;}

	SSpClock&	getStartClock() {return m_clockStart;}
	SSpClock&	getEndClock() {return m_clockEnd;}
};

class SSpTimeRelation : public sspConditional  
{
private:
	SSpClock	m_clock;
	int			m_nRelation;
    DECLARE_SERIAL( SSpTimeRelation )

public:
	enum {BEFORE, AFTER};

	SSpTimeRelation();
	SSpTimeRelation(const SSpTimeRelation& cond);
	SSpTimeRelation& operator= (const SSpTimeRelation& cond);
	virtual ~SSpTimeRelation();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setClock(SSpClock& clock) {m_clock = clock;}
	void setRelation(int nRelation) {m_nRelation = nRelation;}

	SSpClock&	getClock() {return m_clock;}
	int			getRelation() {return m_nRelation;}
};

class SSpMinuteRelation : public sspConditional  
{
private:
	int	m_nMinute;
	int	m_nRelation;
    DECLARE_SERIAL( SSpMinuteRelation )

public:
	enum {EQUAL, BEFORE, AFTER};

	SSpMinuteRelation();
	SSpMinuteRelation(const SSpMinuteRelation& cond);
	SSpMinuteRelation& operator= (const SSpMinuteRelation& cond);
	virtual ~SSpMinuteRelation();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setMinute(int nMinute) {m_nMinute = nMinute;}
	void setRelation(int nRelation) {m_nRelation = nRelation;}

	int	getMinute() {return m_nMinute;}
	int	getRelation() {return m_nRelation;}
};

class SSpDateInterval : public sspConditional  
{
private:
	SSpDate	m_dateStart;
	SSpDate	m_dateEnd;
    DECLARE_SERIAL( SSpDateInterval )

public:
	SSpDateInterval();
	SSpDateInterval(const SSpDateInterval& cond);
	SSpDateInterval& operator= (const SSpDateInterval& cond);
	virtual ~SSpDateInterval();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setStartDate(const SSpDate& date) {m_dateStart = date;}
	void setEndDate(const SSpDate& date) {m_dateEnd = date;}

	SSpDate& getStartDate() {return m_dateStart;}
	SSpDate& getEndDate() {return m_dateEnd;}
};

class SSpDayOfWeek : public sspConditional  
{
public:

private:
  bool m_bDayCheck[sspLocalTime::WEEKDAYS];
	
    DECLARE_SERIAL( SSpDayOfWeek )

public:
	SSpDayOfWeek();
	SSpDayOfWeek(const SSpDayOfWeek& cond);
	SSpDayOfWeek& operator= (const SSpDayOfWeek& cond);
	virtual ~SSpDayOfWeek();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setDayCheck(int nDay, bool bTrue) {m_bDayCheck[nDay] = bTrue;}
	bool getDayCheck(int nDay) const {return m_bDayCheck[nDay];}
};

class SSpIsPlaying : public sspConditional  
{
private:
	sspIntVec m_nObjects;
    DECLARE_SERIAL( SSpIsPlaying )

public:
	SSpIsPlaying();
	SSpIsPlaying(const SSpIsPlaying& cond);
	SSpIsPlaying& operator= (const SSpIsPlaying& cond);
	virtual ~SSpIsPlaying();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setObjects(const sspIntVec& nObjects) {m_nObjects = nObjects;}
	const sspIntVec& getObjects() const {return m_nObjects;}
};

class SSpAnd : public sspConditional  
{
private:
	sspIntVec m_nConditionals;
    DECLARE_SERIAL( SSpAnd )

public:
	SSpAnd();
	SSpAnd(const SSpAnd& cond);
	SSpAnd& operator= (const SSpAnd& cond);
	virtual ~SSpAnd();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setConditionals(const sspIntVec& nConditionals) {m_nConditionals = nConditionals;}
	const sspIntVec& getConditionals() const {return m_nConditionals;}
};

class SSpOr : public sspConditional  
{
private:
	sspIntVec m_nConditionals;
    DECLARE_SERIAL( SSpOr )

public:
	SSpOr();
	SSpOr(const SSpOr& cond);
	SSpOr& operator= (const SSpOr& cond);
	virtual ~SSpOr();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setConditionals(const sspIntVec& nConditionals) {m_nConditionals = nConditionals;}
	const sspIntVec& getConditionals() const {return m_nConditionals;}
};

class SSpNot : public sspConditional  
{
private:
	int		m_nCondIndex;
    DECLARE_SERIAL( SSpNot )

public:
	SSpNot();
	SSpNot(const SSpNot& cond);
	SSpNot& operator= (const SSpNot& cond);
	virtual ~SSpNot();

	void Serialize(CArchive& ar);

	// Virtual methods
	bool isTrue();
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setConditional(int nCondIndex) {m_nCondIndex = nCondIndex;}
	int  getConditional() const {return m_nCondIndex;}
};

class SSpTrigger : public sspConditional  
{
public:
  enum {SSP_TRIG_TRUE, SSP_TRIG_FALSE, SSP_TRIG_BOTH};

protected:
	bool  m_bPrevious;
	int   m_nCondIndex;
  int   m_nMode;
  DECLARE_SERIAL( SSpTrigger )

public:
	SSpTrigger();
	SSpTrigger(const SSpTrigger& trigger);
	SSpTrigger& operator= (const SSpTrigger& trigger);
	virtual ~SSpTrigger();

	void Serialize(CArchive& ar);

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	bool isTrue();
	
	// Local methods
	void	reset();
	void	setConditional(int nCondIndex) {m_nCondIndex = nCondIndex;}
  void  setMode(int nMode) {m_nMode = nMode;}

  int		getConditional() const {return m_nCondIndex;}
  int   getMode() const {return m_nMode;}
};

#endif // !defined(AFX_SSPCONDITIONALS_H__9576A666_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_)
