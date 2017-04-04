// FMData.cpp: implementation of the CFMSang class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fmrytme.h"
#include "mainfrm.h"
#include "fmrytmeDoc.h"
#include "FMData.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CFMSekvens
//////////////////////////////////////////////////////////////////////

CFMSekvens::CFMSekvens()
{
	m_pModulator = NULL;
	m_dKurve = NULL;
	m_nGrunnpuls = 4;
	m_nModOffset = m_nSynkPunkt = 0;
	m_dStart = m_dSlutt = 0.0;
	m_nKurveStart = m_nKurveSlutt = 0;
};

CFMSekvens::~CFMSekvens()
{
	delete m_pModulator;
	delete [] m_dKurve;
	while (m_pSekvens.GetHeadPosition())
		delete m_pSekvens.RemoveHead();
}

void CFMSekvens::LoadSekvens(CMidiSekvens* pMidiSekvens, int taktLng, double minTid)
{
	m_nGrunnpuls = taktLng;
	pMidiSekvens->InitPos();
	CMidiTidEvent* pEvent = pMidiSekvens->GetNeste();
	CFMNote* note = new CFMNote(taktLng);
	CFMNote* prevNote = NULL;
	double pos = 0.0;
	double start = 0.0;
	while (pEvent != NULL) {
		pos += pEvent->GetTid();
		start += pEvent->GetTid();
		pEvent->SetTidsRef(note->GetTidsRef());
		if (pEvent->IsNoteOn() && ((start > minTid) || m_pSekvens.IsEmpty())) {
			if (m_pSekvens.IsEmpty()) {
				m_dStart = pos;
				prevNote = new CFMNote(taktLng);	// Lager en noteenhet
				prevNote->SetPosisjon(-1.0);		// for start av bevegelsen
				prevNote->SetLengde(1.0);			// (foran slag 1)
				if (m_dStart > 0.0) {
					prevNote->SetAmplitude();
					LeggForan(prevNote);
					double figurLng = floor(m_dStart);
					figurLng = (m_dStart > figurLng) ? (m_dStart-figurLng) : 1.0;
					if (m_dStart > 1.0) {					// Lager en noteenhet
						prevNote = new CFMNote(taktLng);	// som teller til taktslag
						prevNote->SetPosisjon(0.0);			// før første note (ved opptakt)
						prevNote->SetLengde(m_dStart - figurLng);
						prevNote->SetAmplitude();
						LeggBak(prevNote);
					}
					prevNote = new CFMNote(taktLng);			// Lager en noteenhet som
					prevNote->SetPosisjon(m_dStart - figurLng);	// teller fram til start
					prevNote->SetLengde(figurLng);				// bevegelse (ved opptakt)
				}
			}
			else {
				prevNote->SetLengde(start);
			}
			prevNote->SetAmplitude(pEvent->GetVelocity());
			LeggBak(prevNote);
			note->SetPosisjon(pos);
			note->SetDistanse(start);
			start = 0.0;
			prevNote = note;
			note = new CFMNote(taktLng);
		}
		pEvent = pMidiSekvens->GetNeste();
	}
	prevNote->SetLengde(start);
	prevNote->SetAmplitude();
	LeggBak(prevNote);
	m_dSlutt = pos;
	note->SetDistanse(start);	// Fungerer kun som tidsreferanse for MIDI-avspilling
	note->SetPosisjon(m_dSlutt);
	LeggBak(note);
	pMidiSekvens->InitPos();				// Justerer tidsfaktor
	pEvent = pMidiSekvens->GetNeste();		// i MidiSekvensen
	while (pEvent != NULL) {				//
		pEvent->SetTidsfaktor();			// Dividerer med tidsReferansen
		pEvent = pMidiSekvens->GetNeste();	// i MidiSekvensen
	}
}

void CFMSekvens::SetModulator(COperator* pMod)
{
	m_pModulator = pMod;
	InitPos();
	while (m_pos != NULL) {
		GetNeste()->SetModulator(pMod);
	}
}

