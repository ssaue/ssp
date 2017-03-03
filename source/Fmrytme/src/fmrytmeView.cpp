// fmrytmeView.cpp : implementation of the CFmrytmeView class
//

#include "stdafx.h"
#include "fmrytme.h"

#include "mainfrm.h"
#include "fmrytmeDoc.h"
#include "fmrytmeView.h"
#include "settings.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeView

IMPLEMENT_DYNCREATE(CFmrytmeView, CView)

BEGIN_MESSAGE_MAP(CFmrytmeView, CView)
	//{{AFX_MSG_MAP(CFmrytmeView)
	ON_COMMAND(ID_FARGEMENY, OnFargemeny)
	ON_COMMAND(ID_LINJER, OnLinjer)
	ON_UPDATE_COMMAND_UI(ID_LINJER, OnUpdateLinjer)
	ON_COMMAND(ID_TAKTSTREKER, OnTaktstreker)
	ON_UPDATE_COMMAND_UI(ID_TAKTSTREKER, OnUpdateTaktstreker)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PARAKNAPP, OnParameter)
	ON_COMMAND(ID_KURVE, OnKurve)
	ON_COMMAND(ID_PARAMETER, OnParameter)
	ON_COMMAND(ID_UPDATE_DRAW, OnUpdateDraw)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_MESSAGE (WM_OPPSETT, OnOppsett)
	ON_MESSAGE (WM_OPPDATER_MODULATOR, OnOppdaterModulator)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeView construction/destruction

CFmrytmeView::CFmrytmeView() : leftPos(FMPlotMarker::LEFT), rightPos(FMPlotMarker::RIGHT)
{
	m_pFMDialog = NULL;
	m_pDialogPos = NULL;
	m_nSpor = 0;
	m_dMaxYverdi = 1.0;
	m_bLinjer = m_bTaktstreker = TRUE;
	m_bSang = m_bCaptured = FALSE;
	m_xFak = m_yFak = 1.0;

	m_xOff = (int) (0.05 * X_EXTENT);		// Definisjoner av kurveplottet:
	m_yStep = (int) (0.05 * Y_EXTENT);		// Endringer i utseendet gjøres med disse tre
	m_yTxtReg = Y_EXTENT - 2 * m_yStep;		// konstantene
}

CFmrytmeView::~CFmrytmeView()
{
	delete m_pFMDialog;
	delete m_pDialogPos;
}

