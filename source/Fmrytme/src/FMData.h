// FMData.h: interface for the CFMSang class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMDATA_H__4E885102_90E6_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_FMDATA_H__4E885102_90E6_11D2_97D2_00105A3B4608__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "operator.h"
#include "mididata.h"

class CFMSekvens : public CObject
{
private:
	CTypedPtrList<CObList, CFMNote*> m_pSekvens;
	COperator* m_pModulator;
	POSITION m_pos;
	int m_nGrunnpuls;
	int m_nModOffset;
	int m_nSynkPunkt;
	double m_dStart;
	double m_dSlutt;
	float *m_dKurve;
	long m_nKurveStart;
	long m_nKurveSlutt;

public:
	CFMSekvens();
	virtual ~CFMSekvens();

	void LoadSekvens(CMidiSekvens* pMidiSekvens, int taktLng, double minTid);
	void SetModulator(COperator* pMod);
	void SetGrunnpuls(FMPlotInfo* pInfo, int nSlag);
	void ResetOffset();
	void SetStart(double dStart) {m_dStart = dStart;}
	void SetSlutt(double dSlutt) {m_dSlutt = dSlutt;}
	COperator* GetModulator(void) const {return m_pModulator;}
	int GetGrunnpuls() const {return m_nGrunnpuls;}
	int GetSynkPunkt() const {return m_nSynkPunkt;}
	int GetModOffset() const {return m_nModOffset;}
	void GetStartSlutt(double &start, double &slutt) const {start = m_dStart; slutt = m_dSlutt;}

	void LeggForan(CFMNote* note) {m_pos = m_pSekvens.AddHead(note);}
	void LeggBak(CFMNote* note) {m_pos = m_pSekvens.AddTail(note);}
	void InitPos(void) {m_pos = m_pSekvens.GetHeadPosition();}
	CFMNote* GetNote(void) {return m_pSekvens.GetAt(m_pos);}
	CFMNote* GetNeste(void) {return m_pSekvens.GetNext(m_pos);}

	double ComputePlot(FMPlotInfo* pInfo);
	void UpdatePeriode(FMPlotInfo* pInfo);
	void UpdateAmplitude(void);
	void Plot(CDC* pDC, FMPlotInfo* pInfo, int linjeNr, double xFak, double yFak, int xOffset, int yOffset);
};

class CFMSang : public CObject
{
private:
	CTypedPtrArray<CObArray, CFMSekvens*> m_pSang;
	int m_nSpor;
	double m_dMaxAmplitude;
	double m_dStart;
	double m_dSlutt;
	FMPlotInfo* m_pInfo;

public:
	enum {ALLE_SPOR = -1};

	CFMSang(FMPlotInfo* pInfo);
	virtual ~CFMSang();

	void LoadSang(CMidiSang* pSang, double minTid = 0.0);
	void SetSekvens(CFMSekvens* pSekv, int nInd);
	void LeggTilSekvens(CFMSekvens* pSekv);
	void FjernSekvens(int nInd);
	CFMSekvens* GetSekvens(int nInd) const {return m_pSang[nInd];}
	double GetSlag() const {return m_dSlutt;}
	double GetMaxAmplitude() const {return m_dMaxAmplitude;}
	void SetTaktLengde(int nSlag);
	void LikeModulatorer(int nSekvNr);
	void SetGrunnpuls(int nGrunnpuls);

	void ComputePlot(int sporNr = ALLE_SPOR);
	void UpdatePeriode(void);
	void UpdateAmplitude(void);
	void Plot(CDC* pDC, double xFak, double yFak, int xOff, int yOff, int yGraf, int sporNr = ALLE_SPOR);
};

#endif // !defined(AFX_FMDATA_H__4E885102_90E6_11D2_97D2_00105A3B4608__INCLUDED_)
