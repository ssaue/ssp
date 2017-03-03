// fmrytmeView.h : interface of the CFmrytmeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMRYTMEVIEW_H__67E5840E_5474_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_FMRYTMEVIEW_H__67E5840E_5474_11D2_97D2_00105A3B4608__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "dialog.h"

class CFmrytmeView : public CView
{
protected: // create from serialization only
	CFmrytmeView();
	DECLARE_DYNCREATE(CFmrytmeView)

public:
	enum {X_EXTENT = 1000, Y_EXTENT = 1000};
	enum {INGEN_SANG=1, NY_SANG, NY_STEMME, SOLO_SANG};
	//{{AFX_DATA(CFmrytmeView)
	enum{ IDD = IDD_FMRYTME_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CFmrytmeDoc* GetDocument();

// Operations
public:
	void TegnTaktStreker(CDC* pDC, int xOff, double xFak, int yOff, int yRange);
	void TegnLinjer(CDC* pDC, int xOff, int xLng, int yOff, int yGraf);

private:
	void LukkDialogVindu();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmrytmeView)
	public:
	virtual void OnDraw(CDC* pDC);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFmrytmeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFmrytmeView)
	afx_msg void OnFargemeny();
	afx_msg void OnLinjer();
	afx_msg void OnUpdateLinjer(CCmdUI* pCmdUI);
	afx_msg void OnTaktstreker();
	afx_msg void OnUpdateTaktstreker(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnParameter();
	afx_msg void OnKurve();
	afx_msg void OnUpdateDraw();
	//}}AFX_MSG
	afx_msg LRESULT OnOppsett(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOppdaterModulator(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CFMDialog* m_pFMDialog;
	CPoint* m_pDialogPos;
	void UpdatePosition (CPoint point);
	int m_xOff;			// Grafiske variable
	int m_yStep;
	int m_yTxtReg;
	int m_yPlotReg;
	double m_xFak;
	double m_yFak;

	int m_nSpor;
	double m_dMaxYverdi;
	FMPlotMarker leftPos;
	FMPlotMarker rightPos;

	BOOL m_bSang;
	BOOL m_bLinjer;
	BOOL m_bTaktstreker;
	BOOL m_bCaptured;
};

#ifndef _DEBUG  // debug version in fmrytmeView.cpp
inline CFmrytmeDoc* CFmrytmeView::GetDocument()
   { return (CFmrytmeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMRYTMEVIEW_H__67E5840E_5474_11D2_97D2_00105A3B4608__INCLUDED_)
