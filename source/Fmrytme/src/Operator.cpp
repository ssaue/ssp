// Operator.cpp: implementation of the COperator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fmrytme.h"
#include "mainfrm.h"
#include <math.h>
#include "Operator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CFMOscillator, CObject, 0)
IMPLEMENT_SERIAL(CSumOp, CObject, 0)

///////////////////////////////////////////////////////////////////////
// COscillator
///////////////////////////////////////////////////////////////////////

const double CFMOscillator::m_dPi = 3.1415926535;

CFMOscillator::CFMOscillator() : COperator(COperator::FMOSCILLATOR)
{
	m_nFunksjon = SIN;
	m_nPotens = 1;
	m_dPeriode = CMainFrame::m_settings.m_nPeriode;
	m_dAmplitude = m_dFase = m_dFrekvens = 0.0;
	BeregnFaktor();
	m_pFrekMod = NULL;
}

CFMOscillator::CFMOscillator(int type) : COperator(type)
{
	m_nFunksjon = SIN;
	m_nPotens = 1;
	m_dPeriode = CMainFrame::m_settings.m_nPeriode;
	m_dAmplitude = m_dFase = m_dFrekvens = 0.0;
	BeregnFaktor();
	m_pFrekMod = NULL;
}

CFMOscillator::CFMOscillator(const CFMOscillator& osc) : COperator(osc.m_nType)
{
	m_nFunksjon = osc.m_nFunksjon;
	m_dAmplitude = osc.m_dAmplitude;
	m_dFase = osc.m_dFase;
	m_dFrekvens = osc.m_dFrekvens;
	m_dPeriode = osc.m_dPeriode;
	m_nPotens = osc.m_nPotens;
	BeregnFaktor();
	if (osc.m_pFrekMod == NULL)
		m_pFrekMod = NULL;
	else {
		switch (osc.m_pFrekMod->GetType())
		{
		case PARALLELLOSC:
		case SUMOPERATOR:
			m_pFrekMod = new CSumOp(*((CSumOp*) osc.m_pFrekMod));
			break;
		case FMOSCILLATOR:
		case SERIEOSC:
		default:
			m_pFrekMod = new CFMOscillator(*((CFMOscillator*) osc.m_pFrekMod));
			break;
		}
	}
}

CFMOscillator::~CFMOscillator()
{
	if (m_nType != FMNOTE) {
		delete m_pFrekMod;
	}
}

void CFMOscillator::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << m_nType << m_dAmplitude << m_dFase << m_dFrekvens << m_nFunksjon << m_nPotens;
		if (m_pFrekMod != NULL) {
			ar << m_pFrekMod->GetType() << m_pFrekMod;
		}
		else {
			ar << COperator::INGEN;
		}
	}
	else {
		ar >> m_nType >> m_dAmplitude >> m_dFase >> m_dFrekvens >> m_nFunksjon >> m_nPotens;
		BeregnFaktor();
		int nOpType;
		ar >> nOpType;
		switch (nOpType)
		{
		case COperator::FMOSCILLATOR:
		case COperator::SERIEOSC:
			CFMOscillator* oscbuf;
			ar >> oscbuf;
			m_pFrekMod = oscbuf;
			break;
		case COperator::PARALLELLOSC:
		case COperator::SUMOPERATOR:
			CSumOp* sumbuf;
			ar >> sumbuf;
			m_pFrekMod = sumbuf;
			break;
		case COperator::INGEN:
		default:
			break;
		}
	}
}

void CFMOscillator::BeregnFaktor()
{
	switch (m_nFunksjon)
	{
	case SIN:
		m_dFrekFak = m_dFrekvens * 2.0 * m_dPi / m_dPeriode;
		m_dFaseFak = 2.0 * m_dPi * m_dFase;
		break;
	case TREKANT:
	case SAGTANN:
	case FIRKANT:
	default:
		m_dFrekFak = m_dFrekvens / m_dPeriode;
		m_dFaseFak = m_dFase;
		break;
	}
}

