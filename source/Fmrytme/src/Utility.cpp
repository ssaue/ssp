// Utility.cpp: implementation of the UTFloatInt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mainfrm.h"
#include "fmrytme.h"
#include "fmrytmeDoc.h"
#include "Utility.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// FMPlotInfo Class
//////////////////////////////////////////////////////////////////////

int FMPlotInfo::m_nSynk = ENER;
double FMPlotInfo::m_dAmpLengde = 0.3;
double FMPlotInfo::m_dAmpVelocity = 0.3;

FMPlotInfo::FMPlotInfo()
{
	m_nLinjeAnt = m_nTaktAnt = 0;
	m_nTaktPrLinje = m_nSlagPrTakt = 1;
	m_nSynkOffset = m_nModOffset = 0;
	m_nSynkStemme = 0;
}

FMPlotInfo::FMPlotInfo(int taktAnt)
{
	m_nLinjeAnt = 0;
	m_nTaktAnt = taktAnt;
	m_nTaktPrLinje = m_nSlagPrTakt = 1;
	m_nSynkOffset = m_nModOffset = 0;
	m_nSynkStemme = 0;
}

void FMPlotInfo::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << m_nLinjeAnt << m_nTaktAnt << m_nTaktPrLinje << m_nSlagPrTakt;
		ar << m_nSynkOffset << m_nModOffset << m_nSynkStemme;
	}
	else {
		ar >> m_nLinjeAnt >> m_nTaktAnt >> m_nTaktPrLinje >> m_nSlagPrTakt;
		ar >> m_nSynkOffset >> m_nModOffset >> m_nSynkStemme;
	}
}

void FMPlotInfo::UpdateTaktInfo(double dSlag)
{
	m_nTaktAnt = (int) (dSlag / m_nSlagPrTakt) + 2;
	m_nTaktPrLinje = (int) ceil((double) m_nTaktAnt / MAX_LINJER);
	m_nLinjeAnt = (int) ceil(m_nTaktAnt / (double) m_nTaktPrLinje);
}

long FMPlotInfo::GetSangLengde()
{
	return m_nTaktAnt * (long) CMainFrame::m_settings.m_nPeriode;
}

long FMPlotInfo::GetLinjeLengde()
{
	return m_nTaktPrLinje * (long) CMainFrame::m_settings.m_nPeriode;
}

int FMPlotInfo::GetSlagLengde()
{
	return CMainFrame::m_settings.m_nPeriode / m_nSlagPrTakt;
}

double FMPlotInfo::GetSekvensPeriode(int nGrunnpuls)
{
	return CMainFrame::m_settings.m_nPeriode * (nGrunnpuls / (double) m_nSlagPrTakt);
}


//////////////////////////////////////////////////////////////////////
// FMPlotMarker Class
//////////////////////////////////////////////////////////////////////

FMPlotMarker::FMPlotMarker()
{
	m_nPlotType = NONE;
	m_dTakt = 0.0;
	m_nAmp = 0;
	m_bSet = FALSE;
}

FMPlotMarker::FMPlotMarker(int type)
{
	m_nPlotType = type;
	m_dTakt = 0.0;
	m_nAmp = 0;
	m_bSet = FALSE;
}


void FMPlotMarker::SetPos(CPoint point, int xOff, int xPlotReg, int yPlotReg, int yStep, FMPlotInfo* pInfo)
{
	m_pos = point;
	if (point.y > pInfo->m_nLinjeAnt*yPlotReg) {		// Dette er oppe i tekstfeltet
		m_bSet = FALSE;
		return;
	}
	else {
		point.x -= xOff;
		point.y -= yStep;
		int linje = point.y / yPlotReg;
		m_bSet = TRUE;
		m_nAmp = point.y - (linje * yPlotReg);
		m_dTakt = ((point.x/(double) xPlotReg) + pInfo->m_nLinjeAnt - (linje+1)) * pInfo->m_nTaktPrLinje;
	}
}

FMPlotMarker FMPlotMarker::operator-(const FMPlotMarker& pos2) const
{
	FMPlotMarker diffPos;
	if (IsSet() && pos2.IsSet()) {
		diffPos.m_nAmp = m_nAmp - pos2.m_nAmp;
		diffPos.m_dTakt = m_dTakt - pos2.m_dTakt;
		diffPos.m_bSet = TRUE;
	}
	return diffPos;
}

void FMPlotMarker::Plot(CDC* pDC, BOOL bOn) const
{
	if (!IsSet())
		return;
	if (bOn)
		pDC->SetTextColor(RGB(0,0,0));
	else
		pDC->SetTextColor(RGB(255,255,255));
	switch (m_nPlotType)
	{
	case LEFT: {
		CSize tegnSize = pDC->GetTextExtent("<");
		pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(m_pos.x - tegnSize.cx/2, m_pos.y + tegnSize.cy/2, "<");
		break;
			   }
	case RIGHT: {
		CSize tegnSize = pDC->GetTextExtent(">");
		pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(m_pos.x - tegnSize.cx/2, m_pos.y + tegnSize.cy/2, ">");
		break;
				}
	case NONE: 
	default:
		break;
	}
}



