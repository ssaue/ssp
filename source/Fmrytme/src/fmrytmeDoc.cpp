// fmrytmeDoc.cpp : implementation of the CFmrytmeDoc class
//

#include "stdafx.h"
#include "fmrytme.h"
#include "fmfiler.h"
#include "mainfrm.h"
#include "mididialog.h"
#include "fmrytmeDoc.h"
#include "fmrytmeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeDoc

IMPLEMENT_DYNCREATE(CFmrytmeDoc, CDocument)

BEGIN_MESSAGE_MAP(CFmrytmeDoc, CDocument)
	//{{AFX_MSG_MAP(CFmrytmeDoc)
	ON_COMMAND(ID_IMPORT_MIDI, OnImportMidi)
	ON_COMMAND(ID_SANGMENY, OnSangmeny)
	ON_UPDATE_COMMAND_UI(ID_SANGMENY, OnUpdateSangmeny)
	ON_COMMAND(ID_SOLO, OnSolo)
	ON_UPDATE_COMMAND_UI(ID_SOLO, OnUpdateSolo)
	ON_UPDATE_COMMAND_UI(ID_PARAMETER, OnUpdateParameter)
	ON_UPDATE_COMMAND_UI(ID_OPPSETT, OnUpdateOppsett)
	ON_COMMAND(ID_FJERN_STEMME, OnFjernStemme)
	ON_UPDATE_COMMAND_UI(ID_FJERN_STEMME, OnUpdateFjernStemme)
	ON_COMMAND(ID_IMPORT_TEKST, OnImportTekst)
	ON_UPDATE_COMMAND_UI(ID_KURVE, OnUpdateKurve)
	ON_UPDATE_COMMAND_UI(ID_GENERELLMENY, OnUpdateGenerellmeny)
	ON_UPDATE_COMMAND_UI(ID_PARAKNAPP, OnUpdateParameter)
	ON_UPDATE_COMMAND_UI(IDC_TEMPO, OnUpdateTempo)
	ON_COMMAND(ID_EXPORT_MIDI, OnExportMidi)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_MIDI, OnUpdateExportMidi)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_STEMME, OnSetStemme)
	ON_EN_KILLFOCUS(IDC_TEMPO, OnEditedTempo)
	ON_EN_CHANGE(IDC_TEMPO, OnSetTempo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeDoc construction/destruction

CFmrytmeDoc::CFmrytmeDoc()
{
	m_pMIDISang = NULL;
	m_pFMSang = NULL;
	m_bSolo = FALSE;
	m_nEditSpor = 0;
}

CFmrytmeDoc::~CFmrytmeDoc()
{
	delete m_pMIDISang;
	delete m_pFMSang;
}

BOOL CFmrytmeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	m_plotInfo.m_nSynkStemme = 0;
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFmrytmeDoc serialization

void CFmrytmeDoc::Serialize(CArchive& ar)
{
	m_plotInfo.Serialize(ar);
	if (ar.IsStoring())
	{
		if (m_pMIDISang != NULL) {
			ar << m_pMIDISang;
			for (int i=0; i<m_pMIDISang->GetSpor(); i++) {
				ar << m_pFMSang->GetSekvens(i)->GetModulator()->GetType();
				ar << m_pFMSang->GetSekvens(i)->GetModulator();
				ar << m_pFMSang->GetSekvens(i)->GetGrunnpuls();
			}
		}
	}
	else
	{
		ar >> m_pMIDISang;
		m_pFMSang = new CFMSang(&m_plotInfo);
		m_pFMSang->LoadSang(m_pMIDISang);
		for (int i=0; i<m_pMIDISang->GetSpor(); i++) {
			int nOpType;
			ar >> nOpType;
			switch (nOpType)
			{
			case COperator::FMOSCILLATOR:
			case COperator::SERIEOSC:
				CFMOscillator* oscbuf;
				ar >> oscbuf;
				m_pFMSang->GetSekvens(i)->SetModulator(oscbuf);
				break;
			case COperator::PARALLELLOSC:
			case COperator::SUMOPERATOR:
				CSumOp* sumbuf;
				ar >> sumbuf;
				m_pFMSang->GetSekvens(i)->SetModulator(sumbuf);
				break;
			case COperator::INGEN:
			default:
				CFMOscillator* pOsc = new CFMOscillator;
				m_pFMSang->GetSekvens(i)->SetModulator(pOsc);
				break;
			}
			int nGrunnpuls;
			ar >> nGrunnpuls;
			m_pFMSang->GetSekvens(i)->SetGrunnpuls(&m_plotInfo, nGrunnpuls);			
		}
		m_pFMSang->SetMetronom(m_pMIDISang->GetMidiTime()->GetMetronom());
		m_pFMSang->ComputePlot();
		UpdateFrame();
		UpdateAllViews(NULL, CFmrytmeView::NY_SANG);		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeDoc diagnostics

#ifdef _DEBUG
void CFmrytmeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFmrytmeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFmrytmeDoc commands

void CFmrytmeDoc::OnImportMidi() 
{
	CFileDialog fdlg(TRUE, NULL, L"*.mid");
	fdlg.m_ofn.lpstrTitle = L"Import MIDI-file";
	if (fdlg.DoModal() == IDOK) {
		CMidiFile midiFile;
		if (midiFile.ReadOpen(fdlg.GetPathName())) {
			BeginWaitCursor();
			midiFile.ReadData();
			DeleteContents();
			m_pMIDISang = midiFile.GetSang();
			m_pMIDISang->NormaliserTid();
			int metronom = m_pMIDISang->GetMidiTime()->GetMetronom();
			m_plotInfo.m_nSlagPrTakt = m_pMIDISang->GetTaktart(CMidiSang::TELLER);
			CMinsteTidDialog dlg;
			dlg.DoModal();
			m_pFMSang = new CFMSang(&m_plotInfo);
			double minTid = (dlg.m_nTidsenhet > 0) ? 4.0 / dlg.m_nTidsenhet : 0.0;
			m_pFMSang->LoadSang(m_pMIDISang, minTid);
			for (int i=0; i<m_pMIDISang->GetSpor(); i++) {
				CFMOscillator* pMod = new CFMOscillator;
				m_pFMSang->GetSekvens(i)->SetModulator(pMod);
				pMod->SetMetronom(metronom);
			}
			m_pFMSang->ComputePlot();
			SetModifiedFlag();
			UpdateFrame();
			UpdateAllViews(NULL, CFmrytmeView::NY_SANG);
			midiFile.Close();
			EndWaitCursor();
		}
	}
}

void CFmrytmeDoc::OnExportMidi() 
{
	CFileDialog fdlg(FALSE, NULL, L"*.mid");
	fdlg.m_ofn.lpstrTitle = L"Export MIDI-file";
	if (fdlg.DoModal() == IDOK) {
		CMidiFile midiFile;
		if (midiFile.WriteOpen(fdlg.GetPathName())) {
			BeginWaitCursor();
			midiFile.LoadSong(m_pMIDISang);
			midiFile.WriteData();
			midiFile.Close();
			EndWaitCursor();
		}
	}
}

void CFmrytmeDoc::OnImportTekst() 
{
	CFileDialog fdlg(TRUE, NULL, L"*.*");
	fdlg.m_ofn.lpstrTitle = L"Import text-file";
	if (fdlg.DoModal() == IDOK) {
		CTekstFile tekstFile;
		if (tekstFile.Open(fdlg.GetPathName())) {
			BeginWaitCursor();
			BOOL bFilOK;
			if (m_pMIDISang == NULL) {
				CStemme1Dialog dlg;
				dlg.DoModal();
				m_pMIDISang = new CMidiSang(0);
				m_pMIDISang->GetMidiTime()->SetTidsformat(1);	//Bruker fjerdedel som enhet
				m_pMIDISang->SetTaktart(dlg.m_nTeller, dlg.m_nNevner);
				m_pMIDISang->SetMetronom(dlg.m_nMetronom);
				m_plotInfo.m_nSlagPrTakt = dlg.m_nTeller;
				m_pFMSang = new CFMSang(&m_plotInfo);
				m_pFMSang->SetTaktLengde(dlg.m_nTeller);
				bFilOK = tekstFile.ReadData(dlg.m_dLengde, dlg.m_nTeller, 0);
			}
			else {
				CNyStemmeDialog dlg;
				dlg.DoModal();
				bFilOK = tekstFile.ReadData(dlg.m_dLengde, m_plotInfo.m_nSlagPrTakt, m_pMIDISang->GetSpor());
			}
			if (bFilOK) {
				m_pMIDISang->LeggTilSekvens(tekstFile.GetMidiSekvens(), m_pMIDISang->GetSpor());
				CFMOscillator* pMod = new CFMOscillator;
				int metronom = m_pMIDISang->GetMidiTime()->GetMetronom();
				pMod->SetMetronom(metronom);
				tekstFile.GetFMSekvens()->SetModulator(pMod);
				m_pFMSang->LeggTilSekvens(tekstFile.GetFMSekvens());
				m_pFMSang->ComputePlot();
				SetModifiedFlag();
				UpdateFrame();
				UpdateAllViews(NULL, CFmrytmeView::NY_SANG);
			}
			tekstFile.Close();
			EndWaitCursor();
		}
	}
}

void CFmrytmeDoc::OnSangmeny() 
{
	CSangDialog dlg;
	dlg.m_nStemmer = m_pMIDISang->GetSpor();
	dlg.m_sSangTittel = m_pMIDISang->GetTittel();
	for (int i=0; i<m_pMIDISang->GetSpor(); i++) {
		dlg.m_nProgram[i] = m_pMIDISang->GetSekvens(i)->GetProgram();
		dlg.m_nKanal[i] = m_pMIDISang->GetSekvens(i)->GetKanal() + 1;
		dlg.m_sTittel[i] = m_pMIDISang->GetSekvens(i)->GetTittel();
	}
	if (dlg.DoModal() == IDOK) {
		m_pMIDISang->SetTittel(dlg.m_sSangTittel);
		for(int i=0; i<m_pMIDISang->GetSpor(); i++) {
			m_pMIDISang->GetSekvens(i)->SetProgram((BYTE) dlg.m_nProgram[i]);
			m_pMIDISang->GetSekvens(i)->SetKanal(dlg.m_nKanal[i] - 1);
			m_pMIDISang->GetSekvens(i)->SetTittel(dlg.m_sTittel[i]);
		}
		UpdateAllViews(NULL);
	}
}

void CFmrytmeDoc::OnUpdateSangmeny(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pMIDISang != NULL);	
}

void CFmrytmeDoc::DeleteContents() 
{
	UpdateAllViews(NULL, CFmrytmeView::INGEN_SANG);
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	if (pFrame->m_midiPlay.IsPlaying()) {
		pFrame->m_midiPlay.Stopp();
		AfxMessageBox(IDS_DOC_PLAY);	// Avslutter spilling først
	}
	delete m_pMIDISang;
	delete m_pFMSang;
	m_pMIDISang = NULL;
	m_pFMSang = NULL;
	m_bSolo = FALSE;
	m_plotInfo.m_nSynkStemme = m_nEditSpor = 0;
	CDocument::DeleteContents();
}

void CFmrytmeDoc::UpdateFrame()
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	if (pFrame->m_midiPlay.IsPlaying()) {
		pFrame->SetDocUpdated();		// Oppdatering må gjøres når avspilling er avsluttet
		return;
	}
	if (m_pMIDISang == NULL) {
		pFrame->InitDialogBar();
	}
	else {
		pFrame->UpdateSpor(m_pMIDISang->GetSpor(), m_nEditSpor);
		pFrame->UpdateTempo(m_pMIDISang->GetMidiTime()->GetMetronom());
		if (m_bSolo)
			pFrame->m_midiPlay.SetSekvens(m_pMIDISang->GetSekvens(m_nEditSpor));
		else
			pFrame->m_midiPlay.SetSekvens(m_pMIDISang);
	}
}

void CFmrytmeDoc::OnSetStemme()
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	int nSpor = pFrame->GetSpor();
	if (m_nEditSpor != nSpor) {
		m_nEditSpor = nSpor;
		if (m_bSolo)
			pFrame->m_midiPlay.SetSekvens(m_pMIDISang->GetSekvens(m_nEditSpor));
		UpdateAllViews(NULL, CFmrytmeView::NY_STEMME);
	}
}

