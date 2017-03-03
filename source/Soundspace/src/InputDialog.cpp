// InputDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "InputDialog.h"
#include "sspGUIinput.h"
#include "sspInputManager.h"

#include <vector>

// CInputDialog dialog

IMPLEMENT_DYNAMIC(CInputDialog, CDialog)
CInputDialog::CInputDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDialog::IDD, pParent)
	, m_nInputType(0)
{
}

CInputDialog::~CInputDialog()
{
}

void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELECT_INPUT, m_lbInputs);
	DDX_CBIndex(pDX, IDC_SELECT_TYPE, m_nInputType);
}

BEGIN_MESSAGE_MAP(CInputDialog, CDialog)
	ON_LBN_SELCHANGE(IDC_SELECT_INPUT, OnSelectInput)
	ON_CBN_SELCHANGE(IDC_SELECT_TYPE, OnSelectType)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
END_MESSAGE_MAP()


// CInputDialog message handlers

void CInputDialog::OnSelectInput()
{
	m_nInputType = -1;
	if (m_lbInputs.GetSelCount() == 1) {
		int nSelect;
		m_lbInputs.GetSelItems(1,&nSelect);
		sspInput* pInput = sspInputManager::Instance().getInput(nSelect);
		if (pInput) m_nInputType = pInput->getType();
	}
	((CComboBox*) GetDlgItem(IDC_SELECT_TYPE))->SetCurSel(m_nInputType);
	UpdateButtons();
}

void CInputDialog::OnSelectType()
{
	UpdateData(TRUE);
	UpdateButtons();
}

void CInputDialog::OnAdd()
{
	int nSelect = -1;
	
	sspInput* pInput = sspGUIinput::loadInputEditor(m_nInputType);
	if (pInput) {
		sspInputManager::Instance().setInput(pInput);
		nSelect = m_lbInputs.InsertString(nSelect, pInput->getName().c_str());
	}
	m_lbInputs.SetSel(-1, FALSE);
	m_lbInputs.SetSel(nSelect, (nSelect >= 0));
	UpdateButtons();
}

void CInputDialog::OnEdit()
{
	int nSelect;
	if (m_lbInputs.GetSelCount() != 1)
		return;
	m_lbInputs.GetSelItems(1,&nSelect);

	sspInput* pInput = sspInputManager::Instance().getInput(nSelect);
	if (pInput) {
		if (sspGUIinput::loadInputEditor(pInput)) {
			m_lbInputs.DeleteString(nSelect);
			nSelect = m_lbInputs.InsertString(nSelect, pInput->getName().c_str());
		}
	}
	m_lbInputs.SetSel(nSelect, (nSelect >= 0));
	UpdateButtons();
}

void CInputDialog::OnRemove()
{
  int nCount = m_lbInputs.GetSelCount();
  if (nCount < 1) return;

  std::vector<int> selected(nCount);
	nCount = m_lbInputs.GetSelItems(nCount, &selected[0]);
  for (int i=0; i<nCount; ++i) {
    // Assumes ordered list of selection 
    // (the index list will be shorter for each call)
    sspInputManager::Instance().removeInput(selected[i]-i);
	}

  resetInputList();
	UpdateButtons();
}

void CInputDialog::UpdateButtons()
{
	GetDlgItem(IDC_ADD)->EnableWindow(m_nInputType >= 0);
	GetDlgItem(IDC_EDIT)->EnableWindow(m_lbInputs.GetSelCount() == 1);
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_lbInputs.GetSelCount() >= 1);
}

BOOL CInputDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
  resetInputList();
	UpdateButtons();
	return TRUE;
}

void CInputDialog::resetInputList()
{
	m_lbInputs.ResetContent();
	for (unsigned int i=0; i<sspInputManager::Instance().getSize(); i++)
		m_lbInputs.InsertString(-1, sspInputManager::Instance().getName(i).c_str());
}
