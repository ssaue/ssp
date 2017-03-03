// Dialog.cpp : implementation file
//

#include "stdafx.h"
#include "fmrytme.h"
#include "mmsystem.h"
#include "mididata.h"
#include "fmrytmeDoc.h"
#include "time.h"
#include "Dialog.h"
#include "FMdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFMDialog dialog

CFMDialog::CFMDialog(CView* pView)
{
	m_pView = pView;
}

BOOL CFMDialog::OnInitDialog() 
{
	GetDlgItem(IDBRUK)->EnableWindow(FALSE);
	return CDialog::OnInitDialog();
}

void CFMDialog::OnEditChange(UINT nID)
{
	GetDlgItem(IDBRUK)->EnableWindow(TRUE);
	GetDlgItem(IDBRUKALLE)->EnableWindow(TRUE);
}

void CFMDialog::OnOK() 
{
	m_pView->PostMessage(WM_OPPSETT, IDOK);
}

void CFMDialog::OnCancel() 
{
	m_pView->PostMessage(WM_OPPSETT, IDCANCEL);
}

void CFMDialog::OnOppsett() 
{
	m_pView->PostMessage(WM_OPPSETT, OPPSETT_MSG);
}

/////////////////////////////////////////////////////////////////////////////
// CFMSimpleDialog dialog


CFMSimpleDialog::CFMSimpleDialog(CView* pView, CFMSekvens* pSekv) : CFMDialog(pView)
{
	m_pSekv = pSekv;
	m_nGrunnpuls = pSekv->GetGrunnpuls();
	CFMOscillator* pMod = (CFMOscillator*) pSekv->GetModulator();
	m_dModFaktor = pMod->GetParameter(CFMOscillator::AMPLITUDE);
	m_dModPuls = pMod->GetParameter(CFMOscillator::FREKVENS);
	m_dModFase = pMod->GetParameter(CFMOscillator::FASE);
	m_nModPotens = (int) pMod->GetParameter(CFMOscillator::POTENS);
	m_nFunksjon = (int) pMod->GetParameter(CFMOscillator::FUNKSJON);
}

void CFMSimpleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFMSimpleDialog)
	DDX_Text(pDX, IDC_GRUNNPULS, m_nGrunnpuls);
	DDX_Text(pDX, IDC_MODFAKTOR1, m_dModFaktor);
	DDX_Text(pDX, IDC_MODPULS1, m_dModPuls);
	DDX_CBIndex(pDX, IDC_FUNKSJON1, m_nFunksjon);
	DDX_Text(pDX, IDC_MODFASE1, m_dModFase);
	DDX_Text(pDX, IDC_MODPOTENS1, m_nModPotens);
	DDV_MinMaxInt(pDX, m_nModPotens, 0, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFMSimpleDialog, CDialog)
	//{{AFX_MSG_MAP(CFMSimpleDialog)
	ON_BN_CLICKED(IDBRUK, OnBruk)
	ON_BN_CLICKED(IDBRUKALLE, OnBrukalle)
	ON_BN_CLICKED(IDC_OPPSETT, OnOppsett)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(EN_CHANGE, IDC_GRUNNPULS, IDC_MODPOTENS2, OnEditChange)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_SYNKRON, IDC_FUNKSJON2, OnEditChange)
END_MESSAGE_MAP()


BOOL CFMSimpleDialog::Create() 
{
	return CDialog::Create(CFMSimpleDialog::IDD);
}

void CFMSimpleDialog::OnBruk() 
{
	UpdateData(TRUE);
	if (m_nGrunnpuls != m_pSekv->GetGrunnpuls()) {
		CFmrytmeDoc* pDoc = (CFmrytmeDoc*) m_pView->GetDocument();
		m_pSekv->SetGrunnpuls(&pDoc->m_plotInfo, m_nGrunnpuls);
	}
	CFMOscillator* pMod = (CFMOscillator*) m_pSekv->GetModulator();
	pMod->SetParameter(CFMOscillator::AMPLITUDE, m_dModFaktor);
	pMod->SetParameter(CFMOscillator::FREKVENS, m_dModPuls);
	pMod->SetParameter(CFMOscillator::FASE, m_dModFase);
	pMod->SetParameter(CFMOscillator::POTENS, m_nModPotens);
	pMod->SetParameter(CFMOscillator::FUNKSJON, m_nFunksjon);
	GetDlgItem(IDBRUK)->EnableWindow(FALSE);
	m_pView->PostMessage(WM_OPPDATER_MODULATOR);
}

