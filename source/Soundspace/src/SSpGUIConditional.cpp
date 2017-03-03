// SSpGUIConditional.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "SSpBasic.h"
#include "SSpGUIConditional.h"
#include "SSpGUIConditionalEditor.h"

CConditionalEditor::CConditionalEditor(unsigned int nIDTemplate, CWnd* pParentWnd) : CDialog(nIDTemplate, pParentWnd)
{
}

bool SSpGUIConditional::loadConditionalEditor(sspConditional* pCond, CString& strName)
{
	bool bOK = false;
	CConditionalEditor* pDlg = NULL;

	switch (pCond->getType()) {
	case SSP_COND_BOOLEAN:
		pDlg = new CCondBooleanDialog;
		pDlg->m_strTitle = "Conditional: Boolean editor";
		break;
	case SSP_COND_RELATION:
		pDlg = new CCondRelationDialog;
		pDlg->m_strTitle = "Conditional: Value relation editor";
		break;
	case SSP_COND_RANGE:
		pDlg = new CCondRangeDialog;
		pDlg->m_strTitle = "Conditional: Value range editor";
		break;
	case SSP_COND_TIME_INTERVAL:
		pDlg = new CCondTimeIntervalDialog;
		pDlg->m_strTitle = "Conditional: Time interval editor";
		break;
	case SSP_COND_TIME_RELATION:
		pDlg = new CCondTimeRelationDialog;
		pDlg->m_strTitle = "Conditional: Time relation editor";
		break;
	case SSP_COND_MINUTE_RELATION:
		pDlg = new CCondMinuteRelationDialog;
		pDlg->m_strTitle = "Conditional: Minute relation editor";
		break;
	case SSP_COND_DATE_INTERVAL:
		pDlg = new CCondDateIntervalDialog;
		pDlg->m_strTitle = "Conditional: Date interval editor";
		break;
	case SSP_COND_DAYOFWEEK:
		pDlg = new CCondDayOfWeekDialog;
		pDlg->m_strTitle = "Conditional: Day of week editor";
		break;
	case SSP_COND_PLAYING:
		pDlg = new CCondPlayingDialog;
		pDlg->m_strTitle = "Conditional: Playing object editor";
		break;
	case SSP_COND_AND:
		pDlg = new CCondAndDialog;
		pDlg->m_strTitle = "Conditional: And editor";
		break;
	case SSP_COND_OR:
		pDlg = new CCondOrDialog;
		pDlg->m_strTitle = "Conditional: Or editor";
		break;
	case SSP_COND_NOT:
		pDlg = new CCondNotDialog;
		pDlg->m_strTitle = "Conditional: Not editor";
		break;
	default:
		pDlg = NULL;
	}

	if (pDlg) {
		pDlg->m_pCond = pCond;
		pDlg->m_strName = strName;
		if (pDlg->DoModal() == IDOK) {
			strName = pDlg->m_strName;
			bOK = true;
		}
		delete pDlg;
	}

	return bOK;
}


sspConditional* SSpGUIConditional::loadConditionalEditor(short nType, CString& strName)
{
	sspConditional* pCond = NULL;
	CConditionalEditor* pDlg = NULL;

	switch (nType) {
	case SSP_COND_BOOLEAN:
		pDlg = new CCondBooleanDialog;
		pDlg->m_strTitle = "Conditional: Boolean editor";
		break;
	case SSP_COND_RELATION:
		pDlg = new CCondRelationDialog;
		pDlg->m_strTitle = "Conditional: Value relation editor";
		break;
	case SSP_COND_RANGE:
		pDlg = new CCondRangeDialog;
		pDlg->m_strTitle = "Conditional: Value range editor";
		break;
	case SSP_COND_TIME_INTERVAL:
		pDlg = new CCondTimeIntervalDialog;
		pDlg->m_strTitle = "Conditional: Time interval editor";
		break;
	case SSP_COND_TIME_RELATION:
		pDlg = new CCondTimeRelationDialog;
		pDlg->m_strTitle = "Conditional: Time relation editor";
		break;
	case SSP_COND_MINUTE_RELATION:
		pDlg = new CCondMinuteRelationDialog;
		pDlg->m_strTitle = "Conditional: Minute relation editor";
		break;
	case SSP_COND_DATE_INTERVAL:
		pDlg = new CCondDateIntervalDialog;
		pDlg->m_strTitle = "Conditional: Date interval editor";
		break;
	case SSP_COND_DAYOFWEEK:
		pDlg = new CCondDayOfWeekDialog;
		pDlg->m_strTitle = "Conditional: Day of week editor";
		break;
	case SSP_COND_PLAYING:
		pDlg = new CCondPlayingDialog;
		pDlg->m_strTitle = "Conditional: Playing object editor";
		break;
	case SSP_COND_AND:
		pDlg = new CCondAndDialog;
		pDlg->m_strTitle = "Conditional: And editor";
		break;
	case SSP_COND_OR:
		pDlg = new CCondOrDialog;
		pDlg->m_strTitle = "Conditional: Or editor";
		break;
	case SSP_COND_NOT:
		pDlg = new CCondNotDialog;
		pDlg->m_strTitle = "Conditional: Not editor";
		break;
	default:
		pDlg = NULL;
	}

	if (pDlg) {
		pDlg->m_pCond = NULL;
		pDlg->m_strName = strName;
		if (pDlg->DoModal() == IDOK) {
			strName = pDlg->m_strName;
			pCond = pDlg->m_pCond;
		}
		delete pDlg;
	}

	return pCond;
}
