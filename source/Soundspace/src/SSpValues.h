// SSpValues.h: interface for the SSpFloat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSPVALUES_H__9576A660_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_)
#define AFX_SSPVALUES_H__9576A660_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include "SSpBasic.h"
#include "ClockDate.h"

class SSpBasicValue : public sspValue  
{
protected:
	float	m_fVal;
    DECLARE_SERIAL( SSpBasicValue )

public:
	SSpBasicValue();
	SSpBasicValue(const SSpBasicValue& val);
	SSpBasicValue& operator= (const SSpBasicValue& val);
	virtual ~SSpBasicValue();

	void Serialize(CArchive& ar);

	// Virtual methods
	float	getValue() {return m_fVal;}
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings) { return true; }

	// Accessors
	void setValue(float fVal) {m_fVal = fVal;}
};

class SSpInputValueRange : public sspValue  
{
private:
	float m_fVal;
	float m_fMin;
	float m_fMax;
    DECLARE_SERIAL( SSpInputValueRange )

public:
	SSpInputValueRange();
	SSpInputValueRange(const SSpInputValueRange& val);
	SSpInputValueRange& operator= (const SSpInputValueRange& val);
	virtual ~SSpInputValueRange();

	void Serialize(CArchive& ar);

	// Virtual methods
	float	getValue() {return m_fVal;}
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setValue(float fVal);          // Value between m_fMin and m_fMax
	void setValueRange(float fmin, float fMax);
  void setRelativeValue(float fVal);  // Value between 0.0 and 1.0
  float getRelativeValue() const; 

	float getRangeMin() const {return m_fMin;}
	float getRangeMax() const {return m_fMax;}
};

class SSpTimeMap : public sspValue  
{
private:
	SSpClock	m_clockStart;
	SSpClock	m_clockEnd;
	float		m_fSeconds;
	float		m_fValStart;
	float		m_fValEnd;
    DECLARE_SERIAL( SSpTimeMap )

public:
	SSpTimeMap();
	SSpTimeMap(const SSpTimeMap& val);
	SSpTimeMap& operator= (const SSpTimeMap& val);
	virtual ~SSpTimeMap();

	void Serialize(CArchive& ar);

	// Virtual methods
	float	getValue();
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setStartClock(SSpClock& clock);
	void setEndClock(SSpClock& clock);
	void setStartValue(float fVal) {m_fValStart = fVal;}
	void setEndValue(float fVal) {m_fValEnd = fVal;}

	SSpClock&	getStartClock() {return m_clockStart;}
	SSpClock&	getEndClock() {return m_clockEnd;}
	float		getStartValue() const {return m_fValStart;}
	float		getEndValue() const {return m_fValEnd;}
};

class SSpDateMap : public sspValue  
{
private:
	SSpDate		m_dateStart;
	SSpDate		m_dateEnd;
	float		m_fDays;
	float		m_fValStart;
	float		m_fValEnd;
    DECLARE_SERIAL( SSpDateMap )

public:
	SSpDateMap();
	SSpDateMap(const SSpDateMap& val);
	SSpDateMap& operator= (const SSpDateMap& val);
	virtual ~SSpDateMap();

	void Serialize(CArchive& ar);

	// Virtual methods
	float	getValue();
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setStartDate(SSpDate& clock);
	void setEndDate(SSpDate& clock);
	void setStartValue(float fVal) {m_fValStart = fVal;}
	void setEndValue(float fVal) {m_fValEnd = fVal;}

	SSpDate&	getStartDate() {return m_dateStart;}
	SSpDate&	getEndDate() {return m_dateEnd;}
	float		getStartValue() const {return m_fValStart;}
	float		getEndValue() const {return m_fValEnd;}
};

class SSpLinearMap : public sspValue  
{
private:
	int		m_nValueIndex;
	float	m_fInputStart;
	float	m_fInputEnd;
	float	m_fOutputStart;
	float	m_fOutputEnd;
    DECLARE_SERIAL( SSpLinearMap )

public:
	SSpLinearMap();
	SSpLinearMap(const SSpLinearMap& val);
	SSpLinearMap& operator= (const SSpLinearMap& val);
	virtual ~SSpLinearMap();

