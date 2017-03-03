// MidiDialog.cpp : implementation file
//

#include "stdafx.h"
#include "fmrytme.h"
#include "MidiDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSangDialog dialog


CSangDialog::CSangDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSangDialog::IDD, pParent)
{
	m_nStemmer = 0;
	//{{AFX_DATA_INIT(CSangDialog)
	m_sSangTittel = _T("");
	//}}AFX_DATA_INIT
	for (int i=0; i<CSettings::KANALER; i++) {
		m_nLydklasse[i] = 0;
		m_nLyd[i] = 0;
		m_nProgram[i] = 0;
		m_nKanal[i] = 1;
		m_sTittel[i] = "";
	}
}

void CSangDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSangDialog)
	DDX_Text(pDX, IDC_TITTEL, m_sSangTittel);
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_LYDKLASSE1, m_nLydklasse[0]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE2, m_nLydklasse[1]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE3, m_nLydklasse[2]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE4, m_nLydklasse[3]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE5, m_nLydklasse[4]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE6, m_nLydklasse[5]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE7, m_nLydklasse[6]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE8, m_nLydklasse[7]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE9, m_nLydklasse[8]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE10, m_nLydklasse[9]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE11, m_nLydklasse[10]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE12, m_nLydklasse[11]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE13, m_nLydklasse[12]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE14, m_nLydklasse[13]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE15, m_nLydklasse[14]);
	DDX_CBIndex(pDX, IDC_LYDKLASSE16, m_nLydklasse[15]);
	DDX_CBIndex(pDX, IDC_LYD1, m_nLyd[0]);
	DDX_CBIndex(pDX, IDC_LYD2, m_nLyd[1]);
	DDX_CBIndex(pDX, IDC_LYD3, m_nLyd[2]);
	DDX_CBIndex(pDX, IDC_LYD4, m_nLyd[3]);
	DDX_CBIndex(pDX, IDC_LYD5, m_nLyd[4]);
	DDX_CBIndex(pDX, IDC_LYD6, m_nLyd[5]);
	DDX_CBIndex(pDX, IDC_LYD7, m_nLyd[6]);
	DDX_CBIndex(pDX, IDC_LYD8, m_nLyd[7]);
	DDX_CBIndex(pDX, IDC_LYD9, m_nLyd[8]);
	DDX_CBIndex(pDX, IDC_LYD10, m_nLyd[9]);
	DDX_CBIndex(pDX, IDC_LYD11, m_nLyd[10]);
	DDX_CBIndex(pDX, IDC_LYD12, m_nLyd[11]);
	DDX_CBIndex(pDX, IDC_LYD13, m_nLyd[12]);
	DDX_CBIndex(pDX, IDC_LYD14, m_nLyd[13]);
	DDX_CBIndex(pDX, IDC_LYD15, m_nLyd[14]);
	DDX_CBIndex(pDX, IDC_LYD16, m_nLyd[15]);
	DDX_Text(pDX, IDC_PROGRAM1, m_nProgram[0]);
	DDX_Text(pDX, IDC_PROGRAM2, m_nProgram[1]);
	DDX_Text(pDX, IDC_PROGRAM3, m_nProgram[2]);
	DDX_Text(pDX, IDC_PROGRAM4, m_nProgram[3]);
	DDX_Text(pDX, IDC_PROGRAM5, m_nProgram[4]);
	DDX_Text(pDX, IDC_PROGRAM6, m_nProgram[5]);
	DDX_Text(pDX, IDC_PROGRAM7, m_nProgram[6]);
	DDX_Text(pDX, IDC_PROGRAM8, m_nProgram[7]);
	DDX_Text(pDX, IDC_PROGRAM9, m_nProgram[8]);
	DDX_Text(pDX, IDC_PROGRAM10, m_nProgram[9]);
	DDX_Text(pDX, IDC_PROGRAM11, m_nProgram[10]);
	DDX_Text(pDX, IDC_PROGRAM12, m_nProgram[11]);
	DDX_Text(pDX, IDC_PROGRAM13, m_nProgram[12]);
	DDX_Text(pDX, IDC_PROGRAM14, m_nProgram[13]);
	DDX_Text(pDX, IDC_PROGRAM15, m_nProgram[14]);
	DDX_Text(pDX, IDC_PROGRAM16, m_nProgram[15]);
	DDV_MinMaxInt(pDX, m_nProgram[0], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[1], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[2], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[3], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[4], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[5], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[6], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[7], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[8], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[9], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[10], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[11], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[12], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[13], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[14], 0, 127);
	DDV_MinMaxInt(pDX, m_nProgram[15], 0, 127);
	DDX_Text(pDX, IDC_TITTEL1, m_sTittel[0]);
	DDX_Text(pDX, IDC_TITTEL2, m_sTittel[1]);
	DDX_Text(pDX, IDC_TITTEL3, m_sTittel[2]);
	DDX_Text(pDX, IDC_TITTEL4, m_sTittel[3]);
	DDX_Text(pDX, IDC_TITTEL5, m_sTittel[4]);
	DDX_Text(pDX, IDC_TITTEL6, m_sTittel[5]);
	DDX_Text(pDX, IDC_TITTEL7, m_sTittel[6]);
	DDX_Text(pDX, IDC_TITTEL8, m_sTittel[7]);
	DDX_Text(pDX, IDC_TITTEL9, m_sTittel[8]);
	DDX_Text(pDX, IDC_TITTEL10, m_sTittel[9]);
	DDX_Text(pDX, IDC_TITTEL11, m_sTittel[10]);
	DDX_Text(pDX, IDC_TITTEL12, m_sTittel[11]);
	DDX_Text(pDX, IDC_TITTEL13, m_sTittel[12]);
	DDX_Text(pDX, IDC_TITTEL14, m_sTittel[13]);
	DDX_Text(pDX, IDC_TITTEL15, m_sTittel[14]);
	DDX_Text(pDX, IDC_TITTEL16, m_sTittel[15]);
	DDX_Text(pDX, IDC_KANAL1, m_nKanal[0]);
	DDX_Text(pDX, IDC_KANAL2, m_nKanal[1]);
	DDX_Text(pDX, IDC_KANAL3, m_nKanal[2]);
	DDX_Text(pDX, IDC_KANAL4, m_nKanal[3]);
	DDX_Text(pDX, IDC_KANAL5, m_nKanal[4]);
	DDX_Text(pDX, IDC_KANAL6, m_nKanal[5]);
	DDX_Text(pDX, IDC_KANAL7, m_nKanal[6]);
	DDX_Text(pDX, IDC_KANAL8, m_nKanal[7]);
	DDX_Text(pDX, IDC_KANAL9, m_nKanal[8]);
	DDX_Text(pDX, IDC_KANAL10, m_nKanal[9]);
	DDX_Text(pDX, IDC_KANAL11, m_nKanal[10]);
	DDX_Text(pDX, IDC_KANAL12, m_nKanal[11]);
	DDX_Text(pDX, IDC_KANAL13, m_nKanal[12]);
	DDX_Text(pDX, IDC_KANAL14, m_nKanal[13]);
	DDX_Text(pDX, IDC_KANAL15, m_nKanal[14]);
	DDX_Text(pDX, IDC_KANAL16, m_nKanal[15]);
	DDV_MinMaxInt(pDX, m_nKanal[0], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[1], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[2], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[3], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[4], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[5], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[6], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[7], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[8], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[9], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[10], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[11], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[12], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[13], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[14], 1, 16);
	DDV_MinMaxInt(pDX, m_nKanal[15], 1, 16);
}