BOOL CFmrytmeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CFmrytmeView::OnDraw(CDC* pDC)
{
	CFmrytmeDoc* CDoc = GetDocument();
	if (m_bSang) {
		// Skriver tekst
		CFont tFont;
		tFont.CreateFont(m_yStep,0,0,0,400, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,  DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
		CFont* pGmlFont = pDC->SelectObject(&tFont);
		CString tekst = "Title: " + CDoc->m_pMIDISang->GetTittel();
		pDC->TextOut(m_xOff, Y_EXTENT, tekst, tekst.GetLength());
		if (CDoc->m_bSolo) {
			tekst.Format(L"Voice %d: ", m_nSpor+1);
			tekst = tekst + CDoc->m_pMIDISang->GetSekvens(m_nSpor)->GetTittel();
			pDC->TextOut(m_xOff, m_yTxtReg+m_yStep, tekst, tekst.GetLength());
		}
		pDC->SelectObject(pGmlFont);
		// Tegner kurvene
		CPen* pOldPen = (CPen*) pDC->SelectStockObject(BLACK_PEN);
		if (m_bTaktstreker)
			TegnTaktStreker(pDC, m_xOff, m_xFak, m_yStep, m_yTxtReg - m_yStep);
		if (m_bLinjer)
			TegnLinjer(pDC, m_xOff, (X_EXTENT - 2*m_xOff), m_yStep, m_yPlotReg);
		CDoc->m_pFMSang->Plot(pDC, m_xFak, m_yFak, m_xOff, (m_yTxtReg+m_yStep), m_yPlotReg, m_nSpor);
		pDC->SelectObject(pOldPen);
		// Tegner markører
		leftPos.Plot(pDC, TRUE);
		rightPos.Plot(pDC, TRUE);
	}
}

void CFmrytmeView::TegnTaktStreker(CDC* pDC, int xOff, double xFak, int yOff, int yRange)
{
	CFmrytmeDoc* pDoc = GetDocument();
	for (int i=0; i <= pDoc->m_plotInfo.m_nTaktPrLinje; i++) {
		pDC->MoveTo(xOff, yOff);
		pDC->LineTo(xOff, yOff+yRange);
		xOff += (int) (xFak * CMainFrame::m_settings.m_nPeriode);
	}
}

void CFmrytmeView::TegnLinjer(CDC* pDC, int xOff, int xLng, int yOff, int yGraf)
{
	CFmrytmeDoc* pDoc = GetDocument();
	for (int i=0; i < pDoc->m_plotInfo.m_nLinjeAnt; i++) {
		pDC->MoveTo(xOff, yOff);
		pDC->LineTo(xOff + xLng, yOff);
		yOff += yGraf;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeView printing

BOOL CFmrytmeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

void CFmrytmeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFmrytmeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


/////////////////////////////////////////////////////////////////////////////
// CFmrytmeView diagnostics

#ifdef _DEBUG
void CFmrytmeView::AssertValid() const
{
	CView::AssertValid();
}

void CFmrytmeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFmrytmeDoc* CFmrytmeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFmrytmeDoc)));
	return (CFmrytmeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeView message handlers

void CFmrytmeView::OnFargemeny() 
{
	CFargeDialog dlg;
	for(int i=0; i<CSettings::KANALER; i++)
		dlg.m_colObjects[i] = CMainFrame::m_settings.m_colFarge[i];
	if (dlg.DoModal() == IDOK) {
		for(int i=0; i<CSettings::KANALER; i++)
			CMainFrame::m_settings.m_colFarge[i] = dlg.m_colObjects[i];
		Invalidate();
	}
}

void CFmrytmeView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CFmrytmeDoc* pDoc = GetDocument();
	if (bActivate) {
		pDoc->UpdateFrame();
	}
	else if (m_pFMDialog && (m_pFMDialog->GetSafeHwnd() != NULL)) {
		LukkDialogVindu();
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CFmrytmeView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CRect rectClient;
	GetClientRect(rectClient);

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(X_EXTENT, Y_EXTENT);
	if (pDC->IsPrinting()) {
		int vindu = 4800;
		pDC->SetViewportExt(vindu, -vindu);
		pDC->SetViewportOrg(0, vindu);
	}
	else {
		pDC->SetViewportExt(rectClient.right, -rectClient.bottom);
		pDC->SetViewportOrg(0, rectClient.bottom);
	}
	CView::OnPrepareDC(pDC, pInfo);
}

void CFmrytmeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CFmrytmeDoc* pDoc = GetDocument();
	switch (lHint)
	{
	case NY_SANG:
		m_bSang = TRUE;
		// Beregner noen plottekonstanter
		m_yPlotReg = m_yTxtReg / pDoc->m_plotInfo.m_nLinjeAnt;
		m_xFak = (X_EXTENT - 2 * m_xOff) / (double) pDoc->m_plotInfo.GetLinjeLengde();
		m_dMaxYverdi = pDoc->m_pFMSang->GetMaxAmplitude();
		m_yFak = (m_yPlotReg - m_yStep) / m_dMaxYverdi;
	case SOLO_SANG:
		if (pDoc->m_bSolo) {
			CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
			m_nSpor = pFrame->GetSpor();
		}
		else
			m_nSpor = CFMSang::ALLE_SPOR;
		break;
	case NY_STEMME:
		if (m_pFMDialog->GetSafeHwnd() != 0) {
			LukkDialogVindu();
			OnParameter();
		}
		if (pDoc->m_bSolo) {
			CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
			m_nSpor = pFrame->GetSpor();
		}
		break;
	case INGEN_SANG:
		m_bSang = FALSE;
		break;
	default:
		break;
	}
	Invalidate();
}

void CFmrytmeView::OnInitialUpdate() 
{
	if (!m_bSang)
		OnUpdate(NULL, INGEN_SANG, NULL);
	else
		OnUpdate(NULL, NY_SANG, NULL);
}

void CFmrytmeView::OnLinjer() 
{
	m_bLinjer = !m_bLinjer;
	Invalidate();
}

void CFmrytmeView::OnUpdateLinjer(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bLinjer);
	pCmdUI->Enable(m_bSang);	
}

void CFmrytmeView::OnTaktstreker() 
{
	m_bTaktstreker = !m_bTaktstreker;
	Invalidate();
}

void CFmrytmeView::OnUpdateTaktstreker(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bTaktstreker);
	pCmdUI->Enable(m_bSang);	
}