void CFmrytmeDoc::OnSetTempo()
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	if (m_pMIDISang != NULL)
		m_pMIDISang->SetMetronom(pFrame->GetTempo());
}

void CFmrytmeDoc::OnEditedTempo()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (m_pFMSang != NULL)
		m_pFMSang->SetMetronom(pFrame->GetTempo());
	m_pFMSang->ComputePlot();
	UpdateAllViews(NULL, CFmrytmeView::NY_SANG);
}

void CFmrytmeDoc::OnSolo() 
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	m_bSolo = !m_bSolo;
	if (m_bSolo)
		pFrame->m_midiPlay.SetSekvens(m_pMIDISang->GetSekvens(m_nEditSpor));
	else
		pFrame->m_midiPlay.SetSekvens(m_pMIDISang);
	UpdateAllViews(NULL, CFmrytmeView::SOLO_SANG);
}

void CFmrytmeDoc::OnUpdateSolo(CCmdUI* pCmdUI) 
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	pCmdUI->SetCheck(m_bSolo);
	pCmdUI->Enable((m_pMIDISang != NULL) && !(pFrame->m_midiPlay.IsPlaying()));
}

void CFmrytmeDoc::OnCloseDocument() 
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	if (pFrame->m_midiPlay.IsPlaying()) {
		pFrame->m_midiPlay.Stopp();
		AfxMessageBox(IDS_DOC_PLAY);		// Avslutter spilling først
	}
	CDocument::OnCloseDocument();
}

