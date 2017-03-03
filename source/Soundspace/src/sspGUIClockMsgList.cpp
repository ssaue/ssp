// sspGUIClockMsgList.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspGUIClockMsgList.h"
#include "SSpGUIMessage.h"
#include "SSpGUIUtilityEditor.h"
#include "ConditionalDialog.h"
#include "sspPools.h"

// sspGUIClockMsgList dialog

IMPLEMENT_DYNAMIC(sspGUIClockMsgList, CDialog)
sspGUIClockMsgList::sspGUIClockMsgList(CWnd* pParent /*=NULL*/)
	: CDialog(sspGUIClockMsgList::IDD, pParent)
{
	m_nMsgType = 0;
}

sspGUIClockMsgList::~sspGUIClockMsgList()
{
}

void sspGUIClockMsgList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLOCKS, m_clockList);
	DDX_Control(pDX, IDC_CONDITIONALS, m_condList);
	DDX_Control(pDX, IDC_MESSAGELIST, m_msgList);
	DDX_CBIndex(pDX, IDC_SELECT_MSG, m_nMsgType);
}


BEGIN_MESSAGE_MAP(sspGUIClockMsgList, CDialog)
	ON_BN_CLICKED(IDC_ADD_CLOCK, OnAddClock)
	ON_BN_CLICKED(IDC_EDIT_CLOCK, OnEditClock)
	ON_BN_CLICKED(IDC_REMOVE_CLOCK, OnRemoveClock)
	ON_LBN_SELCHANGE(IDC_CLOCKS, OnSelectClock)
	ON_BN_CLICKED(IDC_ADD_COND, OnAddCondition)
	ON_BN_CLICKED(IDC_EDIT_COND, OnEditCondition)
	ON_BN_CLICKED(IDC_REMOVE_COND, OnRemoveCondition)
	ON_LBN_SELCHANGE(IDC_CONDITIONALS, OnSelectCondition)
	ON_BN_CLICKED(IDC_ADD_MSG, OnAddMessage)
	ON_BN_CLICKED(IDC_EDIT_MSG, OnEditMessage)
	ON_BN_CLICKED(IDC_REMOVE_MSG, OnRemoveMessage)
	ON_LBN_SELCHANGE(IDC_MESSAGELIST, OnSelectMessage)
	ON_CBN_SELCHANGE(IDC_SELECT_MSG, OnSelectMessageType)
END_MESSAGE_MAP()

// sspGUIClockMsgList message handlers

BOOL sspGUIClockMsgList::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_ADD_COND)->EnableWindow(TRUE);
  updateList();
	return TRUE;
}

void sspGUIClockMsgList::OnAddClock()
{
  CRect rItem;
  m_clockList.GetWindowRect(&rItem);
	CEditClock dlg(rItem);
	if (dlg.DoModal() == IDOK) {
    SSpClock newClock(dlg.m_time.GetHour(), dlg.m_time.GetMinute(), dlg.m_time.GetSecond());
		sspCondMsgList* pNew = new sspCondMsgList;
    m_list.add(newClock, pNew);
    m_pos = m_list.find(newClock);
    if (validClock()) {
      wchar_t buffer[20];
      int nSelect = m_clockList.InsertString(-1, newClock.clockAsString(buffer, 19));
      m_clockList.SetItemData(nSelect, newClock.secondsInDay());
		  m_clockList.SetCurSel(nSelect);
      updateClock();
    }
    else
      delete pNew;
	}
}

