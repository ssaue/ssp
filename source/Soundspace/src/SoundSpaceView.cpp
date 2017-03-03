// SoundSpaceView.cpp : implementation of the CSoundSpaceView class
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "debug.h"

#include <algorithm>

#include "SoundSpaceDoc.h"
#include "SoundSpaceView.h"
#include "sspSetting.h"
#include "sspDateTimeManager.h"
#include "sspExecutiveManager.h"
#include "sspInputManager.h"
#include "sspPools.h"
#include "SSpValues.h"
#include "LayoutDialog.h"
#include "AutostartDialog.h"
#include "SSpGUIUtilityEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundSpaceView

IMPLEMENT_DYNCREATE(CSoundSpaceView, CFormView)

BEGIN_MESSAGE_MAP(CSoundSpaceView, CFormView)
	//{{AFX_MSG_MAP(CSoundSpaceView)
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_INITIALIZE, OnInitialize)
	ON_COMMAND(ID_EXPORT_ASCIIDUMP, OnExportAsciidump)
	ON_COMMAND(ID_EXPORT_CONFIGERRORS, OnExportConfigerrors)
	ON_COMMAND(ID_EDIT_DISPLAY, OnEditDisplay)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DISPLAY, OnUpdateEditDisplay)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EXT_VALUE1, IDC_EXT_VALUE16, OnChangeExtValue)
	ON_MESSAGE(WM_PLAY_AUTOMATIC, OnPlayAutomatic)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundSpaceView construction/destruction

CSoundSpaceView::CSoundSpaceView()
	: CFormView(CSoundSpaceView::IDD)
{
	//{{AFX_DATA_INIT(CSoundSpaceView)
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CSoundSpaceView::~CSoundSpaceView()
{
}

void CSoundSpaceView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundSpaceView)
	//}}AFX_DATA_MAP
}

BOOL CSoundSpaceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSoundSpaceView diagnostics