void CFMSimpleDialog::OnBrukalle() 
{
	UpdateData(TRUE);
	if (m_nGrunnpuls != m_pSekv->GetGrunnpuls()) {
		CFmrytmeDoc* pDoc = (CFmrytmeDoc*) m_pView->GetDocument();
		m_pSekv->SetGrunnpuls(&pDoc->m_plotInfo, m_nGrunnpuls);
	}
	CFMOscillator* pMod = (CFMOscillator*) m_pSekv->GetModulator();
	pMod->SetParameter(CFMOscillator::AMPLITUDE, m_dModFaktor);
	pMod->SetParameter(CFMOscillator::FREKVENS, m_dModPuls);
	pMod->SetParameter(CFMOscillator::FASE, m_dModFase);
	pMod->SetParameter(CFMOscillator::POTENS, m_nModPotens);
	pMod->SetParameter(CFMOscillator::FUNKSJON, m_nFunksjon);
	GetDlgItem(IDBRUKALLE)->EnableWindow(FALSE);
	m_pView->PostMessage(WM_OPPDATER_MODULATOR, ALLE);
}

/////////////////////////////////////////////////////////////////////////////
// CFMParallellDialog dialog


CFMParallellDialog::CFMParallellDialog(CView* pView, CFMSekvens* pSekv)	: CFMDialog(pView)
{
	m_pSekv = pSekv;
	m_nGrunnpuls = pSekv->GetGrunnpuls();
	CFMOscillator* pLeftMod = (CFMOscillator*) ((CSumOp*) pSekv->GetModulator())->GetOperator(CSumOp::LEFT);
	CFMOscillator* pRightMod = (CFMOscillator*) ((CSumOp*) pSekv->GetModulator())->GetOperator(CSumOp::RIGHT);
	m_dModFaktor1 = pRightMod->GetParameter(CFMOscillator::AMPLITUDE);
	m_dModPuls1 = pRightMod->GetParameter(CFMOscillator::FREKVENS);
	m_dModFase1 = pRightMod->GetParameter(CFMOscillator::FASE);
	m_nModPotens1 = (int) pRightMod->GetParameter(CFMOscillator::POTENS);
	m_nFunksjon1 = (int) pRightMod->GetParameter(CFMOscillator::FUNKSJON);
	m_dModFaktor2 = pLeftMod->GetParameter(CFMOscillator::AMPLITUDE);
	m_dModPuls2 = pLeftMod->GetParameter(CFMOscillator::FREKVENS);
	m_dModFase2 = pLeftMod->GetParameter(CFMOscillator::FASE);
	m_nModPotens2 = (int) pLeftMod->GetParameter(CFMOscillator::POTENS);
	m_nFunksjon2 = (int) pLeftMod->GetParameter(CFMOscillator::FUNKSJON);
}


void CFMParallellDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFMParallellDialog)
	DDX_Text(pDX, IDC_GRUNNPULS, m_nGrunnpuls);
	DDX_CBIndex(pDX, IDC_FUNKSJON1, m_nFunksjon1);
	DDX_CBIndex(pDX, IDC_FUNKSJON2, m_nFunksjon2);
	DDX_Text(pDX, IDC_MODFAKTOR1, m_dModFaktor1);
	DDX_Text(pDX, IDC_MODFAKTOR2, m_dModFaktor2);
	DDX_Text(pDX, IDC_MODPULS1, m_dModPuls1);
	DDX_Text(pDX, IDC_MODPULS2, m_dModPuls2);
	DDX_Text(pDX, IDC_MODFASE1, m_dModFase1);
	DDX_Text(pDX, IDC_MODFASE2, m_dModFase2);
	DDX_Text(pDX, IDC_MODPOTENS1, m_nModPotens1);
	DDV_MinMaxInt(pDX, m_nModPotens1, 0, 20);
	DDX_Text(pDX, IDC_MODPOTENS2, m_nModPotens2);
	DDV_MinMaxInt(pDX, m_nModPotens2, 0, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFMParallellDialog, CDialog)
	//{{AFX_MSG_MAP(CFMParallellDialog)
	ON_BN_CLICKED(IDBRUKALLE, OnBrukalle)
	ON_BN_CLICKED(IDBRUK, OnBruk)
	ON_BN_CLICKED(IDC_OPPSETT, OnOppsett)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(EN_CHANGE, IDC_GRUNNPULS, IDC_MODPOTENS2, OnEditChange)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_SYNKRON, IDC_FUNKSJON2, OnEditChange)
