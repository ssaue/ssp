// sspGUITriggerMsgList.h
//

#ifndef SSPGUITRIGGERMSGLIST_H
#define SSPGUITRIGGERMSGLIST_H

#include "sspMsgList.h"

// sspGUITriggerMsgList dialog


class sspGUITriggerMsgList : public CDialog
{
	DECLARE_DYNAMIC(sspGUITriggerMsgList)

public:
	sspGUITriggerMsgList(CWnd* pParent = NULL);   // standard constructor
	virtual ~sspGUITriggerMsgList();

// Dialog Data
	enum { IDD = IDD_MSGLIST_TRIGGER };
  sspTriggerMsgList            m_list;
  sspTriggerMsgList::iterator  m_pos;
  sspCondMsgList::iterator     m_posCond;

	CListCtrl       m_trigList;
	CListBox	      m_condList;
	CListBox	      m_msgList;
	int		          m_nMsgType;
  int             m_nSelected;
  CString         m_strModes[3];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

  afx_msg void OnAddTrigger();
	afx_msg void OnRemoveTrigger();
  afx_msg void OnNMClickTrigger(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTrigger(NMHDR *pNMHDR, LRESULT *pResult);

  afx_msg void OnAddCondition();
	afx_msg void OnEditCondition();
	afx_msg void OnRemoveCondition();
	afx_msg void OnSelectCondition();

  afx_msg void OnAddMessage();
	afx_msg void OnEditMessage();
	afx_msg void OnRemoveMessage();
	afx_msg void OnSelectMessage();
	afx_msg void OnSelectMessageType();
	DECLARE_MESSAGE_MAP()

  void updateList();
  void updateTrigger();
  void updateConditional();
  void updateMessage(bool bValid);
  bool validTrigger();
  bool validCondition();
};

#endif
