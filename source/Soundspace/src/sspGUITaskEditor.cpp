// sspGUITaskEditor.cpp : implementation file
//

#include "stdafx.h"
#include "soundspace.h"
#include "sspGUITaskEditor.h"

#include "sspPools.h"
#include "ValueDialog.h"
#include "PlayObjectDialog.h"
#include "ConditionalDialog.h"
#include "SSpGUIMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// sspGUITaskEditor dialog


sspGUITaskEditor::sspGUITaskEditor(CWnd* pParent /*=NULL*/)
	: CDialog(sspGUITaskEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(sspGUITaskEditor)
	m_strName = _T("");
	m_nMsgType = -1;
	//}}AFX_DATA_INIT
}


void sspGUITaskEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(sspGUITaskEditor)
	DDX_Control(pDX, IDC_MESSAGELIST, m_msgList);
	DDX_Control(pDX, IDC_CONDITIONS, m_condList);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_CBIndex(pDX, IDC_SELECT_MSG, m_nMsgType);
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_PRIORITY, m_task.priority());
}


BEGIN_MESSAGE_MAP(sspGUITaskEditor, CDialog)
	//{{AFX_MSG_MAP(sspGUITaskEditor)
	ON_BN_CLICKED(IDC_SELECT_OBJECT, OnSelectObject)
	ON_BN_CLICKED(IDC_SELECT_FACTOR, OnSelectFactor)
	ON_BN_CLICKED(IDC_SELECT_CONDITION, OnSelectCondition)

	ON_BN_CLICKED(IDC_ADD_COND, OnAddCondition)
 	ON_BN_CLICKED(IDC_EDIT_COND, OnEditCondition)
 	ON_BN_CLICKED(IDC_REMOVE_COND, OnRemoveCondition)
	ON_LBN_SELCHANGE(IDC_CONDITIONS, OnSelectMessageCondition)

  ON_BN_CLICKED(IDC_ADD_MSG, OnAddMessage)
	ON_BN_CLICKED(IDC_EDIT_MSG, OnEditMessage)
	ON_BN_CLICKED(IDC_REMOVE_MSG, OnRemoveMessage)
	ON_LBN_SELCHANGE(IDC_MESSAGELIST, OnSelectMessage)
	ON_CBN_SELCHANGE(IDC_SELECT_MSG, OnSelectMessageType)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_EVENT_SELECT1, IDC_EVENT_SELECT4, OnSelectEvent)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// sspGUITaskEditor message handlers

BOOL sspGUITaskEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (m_task.getObjectIndex() >= 0)
		GetDlgItem(IDC_SELECT_OBJECT)->SetWindowText(sspPool::Instance().objects.GetName(m_task.getObjectIndex()));
	if (m_task.getVolumeFactor() >= 0)
		GetDlgItem(IDC_SELECT_FACTOR)->SetWindowText(sspPool::Instance().values.GetName(m_task.getVolumeFactor()));
	if (m_task.getPlayCondition() >= 0)
		GetDlgItem(IDC_SELECT_CONDITION)->SetWindowText(sspPool::Instance().conditionals.GetName(m_task.getPlayCondition()));
  CButton* pBtn = (CButton*) GetDlgItem(IDC_EVENT_SELECT1);
  pBtn->SetCheck(BST_CHECKED);
  m_pList = m_task.getMsgList(0);
  updateList();
	return TRUE;
}

void sspGUITaskEditor::OnSelectObject() 
{
	CPlayObjectDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_task.setObjectIndex(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_OBJECT)->SetWindowText(sspPool::Instance().objects.GetName(m_task.getObjectIndex()));
	}
}

void sspGUITaskEditor::OnSelectFactor() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_task.setVolumeFactor(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_FACTOR)->SetWindowText(sspPool::Instance().values.GetName(m_task.getVolumeFactor()));
	}
}

void sspGUITaskEditor::OnSelectCondition() 
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_task.setPlayCondition(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_CONDITION)->SetWindowText(sspPool::Instance().conditionals.GetName(m_task.getPlayCondition()));
	}
}

