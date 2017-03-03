// Time.h: interface for the CTimeRef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIME_H__BEA86C42_838A_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_TIME_H__BEA86C42_838A_11D2_97D2_00105A3B4608__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTimeRef : public CObject  
{
public:
	virtual double const GetTid() = 0;
};

class CMidiTime : public CTimeRef
{
private:
	int m_nMetronom;
	int m_nTidsformat;
	double m_dTidsfaktor;
	void BeregnFaktor(void);
	DECLARE_SERIAL(CMidiTime)

public:
	enum {DEFAULT_METRO = 120, DEFAULT_FORMAT = 192};
 
	CMidiTime();
	virtual void Serialize(CArchive& ar);

	void SetMetronom(int metronom);
	void SetTempoEvent(long tempo);
	void SetTidsformat(int tidsformat);
	int const GetMetronom() {return m_nMetronom;}
	int const GetTidsformat() {return m_nTidsformat;}
	double const GetTid() {return m_dTidsfaktor;}
};

class CFMTime : public CTimeRef
{
private:
	volatile double m_dDistanse;

public:
	CFMTime();

	void SetDistanse(double distanse) {m_dDistanse = distanse;}
	double const GetTid() {return m_dDistanse;}
};

#endif // !defined(AFX_TIME_H__BEA86C42_838A_11D2_97D2_00105A3B4608__INCLUDED_)