void CFMSekvens::UpdatePeriode(FMPlotInfo* pInfo)
{
	m_pModulator->UpdatePeriode(pInfo->GetSekvensPeriode(m_nGrunnpuls));
	InitPos();
	while (m_pos != NULL) {
		GetNeste()->UpdatePeriode(pInfo->GetSekvensPeriode(m_nGrunnpuls));
	}
}

void CFMSekvens::UpdateAmplitude()
{
	InitPos();
	while (m_pos != NULL) {
		GetNeste()->UpdateAmplitude();
	}
}

void CFMSekvens::SetGrunnpuls(FMPlotInfo* pInfo, int nSlag)
{
	m_nGrunnpuls = nSlag;
	m_pModulator->UpdatePeriode(pInfo->GetSekvensPeriode(m_nGrunnpuls));
	InitPos();
	while (m_pos != NULL) {
		GetNeste()->SetGrunnpuls(nSlag, pInfo->GetSekvensPeriode(m_nGrunnpuls));
	}
}

void CFMSekvens::ResetOffset()
{
	InitPos();
	CFMNote* pNote = GetNote();
	// Finner nærmeste nullpunkt og initialiserer modulasjons-offset
	m_nModOffset = 0;
	double dForrige = pNote->GetValue(0);
	double dVerdi = pNote->GetValue(m_nModOffset - 1);
	while (dVerdi < dForrige) {
		dForrige = dVerdi;
		m_nModOffset--;
		dVerdi = pNote->GetValue(m_nModOffset - 1);
	}
	dVerdi = pNote->GetValue(m_nModOffset + 1);
	while (dVerdi < dForrige) {
		dForrige = dVerdi;
		m_nModOffset++;
		dVerdi = pNote->GetValue(m_nModOffset + 1);
	}
	BOOL bTopp = FALSE;
	UINT j;
	for(j=1;j < CMainFrame::m_settings.m_nPeriode;j++) {
		dVerdi = pNote->GetValue(m_nModOffset-j);
		if (!bTopp && dVerdi < dForrige) {
			bTopp = TRUE;
		}
		else if (bTopp && dVerdi < pNote->GetParameter(CFMOscillator::AMPLITUDE)) {
			break;
		}
		dForrige = dVerdi;
	}
	m_nSynkPunkt = --j;
}

