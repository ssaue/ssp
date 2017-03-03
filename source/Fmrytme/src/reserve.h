// Operator.h: interface for the COperator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPERATOR_H__741206A0_8F5B_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_OPERATOR_H__741206A0_8F5B_11D2_97D2_00105A3B4608__INCLUDED_

#include "time.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class COperator : public CObject  
{
protected:
	enum {OSCILLATOR, FMOSCILLATOR, SUMOPERATOR, FMNOTE, ENKELOSC, SERIEOSC, PARALLELLOSC};
	int m_nType;
public:
	COperator(int type) {m_nType = type;}
	virtual double GetValue(double tid) const = 0;
};

class COscillator : public COperator
{
public:
	enum {SIN, COS};
	enum {AMPLITUDE, FREKVENS, FASE};
	static const double m_dPi;
	static int m_nPeriode;
	static void SetPeriode(int periode) {m_nPeriode = periode;}

protected:
	int m_nFunksjon;
	double m_dAmplitude;
	double m_dFrekvens;
	double m_dFase;

public:
	COscillator();
	COscillator(int type);
	virtual ~COscillator() {}
	void SetParameter(int nPara, double dValue);
	void SetFunksjon(int nFunk) {m_nFunksjon = nFunk;}
	double GetValue(double tid) const;
};

class CFMOsc : public COscillator
{
protected:
	COperator* m_pFrekMod;

public:
	CFMOsc();
	CFMOsc(int type);
	virtual ~CFMOsc() {}
	void SetModulator(COperator* pMod) {m_pFrekMod = pMod;}
	double GetValue(double tid) const;
};

class CFMNote : public CFMOsc, public CTimeRef
{
private:
	double m_dLengde;
	double m_dPosisjon;
	int m_nTaktslag;
	CFMTime m_cTid;
	void BeregnFase();

public:
	CFMNote();
	CFMNote(int m_nTaktslag);
	virtual ~CFMNote() {}

	void SetPosisjon(double pos) {m_dPosisjon = pos;}
	void SetLengde(double lengde) {m_dLengde = lengde;}
	void SetTaktslag(int tslag) {m_nTaktslag = tslag;}
	void SetDistanse(double distanse) {m_cTid.SetDistanse(distanse);}
	void SetAmplitude(BYTE velocity);
	const CFMTime* GetTimeRef() const {return &m_cTid;}
	double GetValue(double tid) const;
};

class CSumOp : public COperator
{
private:
	COperator* m_pLeftOp;
	COperator* m_pRightOp;

public:
	CSumOp();
	virtual ~CSumOp() {}
	void SetOperator(COperator* pLeft, COperator* pRight);
	double GetValue(double tid) const;
};

class CSimpleFM : public COperator
{
#endif // !defined(AFX_OPERATOR_H__741206A0_8F5B_11D2_97D2_00105A3B4608__INCLUDED_)
