// SSpGUIinput.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspInput.h"
#include "SSpGUIinput.h"
#include "SSpGUIinputEditor.h"

CInputEditor::CInputEditor(unsigned int nIDTemplate, CWnd* pParentWnd) : CDialog(nIDTemplate, pParentWnd)
{
}

bool sspGUIinput::loadInputEditor(sspInput* pInput)
{
	bool bOK = false;
	CInputEditor* pDlg = NULL;

	switch (pInput->getType()) {
	case SSP_ICPCON_ANALOG:
		pDlg = new CInputICPanalogDialog;
		pDlg->m_strTitle = "Input: ICP analog editor";
		break;
	case SSP_ICPCON_DIGITAL:
		pDlg = new CInputICPdigitalDialog;
		pDlg->m_strTitle = "Input: ICP digital editor";
		break;
	case SSP_PARALLEL:
		pDlg = new CInputParallelDialog;
		pDlg->m_strTitle = "Input: Parallel port editor";
		break;
	case SSP_MIDIIN_EVENT:
		pDlg = new CInputMIDIeventDialog;
		pDlg->m_strTitle = "Input: MIDI event editor";
		break;
	default:
		pDlg = NULL;
	}

	if (pDlg) {
		pDlg->m_pInput = pInput;
		if (pDlg->DoModal() == IDOK) {
			bOK = true;
		}
		delete pDlg;
	}

	return bOK;
}

sspInput* sspGUIinput::loadInputEditor(short nType)
{
	sspInput* pInput = NULL;
	CInputEditor* pDlg = NULL;

	switch (nType) {
	case SSP_ICPCON_ANALOG:
		pDlg = new CInputICPanalogDialog;
		pDlg->m_strTitle = "Input: ICP analog editor";
		break;
	case SSP_ICPCON_DIGITAL:
		pDlg = new CInputICPdigitalDialog;
		pDlg->m_strTitle = "Input: ICP digital editor";
		break;
	case SSP_PARALLEL:
		pDlg = new CInputParallelDialog;
		pDlg->m_strTitle = "Input: Parallel port editor";
		break;
	case SSP_MIDIIN_EVENT:
		pDlg = new CInputMIDIeventDialog;
		pDlg->m_strTitle = "Input: MIDI event editor";
		break;
	default:
		pDlg = NULL;
	}

	if (pDlg) {
		pDlg->m_pInput = NULL;
		if (pDlg->DoModal() == IDOK) {
			pInput = pDlg->m_pInput;
		}
		delete pDlg;
	}

	return pInput;
}