	void Serialize(CArchive& ar);

	// Virtual methods
	float	getValue();
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setValueIndex(int nIndex) {m_nValueIndex = nIndex;}
	void setInputStartValue(float fVal) {m_fInputStart = fVal;}
	void setInputEndValue(float fVal) {m_fInputEnd = fVal;}
	void setOutputStartValue(float fVal) {m_fOutputStart = fVal;}
	void setOutputEndValue(float fVal) {m_fOutputEnd = fVal;}

	int	  getValueIndex() const {return m_nValueIndex;}
	float getInputStartValue() const {return m_fInputStart;}
	float getInputEndValue() const {return m_fInputEnd;}
	float getOutputStartValue() const {return m_fOutputStart;}
	float getOutputEndValue() const {return m_fOutputEnd;}
};

class SSpAddValue : public sspValue  
{
private:
	sspIntVec m_nValues;
    DECLARE_SERIAL( SSpAddValue )

public:
	SSpAddValue();
	SSpAddValue(const SSpAddValue& val);
	SSpAddValue& operator= (const SSpAddValue& val);
	virtual ~SSpAddValue();

	void Serialize(CArchive& ar);

	// Virtual methods
	float	getValue();
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setValueList(const sspIntVec& nValues) {m_nValues = nValues;}
	const sspIntVec& getValueList() const {return m_nValues;}
};

class SSpProductValue : public sspValue  
{
private:
	sspIntVec m_nValues;
    DECLARE_SERIAL( SSpProductValue )

public:
	SSpProductValue();
	SSpProductValue(const SSpProductValue& val);
	SSpProductValue& operator= (const SSpProductValue& val);
	virtual ~SSpProductValue();

	void Serialize(CArchive& ar);

	// Virtual methods
	float	getValue();
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setValueList(const sspIntVec& nValues) {m_nValues = nValues;}
	const sspIntVec& getValueList() const {return m_nValues;}
};

class SSpConditionalValue : public sspValue  
{
private:
	int			m_nDefaultValue;
	sspIntVec	m_nValues;
	sspIntVec	m_nConditionals;
    DECLARE_SERIAL( SSpConditionalValue )

public:
	SSpConditionalValue();
	SSpConditionalValue(const SSpConditionalValue& val);
	SSpConditionalValue& operator= (const SSpConditionalValue& val);
	virtual ~SSpConditionalValue();

	void Serialize(CArchive& ar);

	// Virtual methods
	float	getValue();
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setDefaultValue(int nValIndex) {m_nDefaultValue = nValIndex;}
	void setValueList(const sspIntVec& nValues) {m_nValues = nValues;}
	void setConditionals(const sspIntVec& nConditionals) {m_nConditionals = nConditionals;}

	int	 getDefaultValue() const {return m_nDefaultValue;}
	const sspIntVec& getValueList() const {return m_nValues;}
	const sspIntVec& getConditionals() const {return m_nConditionals;}
};

class SSpRandomValue : public sspValue  
{
private:
	int	m_nValueLow;
	int m_nValueHigh;
  DECLARE_SERIAL( SSpRandomValue )

public:
	SSpRandomValue();
	SSpRandomValue(const SSpRandomValue& val);
	SSpRandomValue& operator= (const SSpRandomValue& val);
	virtual ~SSpRandomValue();

	void Serialize(CArchive& ar);

	// Virtual methods
	float	getValue();
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setValueLow(int nValIndex) {m_nValueLow = nValIndex;}
	void setValueHigh(int nValIndex) {m_nValueHigh = nValIndex;}

	int getValueLow() const {return m_nValueLow;}
	int getValueHigh() const {return m_nValueHigh;}
};

class sspSecondsRun : public sspValue  
{
protected:
    DECLARE_SERIAL( sspSecondsRun )

public:
	sspSecondsRun();
	sspSecondsRun(const sspSecondsRun& val);
	sspSecondsRun& operator= (const sspSecondsRun& val);
	virtual ~sspSecondsRun();

	void Serialize(CArchive& ar);

	// Virtual methods
	float	getValue();
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings) {return true;}
};

#endif // !defined(AFX_SSPVALUES_H__9576A660_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_)