#ifdef _DEBUG
void CSoundSpaceView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSoundSpaceView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSoundSpaceDoc* CSoundSpaceView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSoundSpaceDoc)));
	return (CSoundSpaceDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSoundSpaceView message handlers

void CSoundSpaceView::OnTimer(unsigned int nIDEvent) 
{
  OnUpdate(NULL, CSoundSpaceDoc::STATE, NULL);
  if (!sspExecutiveManager::Instance().isRunning()) {
    OnStop();
    if (sspExecutiveManager::Instance().getShutdownProcedure() == SSP_EMPTY_EXIT)
      AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
  }
  CFormView::OnTimer(nIDEvent);
}

// Manual changes to the external values
void CSoundSpaceView::OnChangeExtValue(unsigned int nID)
{
	CSoundSpaceDoc* pDoc = GetDocument();
	int nValue = nID - IDC_EXT_VALUE1;
	if (nValue >= 0 && nValue < (int) pDoc->m_nDisplaySelection.size()) {
		SSpBasicValue* pExt = dynamic_cast <SSpBasicValue*> (sspPool::Instance().values.getValue(pDoc->m_nDisplaySelection[nValue]));
		if (pExt != NULL) {
			CString text;
			GetDlgItem(nID)->GetWindowText(text);
			pExt->setValue((float) _wtof(text));
		}
	}
}

void CSoundSpaceView::OnInitialUpdate() 
{
	OnUpdate(NULL, CSoundSpaceDoc::DISPLAY, NULL);
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
}

void CSoundSpaceView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CSoundSpaceDoc* pDoc = GetDocument();
	switch (lHint) {
	case CSoundSpaceDoc::DISPLAY: {

    size_t i, nMax;
	
		// Set value display
		nMax = pDoc->m_nDisplaySelection.size() > VALUE_MAX 
			? VALUE_MAX : pDoc->m_nDisplaySelection.size();
		for (i=0; i<nMax; i++) {
			GetDlgItem(IDC_EXT_VALUE1+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EXT_TEXT1+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EXT_TEXT1+i)->
				SetWindowText(sspPool::Instance().values.GetName(pDoc->m_nDisplaySelection[i]));
		}
		for (; i<VALUE_MAX; i++) {
			GetDlgItem(IDC_EXT_VALUE1+i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EXT_TEXT1+i)->ShowWindow(SW_HIDE);
		}

		// Set slider layout (all faders range from 0 to SLIDER_RANGE)
    nMax = std::min(sspInputManager::Instance().getFaderSize(), (size_t) sspSetting::MAX_SLIDERS);
		for (i=0; i<nMax; i++) {
      const SSpInputValueRange* pVal = sspInputManager::Instance().getFader(i);
			if (pVal != NULL) {
				GetDlgItem(IDC_LAYER_TXT0+i)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_LAYER_TXT0+i)->SetWindowText(pVal->getName().c_str());
				CSliderCtrl* pSC = (CSliderCtrl*) GetDlgItem(IDC_LAYER_VOLUME0+i);
				pSC->ShowWindow(SW_SHOW);
				pSC->SetRange(0, SLIDER_RANGE);
				pSC->EnableWindow(FALSE);
			}
			else {
				GetDlgItem(IDC_LAYER_VOLUME0+i)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_LAYER_TXT0+i)->ShowWindow(SW_HIDE);
			}
		}
		for (; i<sspSetting::MAX_SLIDERS; i++) {
			GetDlgItem(IDC_LAYER_VOLUME0+i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_LAYER_TXT0+i)->ShowWindow(SW_HIDE);
		}
		refreshFaders();

#if defined(WATCHDOG_DECISION_OCX)
		if (sspSetting::watchdog.enable)
			GetDlgItem(IDC_WATCHDOGOCXCTRL1)->ShowWindow(SW_SHOW);
		else
			GetDlgItem(IDC_WATCHDOGOCXCTRL1)->ShowWindow(SW_HIDE);
#endif

		break;
								  }

	case CSoundSpaceDoc::INITIALIZE:
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
		break;

	case CSoundSpaceDoc::STATE: {
		CString text;
		int nMax = pDoc->m_nDisplaySelection.size() > VALUE_MAX 
			? VALUE_MAX : pDoc->m_nDisplaySelection.size();
		for (int i=0; i<nMax; i++) {
			text.Format(_T("%.2f"), sspPool::Instance().values.getValue(pDoc->m_nDisplaySelection[i])->getValue());
			GetDlgItem(IDC_EXT_VALUE1+i)->SetWindowText(text);
		}
    wchar_t buffer[20];
    GetDlgItem(IDC_DATE)->SetWindowText(sspDateTimeManager::Instance().current().dateAsString(buffer, 19));
		GetDlgItem(IDC_TIME)->SetWindowText(sspDateTimeManager::Instance().current().clockAsString(buffer, 19));
		break;
										  }
	}
}

void CSoundSpaceView::OnVScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) 
{
  if (pScrollBar == NULL)
		CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
  else {
		if (nSBCode == TB_THUMBTRACK) {
			int nFader = pScrollBar->GetDlgCtrlID() - IDC_LAYER_VOLUME0;
      sspInputManager::Instance().setFaderValue(nFader, (SLIDER_RANGE - nPos) /(float) SLIDER_RANGE);
		}
		else if (nSBCode == TB_THUMBPOSITION) {
			sspSetting::WriteSliderRegistry();
		}
	}
}

void CSoundSpaceView::OnStart() 
{
	GetDlgItem(IDC_INITIALIZE)->EnableWindow(FALSE);
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
	sspExecutiveManager::Instance().start();
  m_nTimer = SetTimer(1, (unsigned int) (1000 * sspSetting::update.gui), NULL);
}