END_MESSAGE_MAP()

BOOL CFMParallellDialog::Create() 
{
	return CDialog::Create(CFMParallellDialog::IDD);
}

void CFMParallellDialog::OnBruk() 
{
	UpdateData(TRUE);
	if (m_nGrunnpuls != m_pSekv->GetGrunnpuls()) {
		CFmrytmeDoc* pDoc = (CFmrytmeDoc*) m_pView->GetDocument();
		m_pSekv->SetGrunnpuls(&pDoc->m_plotInfo, m_nGrunnpuls);
	}
	CFMOscillator* pLeftMod = (CFMOscillator*) ((CSumOp*) m_pSekv->GetModulator())->GetOperator(CSumOp::LEFT);
	CFMOscillator* pRightMod = (CFMOscillator*) ((CSumOp*) m_pSekv->GetModulator())->GetOperator(CSumOp::RIGHT);
	pRightMod->SetParameter(CFMOscillator::AMPLITUDE, m_dModFaktor1);
	pRightMod->SetParameter(CFMOscillator::FREKVENS, m_dModPuls1);
	pRightMod->SetParameter(CFMOscillator::FASE, m_dModFase1);
	pRightMod->SetParameter(CFMOscillator::POTENS, m_nModPotens1);
	pRightMod->SetParameter(CFMOscillator::FUNKSJON, m_nFunksjon1);
	pLeftMod->SetParameter(CFMOscillator::AMPLITUDE, m_dModFaktor2);
	pLeftMod->SetParameter(CFMOscillator::FREKVENS, m_dModPuls2);
	pLeftMod->SetParameter(CFMOscillator::FASE, m_dModFase2);
	pLeftMod->SetParameter(CFMOscillator::POTENS, m_nModPotens2);
	pLeftMod->SetParameter(CFMOscillator::FUNKSJON, m_nFunksjon2);
	GetDlgItem(IDBRUK)->EnableWindow(FALSE);
	m_pView->PostMessage(WM_OPPDATER_MODULATOR);
}

void CFMParallellDialog::OnBrukalle() 
{
	UpdateData(TRUE);
	if (m_nGrunnpuls != m_pSekv->GetGrunnpuls()) {
		CFmrytmeDoc* pDoc = (CFmrytmeDoc*) m_pView->GetDocument();
		m_pSekv->SetGrunnpuls(&pDoc->m_plotInfo, m_nGrunnpuls);
	}
	CFMOscillator* pLeftMod = (CFMOscillator*) ((CSumOp*) m_pSekv->GetModulator())->GetOperator(CSumOp::LEFT);
	CFMOscillator* pRightMod = (CFMOscillator*) ((CSumOp*) m_pSekv->GetModulator())->GetOperator(CSumOp::RIGHT);
	pRightMod->SetParameter(CFMOscillator::AMPLITUDE, m_dModFaktor1);
	pRightMod->SetParameter(CFMOscillator::FREKVENS, m_dModPuls1);
	pRightMod->SetParameter(CFMOscillator::FASE, m_dModFase1);
	pRightMod->SetParameter(CFMOscillator::POTENS, m_nModPotens1);
	pRightMod->SetParameter(CFMOscillator::FUNKSJON, m_nFunksjon1);
	pLeftMod->SetParameter(CFMOscillator::AMPLITUDE, m_dModFaktor2);
	pLeftMod->SetParameter(CFMOscillator::FREKVENS, m_dModPuls2);
	pLeftMod->SetParameter(CFMOscillator::FASE, m_dModFase2);
	pLeftMod->SetParameter(CFMOscillator::POTENS, m_nModPotens2);
	pLeftMod->SetParameter(CFMOscillator::FUNKSJON, m_nFunksjon2);
	GetDlgItem(IDBRUKALLE)->EnableWindow(FALSE);
	m_pView->PostMessage(WM_OPPDATER_MODULATOR, ALLE);
}