void CFMOscillator::UpdatePeriode(double dPeriode)
{
	m_dPeriode = dPeriode;
	BeregnFaktor();
	if (m_pFrekMod != NULL)
		m_pFrekMod->UpdatePeriode(dPeriode);
}

void CFMOscillator::SetParameter(int nPara, double dValue)
{
	switch (nPara)
	{
	case AMPLITUDE:
		m_dAmplitude = dValue;
		break;
	case FREKVENS:
		m_dFrekvens = dValue;
		BeregnFaktor();
		break;
	case FASE:
		m_dFase = dValue;
		BeregnFaktor();
		break;
	case POTENS:
		m_nPotens = (int) dValue;
		break;
	case FUNKSJON:
		m_nFunksjon = (int) dValue;
		BeregnFaktor();
		break;
	default:
		break;
	}
}

double CFMOscillator::GetParameter(int nPara) const
{
	switch (nPara)
	{
	case AMPLITUDE:
		return m_dAmplitude;
		break;
	case FREKVENS:
		return m_dFrekvens;
		break;
	case FASE:
		return m_dFase;
		break;
	case POTENS:
		return m_nPotens;
		break;
	case FUNKSJON:
		return m_nFunksjon;
		break;
	default:
		return 0.0;
		break;
	}
}

inline double CFMOscillator::trekant(double tid) const
{
	tid -= floor(tid);
	if (tid < 0.25)
		return 4 * tid;
	else if (tid > 0.75)
		return 4 * tid - 4.0;
	else
		return 2.0 - 4 * tid;
}

inline double CFMOscillator::sagtann(double tid) const
{
	tid -= floor(tid);
	if (tid < 0.5)
		return 2 * tid;
	else
		return 2 * tid - 2.0;
}

inline double CFMOscillator::firkant(double tid) const
{
	tid -= floor(tid);
	if (tid == 0.0 || tid == 0.5)
		return 0.0;
	else if (tid < 0.5)
		return 1.0;
	else
		return -1.0;
}

double CFMOscillator::GetValue(long tid) const
{
	double utr = m_dFrekFak * tid + ((m_pFrekMod != NULL) ? m_pFrekMod->GetValue(tid) : 0) + m_dFaseFak;
	switch (m_nFunksjon) 
	{
	case SIN:
		utr = sin(utr);
		break;
	case TREKANT:
		utr = trekant(utr);
		break;
	case SAGTANN:
		utr = sagtann(utr);
		break;
	case FIRKANT:
		utr = firkant(utr);
		break;
	default:
		utr = 0;
		break;
	}
	return m_dAmplitude * pow(utr, m_nPotens);
	}


///////////////////////////////////////////////////////////////////////
// CFMNote
///////////////////////////////////////////////////////////////////////

CFMNote::CFMNote() : CFMOscillator(COperator::FMNOTE)
{
	m_dLengde = 0.0;
	BeregnFaktor();
}

CFMNote::CFMNote(int taktslag) : CFMOscillator(COperator::FMNOTE) 
{
	m_dLengde = 0.0;
	m_dFrekvens = taktslag;
	BeregnFaktor();
}

void CFMNote::SetAmplitude(BYTE velocity)
{
	m_nVelocity = velocity;
	UpdateAmplitude();
}

void CFMNote::UpdateAmplitude()
{
	m_dAmplitude = pow(m_nVelocity, FMPlotInfo::m_dAmpVelocity);
}

inline double CFMNote::GetValue(long tid) const
{
	double utr = m_dFrekFak * tid + ((m_pFrekMod != NULL) ? m_pFrekMod->GetValue(tid) : 0) + m_dFaseFak;
	return m_dAmplitude * (1 - cos(utr/m_dLengde));
}

///////////////////////////////////////////////////////////////////////
// CSumOp
///////////////////////////////////////////////////////////////////////