double CFMSekvens::ComputePlot(FMPlotInfo* pInfo)
{
	InitPos();
	CFMNote* pNote = GetNeste();
	delete m_dKurve;
	m_dKurve = new float[pInfo->GetSangLengde()];

	int nOffset; 
	switch (FMPlotInfo::m_nSynk)
	{
	case FMPlotInfo::INGEN:
		nOffset = m_nModOffset;
		break;
	case FMPlotInfo::STEMME:
		nOffset = m_nModOffset - pInfo->m_nModOffset;
		break;
	case FMPlotInfo::START_BEVEGELSE:
		nOffset = m_nSynkPunkt - pInfo->m_nSynkOffset;
		break;
	case FMPlotInfo::ENER:
	default:
		nOffset = 0;
		break;
	}
	int nForrigeTid = (-nOffset) - pInfo->GetSlagLengde();
	nOffset += CMainFrame::m_settings.m_nPeriode;

	long nSiste = pInfo->GetSangLengde() - nOffset;
	long i = 0;
	double dForrige = 0.0;
	double dVerdi;
	double dMaxAmp = 0.0;
	double dAmpFak;
	BOOL bTopp = FALSE;
	int j;
	if (m_dStart == 0.0) {				// En event spores bakover til nullgjennomgang
		m_dKurve[nOffset] = (float) dForrige;
		dAmpFak = pow((4.0/3.0) * m_nSynkPunkt/(double) pInfo->GetSlagLengde(), FMPlotInfo::m_dAmpLengde);
		for(j=1;j < nOffset;j++) {
			dVerdi = pNote->GetValue(m_nModOffset-j);
			if (!bTopp && dVerdi < dForrige) {
				bTopp = TRUE;
				dMaxAmp = dAmpFak * dForrige;
			}
			else if (bTopp && dVerdi < pNote->GetParameter(CFMOscillator::AMPLITUDE)) {
				break;
			}
			m_dKurve[nOffset - j] = (float) (dAmpFak * dVerdi);
			dForrige = dVerdi;
		}
		dForrige = 0.0;
		j--;
		m_nKurveStart = nOffset - j;
	}
	else {
		pNote = GetNeste();
		if (m_dStart > 1.0) {
			for(i=1;i < nSiste;i++) {		// Søker fram til første event
				dVerdi = pNote->GetValue(i+m_nModOffset);
				if (!bTopp && dVerdi < dForrige) {
					bTopp = TRUE;
				}
				else if (bTopp && dVerdi > dForrige) {
					break;
				}
				dForrige = dVerdi;
			}
			pNote = GetNeste();
		}
		for(;i < nSiste;i++) {	//Søker fram til nullgjennomgang
			dVerdi = pNote->GetValue(i+m_nModOffset);
			if (dVerdi >= pNote->GetParameter(CFMOscillator::AMPLITUDE))
				break;
		}
		m_nKurveStart = nOffset + i;
		dForrige = dVerdi;
		bTopp = FALSE;
		for (;i < nSiste;i++) {	// Tegner opp opptakten
			dVerdi = pNote->GetValue(i+m_nModOffset);
			m_dKurve[nOffset+i] = (float) dVerdi;
			if (!bTopp && dVerdi < dForrige) {
				bTopp = TRUE;
			}
			else if (bTopp && dVerdi > dForrige) {
				i--;
				break;
			}
			dForrige = dVerdi;
		}
	
		// Beregner amplitude som funksjon av notens faktiske lengde
		dAmpFak = pow((4.0/3.0) * (i+nOffset-m_nKurveStart)/(double) pInfo->GetSlagLengde(), FMPlotInfo::m_dAmpLengde);
		for (int k=m_nKurveStart; k<nOffset+i; k++) {
			m_dKurve[k] *= (float) dAmpFak;
			if (m_dKurve[k] > dMaxAmp)
				dMaxAmp = m_dKurve[k];
		}

	}
	BOOL bStartNote = TRUE;
	while (m_pos != NULL) {

		// Beregner amplitude som funksjon av notens faktiske lengde
		if (bStartNote)
			bStartNote = FALSE;
		else {
			dAmpFak = pow((i-nForrigeTid) / (double) pInfo->GetSlagLengde(), FMPlotInfo::m_dAmpLengde);
			for (int k=nForrigeTid; k<i; k++) {
				m_dKurve[nOffset+k] *= (float) dAmpFak;
				if (m_dKurve[nOffset+k] > dMaxAmp)
					dMaxAmp = m_dKurve[nOffset+k];
			}
		}

		pNote = GetNeste();
		pNote->SetDistanse((i-nForrigeTid) / (double) pInfo->GetSlagLengde());
		nForrigeTid = i;
		if (m_pos != NULL) {
			bTopp = FALSE;
			dForrige = 0.0;
			for (;i < nSiste;i++) {
				dVerdi = pNote->GetValue(i+m_nModOffset);
				if (!bTopp && dVerdi < dForrige) {
					bTopp = TRUE;
				}
				else if (bTopp && dVerdi > dForrige) {
					break;
				}
				m_dKurve[nOffset+i] = (float) dVerdi;
				dForrige = dVerdi;
			}
			i--;
		}
	}
	m_nKurveSlutt = nOffset + i;
	return dMaxAmp;
}

