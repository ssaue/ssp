// sspGUITriggerMsgList.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspGUITriggerMsgList.h"
#include "SSpGUIMessage.h"
#include "SSpGUIUtilityEditor.h"
#include "ConditionalDialog.h"
#include "sspPools.h"
#include "SSpConditionals.h"

// sspGUITriggerMsgList dialog

IMPLEMENT_DYNAMIC(sspGUITriggerMsgList, CDialog)
sspGUITriggerMsgList::sspGUITriggerMsgList(CWnd* pParent /*=NULL*/)
	: CDialog(sspGUITriggerMsgList::IDD, pParent)
{
	m_nMsgType = 0;
  m_strModes[0] = CString("Trigger on true");
  m_strModes[1] = CString("Trigger on false");
  m_strModes[2] = CString("Trigger on both");
}

sspGUITriggerMsgList::~sspGUITriggerMsgList()
{
}

void sspGUITriggerMsgList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRIGGERS, m_trigList);
	DDX_Control(pDX, IDC_CONDITIONALS, m_condList);
	DDX_Control(pDX, IDC_MESSAGELIST, m_msgList);
	DDX_CBIndex(pDX, IDC_SELECT_MSG, m_nMsgType);
}


BEGIN_MESSAGE_MAP(sspGUITriggerMsgList, CDialog)
	ON_BN_CLICKED(IDC_ADD_TRIGGER, OnAddTrigger)
	ON_BN_CLICKED(IDC_REMOVE_TRIGGER, OnRemoveTrigger)
	ON_NOTIFY(NM_RCLICK, IDC_TRIGGERS, OnNMDblclkTrigger)
	ON_NOTIFY(NM_CLICK, IDC_TRIGGERS, OnNMClickTrigger)
	ON_BN_CLICKED(IDC_ADD_COND, OnAddCondition)
	ON_BN_CLICKED(IDC_EDIT_COND, OnEditCondition)
	ON_BN_CLICKED(IDC_REMOVE_COND, OnRemoveCondition)
	ON_LBN_SELCHANGE(IDC_CONDITIONALS, OnSelectCondition)
	ON_BN_CLICKED(IDC_ADD_MSG, OnAddMessage)
	ON_BN_CLICKED(IDC_EDIT_MSG, OnEditMessage)
	ON_BN_CLICKED(IDC_REMOVE_MSG, OnRemoveMessage)
	ON_LBN_SELCHANGE(IDC_MESSAGELIST, OnSelectMessage)
	ON_CBN_SELCHANGE(IDC_SELECT_MSG, OnSelectMessageType)
	ON_NOTIFY(NM_DBLCLK, IDC_TRIGGERS, OnNMDblclkTrigger)
END_MESSAGE_MAP()

// sspGUITriggerMsgList message handlers

BOOL sspGUITriggerMsgList::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_ADD_COND)->EnableWindow(TRUE);

  CRect ctrlRect;
	m_trigList.GetClientRect(ctrlRect);
	int nHalfWidth = ctrlRect.Width() / 2;
	m_trigList.InsertColumn(0, _T("Condition"), LVCFMT_LEFT, nHalfWidth, 0);
	m_trigList.InsertColumn(1, _T("Trigger mode"), LVCFMT_LEFT, nHalfWidth, 1);
	m_trigList.SetExtendedStyle(m_trigList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

  updateList();
	return TRUE;
}

void sspGUITriggerMsgList::OnAddTrigger()
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    int nCond = dlg.m_nSelection[0];
    SSpTrigger* pTrig = new SSpTrigger();
    pTrig->setConditional(nCond);
		sspCondMsgList* pNew = new sspCondMsgList;
    m_list.add(pTrig, pNew);
    m_pos = m_list.find(pTrig);
    if (validTrigger()) {
			m_trigList.SetItemState(m_nSelected, 0, LVIS_SELECTED|LVIS_FOCUSED);
      m_nSelected = m_trigList.GetItemCount();
		  m_trigList.InsertItem(LVIF_TEXT|LVIF_STATE, m_nSelected, 
		    sspPool::Instance().conditionals.GetName(nCond),
        0, LVIS_SELECTED, 0, 0);
		  m_trigList.SetItemText(m_nSelected, 1, m_strModes[pTrig->getMode()]);
      m_trigList.SetItemData(m_nSelected, (DWORD_PTR) pTrig);
      updateTrigger();
    }
    else
      delete pNew;
	}
}

void sspGUITriggerMsgList::OnRemoveTrigger() 
{
  if (m_nSelected >= 0 && validTrigger()) {
    m_list.remove((SSpTrigger*) m_trigList.GetItemData(m_nSelected));
		m_trigList.DeleteItem(m_nSelected);
    m_pos = m_list.begin();
    if (validTrigger()) {
      m_nSelected = 0;
      m_trigList.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }
    updateTrigger();
  }
}