void CFmrytmeDoc::LagEnkelModulator()
{
	switch (GetEditSekvens()->GetModulator()->GetType())
	{
	case COperator::FMOSCILLATOR:
		return;
	case COperator::SERIEOSC: {
		CFMOscillator* pGmlOp = (CFMOscillator*) GetEditSekvens()->GetModulator();
		delete pGmlOp->GetModulator();
		pGmlOp->SetModulator(NULL);
		pGmlOp->SetType(COperator::FMOSCILLATOR);
		return;
		}
	case COperator::PARALLELLOSC: {
		CSumOp* pGmlOp = (CSumOp*) GetEditSekvens()->GetModulator();
		GetEditSekvens()->SetModulator(pGmlOp->GetOperator(CSumOp::RIGHT));
		pGmlOp->SetOperator(CSumOp::RIGHT, NULL);
		delete pGmlOp;
		return;
		}
	}
}

void CFmrytmeDoc::LagParallellModulator()
{
	switch (GetEditSekvens()->GetModulator()->GetType())
	{
	case COperator::FMOSCILLATOR: {
		CFMOscillator* pGmlOp = (CFMOscillator*) GetEditSekvens()->GetModulator();
		CSumOp* pNySumOp = new CSumOp(COperator::PARALLELLOSC);
		pNySumOp->SetOperator(CSumOp::RIGHT, pGmlOp);
		CFMOscillator* pNyOsc = new CFMOscillator;
		pNySumOp->SetOperator(CSumOp::LEFT, pNyOsc);
		GetEditSekvens()->SetModulator(pNySumOp);
		return;
		}
	case COperator::SERIEOSC: {
		CFMOscillator* pGmlOp = (CFMOscillator*) GetEditSekvens()->GetModulator();
		CSumOp* pNySumOp = new CSumOp(COperator::PARALLELLOSC);
		pNySumOp->SetOperator(CSumOp::LEFT, pGmlOp->GetModulator());
		pGmlOp->SetModulator(NULL);
		pGmlOp->SetType(COperator::FMOSCILLATOR);
		pNySumOp->SetOperator(CSumOp::RIGHT, pGmlOp);
		GetEditSekvens()->SetModulator(pNySumOp);
		return;
		}
	case COperator::PARALLELLOSC:
		return;
	}
}

