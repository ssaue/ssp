// C:\Programvare\Soundspace\SSpGUIMsgList.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspGUIMsgList.h"
#include "SSpGUIMessage.h"
#include "ConditionalDialog.h"
#include "sspPools.h"

// sspGUIMsgList dialog

IMPLEMENT_DYNAMIC(sspGUIMsgList, CDialog)
sspGUIMsgList::sspGUIMsgList(CWnd* pParent /*=NULL*/)
	: CDialog(sspGUIMsgList::IDD, pParent)
{
	m_nMsgType = 0;
}

sspGUIMsgList::~sspGUIMsgList()
{
}

void sspGUIMsgList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONDITIONALS, m_condList);
	DDX_Control(pDX, IDC_MESSAGELIST, m_msgList);
	DDX_CBIndex(pDX, IDC_SELECT_MSG, m_nMsgType);
}

BEGIN_MESSAGE_MAP(sspGUIMsgList, CDialog)
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

// sspGUIMsgList message handlers

BOOL sspGUIMsgList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_ADD_COND)->EnableWindow(TRUE);
  updateList();
	return TRUE;
}

void sspGUIMsgList::OnAddCondition() 
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    int nCond = dlg.m_nSelection[0];
		sspMsgList* pNew = new sspMsgList;
    m_list.add(nCond, pNew);
    m_pos = m_list.find(nCond);
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

void sspGUIMsgList::OnEditCondition() 
{
  int nSelect = m_condList.GetCurSel();
  m_pos = m_list.find(m_condList.GetItemData(nSelect));

  CConditionalDialog dlg;
	if (validCondition() && dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    int nCond = dlg.m_nSelection[0];
		sspMsgList* pNew = new sspMsgList(*m_pos->second);
    m_list.remove(m_condList.GetItemData(nSelect));
    m_condList.DeleteString(nSelect);
    m_list.add(nCond, pNew);
    m_pos = m_list.find(nCond);
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

void sspGUIMsgList::OnRemoveCondition() 
{
  int nSelect = m_condList.GetCurSel();
  if (nSelect >= 0 && validCondition()) {
    m_list.remove(m_condList.GetItemData(nSelect));
    m_pos = m_list.begin();
		m_condList.DeleteString(nSelect);
    if (validCondition()) m_condList.SetCurSel(0);
    updateConditional();
  }
}

void sspGUIMsgList::OnSelectCondition() 
{
  int nSelect = m_condList.GetCurSel();
  if (nSelect >= 0) {
    m_pos = m_list.find(m_condList.GetItemData(nSelect));
    updateConditional();
  }
}

void sspGUIMsgList::OnAddMessage() 
{
  if (validCondition() && m_nMsgType >= 0) {
	  SSpStreamMsg* pNew = new SSpStreamMsg;
	  SSpGUIMessage::FillMessageData(pNew, m_nMsgType);
	  int nSelect = -1;
	  if (SSpGUIMessage::LoadMessageEditor(pNew)) {
      m_pos->second->add(pNew);
		  nSelect = m_msgList.InsertString(nSelect, SSpGUIMessage::GetMessageText(pNew));
      m_msgList.SetItemDataPtr(nSelect, pNew);
		  m_msgList.SetCurSel(nSelect);
	  }
    if (nSelect < 0) delete pNew;
    updateMessage(nSelect < 0);
  }
}

void sspGUIMsgList::OnEditMessage() 
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

void sspGUIMsgList::OnRemoveMessage() 
{
  int nSelect = m_msgList.GetCurSel();
  if (nSelect >= 0 && validCondition()) {
    m_pos->second->remove((SSpStreamMsg*) m_msgList.GetItemDataPtr(nSelect));
		m_msgList.DeleteString(nSelect);
    bool bValidMsg = !m_pos->second->empty();
    if (bValidMsg) m_msgList.SetCurSel(0);
    updateMessage(bValidMsg);
  }
}

void sspGUIMsgList::OnSelectMessage() 
{
  updateMessage(m_msgList.GetCurSel() >= 0 && validCondition());
}

void sspGUIMsgList::OnSelectMessageType() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ADD_MSG)->EnableWindow(TRUE);
}

void sspGUIMsgList::updateList()
{
	m_condList.ResetContent();
  sspCondMsgList::iterator iter;
  for (iter=m_list.begin(); iter != m_list.end(); ++iter) {
    int nIndex = m_condList.InsertString(-1, sspPool::Instance().conditionals.GetName((*iter).first));
    m_condList.SetItemData(nIndex, (*iter).first);
  }
  // Default select first conditional (and show corresponding messages)
  m_pos = m_list.begin();
  if (validCondition()) m_condList.SetCurSel(0);
  updateConditional();
}

void sspGUIMsgList::updateConditional()
{
  bool bValidMsg = false;
	GetDlgItem(IDC_ADD_MSG)->EnableWindow(validCondition());
	GetDlgItem(IDC_SELECT_MSG)->EnableWindow(validCondition());
 	GetDlgItem(IDC_REMOVE_COND)->EnableWindow(validCondition());
	m_msgList.ResetContent();
  if (validCondition()) {
    sspMsgList::iterator iter;
    for (iter=m_pos->second->begin(); iter != m_pos->second->end(); ++iter) {
		  int nIndex = m_msgList.InsertString(-1, SSpGUIMessage::GetMessageText((*iter)));
      m_msgList.SetItemDataPtr(nIndex, (*iter));
	  }
    bValidMsg = !m_pos->second->empty();
    if (bValidMsg) m_msgList.SetCurSel(0);
  }
  updateMessage(bValidMsg);
}

void sspGUIMsgList::updateMessage(bool bValid)
{
 	GetDlgItem(IDC_EDIT_MSG)->EnableWindow(bValid);
 	GetDlgItem(IDC_REMOVE_MSG)->EnableWindow(bValid);
}

bool sspGUIMsgList::validCondition()
{
  return (m_pos != m_list.end());
}