void sspGUIClockMsgList::OnEditClock() 
{
  int nSelect = m_clockList.GetCurSel();
  SSpClock cl(m_clockList.GetItemData(nSelect));
  m_pos = m_list.find(cl);
  if (!validClock()) return;

  CRect rItem, rWnd;
  m_clockList.GetWindowRect(rWnd);
  m_clockList.GetItemRect(nSelect, &rItem);
  rItem += rWnd.TopLeft();
	CEditClock dlg(rItem);
  dlg.m_time = CTime(2000,1,1, cl.hour(), cl.min(), cl.sec(), 0);
	if (dlg.DoModal() == IDOK) {
    SSpClock newClock(dlg.m_time.GetHour(), dlg.m_time.GetMinute(), dlg.m_time.GetSecond());
		sspCondMsgList* pNew = new sspCondMsgList(*m_pos->second);
    m_list.remove(SSpClock(m_clockList.GetItemData(nSelect)));
    m_clockList.DeleteString(nSelect);
    m_list.add(newClock, pNew);
    m_pos = m_list.find(newClock);
    if (validClock()) {
			wchar_t buffer[20];
		  m_clockList.InsertString(nSelect, newClock.clockAsString(buffer, 19));
      m_clockList.SetItemData(nSelect, newClock.secondsInDay());
		  m_clockList.SetCurSel(nSelect);
      updateClock();
    }
    else
      delete pNew;
	}
}

void sspGUIClockMsgList::OnRemoveClock() 
{
  int nSelect = m_clockList.GetCurSel();
  if (nSelect >= 0 && validClock()) {
    m_list.remove(SSpClock(m_clockList.GetItemData(nSelect)));
    m_pos = m_list.begin();
		m_clockList.DeleteString(nSelect);
    if (validClock()) m_clockList.SetCurSel(0);
    updateClock();
  }
}

void sspGUIClockMsgList::OnSelectClock() 
{
  int nSelect = m_clockList.GetCurSel();
  if (nSelect >= 0) {
    m_pos = m_list.find(SSpClock(m_clockList.GetItemData(nSelect)));
    updateClock();
  }
}

void sspGUIClockMsgList::OnAddCondition()
{
	CConditionalDialog dlg;
	if (validClock() && dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
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

void sspGUIClockMsgList::OnEditCondition() 
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

void sspGUIClockMsgList::OnRemoveCondition() 
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

void sspGUIClockMsgList::OnSelectCondition() 
{
  int nSelect = m_condList.GetCurSel();
  if (nSelect >= 0 && validClock()) {
    m_posCond = m_pos->second->find(m_condList.GetItemData(nSelect));
    updateConditional();
  }
}

void sspGUIClockMsgList::OnAddMessage() 
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
    updateMessage(nSelect < 0);
  }
}

void sspGUIClockMsgList::OnEditMessage() 
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

void sspGUIClockMsgList::OnRemoveMessage() 
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

void sspGUIClockMsgList::OnSelectMessage() 
{
  updateMessage(m_msgList.GetCurSel() >= 0 && validCondition());
}

void sspGUIClockMsgList::OnSelectMessageType() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ADD_MSG)->EnableWindow(TRUE);
}

void sspGUIClockMsgList::updateList()
{
	wchar_t buffer[20];
	m_clockList.ResetContent();
  sspClockMsgList::iterator iter;
  for (iter=m_list.begin(); iter != m_list.end(); ++iter) {
    int nIndex = m_clockList.InsertString(-1, (*iter).first.clockAsString(buffer, 19));
    m_clockList.SetItemData(nIndex, (*iter).first.secondsInDay());
  }
  // Default select first conditional (and show corresponding messages)
  m_pos = m_list.begin();
  if (validClock()) m_clockList.SetCurSel(0);
  updateClock();
}

void sspGUIClockMsgList::updateClock()
{
	m_condList.ResetContent();
  if (validClock()) {
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

void sspGUIClockMsgList::updateConditional()
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

void sspGUIClockMsgList::updateMessage(bool bValid)
{
 	GetDlgItem(IDC_EDIT_MSG)->EnableWindow(bValid);
 	GetDlgItem(IDC_REMOVE_MSG)->EnableWindow(bValid);
}

bool sspGUIClockMsgList::validClock()
{
  return (m_pos != m_list.end());
}

bool sspGUIClockMsgList::validCondition()
{
  return validClock() && m_posCond != m_pos->second->end();
}