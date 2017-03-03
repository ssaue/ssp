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
	int m_nType;

public:
	enum {INGEN, FMOSCILLATOR, SUMOPERATOR, FMNOTE, SERIEOSC, PARALLELLOSC};

	COperator(int type) {m_nType = type;}
	COperator(const COperator& op) {m_nType = op.m_nType;}
	virtual double GetValue(long tid) const = 0;
	virtual void UpdatePeriode(double dPeriode) = 0;
	int GetType(void) const {return m_nType;}
	void SetType(int type) {m_nType = type;}
};

class CFMOscillator : public COperator
{
public:
	enum {SIN, TREKANT, SAGTANN, FIRKANT};
	enum {AMPLITUDE, FREKVENS, FASE, POTENS, FUNKSJON};
	static const double m_dPi;

protected:
	int m_nFunksjon;
	int m_nPotens;
	double m_dAmplitude;
	double m_dFrekvens;
	double m_dFase;
	double m_dPeriode;
	double m_dFrekFak;
	double m_dFaseFak;
	COperator* m_pFrekMod;
	DECLARE_SERIAL(CFMOscillator)

	void BeregnFaktor();

public:
	CFMOscillator();
	CFMOscillator(int type);
	CFMOscillator(const CFMOscillator& osc);
	virtual ~CFMOscillator();
	virtual void Serialize(CArchive& ar);

	void SetParameter(int nPara, double dValue);
	void SetModulator(COperator* pMod) {m_pFrekMod = pMod;}
	double GetParameter(int nPara) const;
	COperator* GetModulator() const {return m_pFrekMod;}
	double GetValue(long tid) const;
	void UpdatePeriode(double dPeriode);

private:
	double trekant(double tid) const;
	double sagtann(double tid) const;
	double firkant(double tid) const;
};

class CFMNote : public CFMOscillator
{
private:
	enum {DEFAULT_VELOCITY = 64};
	double m_dLengde;
	int m_nVelocity;
	CFMTime m_cTime;

public:
	CFMNote();
	CFMNote(int taktslag);
	virtual ~CFMNote() {}

	void SetPosisjon(double pos) {m_dFase = -pos; BeregnFaktor();}
	void SetLengde(double lengde) {m_dLengde = lengde;}
	void SetGrunnpuls(int tslag, double dPeriode) {m_dFrekvens = tslag; UpdatePeriode(dPeriode);}
	void SetDistanse(double distanse) {m_cTime.SetDistanse(distanse);}
	void SetAmplitude(BYTE velocity = DEFAULT_VELOCITY);
	CFMTime* GetTidsRef() {return &m_cTime;}
	double GetValue(long tid) const;
	void UpdatePeriode(double dPeriode) {m_dPeriode = dPeriode; BeregnFaktor();}
	void UpdateAmplitude();
};

class CSumOp : public COperator
{
private:
	COperator* m_pLeftOp;
	COperator* m_pRightOp;
	DECLARE_SERIAL(CSumOp)

public:
	enum {LEFT, RIGHT};
	CSumOp();
	CSumOp(int type);
	CSumOp(const CSumOp& sumOp);
	virtual ~CSumOp();
	virtual void Serialize(CArchive& ar);

	void SetOperator(int pos, COperator* pOp);
	COperator* GetOperator(int pos);
	double GetValue(long tid) const;
	void UpdatePeriode(double dPeriode);
};

#endif // !defined(AFX_OPERATOR_H__741206A0_8F5B_11D2_97D2_00105A3B4608__INCLUDED_)