void CFmrytmeDoc::LagSerieModulator()
{
	switch (GetEditSekvens()->GetModulator()->GetType())
	{
	case COperator::FMOSCILLATOR: {
		CFMOscillator* pGmlOp = (CFMOscillator*) GetEditSekvens()->GetModulator();
		CFMOscillator* pNyOsc = new CFMOscillator;
		pGmlOp->SetModulator(pNyOsc);
		pGmlOp->SetType(COperator::SERIEOSC);
		return;
		}
	case COperator::SERIEOSC:
		return;
	case COperator::PARALLELLOSC: {
		CSumOp* pGmlOp = (CSumOp*) GetEditSekvens()->GetModulator();
		CFMOscillator* pGmlOsc = (CFMOscillator*) pGmlOp->GetOperator(CSumOp::RIGHT);
		pGmlOp->SetOperator(CSumOp::RIGHT, NULL);
		pGmlOsc->SetModulator(pGmlOp->GetOperator(CSumOp::LEFT));
		pGmlOp->SetOperator(CSumOp::LEFT, NULL);
		pGmlOsc->SetType(COperator::SERIEOSC);
		GetEditSekvens()->SetModulator(pGmlOsc);
		delete pGmlOp;
		return;
		}
	}
}

void CFmrytmeDoc::OnUpdateParameter(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pFMSang != NULL);
}

