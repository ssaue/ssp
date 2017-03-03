// ConditionalDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "ConditionalDialog.h"
#include "SSpGUIConditional.h"
#include "sspPools.h"
#include "SSpBasic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConditionalDialog dialog


CConditionalDialog::CConditionalDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConditionalDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConditionalDialog)
	m_nConditionalType = -1;
	//}}AFX_DATA_INIT
}


void CConditionalDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConditionalDialog)
	DDX_Control(pDX, IDC_SELECT_CONDITIONAL, m_condList);
	DDX_CBIndex(pDX, IDC_SELECT_TYPE, m_nConditionalType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConditionalDialog, CDialog)
	//{{AFX_MSG_MAP(CConditionalDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_REPLACE, OnReplace)
	ON_CBN_SELCHANGE(IDC_SELECT_TYPE, OnSelectType)
	ON_LBN_SELCHANGE(IDC_SELECT_CONDITIONAL, OnSelectConditional)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConditionalDialog message handlers

BOOL CConditionalDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPLACE)->EnableWindow(FALSE);
	m_condList.ResetContent();
	for (unsigned int i=0; i<sspPool::Instance().conditionals.GetSize(); i++)
		m_condList.InsertString(-1, sspPool::Instance().conditionals.GetName(i));
	return TRUE;
}

void CConditionalDialog::OnAdd() 
{
	int nSelect = -1;
	
	CString strName = _T("");
	sspConditional* pCond = SSpGUIConditional::loadConditionalEditor(m_nConditionalType, strName);
	if (pCond) {
		sspPool::Instance().conditionals.LoadConditional(pCond, strName);
		nSelect = m_condList.InsertString(nSelect, strName);
	}
	m_condList.SetSel(-1, FALSE);
	m_condList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_condList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_condList.GetSelCount() == 1);
}

void CConditionalDialog::OnEdit() 
{
	if (m_condList.GetSelCount() != 1)
		return;
	int nSelect;
	m_condList.GetSelItems(1,&nSelect);

	CString strName = _T("");
	sspConditional* pCond = sspPool::Instance().conditionals.GetConditional(nSelect);
	if (pCond) {
		strName = sspPool::Instance().conditionals.GetName(nSelect);
		if (SSpGUIConditional::loadConditionalEditor(pCond, strName)) {
			sspPool::Instance().conditionals.ExchangeConditionalName(strName, nSelect);
			m_condList.DeleteString(nSelect);
			nSelect = m_condList.InsertString(nSelect, strName);
		}
	}

	m_condList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_condList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_condList.GetSelCount() == 1);
}

void CConditionalDialog::OnReplace() 
{
	int nSelect;
	if (m_condList.GetSelCount() != 1)
		return;
	m_condList.GetSelItems(1,&nSelect);

	CString strName = _T("");
	sspConditional* pCond = SSpGUIConditional::loadConditionalEditor(m_nConditionalType, strName);
	if (pCond) {
		sspPool::Instance().conditionals.ExchangeConditional(pCond, strName, nSelect);
		m_condList.DeleteString(nSelect);
		nSelect = m_condList.InsertString(nSelect, strName);
	}

	m_condList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_condList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_condList.GetSelCount() == 1);
}

void CConditionalDialog::OnSelectType() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_condList.GetSelCount() == 1);
}

void CConditionalDialog::OnSelectConditional() 
{
	m_nConditionalType = -1;
	if (m_condList.GetSelCount() == 1) {
		int nSelect;
		m_condList.GetSelItems(1,&nSelect);
		sspConditional* pCond = sspPool::Instance().conditionals.GetConditional(nSelect);
		if (pCond) m_nConditionalType = pCond->getType();
	}
	((CComboBox*) GetDlgItem(IDC_SELECT_TYPE))->SetCurSel(m_nConditionalType);
	GetDlgItem(IDC_ADD)->EnableWindow(m_nConditionalType >= 0);
	GetDlgItem(IDC_EDIT)->EnableWindow(m_nConditionalType >= 0);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_nConditionalType >= 0);
}

void CConditionalDialog::OnOK() 
{
	int nSize = m_condList.GetSelCount();
	m_nSelection.resize(nSize);
  if (nSize > 0) m_condList.GetSelItems(nSize,&m_nSelection[0]);
	CDialog::OnOK();
}