void CFMSekvens::Plot(CDC* pDC, FMPlotInfo* pInfo, int linjeNr, double xFak, double yFak, int xOffset, int yOffset)
{
	long n1 = linjeNr * pInfo->GetLinjeLengde();
	long n2 = n1 + pInfo->GetLinjeLengde();
	if (n2 < m_nKurveStart) 
		return;
	long i = 0;
	n2 = min(n2, m_nKurveSlutt);
	if (m_nKurveStart > n1) {
		i = m_nKurveStart - n1;
		n1 = m_nKurveStart;
	}
	pDC->MoveTo((int) (i*xFak + xOffset), (int) (m_dKurve[n1]*yFak + yOffset));
	for (i++, n1++; n1<=n2; n1++, i++)
		pDC->LineTo((int) (i*xFak + xOffset), (int) (m_dKurve[n1]*yFak + yOffset));
}

//////////////////////////////////////////////////////////////////////
// CFMSang
//////////////////////////////////////////////////////////////////////

CFMSang::CFMSang(FMPlotInfo* pInfo)
{
	m_pInfo = pInfo;
	m_nSpor = 0;
	m_dStart = m_dSlutt = 0.0;
}

CFMSang::~CFMSang()
{
	for (int i=0; i<m_nSpor; i++) {
		delete m_pSang[i];
	}
	m_pSang.RemoveAll();
}

void CFMSang::SetSekvens(CFMSekvens* pSekv, int nInd)
{
	m_pSang[nInd] = pSekv;
	double start, slutt;
	pSekv->GetStartSlutt(start, slutt);
	if (m_dSlutt == 0.0) {			
		m_dStart = start;	// Initieres første gang
		m_dSlutt = slutt;
	}
	else {
		m_dStart = (start < m_dStart) ? start : m_dStart;
		m_dSlutt = (slutt > m_dSlutt) ? slutt : m_dSlutt;
	}
	m_pInfo->UpdateTaktInfo(m_dSlutt);
}

void CFMSang::LeggTilSekvens(CFMSekvens* pSekv)
{
	int stemme = m_pSang.Add(pSekv);
	m_pSang.FreeExtra();
	m_nSpor = stemme + 1;
	double start, slutt;
	pSekv->GetStartSlutt(start, slutt);
	if (stemme == 0) {			
		m_dStart = start;	// Initieres første gang
		m_dSlutt = slutt;
	}
	else {
		m_dStart = (start < m_dStart) ? start : m_dStart;
		m_dSlutt = (slutt > m_dSlutt) ? slutt : m_dSlutt;
	}
	m_pInfo->UpdateTaktInfo(m_dSlutt);
}

void CFMSang::FjernSekvens(int nInd)
{
	CFMSekvens* pGml = m_pSang[nInd];
	if (pGml == NULL)
		return;			// Allerede tom
	m_pSang.RemoveAt(nInd);
	delete pGml;
	m_pSang.FreeExtra();
	m_nSpor--;
	for (int i=0; i<m_nSpor; i++) {		// Må initiere sang-data på nytt
		double start, slutt;
		m_pSang[i]->GetStartSlutt(start, slutt);
		if (i == 0) {			
			m_dStart = start;	// Initieres første gang
			m_dSlutt = slutt;
		}
		else {
			m_dStart = (start < m_dStart) ? start : m_dStart;
			m_dSlutt = (slutt > m_dSlutt) ? slutt : m_dSlutt;
		}
	}
	m_pInfo->UpdateTaktInfo(m_dSlutt);
}

void CFMSang::LoadSang(CMidiSang* pSang, double minTid)
{
	m_nSpor = pSang->GetSpor();
	m_pSang.SetSize(m_nSpor);
	for (int i=0; i<m_nSpor; i++) {
		CFMSekvens* pSekv = new CFMSekvens();
		pSekv->LoadSekvens(pSang->GetSekvens(i), pSang->GetTaktart(CMidiSang::TELLER), minTid);
		SetSekvens(pSekv, i);
	}
}