/////////////////////////////////////////////////////////////////////////////
// CFMSerieDialog dialog


CFMSerieDialog::CFMSerieDialog(CView* pView, CFMSekvens* pSekv)	: CFMDialog(pView)
{
	m_pSekv = pSekv;
	m_nGrunnpuls = pSekv->GetGrunnpuls();
	CFMOscillator* pMod1 = (CFMOscillator*) pSekv->GetModulator();
	CFMOscillator* pMod2 = (CFMOscillator*) pMod1->GetModulator();
	m_dModFaktor1 = pMod1->GetParameter(CFMOscillator::AMPLITUDE);
	m_dModPuls1 = pMod1->GetParameter(CFMOscillator::FREKVENS);
	m_dModFase1 = pMod1->GetParameter(CFMOscillator::FASE);
	m_nModPotens1 = (int) pMod1->GetParameter(CFMOscillator::POTENS);
	m_nFunksjon1 = (int) pMod1->GetParameter(CFMOscillator::FUNKSJON);
	m_dModFaktor2 = pMod2->GetParameter(CFMOscillator::AMPLITUDE);
	m_dModPuls2 = pMod2->GetParameter(CFMOscillator::FREKVENS);
	m_dModFase2 = pMod2->GetParameter(CFMOscillator::FASE);
	m_nModPotens2 = (int) pMod2->GetParameter(CFMOscillator::POTENS);
	m_nFunksjon2 = (int) pMod2->GetParameter(CFMOscillator::FUNKSJON);
}

void CFMSerieDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFMSerieDialog)
	DDX_Text(pDX, IDC_GRUNNPULS, m_nGrunnpuls);
	DDX_CBIndex(pDX, IDC_FUNKSJON1, m_nFunksjon1);
	DDX_CBIndex(pDX, IDC_FUNKSJON2, m_nFunksjon2);
	DDX_Text(pDX, IDC_MODFAKTOR2, m_dModFaktor2);
	DDX_Text(pDX, IDC_MODFAKTOR1, m_dModFaktor1);
	DDX_Text(pDX, IDC_MODPULS1, m_dModPuls1);
	DDX_Text(pDX, IDC_MODPULS2, m_dModPuls2);
	DDX_Text(pDX, IDC_MODFASE1, m_dModFase1);
	DDX_Text(pDX, IDC_MODFASE2, m_dModFase2);
	DDX_Text(pDX, IDC_MODPOTENS1, m_nModPotens1);
	DDV_MinMaxInt(pDX, m_nModPotens1, 0, 20);
	DDX_Text(pDX, IDC_MODPOTENS2, m_nModPotens2);
	DDV_MinMaxInt(pDX, m_nModPotens2, 0, 20);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFMSerieDialog, CDialog)
	//{{AFX_MSG_MAP(CFMSerieDialog)
	ON_BN_CLICKED(IDBRUK, OnBruk)
	ON_BN_CLICKED(IDBRUKALLE, OnBrukalle)
	ON_BN_CLICKED(IDC_OPPSETT, OnOppsett)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(EN_CHANGE, IDC_GRUNNPULS, IDC_MODPOTENS2, OnEditChange)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_SYNKRON, IDC_FUNKSJON2, OnEditChange)
END_MESSAGE_MAP()

BOOL CFMSerieDialog::Create() 
{
	return CDialog::Create(CFMSerieDialog::IDD);
}

