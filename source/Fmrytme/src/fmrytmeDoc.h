// fmrytmeDoc.h : interface of the CFmrytmeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMRYTMEDOC_H__67E5840C_5474_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_FMRYTMEDOC_H__67E5840C_5474_11D2_97D2_00105A3B4608__INCLUDED_

#include "mididata.h"
#include "fmdata.h"
#include "dialog.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CFmrytmeDoc : public CDocument
{
protected: // create from serialization only
	CFmrytmeDoc();
	DECLARE_DYNCREATE(CFmrytmeDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmrytmeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	void ComputeEditSekvens(void);
	void ComputeAlle(void);
	void UpdatePeriode(void);
	CFMSekvens* GetEditSekvens(void);
	void LagSerieModulator();
	void LagParallellModulator();
	void LagEnkelModulator();
	void UpdateFrame();

	CMidiSang* m_pMIDISang;
	CFMSang* m_pFMSang;
	BOOL m_bSolo;
	FMPlotInfo m_plotInfo;

private:
	int m_nEditSpor;

	virtual ~CFmrytmeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFmrytmeDoc)
	afx_msg void OnImportMidi();
	afx_msg void OnSangmeny();
	afx_msg void OnUpdateSangmeny(CCmdUI* pCmdUI);
	afx_msg void OnSolo();
	afx_msg void OnUpdateSolo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateParameter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOppsett(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTempo(CCmdUI* pCmdUI);
	afx_msg void OnFjernStemme();
	afx_msg void OnUpdateFjernStemme(CCmdUI* pCmdUI);
	afx_msg void OnImportTekst();
	afx_msg void OnUpdateKurve(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGenerellmeny(CCmdUI* pCmdUI);
	afx_msg void OnExportMidi();
	afx_msg void OnUpdateExportMidi(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnSetStemme();
	afx_msg void OnSetTempo();
	afx_msg void OnEditedTempo();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMRYTMEDOC_H__67E5840C_5474_11D2_97D2_00105A3B4608__INCLUDED_)