void CSoundSpaceView::OnStop() 
{
	KillTimer(m_nTimer);
	sspExecutiveManager::Instance().stop();
	GetDlgItem(IDC_INITIALIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
}

void CSoundSpaceView::OnInitialize() 
{
	UpdateData(false);
  if (GetDocument()->initialize(GetSafeHwnd())) {
		GetDlgItem(IDC_START)->EnableWindow(TRUE);
		refreshFaders();
    OnUpdate(NULL, CSoundSpaceDoc::STATE, NULL);
	}
}

LRESULT CSoundSpaceView::OnPlayAutomatic(WPARAM wParam, LPARAM lParam)
{
  if (sspExecutiveManager::Instance().getStartupProcedure() == SSP_START_NOTHING)
    return 0L;

	CAutostartDialog dlg;
	if (dlg.DoModal() == IDOK) {
		OnInitialize();
    if (sspExecutiveManager::Instance().getStartupProcedure() == SSP_START_PLAY)
		  OnStart();
	}
	return 0L;
}

void CSoundSpaceView::OnEditDisplay() 
{
	CLayoutDialog dlg;
	CSoundSpaceDoc* pDoc = GetDocument();
	dlg.m_nValueSelection = pDoc->m_nDisplaySelection;
  dlg.m_nFaderSelection = sspInputManager::Instance().getFaderSelection();
	if (dlg.DoModal()) {
		pDoc->m_nDisplaySelection = dlg.m_nValueSelection;
    sspInputManager::Instance().setFaderSelection(dlg.m_nFaderSelection);
		OnUpdate(NULL, CSoundSpaceDoc::DISPLAY, NULL);
	}
}

void CSoundSpaceView::OnUpdateEditDisplay(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(sspPool::Instance().values.IsLoaded());
}

void CSoundSpaceView::refreshFaders()
{
  size_t nSize = std::min(sspInputManager::Instance().getFaderSize(), (size_t) sspSetting::MAX_SLIDERS);
	for (size_t i=0; i<nSize; i++) {
    const SSpInputValueRange* pVal = sspInputManager::Instance().getFader(i);
  	CSliderCtrl* pSC = (CSliderCtrl*) GetDlgItem(IDC_LAYER_VOLUME0+i);
		if (pVal != NULL) {
			pSC->EnableWindow(TRUE);
      pSC->SetPos((SLIDER_RANGE - (int) (pVal->getRelativeValue() * SLIDER_RANGE)));
		}
    else {
			pSC->EnableWindow(FALSE);
    }
	}
}

void CSoundSpaceView::OnDestroy() 
{
	KillTimer(m_nTimer);
  GetDocument()->terminate();
	CFormView::OnDestroy();
}

void CSoundSpaceView::OnExportAsciidump()
{
  CString filename("dumpAscii.txt");
  bool bSuccess = false;

  sspSaveDialog fdlg(filename);
	fdlg.m_ofn.lpstrTitle = _T("Select ASCII dump file");
	if (fdlg.DoModal() == IDOK) {
		filename = fdlg.GetPathName();
    bSuccess = GetDocument()->exportAscii(filename);
  }
  if (bSuccess) {
	  AfxMessageBox(L"Configuration is written to file " + filename);
	}
  else {
	  AfxMessageBox(L"Unable to write to " + filename);
  }
}

void CSoundSpaceView::OnExportConfigerrors()
{
  CString filename("dumpErrors.txt");
  int nErrors, nWarnings;
  bool bSuccess = false;

  sspSaveDialog fdlg(filename);
	fdlg.m_ofn.lpstrTitle = _T("Select configuration error file");
	if (fdlg.DoModal() == IDOK) {
		filename = fdlg.GetPathName();
    bSuccess = GetDocument()->exportErrors(filename, nErrors, nWarnings);
	}
  if (bSuccess) {
	  AfxMessageBox(L"Success! No errors or warnings.");
	}
  else if (nErrors == 0 && nWarnings == 0) {
	  AfxMessageBox(L"Unable to write to " + filename);
  }
  else {
	  CString strMessage;
		strMessage.Format(_T("There are %d errors and %d warnings"), nErrors, nWarnings);
		AfxMessageBox(strMessage);
  }
}