void sspGUITriggerMsgList::OnNMClickTrigger(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  int nSelect = pNMLV->iItem;
	if (nSelect == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_trigList.SubItemHitTest(&lvhti);
		nSelect = lvhti.iItem;
	}
  if (nSelect  >= 0) {
    m_trigList.SetItemState(m_nSelected, 0, LVIS_SELECTED | LVIS_FOCUSED);
    m_nSelected = nSelect;
    m_pos = m_list.find((SSpTrigger*) m_trigList.GetItemData(m_nSelected));
    if (validTrigger()) {
	    m_trigList.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }
	  updateTrigger();
  }
	*pResult = 0;
}

void sspGUITriggerMsgList::OnNMDblclkTrigger(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  int nSelect = pNMLV->iItem;
	if (nSelect == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_trigList.SubItemHitTest(&lvhti);
		nSelect = lvhti.iItem;
	}
  if (nSelect  >= 0) {
    m_trigList.SetItemState(m_nSelected, 0, LVIS_SELECTED | LVIS_FOCUSED);
    m_nSelected = nSelect;
    SSpTrigger* pTrig = (SSpTrigger*) m_trigList.GetItemData(m_nSelected);
    m_pos = m_list.find(pTrig);
    if (validTrigger()) {
	    if (pNMLV->iSubItem == 0) {
		    CConditionalDialog dlg;
		    if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
          int nCond = dlg.m_nSelection[0];
          pTrig->setConditional(nCond);
			    m_trigList.SetItemText(m_nSelected, 0, sspPool::Instance().conditionals.GetName(nCond));
		    }
	    }
	    else if (pNMLV->iSubItem == 1) {
        CRect rItem, rWnd;
        m_trigList.GetWindowRect(rWnd);
        m_trigList.GetSubItemRect(m_nSelected, pNMLV->iSubItem, LVIR_BOUNDS, rItem);
        rItem += rWnd.TopLeft();
        CEditCombo dlg(rItem);
        for (int i=0; i<3; ++i)
          dlg.addChoice(i, m_strModes[i]);
		    dlg.m_nChoice = pTrig->getMode();
		    if (dlg.DoModal() == IDOK) {
          pTrig->setMode(dlg.m_nChoice);
      		m_trigList.SetItemText(m_nSelected, 1, m_strModes[dlg.m_nChoice]);
		    }
      }
	    m_trigList.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }
	  updateTrigger();
  }
	*pResult = 0;
}

void sspGUITriggerMsgList::OnAddCondition()
{
	CConditionalDialog dlg;
	if (validTrigger() && dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    int nCond = dlg.m_nSelection[0];
		sspMsgList* pNew = new sspMsgList;
    m_pos->second->add(nCond, pNew);
    m_posCond = m_pos->second->find(nCond);
    if (validCondition()) {
		  int nSelect = m_condList.InsertString(-1, sspPool::Instance().conditionals.GetName(nCond));
      m_condList.SetItemData(nSelect, nCond);
		  m_condList.SetCurSel(nSelect);
      updateConditional();
    }
    else
      delete pNew;
	}
}

void sspGUITriggerMsgList::OnEditCondition() 
{
  int nSelect = m_condList.GetCurSel();
  m_posCond = m_pos->second->find(m_condList.GetItemData(nSelect));

  CConditionalDialog dlg;
	if (validCondition() && dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    int nCond = dlg.m_nSelection[0];
		sspMsgList* pNew = new sspMsgList(*m_posCond->second);
    m_pos->second->remove(m_condList.GetItemData(nSelect));
    m_condList.DeleteString(nSelect);
    m_pos->second->add(nCond, pNew);
    m_posCond = m_pos->second->find(nCond);
    if (validCondition()) {
		  m_condList.InsertString(nSelect, sspPool::Instance().conditionals.GetName(nCond));
      m_condList.SetItemData(nSelect, nCond);
		  m_condList.SetCurSel(nSelect);
      updateConditional();
    }
    else
      delete pNew;
	}
}

void sspGUITriggerMsgList::OnRemoveCondition() 
{
  int nSelect = m_condList.GetCurSel();
  if (nSelect >= 0 && validCondition()) {
    m_pos->second->remove(m_condList.GetItemData(nSelect));
    m_posCond = m_pos->second->begin();
		m_condList.DeleteString(nSelect);
    if (validCondition()) m_condList.SetCurSel(0);
    updateConditional();
  }
}

void sspGUITriggerMsgList::OnSelectCondition() 
{
  int nSelect = m_condList.GetCurSel();
  if (nSelect >= 0 && validTrigger()) {
    m_posCond = m_pos->second->find(m_condList.GetItemData(nSelect));
    updateConditional();
  }
}