BEGIN_MESSAGE_MAP(CSangDialog, CDialog)
	//{{AFX_MSG_MAP(CSangDialog)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_LYDKLASSE1, IDC_LYDKLASSE16, OnKlasseChange)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_LYD1, IDC_LYD16, OnLydChange)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_PROGRAM1, IDC_PROGRAM16, OnProgramChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSangDialog message handlers

BOOL CSangDialog::OnInitDialog() 
{
	int i;
	for (i=0; i<m_nStemmer; i++) {
		m_nLydklasse[i] = m_nProgram[i] / 8;
		m_nLyd[i] = m_nProgram[i] % 8;
		int selekt = (m_nLydklasse[i] * 8) + IDS_MIDI1;

		CString sCombo;
		CComboBox* pCB = (CComboBox*) GetDlgItem(IDC_LYD1 + i);
		pCB->ResetContent();
		for (int k = selekt; k < selekt+8; k++) {
			sCombo.LoadString(k);
			pCB->InsertString(-1,sCombo);
		}
		pCB->SetCurSel(0);
	}
	CWnd* kontroll;
	for (; i<CSettings::KANALER; i++) {
		kontroll = GetDlgItem(IDC_LYDKLASSE1+i);
		kontroll->EnableWindow(FALSE);
		kontroll = GetDlgItem(IDC_LYD1+i);
		kontroll->EnableWindow(FALSE);
		kontroll = GetDlgItem(IDC_PROGRAM1+i);
		kontroll->EnableWindow(FALSE);
		kontroll = GetDlgItem(IDC_TITTEL1+i);
		kontroll->EnableWindow(FALSE);
		kontroll = GetDlgItem(IDC_KANAL1+i);
		kontroll->EnableWindow(FALSE);
	}
	return CDialog::OnInitDialog();
}

void CSangDialog::OnKlasseChange(UINT nID)
{
	CComboBox* pCB = (CComboBox*) GetDlgItem(nID);
	int selekt = pCB->GetCurSel()*8;

	CString sProgram;
	sProgram.Format(L"%u", selekt);
	GetDlgItem(nID+2*CSettings::KANALER)->SetWindowText(sProgram);

	CString sCombo;
	pCB = (CComboBox*) GetDlgItem(nID+CSettings::KANALER);
	pCB->ResetContent();
	for (int i = selekt+IDS_MIDI1; i < selekt+(IDS_MIDI1+8); i++) {
		sCombo.LoadString(i);
		pCB->InsertString(-1,sCombo);
	}
	pCB->SetCurSel(0);
}

void CSangDialog::OnLydChange(UINT nID)
{
	CComboBox* pCB = (CComboBox*) GetDlgItem(nID);
	int selekt = pCB->GetCurSel();
	pCB = (CComboBox*) GetDlgItem(nID-CSettings::KANALER);
	selekt += pCB->GetCurSel()*8;
	CString sProgram;
	sProgram.Format(L"%u", selekt);
	GetDlgItem(nID+CSettings::KANALER)->SetWindowText(sProgram);
}

void CSangDialog::OnProgramChange(UINT nID)
{
	CString sProgram;
	GetDlgItem(nID)->GetWindowText(sProgram);
	int nProg = atoi(CT2A(sProgram));
	int nKlasse = nProg / 8;
	CComboBox* pCB = (CComboBox*) GetDlgItem(nID-2*CSettings::KANALER);
	pCB->SetCurSel(nKlasse);

	pCB = (CComboBox*) GetDlgItem(nID-CSettings::KANALER);
	pCB->ResetContent();
	int nTekst = nKlasse * 8 + IDS_MIDI1;
	CString sCombo;
	for (int i = nTekst; i < nTekst+8; i++) {
		sCombo.LoadString(i);
		pCB->InsertString(-1,sCombo);
	}
	pCB->SetCurSel(nProg % 8);
}