void CFmrytmeView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	leftPos.Plot(&dc, FALSE);
	CFmrytmeDoc* CDoc = GetDocument();
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus = &(pFrame->m_wndStatusBar);
	CString strLeft = "";
	CString strDiff = "";
	if (CDoc->m_pFMSang != NULL) {
		dc.DPtoLP(&point);
		leftPos.SetPos(point, m_xOff, (X_EXTENT - 2*m_xOff), m_yPlotReg, m_yStep, &CDoc->m_plotInfo);
		if (leftPos.IsSet())
			strLeft.Format(L"LEFT: x = %.2f, y = %.2f",
				leftPos.GetTakt(), leftPos.GetAmp()/m_yFak);
		FMPlotMarker diffPos = rightPos - leftPos;
		if (diffPos.IsSet())
			strDiff.Format(L"DIFFERENCE: x = %.2f, y = %.2f",
				diffPos.GetTakt(), diffPos.GetAmp()/m_yFak);
	}
	pStatus->SetPaneText(CMainFrame::LEFT, strLeft);
	pStatus->SetPaneText(CMainFrame::DIFFERANSE, strDiff);
	leftPos.Plot(&dc, TRUE);
}

void CFmrytmeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	rightPos.Plot(&dc, FALSE);
	CFmrytmeDoc* CDoc = GetDocument();
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus = &(pFrame->m_wndStatusBar);
	CString strRight = "";
	CString strDiff = "";
	if (m_bSang) {
		dc.DPtoLP(&point);
		rightPos.SetPos(point, m_xOff, (X_EXTENT - 2*m_xOff), m_yPlotReg, m_yStep, &CDoc->m_plotInfo);
		if (rightPos.IsSet())
			strRight.Format(L"RIGHT: x = %.2f, y = %.2f",
				rightPos.GetTakt(), rightPos.GetAmp()/m_yFak);
		FMPlotMarker diffPos = rightPos - leftPos;
		if (diffPos.IsSet())
			strDiff.Format(L"DIFFERENCE: x = %.2f, y = %.2f",
				diffPos.GetTakt(), diffPos.GetAmp()/m_yFak);
	}
	pStatus->SetPaneText(CMainFrame::RIGHT, strRight);
	pStatus->SetPaneText(CMainFrame::DIFFERANSE, strDiff);
	rightPos.Plot(&dc, TRUE);
}

void CFmrytmeView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CFmrytmeDoc* CDoc = GetDocument();
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus = &(pFrame->m_wndStatusBar);
	CString strX = "";
	CString strY = "";
	if (m_bSang) {
		CClientDC dc(this);
		OnPrepareDC(&dc);
		dc.DPtoLP(&point);
		FMPlotMarker plotPos;
		plotPos.SetPos(point, m_xOff, (X_EXTENT - 2*m_xOff), m_yPlotReg, m_yStep, &CDoc->m_plotInfo);
		if (plotPos.IsSet()) {
			strX.Format(L"x = %.2f", plotPos.GetTakt());
			strY.Format(L"y = %.2f", plotPos.GetAmp()/m_yFak);
		}
	}
	pStatus->SetPaneText(CMainFrame::X_KOORD, strX);
	pStatus->SetPaneText(CMainFrame::Y_KOORD, strY);
}

void CFmrytmeView::LukkDialogVindu()
{
	if (m_pFMDialog != NULL && m_pFMDialog->GetSafeHwnd() != 0) {
		CRect gmlVindu;
		m_pFMDialog->GetWindowRect(&gmlVindu);
		delete m_pDialogPos;
		m_pDialogPos = new CPoint(gmlVindu.TopLeft());
		m_pFMDialog->DestroyWindow();
	}
}

LRESULT CFmrytmeView::OnOppsett(WPARAM wParam, LPARAM lParam) 
{
	LukkDialogVindu();
	if (wParam == CFMDialog::OPPSETT_MSG) {
		CFmrytmeDoc* CDoc = GetDocument();
		CFMSekvens* pSekv = CDoc->GetEditSekvens();
		CFMOppsettDialog dlg;
		dlg.m_nOppsett = pSekv->GetModulator()->GetType();
		if (dlg.DoModal() == IDOK) {
			switch (dlg.m_nOppsett)
			{
			case CFMOppsettDialog::ENKEL:
				CDoc->LagEnkelModulator();
				break;
			case CFMOppsettDialog::PARALLELL:
				CDoc->LagParallellModulator();
				break;
			case CFMOppsettDialog::SERIE:
				CDoc->LagSerieModulator();
				break;
			}
			OnParameter();
		}
	}
	return 0L;
}

