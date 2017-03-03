// SSpGUIValue.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "SSpBasic.h"
#include "SSpGUIValue.h"
#include "SSpGUIValueEditor.h"

CValueEditor::CValueEditor(unsigned int nIDTemplate, CWnd* pParentWnd) : CDialog(nIDTemplate, pParentWnd)
{
}

bool SSpGUIValue::loadValueEditor(sspValue* pVal, CString& strName)
{
	bool bOK = false;
	CValueEditor* pDlg = NULL;

	switch (pVal->getType()) {
	case SSP_VALUE_SINGLE:
		pDlg = new CValueFloatDialog;
		pDlg->m_strTitle = "Value: Single editor";
		break;
	case SSP_VALUE_INPUT_RANGE:
		pDlg = new CValueInputRangeDialog;
		pDlg->m_strTitle = "Value: Input range editor";
		break;
	case SSP_VALUE_CLOCK:
		pDlg = new CValueClockDialog;
		pDlg->m_strTitle = "Value: Clock map editor";
		break;
	case SSP_VALUE_DATE:
		pDlg = new CValueDateDialog;
		pDlg->m_strTitle = "Value: Date map editor";
		break;
	case SSP_VALUE_MAP:
		pDlg = new CValueMapDialog;
		pDlg->m_strTitle = "Value: Linear map editor";
		break;
	case SSP_VALUE_SUM:
		pDlg = new CValueSumDialog;
		pDlg->m_strTitle = "Value: Sum editor";
		break;
	case SSP_VALUE_PRODUCT:
		pDlg = new CValueProductDialog;
		pDlg->m_strTitle = "Value: Product editor";
		break;
	case SSP_VALUE_CONDITIONAL:
		pDlg = new CValueConditionalDialog;
		pDlg->m_strTitle = "Value: Conditional editor";
		break;
	case SSP_VALUE_RANDOM:
		pDlg = new CValueRandomDialog;
		pDlg->m_strTitle = "Value: Random editor";
		break;
	case SSP_VALUE_SECONDSRUN:
	default:
		pDlg = NULL;
	}

	if (pDlg) {
		pDlg->m_pVal = pVal;
		pDlg->m_strName = strName;
		if (pDlg->DoModal() == IDOK) {
			strName = pDlg->m_strName;
			bOK = true;
		}
		delete pDlg;
	}

	return bOK;
}


sspValue* SSpGUIValue::loadValueEditor(short nType, CString& strName)
{
	sspValue* pVal = NULL;
	CValueEditor* pDlg = NULL;

	switch (nType) {
	case SSP_VALUE_SINGLE:
		pDlg = new CValueFloatDialog;
		pDlg->m_strTitle = "Value: Single editor";
		break;
	case SSP_VALUE_INPUT_RANGE:
		pDlg = new CValueInputRangeDialog;
		pDlg->m_strTitle = "Value: Input range editor";
		break;
	case SSP_VALUE_CLOCK:
		pDlg = new CValueClockDialog;
		pDlg->m_strTitle = "Value: Clock map editor";
		break;
	case SSP_VALUE_DATE:
		pDlg = new CValueDateDialog;
		pDlg->m_strTitle = "Value: Date map editor";
		break;
	case SSP_VALUE_MAP:
		pDlg = new CValueMapDialog;
		pDlg->m_strTitle = "Value: Linear map editor";
		break;
	case SSP_VALUE_SUM:
		pDlg = new CValueSumDialog;
		pDlg->m_strTitle = "Value: Sum editor";
		break;
	case SSP_VALUE_PRODUCT:
		pDlg = new CValueProductDialog;
		pDlg->m_strTitle = "Value: Product editor";
		break;
	case SSP_VALUE_CONDITIONAL:
		pDlg = new CValueConditionalDialog;
		pDlg->m_strTitle = "Value: Conditional editor";
		break;
	case SSP_VALUE_RANDOM:
		pDlg = new CValueRandomDialog;
		pDlg->m_strTitle = "Value: Random editor";
		break;
	case SSP_VALUE_SECONDSRUN:
		strName = "Seconds elapsed";
		return new sspSecondsRun();
		break;
	default:
		pDlg = NULL;
	}

	if (pDlg) {
		pDlg->m_pVal = NULL;
		pDlg->m_strName = strName;
		if (pDlg->DoModal() == IDOK) {
			strName = pDlg->m_strName;
			pVal = pDlg->m_pVal;
		}
		delete pDlg;
	}

	return pVal;
}