CSumOp::CSumOp() : COperator(COperator::SUMOPERATOR)
{
	m_pLeftOp = m_pRightOp = NULL;
};

CSumOp::CSumOp(int type) : COperator(type)
{
	m_pLeftOp = m_pRightOp = NULL;
};

CSumOp::CSumOp(const CSumOp& sumOp) : COperator(sumOp.m_nType)
{
	if (sumOp.m_pLeftOp == NULL)
		m_pLeftOp = NULL;
	else {
		switch (sumOp.m_pLeftOp->GetType())
		{
		case PARALLELLOSC:
		case SUMOPERATOR:
			m_pLeftOp = new CSumOp(*((CSumOp*) sumOp.m_pLeftOp));
			break;
		case FMOSCILLATOR:
		case SERIEOSC:
		default:
			m_pLeftOp = new CFMOscillator(*((CFMOscillator*) sumOp.m_pLeftOp));
			break;
		}
	}
	if (sumOp.m_pRightOp == NULL)
		m_pRightOp = NULL;
	else {
		switch (sumOp.m_pRightOp->GetType())
		{
		case PARALLELLOSC:
		case SUMOPERATOR:
			m_pRightOp = new CSumOp(*((CSumOp*) sumOp.m_pRightOp));
			break;
		case FMOSCILLATOR:
		case SERIEOSC:
		default:
			m_pRightOp = new CFMOscillator(*((CFMOscillator*) sumOp.m_pRightOp));
			break;
		}
	}
};

CSumOp::~CSumOp()
{
	delete m_pLeftOp;
	delete m_pRightOp;
}

void CSumOp::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << m_nType;
		if (m_pLeftOp != NULL) {
			ar << m_pLeftOp->GetType() << m_pLeftOp;
		}
		else {
			ar << COperator::INGEN;
		}
		if (m_pRightOp != NULL) {
			ar << m_pRightOp->GetType() << m_pRightOp;
		}
		else {
			ar << COperator::INGEN;
		}
	}
	else {
		int nOpType;
		ar >> m_nType >> nOpType;
		switch (nOpType)
		{
		case COperator::FMOSCILLATOR:
		case COperator::SERIEOSC:
			CFMOscillator* oscbuf;
			ar >> oscbuf;
			m_pLeftOp = oscbuf;
			break;
		case COperator::PARALLELLOSC:
		case COperator::SUMOPERATOR:
			CSumOp* sumbuf;
			ar >> sumbuf;
			m_pLeftOp = sumbuf;
			break;
		case COperator::INGEN:
		default:
			break;
		}
		ar >> nOpType;
		switch (nOpType)
		{
		case COperator::FMOSCILLATOR:
		case COperator::SERIEOSC:
			CFMOscillator* oscbuf;
			ar >> oscbuf;
			m_pRightOp = oscbuf;
			break;
		case COperator::PARALLELLOSC:
		case COperator::SUMOPERATOR:
			CSumOp* sumbuf;
			ar >> sumbuf;
			m_pRightOp = sumbuf;
			break;
		case COperator::INGEN:
		default:
			break;
		}
	}
}

void CSumOp::SetOperator(int pos, COperator* pOp)
{
	if (pos == LEFT)
		m_pLeftOp = pOp;
	else
		m_pRightOp = pOp;
}

COperator* CSumOp::GetOperator(int pos)
{
	if (pos == LEFT)
		return m_pLeftOp;
	else
		return m_pRightOp;
}

inline double CSumOp::GetValue(long tid) const
{
	return ((m_pLeftOp != NULL) ? m_pLeftOp->GetValue(tid) : 0) + ((m_pRightOp != NULL) ? m_pRightOp->GetValue(tid) : 0);
}

void CSumOp::UpdatePeriode(double dPeriode)
{
	if (m_pLeftOp != NULL)
		m_pLeftOp->UpdatePeriode(dPeriode);
	if (m_pRightOp != NULL)
		m_pRightOp->UpdatePeriode(dPeriode);
}