void sspGUITaskEditor::OnSelectEvent(unsigned int nID) 
{
  int nEvent = nID - IDC_EVENT_SELECT1;
	bool bSelectOK = (nEvent >= 0);
	GetDlgItem(IDC_ADD_COND)->EnableWindow(bSelectOK);
	if (bSelectOK) {
    m_pList = m_task.getMsgList(nEvent);
    updateList();
  }
}

void sspGUITaskEditor::OnAddCondition() 
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    int nCond = dlg.m_nSelection[0];
		sspMsgList* pNew = new sspMsgList;
    m_pList->add(nCond, pNew);
    m_pos = m_pList->find(nCond);
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

void sspGUITaskEditor::OnEditCondition() 
{
  int nSelect = m_condList.GetCurSel();
  m_pos = m_pList->find(m_condList.GetItemData(nSelect));

  CConditionalDialog dlg;
	if (validCondition() && dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    int nCond = dlg.m_nSelection[0];
		sspMsgList* pNew = new sspMsgList(*m_pos->second);
    m_pList->remove(m_condList.GetItemData(nSelect));
    m_condList.DeleteString(nSelect);
    m_pList->add(nCond, pNew);
    m_pos = m_pList->find(nCond);
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

void sspGUITaskEditor::OnRemoveCondition() 
{
  int nSelect = m_condList.GetCurSel();
  if (nSelect >= 0 && validCondition()) {
    m_pList->remove(m_condList.GetItemData(nSelect));
    m_pos = m_pList->begin();
		m_condList.DeleteString(nSelect);
    if (validCondition()) m_condList.SetCurSel(0);
    updateConditional();
  }
}

void sspGUITaskEditor::OnSelectMessageCondition() 
{
  int nSelect = m_condList.GetCurSel();
  if (nSelect >= 0) {
    m_pos = m_pList->find(m_condList.GetItemData(nSelect));
    updateConditional();
  }
}

void sspGUITaskEditor::OnAddMessage() 
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

void sspGUITaskEditor::OnEditMessage() 
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

void sspGUITaskEditor::OnRemoveMessage() 
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

void sspGUITaskEditor::OnSelectMessage() 
{
  updateMessage(m_msgList.GetCurSel() >= 0 && validCondition());
}

void sspGUITaskEditor::OnSelectMessageType() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ADD_MSG)->EnableWindow(TRUE);
}

void sspGUITaskEditor::OnOK() 
{
	if (m_task.getObjectIndex() < 0 || m_task.getObjectIndex() > (int) sspPool::Instance().objects.GetSize()) {
		AfxMessageBox(L"Please select a play object.");
		return;
	}
	if (m_task.getVolumeFactor() < 0 || m_task.getVolumeFactor() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Please select a volume factor.");
		return;
	}
	if (m_task.getPlayCondition() < 0 || m_task.getPlayCondition() > (int) sspPool::Instance().conditionals.GetSize()) {
		AfxMessageBox(L"Please select a play condition.");
		return;
	}
	CDialog::OnOK();
}

void sspGUITaskEditor::updateList()
{
	m_condList.ResetContent();
  sspCondMsgList::iterator iter;
  for (iter=m_pList->begin(); iter != m_pList->end(); ++iter) {
    int nIndex = m_condList.InsertString(-1, sspPool::Instance().conditionals.GetName((*iter).first));
    m_condList.SetItemData(nIndex, (*iter).first);
  }
  // Default select first conditional (and show corresponding messages)
  m_pos = m_pList->begin();
  if (validCondition()) m_condList.SetCurSel(0);
  updateConditional();
}

void sspGUITaskEditor::updateConditional()
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

void sspGUITaskEditor::updateMessage(bool bValid)
{
 	GetDlgItem(IDC_EDIT_MSG)->EnableWindow(bValid);
 	GetDlgItem(IDC_REMOVE_MSG)->EnableWindow(bValid);
}

bool sspGUITaskEditor::validCondition()
{
  return (m_pos != m_pList->end());
}
