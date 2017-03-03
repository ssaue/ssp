// sspGUIStringDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspGUIStringDialog.h"
#include "sspGUIString.h"
#include "sspPools.h"
#include "SSpBasic.h"

// sspGUIStringDialog dialog

IMPLEMENT_DYNAMIC(sspGUIStringDialog, CDialog)

sspGUIStringDialog::sspGUIStringDialog(CWnd* pParent /*=NULL*/)
	: CDialog(sspGUIStringDialog::IDD, pParent)
{
	m_nStringType = -1;
}

void sspGUIStringDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELECT_STRING, m_stringList);
	DDX_CBIndex(pDX, IDC_SELECT_TYPE, m_nStringType);
}


BEGIN_MESSAGE_MAP(sspGUIStringDialog, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_REPLACE, OnReplace)
	ON_CBN_SELCHANGE(IDC_SELECT_TYPE, OnSelectType)
	ON_LBN_SELCHANGE(IDC_SELECT_STRING, OnSelectString)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// sspGUIStringDialog message handlers

BOOL sspGUIStringDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPLACE)->EnableWindow(FALSE);
	m_stringList.ResetContent();
	for (unsigned int i=0; i<sspPool::Instance().strings.GetSize(); i++)
		m_stringList.InsertString(-1, sspPool::Instance().strings.GetName(i));
	return TRUE;
}

void sspGUIStringDialog::OnAdd() 
{
	int nSelect = -1;
	
	CString strName = _T("");
  sspStringObject* pString = sspGUIString::loadStringEditor(m_nStringType, strName);
	if (pString) {
		sspPool::Instance().strings.LoadString(pString, strName);
		nSelect = m_stringList.InsertString(nSelect, strName);
	}
	m_stringList.SetSel(-1, FALSE);
	m_stringList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_stringList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_stringList.GetSelCount() == 1);
}

void sspGUIStringDialog::OnEdit() 
{
	if (m_stringList.GetSelCount() != 1)
		return;
	int nSelect;
	m_stringList.GetSelItems(1,&nSelect);

	CString strName = _T("");
  sspStringObject* pString = sspPool::Instance().strings.getString(nSelect);
	if (pString) {
		strName = sspPool::Instance().strings.GetName(nSelect);
		if (sspGUIString::loadStringEditor(pString, strName)) {
      sspPool::Instance().strings.ExchangeStringName(strName, nSelect);
			m_stringList.DeleteString(nSelect);
			nSelect = m_stringList.InsertString(nSelect, strName);
		}
	}

	m_stringList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_stringList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_stringList.GetSelCount() == 1);
}

void sspGUIStringDialog::OnReplace() 
{
	int nSelect;
	if (m_stringList.GetSelCount() != 1)
		return;
	m_stringList.GetSelItems(1,&nSelect);

	CString strName = _T("");
	sspStringObject* pString = sspGUIString::loadStringEditor(m_nStringType, strName);
	if (pString) {
    sspPool::Instance().strings.ExchangeString(pString, strName, nSelect);
		m_stringList.DeleteString(nSelect);
		nSelect = m_stringList.InsertString(nSelect, strName);
	}

	m_stringList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_stringList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_stringList.GetSelCount() == 1);
}

void sspGUIStringDialog::OnSelectType() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_stringList.GetSelCount() == 1);
}

void sspGUIStringDialog::OnSelectString() 
{
	m_nStringType = -1;
	if (m_stringList.GetSelCount() == 1) {
		int nSelect;
		m_stringList.GetSelItems(1,&nSelect);
    sspStringObject* pString = sspPool::Instance().strings.getString(nSelect);
		if (pString) m_nStringType = pString->getType();
	}
	((CComboBox*) GetDlgItem(IDC_SELECT_TYPE))->SetCurSel(m_nStringType);
	GetDlgItem(IDC_ADD)->EnableWindow(m_nStringType >= 0);
	GetDlgItem(IDC_EDIT)->EnableWindow(m_nStringType >= 0);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_nStringType >= 0);
}

void sspGUIStringDialog::OnOK() 
{
	int nSize = m_stringList.GetSelCount();
	m_nSelection.resize(nSize);
	if (nSize > 0) m_stringList.GetSelItems(nSize,&m_nSelection[0]);
	CDialog::OnOK();
}