void CFMSang::ComputePlot(int sporNr)
{
	if (sporNr == ALLE_SPOR || sporNr == m_pInfo->m_nSynkStemme) {
		GetSekvens(m_pInfo->m_nSynkStemme)->ResetOffset();
		m_pInfo->m_nSynkOffset = GetSekvens(m_pInfo->m_nSynkStemme)->GetSynkPunkt();
		m_pInfo->m_nModOffset = GetSekvens(m_pInfo->m_nSynkStemme)->GetModOffset();
		m_dMaxAmplitude = 0.0;
		for (int i=0; i<m_nSpor; i++) {
			GetSekvens(i)->ResetOffset();
			double dAmp = GetSekvens(i)->ComputePlot(m_pInfo);
			if (dAmp > m_dMaxAmplitude)
				m_dMaxAmplitude = dAmp;
		}
	}
	else {
		GetSekvens(sporNr)->ResetOffset();
		double dAmp = GetSekvens(sporNr)->ComputePlot(m_pInfo);
		if (dAmp > m_dMaxAmplitude)
			m_dMaxAmplitude = dAmp;
	}
}

void CFMSang::UpdatePeriode()
{
	for (int i=0; i<m_nSpor; i++)
		GetSekvens(i)->UpdatePeriode(m_pInfo);
}

void CFMSang::UpdateAmplitude()
{
	for (int i=0; i<m_nSpor; i++)
		GetSekvens(i)->UpdateAmplitude();
}

void CFMSang::SetTaktLengde(int nSlag)
{
	for (int i=0; i<m_nSpor; i++)
		GetSekvens(i)->SetGrunnpuls(m_pInfo, nSlag);
}

void CFMSang::Plot(CDC* pDC, double xFak, double yFak, int xOff, int yOff, int yGraf, int sporNr)
{
	if (sporNr == ALLE_SPOR) {
		for (int i=0; i<m_nSpor; i++) {
			CPen newPen;
			newPen.CreatePen(PS_SOLID, 2, CMainFrame::m_settings.m_colFarge[i]);
			pDC->SelectObject(&newPen);
			for (int j=0; j<m_pInfo->m_nLinjeAnt; j++)
				GetSekvens(i)->Plot(pDC, m_pInfo, j, xFak, yFak, xOff, yOff-(j+1)*yGraf);
		}
	}
	else {
		CPen newPen;
		newPen.CreatePen(PS_SOLID, 2, CMainFrame::m_settings.m_colFarge[sporNr]);
		pDC->SelectObject(&newPen);
		for (int j=0; j<m_pInfo->m_nLinjeAnt; j++)
			GetSekvens(sporNr)->Plot(pDC, m_pInfo, j , xFak, yFak, xOff, yOff-(j+1)*yGraf);
	}
}

void CFMSang::SetGrunnpuls(int nGrunnpuls)
{
	for (int i=0; i<m_nSpor; i++)
		GetSekvens(i)->SetGrunnpuls(m_pInfo, nGrunnpuls);
}

void CFMSang::SetMetronom(int metronom)
{
	for (int i = 0; i < m_nSpor; i++)
		GetSekvens(i)->GetModulator()->SetMetronom(metronom);
}

void CFMSang::LikeModulatorer(int nSekvNr)
{
	int nGrunnpuls = GetSekvens(nSekvNr)->GetGrunnpuls();
	COperator* pMod = GetSekvens(nSekvNr)->GetModulator();
	for (int i=0; i<m_nSpor; i++) {
		if (i == nSekvNr)	// Denne er allerede i orden!
			continue;
		GetSekvens(i)->SetGrunnpuls(m_pInfo, nGrunnpuls);
		COperator* pGmlMod = GetSekvens(i)->GetModulator();
		switch (pMod->GetType())
		{
		case COperator::PARALLELLOSC:
			{
			CSumOp* pNyOsc = new CSumOp(*((CSumOp*) pMod));
			GetSekvens(i)->SetModulator(pNyOsc);
			break;
			}
		case COperator::FMOSCILLATOR:
		case COperator::SERIEOSC:
		default:
			{
			CFMOscillator* pNyOsc = new CFMOscillator(*((CFMOscillator*) pMod));
			GetSekvens(i)->SetModulator(pNyOsc);
			break;
			}
		}
		delete pGmlMod;
	}
}
