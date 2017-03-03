// ValueDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "ValueDialog.h"
#include "SSpGUIValueEditor.h"
#include "sspPools.h"
#include "SSpValues.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CValueDialog dialog


CValueDialog::CValueDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CValueDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueDialog)
	m_nValueType = -1;
	//}}AFX_DATA_INIT
}


void CValueDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueDialog)
	DDX_Control(pDX, IDC_SELECT_VALUE, m_valueList);
	DDX_CBIndex(pDX, IDC_SELECT_TYPE, m_nValueType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueDialog, CDialog)
	//{{AFX_MSG_MAP(CValueDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_REPLACE, OnReplace)
	ON_CBN_SELCHANGE(IDC_SELECT_TYPE, OnSelectType)
	ON_LBN_SELCHANGE(IDC_SELECT_VALUE, OnSelectValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CValueDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPLACE)->EnableWindow(FALSE);
	m_valueList.ResetContent();
	for (unsigned int i=0; i<sspPool::Instance().values.GetSize(); i++)
		m_valueList.InsertString(-1, sspPool::Instance().values.GetName(i));
	return TRUE;
}

void CValueDialog::OnAdd() 
{
	int nSelect = -1;
	
	CString strName = _T("");
	sspValue* pVal = SSpGUIValue::loadValueEditor(m_nValueType, strName);
	if (pVal) {
		sspPool::Instance().values.LoadValue(pVal, strName);
		nSelect = m_valueList.InsertString(nSelect, strName);
	}

	m_valueList.SetSel(-1, FALSE);
	m_valueList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_valueList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_valueList.GetSelCount() == 1);
}

void CValueDialog::OnEdit() 
{
	int nSelect;
	if (m_valueList.GetSelCount() != 1)
		return;
	m_valueList.GetSelItems(1,&nSelect);

	CString strName = _T("");
	sspValue* pVal = sspPool::Instance().values.getValue(nSelect);
	if (pVal) {
		strName = sspPool::Instance().values.GetName(nSelect);
		if (SSpGUIValue::loadValueEditor(pVal, strName)) {
			sspPool::Instance().values.ExchangeValueName(strName, nSelect);
			m_valueList.DeleteString(nSelect);
			nSelect = m_valueList.InsertString(nSelect, strName);
		}
	}

	m_valueList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_valueList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_valueList.GetSelCount() == 1);
}

void CValueDialog::OnReplace() 
{
	int nSelect;
	if (m_valueList.GetSelCount() != 1)
		return;
	m_valueList.GetSelItems(1,&nSelect);

	CString strName = _T("");
	sspValue* pVal = SSpGUIValue::loadValueEditor(m_nValueType, strName);
	if (pVal) {
		sspPool::Instance().values.ExchangeValue(pVal, strName, nSelect);
		m_valueList.DeleteString(nSelect);
		nSelect = m_valueList.InsertString(nSelect, strName);
	}

	m_valueList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_valueList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_valueList.GetSelCount() == 1);
}

void CValueDialog::OnSelectType() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_valueList.GetSelCount() == 1);
}

void CValueDialog::OnSelectValue() 
{
	m_nValueType = -1;
	if (m_valueList.GetSelCount() == 1) {
		int nSelect;
		m_valueList.GetSelItems(1,&nSelect);
		sspValue* pVal = sspPool::Instance().values.getValue(nSelect);
		if (pVal) m_nValueType = pVal->getType();
	}
	((CComboBox*) GetDlgItem(IDC_SELECT_TYPE))->SetCurSel(m_nValueType);
	GetDlgItem(IDC_ADD)->EnableWindow(m_nValueType >= 0);
	GetDlgItem(IDC_EDIT)->EnableWindow(m_nValueType >= 0);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_nValueType >= 0);
}

void CValueDialog::OnOK() 
{
	int nSize = m_valueList.GetSelCount();
	m_nSelection.resize(nSize);
  if (nSize > 0) m_valueList.GetSelItems(nSize,&m_nSelection[0]);
	CDialog::OnOK();
}

