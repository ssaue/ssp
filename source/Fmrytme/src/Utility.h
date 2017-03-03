// Utility.h: interface for the UTFloatInt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILITY_H__ACAB8D62_889E_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_UTILITY_H__ACAB8D62_889E_11D2_97D2_00105A3B4608__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class UTFloatInt  
{
public:
	int	m_nIndeks;
	double m_dVerdi;

	UTFloatInt(int indeks, double verdi) {m_nIndeks = indeks; m_dVerdi = verdi;}
};

typedef CTypedPtrList<CPtrList, UTFloatInt*> UTFloatIntList;

class FMPlotInfo : public CObject
{
public:
	enum {MAX_LINJER = 3};
	enum {INGEN, START_BEVEGELSE, ENER, STEMME};
	int m_nLinjeAnt;
	int m_nTaktAnt;
	int m_nTaktPrLinje;
	int m_nSlagPrTakt;
	int m_nSynkOffset;
	int m_nModOffset;
	int m_nSynkStemme;
	static int m_nSynk;
	static double m_dAmpLengde;
	static double m_dAmpVelocity;

	FMPlotInfo();
	FMPlotInfo(int taktArt);
	virtual void Serialize(CArchive& ar);
	void UpdateTaktInfo(double dSlag);
	long GetSangLengde();
	long GetLinjeLengde();
	int GetSlagLengde();
	double GetSekvensPeriode(int nGrunnpuls);
};

class FMPlotMarker
{
public:
	enum {NONE, LEFT, RIGHT};

private:
	int m_nPlotType;
	double m_dTakt;
	int m_nAmp;
	BOOL m_bSet;
	CPoint m_pos;

public:
	FMPlotMarker();
	FMPlotMarker(int type);
	virtual ~FMPlotMarker() {}

	void SetPos(CPoint point, int xOff, int xPlotReg, int yPlotReg, int yStep, FMPlotInfo* pInfo);
	BOOL IsSet(void) const {return m_bSet;}
	double GetTakt(void) const {return m_dTakt;}
	int GetAmp(void) const {return m_nAmp;}
	void Plot(CDC* pDC, BOOL bOn) const;
	FMPlotMarker operator-(const FMPlotMarker&) const;
};


#endif // !defined(AFX_UTILITY_H__ACAB8D62_889E_11D2_97D2_00105A3B4608__INCLUDED_)
