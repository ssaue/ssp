// SSpGUIPlayObject.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "SSpTasks.h"
#include "SSpGUIPlayObject.h"
#include "SSpGUIPlayObjectEditor.h"

CPlayObjectEditor::CPlayObjectEditor(unsigned int nIDTemplate, CWnd* pParentWnd) : CDialog(nIDTemplate, pParentWnd)
{
}

bool SSpGUIPlayObject::loadPlayObjectEditor(sspPlayObject* pObj, CString& strName)
{
	bool bOK = false;
	CPlayObjectEditor* pDlg = NULL;

	switch (pObj->getType()) {
	case SSP_PLAY_AUDIO_FILE:
		pDlg = new CObjectWavDialog;
		pDlg->m_strTitle = "PlayObject: Audio file editor";
		break;
	case SSP_PLAY_MIDI_FILE:
		pDlg = new CObjectMidiDialog;
		pDlg->m_strTitle = "PlayObject: Midi file editor";
		break;
	case SSP_PLAY_DS_FILE:
		pDlg = new CObjectDSControlDialog;
		pDlg->m_strTitle = "PlayObject: DS control file editor";
		break;
	case SSP_PLAY_DMX_FILE:
		pDlg = new CObjectDMXDialog;
		pDlg->m_strTitle = "PlayObject: DMX file editor";
		break;
	case SSP_PLAY_MIDI_LIST:
		pDlg = new CObjectMidiListDialog;
		pDlg->m_strTitle = "PlayObject: Midi event list editor";
		break;
	case SSP_PLAY_SILENCE:
		pDlg = new CObjectSilenceDialog;
		pDlg->m_strTitle = "PlayObject: Silence editor";
		break;
	case SSP_PLAY_SIMULTANEOUS:
		pDlg = new CObjectSimultaneousDialog;
		pDlg->m_strTitle = "PlayObject: Simultaneous editor";
		break;
	case SSP_PLAY_SEQUENTIAL:
		pDlg = new CObjectSequentialDialog;
		pDlg->m_strTitle = "PlayObject: Sequential editor";
		break;
	case SSP_PLAY_LINEAR_SELECT:
		pDlg = new CObjectLinearSelectDialog;
		pDlg->m_strTitle = "PlayObject: Linear select editor";
		break;
	case SSP_PLAY_RANDOM:
		pDlg = new CObjectRandomDialog;
		pDlg->m_strTitle = "PlayObject: Random editor";
		break;
	case SSP_PLAY_CHAIN:
		pDlg = new CObjectChainDialog;
		pDlg->m_strTitle = "PlayObject: Chain editor";
		break;
	case SSP_PLAY_CONDITIONAL:
		pDlg = new CObjectConditionalDialog;
		pDlg->m_strTitle = "PlayObject: Conditional editor";
		break;
	case SSP_PLAY_DISTRIBUTION:
		pDlg = new CObjectDistributionDialog;
		pDlg->m_strTitle = "PlayObject: Distribution editor";
		break;
	case SSP_PLAY_STATE:
		pDlg = new CObjectSetState;
		pDlg->m_strTitle = "PlayObject: Global state editor";
		break;
	default:
		pDlg = NULL;
	}

	if (pDlg) {
		sspMultipleObject mo;
		mo.pObj = pObj;
		mo.strName = strName;
		pDlg->m_pObjs.push_back(&mo);
		pDlg->m_bMultipleSelect = false;
		if (pDlg->DoModal() == IDOK) {
			mo = *pDlg->m_pObjs[0];
			strName = mo.strName;
			bOK = true;
		}
		delete pDlg;
	}
	return bOK;
}

sspObjVector SSpGUIPlayObject::loadPlayObjectEditor(short nType, bool bMultiple)
{
	sspObjVector vObj;
	CPlayObjectEditor* pDlg = NULL;

	switch (nType) {
	case SSP_PLAY_AUDIO_FILE:
		pDlg = new CObjectWavDialog;
		pDlg->m_strTitle = "PlayObject: Audio file editor";
		break;
	case SSP_PLAY_MIDI_FILE:
		pDlg = new CObjectMidiDialog;
		pDlg->m_strTitle = "PlayObject: Midi file editor";
		break;
	case SSP_PLAY_DS_FILE:
		pDlg = new CObjectDSControlDialog;
		pDlg->m_strTitle = "PlayObject: DS control file editor";
		break;
	case SSP_PLAY_DMX_FILE:
		pDlg = new CObjectDMXDialog;
		pDlg->m_strTitle = "PlayObject: DMX file editor";
		break;
	case SSP_PLAY_MIDI_LIST:
		pDlg = new CObjectMidiListDialog;
		pDlg->m_strTitle = "PlayObject: Midi event list editor";
		break;
	case SSP_PLAY_SILENCE:
		pDlg = new CObjectSilenceDialog;
		pDlg->m_strTitle = "PlayObject: Silence editor";
		break;
	case SSP_PLAY_SIMULTANEOUS:
		pDlg = new CObjectSimultaneousDialog;
		pDlg->m_strTitle = "PlayObject: Simultaneous editor";
		break;
	case SSP_PLAY_SEQUENTIAL:
		pDlg = new CObjectSequentialDialog;
		pDlg->m_strTitle = "PlayObject: Sequential editor";
		break;
	case SSP_PLAY_LINEAR_SELECT:
		pDlg = new CObjectLinearSelectDialog;
		pDlg->m_strTitle = "PlayObject: Linear select editor";
		break;
	case SSP_PLAY_RANDOM:
		pDlg = new CObjectRandomDialog;
		pDlg->m_strTitle = "PlayObject: Random editor";
		break;
	case SSP_PLAY_CHAIN:
		pDlg = new CObjectChainDialog;
		pDlg->m_strTitle = "PlayObject: Chain editor";
		break;
	case SSP_PLAY_CONDITIONAL:
		pDlg = new CObjectConditionalDialog;
		pDlg->m_strTitle = "PlayObject: Conditional editor";
		break;
	case SSP_PLAY_DISTRIBUTION:
		pDlg = new CObjectDistributionDialog;
		pDlg->m_strTitle = "PlayObject: Distribution editor";
		break;
	case SSP_PLAY_STATE:
		pDlg = new CObjectSetState;
		pDlg->m_strTitle = "PlayObject: Global state editor";
		break;
	default:
		pDlg = NULL;
	}

	if (pDlg) {
		pDlg->m_bMultipleSelect = bMultiple;
		if (pDlg->DoModal() == IDOK) {
			vObj = pDlg->m_pObjs;
		}
		delete pDlg;
	}

	return vObj;
}