void CFMSerieDialog::OnBruk() 
{
	UpdateData(TRUE);
	if (m_nGrunnpuls != m_pSekv->GetGrunnpuls()) {
		CFmrytmeDoc* pDoc = (CFmrytmeDoc*) m_pView->GetDocument();
		m_pSekv->SetGrunnpuls(&pDoc->m_plotInfo, m_nGrunnpuls);
	}
	CFMOscillator* pMod1 = (CFMOscillator*) m_pSekv->GetModulator();
	CFMOscillator* pMod2 = (CFMOscillator*) pMod1->GetModulator();
	pMod1->SetParameter(CFMOscillator::AMPLITUDE, m_dModFaktor1);
	pMod1->SetParameter(CFMOscillator::FREKVENS, m_dModPuls1);
	pMod1->SetParameter(CFMOscillator::FASE, m_dModFase1);
	pMod1->SetParameter(CFMOscillator::POTENS, m_nModPotens1);
	pMod1->SetParameter(CFMOscillator::FUNKSJON, m_nFunksjon1);
	pMod2->SetParameter(CFMOscillator::AMPLITUDE, m_dModFaktor2);
	pMod2->SetParameter(CFMOscillator::FREKVENS, m_dModPuls2);
	pMod2->SetParameter(CFMOscillator::FASE, m_dModFase2);
	pMod2->SetParameter(CFMOscillator::POTENS, m_nModPotens2);
	pMod2->SetParameter(CFMOscillator::FUNKSJON, m_nFunksjon2);
	GetDlgItem(IDBRUK)->EnableWindow(FALSE);
	m_pView->PostMessage(WM_OPPDATER_MODULATOR);
}

void CFMSerieDialog::OnBrukalle() 
{
	UpdateData(TRUE);
	if (m_nGrunnpuls != m_pSekv->GetGrunnpuls()) {
		CFmrytmeDoc* pDoc = (CFmrytmeDoc*) m_pView->GetDocument();
		m_pSekv->SetGrunnpuls(&pDoc->m_plotInfo, m_nGrunnpuls);
	}
	CFMOscillator* pMod1 = (CFMOscillator*) m_pSekv->GetModulator();
	CFMOscillator* pMod2 = (CFMOscillator*) pMod1->GetModulator();
	pMod1->SetParameter(CFMOscillator::AMPLITUDE, m_dModFaktor1);
	pMod1->SetParameter(CFMOscillator::FREKVENS, m_dModPuls1);
	pMod1->SetParameter(CFMOscillator::FASE, m_dModFase1);
	pMod1->SetParameter(CFMOscillator::POTENS, m_nModPotens1);
	pMod1->SetParameter(CFMOscillator::FUNKSJON, m_nFunksjon1);
	pMod2->SetParameter(CFMOscillator::AMPLITUDE, m_dModFaktor2);
	pMod2->SetParameter(CFMOscillator::FREKVENS, m_dModPuls2);
	pMod2->SetParameter(CFMOscillator::FASE, m_dModFase2);
	pMod2->SetParameter(CFMOscillator::POTENS, m_nModPotens2);
	pMod2->SetParameter(CFMOscillator::FUNKSJON, m_nFunksjon2);
	GetDlgItem(IDBRUKALLE)->EnableWindow(FALSE);
	m_pView->PostMessage(WM_OPPDATER_MODULATOR, ALLE);
}

/////////////////////////////////////////////////////////////////////////////
// CFargeDialog dialog


CFargeDialog::CFargeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFargeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFargeDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFargeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFargeDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFargeDialog, CDialog)
	//{{AFX_MSG_MAP(CFargeDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_STEMME1, IDC_STEMME16, OnButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFargeDialog message handlers

void CFargeDialog::OnButtonClicked(UINT nID)
{
	int nButton = nID - IDC_STEMME1;
	ASSERT (nButton >= 0 && nButton < CSettings::KANALER);
	CColorDialog dlg(m_colObjects[nButton],
		CC_PREVENTFULLOPEN | CC_SOLIDCOLOR, NULL);
	if (dlg.DoModal() == IDOK)
		m_colObjects[nButton] = dlg.GetColor();
	Invalidate();
}

