// SSpGUIString.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "SSpGUIString.h"
#include "sspGUIStringEditors.h"
#include "SSpBasic.h"

sspGUIStringEditor::sspGUIStringEditor(unsigned int nIDTemplate, CWnd* pParentWnd) : CDialog(nIDTemplate, pParentWnd)
{
}

bool sspGUIString::loadStringEditor(sspStringObject* pString, CString& strName)
{
	bool bOK = false;
	sspGUIStringEditor* pDlg = NULL;

	switch (pString->getType()) {
	case SSP_SIMPLE_STRING:
		pDlg = new sspSimpleStringEditor;
		pDlg->m_strTitle = "String object: Simple editor";
		break;
	case SSP_ALPHABET_STRING:
		pDlg = new sspAlphabetEditor;
		pDlg->m_strTitle = "String object: Linear alphabet editor";
		break;
	case SSP_CONCAT_STRING:
		pDlg = new sspConcatStringEditor;
		pDlg->m_strTitle = "String object: Concatenation editor";
		break;
	default:
		pDlg = NULL;
	}

	if (pDlg) {
		pDlg->m_pString = pString;
		pDlg->m_strName = strName;
		if (pDlg->DoModal() == IDOK) {
			strName = pDlg->m_strName;
			bOK = true;
		}
		delete pDlg;
	}

	return bOK;
}


sspStringObject* sspGUIString::loadStringEditor(short nType, CString& strName)
{
	sspStringObject* pString = NULL;
	sspGUIStringEditor* pDlg = NULL;

	switch (nType) {
	case SSP_SIMPLE_STRING:
		pDlg = new sspSimpleStringEditor;
		pDlg->m_strTitle = "String object: Simple editor";
		break;
	case SSP_ALPHABET_STRING:
		pDlg = new sspAlphabetEditor;
		pDlg->m_strTitle = "String object: Linear alphabet editor";
		break;
	case SSP_CONCAT_STRING:
		pDlg = new sspConcatStringEditor;
		pDlg->m_strTitle = "String object: Concatenation editor";
		break;
	default:
		pDlg = NULL;
	}

	if (pDlg) {
		pDlg->m_pString = NULL;
		pDlg->m_strName = strName;
		if (pDlg->DoModal() == IDOK) {
			strName = pDlg->m_strName;
			pString = pDlg->m_pString;
		}
		delete pDlg;
	}

	return pString;
}