void sspGUITriggerMsgList::OnAddMessage() 
{
  if (validCondition() && m_nMsgType >= 0) {
	  SSpStreamMsg* pNew = new SSpStreamMsg;
	  SSpGUIMessage::FillMessageData(pNew, m_nMsgType);
	  int nSelect = -1;
	  if (SSpGUIMessage::LoadMessageEditor(pNew)) {
      m_posCond->second->add(pNew);
		  nSelect = m_msgList.InsertString(nSelect, SSpGUIMessage::GetMessageText(pNew));
      m_msgList.SetItemDataPtr(nSelect, pNew);
		  m_msgList.SetCurSel(nSelect);
	  }
    if (nSelect < 0) delete pNew;
    updateMessage(nSelect >= 0);
  }
}

void sspGUITriggerMsgList::OnEditMessage() 
{
  int nSelect = m_msgList.GetCurSel();
  if (nSelect >= 0) {
    SSpStreamMsg* pMsg = (SSpStreamMsg*) m_msgList.GetItemDataPtr(nSelect);
	  if (pMsg != NULL && SSpGUIMessage::LoadMessageEditor(pMsg)) {
		  m_msgList.DeleteString(nSelect);
		  nSelect = m_msgList.InsertString(nSelect, SSpGUIMessage::GetMessageText(pMsg));
		  m_msgList.SetCurSel(nSelect);
	  }
  }
}

void sspGUITriggerMsgList::OnRemoveMessage() 
{
  int nSelect = m_msgList.GetCurSel();
  if (nSelect >= 0 && validCondition()) {
    m_posCond->second->remove((SSpStreamMsg*) m_msgList.GetItemDataPtr(nSelect));
		m_msgList.DeleteString(nSelect);
    bool bValidMsg = !m_posCond->second->empty();
    if (bValidMsg) m_msgList.SetCurSel(0);
    updateMessage(bValidMsg);
  }
}

void sspGUITriggerMsgList::OnSelectMessage() 
{
  updateMessage(m_msgList.GetCurSel() >= 0 && validCondition());
}

void sspGUITriggerMsgList::OnSelectMessageType() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ADD_MSG)->EnableWindow(TRUE);
}

void sspGUITriggerMsgList::updateList()
{
  unsigned int i;
	m_trigList.DeleteAllItems();
  sspTriggerMsgList::iterator iter;
  for (iter=m_list.begin(), i=0; iter != m_list.end(); ++iter, ++i) {
		m_trigList.InsertItem(LVIF_TEXT|LVIF_STATE, i, 
		   sspPool::Instance().conditionals.GetName((*iter).first->getConditional()),
       0, LVIS_SELECTED, 0, 0);
		m_trigList.SetItemText(i, 1, m_strModes[(*iter).first->getMode()]);
    m_trigList.SetItemData(i, (DWORD_PTR)(*iter).first);
  }

  // Default select first conditional (and show corresponding messages)
  m_pos = m_list.begin();
  if (validTrigger()) {
    m_nSelected = 0;
    m_trigList.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
  }
  updateTrigger();
}

void sspGUITriggerMsgList::updateTrigger()
{
	m_condList.ResetContent();
  if (validTrigger()) {
    sspCondMsgList::iterator iter;
    for (iter=m_pos->second->begin(); iter != m_pos->second->end(); ++iter) {
      int nIndex = m_condList.InsertString(-1, sspPool::Instance().conditionals.GetName((*iter).first));
      m_condList.SetItemData(nIndex, (*iter).first);
    }
    // Default select first conditional (and show corresponding messages)
    m_posCond = m_pos->second->begin();
    if (validCondition()) m_condList.SetCurSel(0);
  }
  updateConditional();
}

void sspGUITriggerMsgList::updateConditional()
{
  bool bValidMsg = false;
	GetDlgItem(IDC_ADD_MSG)->EnableWindow(validCondition());
	GetDlgItem(IDC_SELECT_MSG)->EnableWindow(validCondition());
 	GetDlgItem(IDC_REMOVE_COND)->EnableWindow(validCondition());
	m_msgList.ResetContent();
  if (validCondition()) {
    sspMsgList::iterator iter;
    for (iter=m_posCond->second->begin(); iter != m_posCond->second->end(); ++iter) {
		  int nIndex = m_msgList.InsertString(-1, SSpGUIMessage::GetMessageText((*iter)));
      m_msgList.SetItemDataPtr(nIndex, (*iter));
	  }
    bValidMsg = !m_posCond->second->empty();
    if (bValidMsg) m_msgList.SetCurSel(0);
  }
  updateMessage(bValidMsg);
}

void sspGUITriggerMsgList::updateMessage(bool bValid)
{
 	GetDlgItem(IDC_EDIT_MSG)->EnableWindow(bValid);
 	GetDlgItem(IDC_REMOVE_MSG)->EnableWindow(bValid);
}

bool sspGUITriggerMsgList::validTrigger()
{
  return (m_pos != m_list.end());
}

bool sspGUITriggerMsgList::validCondition()
{
  return validTrigger() && m_posCond != m_pos->second->end();
}