CFMSekvens* CFmrytmeDoc::GetEditSekvens()
{
	if (m_pFMSang == NULL)
		return NULL;
	else
		return m_pFMSang->GetSekvens(m_nEditSpor);
}

void CFmrytmeDoc::OnUpdateOppsett(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pFMSang != NULL);
}

void CFmrytmeDoc::ComputeAlle()
{
	m_pFMSang->LikeModulatorer(m_nEditSpor);
	m_pFMSang->ComputePlot();
}

void CFmrytmeDoc::ComputeEditSekvens()
{
	m_pFMSang->ComputePlot(m_nEditSpor);
}

void CFmrytmeDoc::UpdatePeriode()
{
	m_pFMSang->UpdatePeriode();
	m_pFMSang->ComputePlot();
	UpdateAllViews(NULL, CFmrytmeView::NY_SANG);
}

void CFmrytmeDoc::OnFjernStemme() 
{
	if (m_pMIDISang == NULL)
		return;
	CString tekst;
	tekst.Format(L"The program is about to delete voice %d: ", m_nEditSpor+1);
	tekst = tekst + m_pMIDISang->GetSekvens(m_nEditSpor)->GetTittel();
	if (AfxMessageBox(tekst, MB_OKCANCEL) == IDOK) {
		m_pMIDISang->FjernSekvens(m_nEditSpor);
		m_pFMSang->FjernSekvens(m_nEditSpor);
		m_plotInfo.m_nSynkStemme = m_nEditSpor = 0;		
		if (m_pMIDISang->GetSpor() == 0) {
			DeleteContents();
			UpdateFrame();
			UpdateAllViews(NULL, CFmrytmeView::INGEN_SANG);
		}
		else {
			UpdateFrame();
			m_pFMSang->ComputePlot();
			UpdateAllViews(NULL, CFmrytmeView::NY_SANG);
		}
	}
}

void CFmrytmeDoc::OnUpdateTempo(CCmdUI* pCmdUI)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pCmdUI->Enable((m_pMIDISang != NULL) && !(pFrame->m_midiPlay.IsPlaying()));
}

void CFmrytmeDoc::OnUpdateFjernStemme(CCmdUI* pCmdUI) 
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	pCmdUI->Enable((m_pMIDISang != NULL) && !(pFrame->m_midiPlay.IsPlaying()));
}

void CFmrytmeDoc::OnUpdateKurve(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pFMSang != NULL);
}

void CFmrytmeDoc::OnUpdateGenerellmeny(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pFMSang != NULL);
}

void CFmrytmeDoc::OnUpdateExportMidi(CCmdUI* pCmdUI) 
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	pCmdUI->Enable((m_pMIDISang != NULL) && !(pFrame->m_midiPlay.IsPlaying()));
}