void CFargeDialog::OnPaint() 
{
	LPPAINTSTRUCT lpPaint = new PAINTSTRUCT;
	BeginPaint(lpPaint);
	for (int i=IDC_DEMO1; i<=IDC_DEMO16; i++) {
		int nID = i-IDC_DEMO1;
		CWnd* ctrTest = (CWnd*) GetDlgItem(i);
		CRect* vindu = new CRect;
		ctrTest->GetWindowRect(vindu);
		vindu->OffsetRect(-vindu->TopLeft());
		ctrTest->Invalidate();
		ctrTest->UpdateWindow();
		CDC* pctrDC = ctrTest->GetDC();
		pctrDC->FillSolidRect(vindu,m_colObjects[nID]);
		ctrTest->ReleaseDC(pctrDC);
		delete vindu;
	}
	EndPaint(lpPaint);
	delete lpPaint;
	return;
}
/////////////////////////////////////////////////////////////////////////////
// CFMOppsettDialog dialog


CFMOppsettDialog::CFMOppsettDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFMOppsettDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFMOppsettDialog)
	m_nOppsett = -1;
	//}}AFX_DATA_INIT
}

void CFMOppsettDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFMOppsettDialog)
	DDX_Radio(pDX, IDC_FMVALG, m_nOppsett);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFMOppsettDialog, CDialog)
	//{{AFX_MSG_MAP(CFMOppsettDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFMOppsettDialog::OnInitDialog() 
{
	switch (m_nOppsett)
	{
	case COperator::FMOSCILLATOR:
		m_nOppsett = ENKEL;
		break;
	case COperator::SERIEOSC:
		m_nOppsett = SERIE;
		break;
	case COperator::PARALLELLOSC:
		m_nOppsett = PARALLELL;
		break;
	}
	return CDialog::OnInitDialog();
}

/////////////////////////////////////////////////////////////////////////////
// CGenerellDialog dialog


CGenerellDialog::CGenerellDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGenerellDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenerellDialog)
	m_nDeviceNr = -1;
	m_nPeriode = 0;
	//}}AFX_DATA_INIT
}


void CGenerellDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenerellDialog)
	DDX_CBIndex(pDX, IDC_MIDIDEVICE, m_nDeviceNr);
	DDX_Text(pDX, IDC_SAMPFREK, m_nPeriode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenerellDialog, CDialog)
	//{{AFX_MSG_MAP(CGenerellDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenerellDialog message handlers

BOOL CGenerellDialog::OnInitDialog() 
{
	//	Initialiserer popup-meny for MIDI-device
	CComboBox* pCB = (CComboBox*) GetDlgItem(IDC_MIDIDEVICE);
	m_nAntall = CMidiUtility::GetDevices();
	CString tekst = "";
	for (int i=0; i < m_nAntall; i++)
		if (CMidiUtility::GetDeviceName(i, tekst))
			pCB->InsertString(i, tekst);
	return CDialog::OnInitDialog();
}

/////////////////////////////////////////////////////////////////////////////
// CMinsteTidDialog dialog

CMinsteTidDialog::CMinsteTidDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMinsteTidDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMinsteTidDialog)
	m_nTidsenhet = 0;
	//}}AFX_DATA_INIT
}


void CMinsteTidDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMinsteTidDialog)
	DDX_Text(pDX, IDC_TIDSENHET, m_nTidsenhet);
	DDV_MinMaxInt(pDX, m_nTidsenhet, 0, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMinsteTidDialog, CDialog)
	//{{AFX_MSG_MAP(CMinsteTidDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNyStemmeDialog dialog


CNyStemmeDialog::CNyStemmeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNyStemmeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNyStemmeDialog)
	m_dLengde = 0.9;
	//}}AFX_DATA_INIT
}


void CNyStemmeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNyStemmeDialog)
	DDX_Text(pDX, IDC_FRAKSJON, m_dLengde);
	DDV_MinMaxDouble(pDX, m_dLengde, 1.e-002, 1.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNyStemmeDialog, CDialog)
	//{{AFX_MSG_MAP(CNyStemmeDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNyStemmeDialog message handlers
/////////////////////////////////////////////////////////////////////////////
// CStemme1Dialog dialog


CStemme1Dialog::CStemme1Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStemme1Dialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStemme1Dialog)
	m_dLengde = 0.9;
	m_nMetronom = CMidiTime::DEFAULT_METRO;
	m_nNevner = CMidiSang::DEFAULT_TAKTART1;
	m_nTeller = CMidiSang::DEFAULT_TAKTART2;
	//}}AFX_DATA_INIT
}