void CFmrytmeView::OnParameter() 
{
	if (m_pFMDialog)
		if (m_pFMDialog->GetSafeHwnd() == 0)
			delete m_pFMDialog;
		else
			return;
	CFmrytmeDoc* pDoc = GetDocument();
	CFMSekvens* pSekv = pDoc->GetEditSekvens();
	switch (pSekv->GetModulator()->GetType())
	{
	case COperator::FMOSCILLATOR:
		m_pFMDialog = new CFMSimpleDialog (this, pSekv);
		break;
	case COperator::PARALLELLOSC:
		m_pFMDialog = new CFMParallellDialog (this, pSekv);
		break;
	case COperator::SERIEOSC:
		m_pFMDialog = new CFMSerieDialog (this, pSekv);
		break;
	}
	if (m_pFMDialog->GetSafeHwnd() == 0) {
		m_pFMDialog->Create();
		if (m_pDialogPos != NULL) {	// Flytter vinduet til samme posisjon som det forrige
			CRect gmlVindu;
			m_pFMDialog->GetWindowRect(&gmlVindu);
			CRect nyVindu(*m_pDialogPos, gmlVindu.Size());
			m_pFMDialog->MoveWindow(nyVindu);
		}
		m_pFMDialog->ShowWindow(SW_SHOW);
	}
}


LRESULT CFmrytmeView::OnOppdaterModulator(WPARAM wParam, LPARAM lParam)
{
	if (!m_bSang)
		return -1L;
	BeginWaitCursor();
	CFmrytmeDoc* pDoc = GetDocument();
	if (wParam == CFMDialog::ALLE) {
		pDoc->ComputeAlle();
		OnUpdate(NULL, NY_SANG, NULL);
	}
	else {
		pDoc->ComputeEditSekvens();
		OnUpdate(NULL, NY_SANG, NULL);
	}
	EndWaitCursor();
	return 0L;
}

void CFmrytmeView::OnKurve() 
{
	CPlotDialog dlg;
	CFmrytmeDoc* pDoc = GetDocument();
	dlg.m_nAntallSpor = pDoc->m_pMIDISang->GetSpor();
	if (FMPlotInfo::m_nSynk == FMPlotInfo::INGEN)
		dlg.m_nStemme = 0;
	else		
		dlg.m_nStemme = pDoc->m_plotInfo.m_nSynkStemme + 1;
	if (FMPlotInfo::m_nSynk == FMPlotInfo::STEMME)
		dlg.m_nSynk = FMPlotInfo::INGEN;
	else
		dlg.m_nSynk = FMPlotInfo::m_nSynk;
	dlg.m_nTaktSlag = pDoc->m_plotInfo.m_nSlagPrTakt;
	dlg.m_dAmpLengde = FMPlotInfo::m_dAmpLengde;
	dlg.m_dAmpVelocity = FMPlotInfo::m_dAmpVelocity;
	if (dlg.DoModal() == IDOK) {
		if (dlg.m_nSynk == FMPlotInfo::INGEN && dlg.m_nStemme > 0)
			FMPlotInfo::m_nSynk = FMPlotInfo::STEMME;
		else
			FMPlotInfo::m_nSynk = dlg.m_nSynk;
		if (dlg.m_nStemme > 0)
			pDoc->m_plotInfo.m_nSynkStemme = dlg.m_nStemme - 1;
		pDoc->m_plotInfo.m_nSlagPrTakt = dlg.m_nTaktSlag;
		pDoc->m_plotInfo.UpdateTaktInfo(pDoc->m_pFMSang->GetSlag());
		if (dlg.m_bOppdaterPuls)
			pDoc->m_pFMSang->SetGrunnpuls(dlg.m_nTaktSlag);
		else
			pDoc->m_pFMSang->UpdatePeriode();
		FMPlotInfo::m_dAmpLengde = dlg.m_dAmpLengde;
		FMPlotInfo::m_dAmpVelocity = dlg.m_dAmpVelocity;
		pDoc->m_pFMSang->UpdateAmplitude();
		pDoc->m_pFMSang->ComputePlot();
		OnUpdate(NULL, NY_SANG, NULL);
	}
}

void CFmrytmeView::OnUpdateDraw() 
{
	Invalidate();
}