void CStemme1Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStemme1Dialog)
	DDX_Text(pDX, IDC_FRAKSJON, m_dLengde);
	DDV_MinMaxDouble(pDX, m_dLengde, 1.e-002, 1.);
	DDX_Text(pDX, IDC_METRONOM, m_nMetronom);
	DDV_MinMaxInt(pDX, m_nMetronom, 1, 500);
	DDX_Text(pDX, IDC_NEVNER, m_nNevner);
	DDV_MinMaxInt(pDX, m_nNevner, 1, 20);
	DDX_Text(pDX, IDC_TELLER, m_nTeller);
	DDV_MinMaxInt(pDX, m_nTeller, 1, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStemme1Dialog, CDialog)
	//{{AFX_MSG_MAP(CStemme1Dialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStemme1Dialog message handlers
/////////////////////////////////////////////////////////////////////////////
// CPlotDialog dialog


CPlotDialog::CPlotDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPlotDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlotDialog)
	m_dAmpLengde = 0.0;
	m_dAmpVelocity = 0.0;
	m_nStemme = 0;
	m_nSynk = -1;
	m_nTaktSlag = 0;
	m_bOppdaterPuls = FALSE;
	//}}AFX_DATA_INIT
	m_nAntallSpor = 0;
}


void CPlotDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlotDialog)
	DDX_Text(pDX, IDC_AMP_LENGDE, m_dAmpLengde);
	DDV_MinMaxDouble(pDX, m_dAmpLengde, 0., 5.);
	DDX_Text(pDX, IDC_AMP_VELOCITY, m_dAmpVelocity);
	DDV_MinMaxDouble(pDX, m_dAmpVelocity, 0., 5.);
	DDX_Text(pDX, IDC_STEMME, m_nStemme);
	DDX_CBIndex(pDX, IDC_SYNKRON, m_nSynk);
	DDX_Text(pDX, IDC_TAKTSLAG, m_nTaktSlag);
	DDV_MinMaxInt(pDX, m_nTaktSlag, 1, 24);
	DDX_Check(pDX, IDC_OPPDATER_PULS, m_bOppdaterPuls);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotDialog, CDialog)
	//{{AFX_MSG_MAP(CPlotDialog)
	ON_CBN_SELCHANGE(IDC_SYNKRON, OnSelchangeSynkron)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotDialog message handlers

BOOL CPlotDialog::OnInitDialog() 
{
	if (m_nSynk == FMPlotInfo::ENER) {
		CWnd* kontroll = GetDlgItem(IDC_STEMME);
		kontroll->EnableWindow(FALSE);
	}
	else {
		CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPINSTEMME);
		if (m_nSynk == FMPlotInfo::INGEN)
			pSpin->SetRange(0,m_nAntallSpor);
		else
			pSpin->SetRange(1,m_nAntallSpor);
		pSpin->SetPos(m_nStemme);
	}
	return CDialog::OnInitDialog();
}

void CPlotDialog::OnSelchangeSynkron() 
{
	CComboBox* pCB = (CComboBox*) GetDlgItem(IDC_SYNKRON);
	m_nSynk = pCB->GetCurSel();
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPINSTEMME);
	CWnd* vindu = GetDlgItem(IDC_STEMME);
	CString sProgram;
	vindu->GetWindowText(sProgram);
	int nStemme = atoi(CT2A(sProgram));
	switch (m_nSynk)
	{
	case FMPlotInfo::ENER:
		vindu->EnableWindow(FALSE);
		break;
	case FMPlotInfo::INGEN:
		vindu->EnableWindow(TRUE);
		pSpin->SetRange(0, m_nAntallSpor);
		pSpin->SetPos(nStemme);
		break;
	case FMPlotInfo::START_BEVEGELSE:
		vindu->EnableWindow(TRUE);
		pSpin->SetRange(1, m_nAntallSpor);
		pSpin->SetPos((nStemme > 0) ? nStemme : 1);
		break;
	}
}
