// SSpGUIMsgList.h
//

#ifndef SSPGUIMSGLIST_H
#define SSPGUIMSGLIST_H

#include "sspMsgList.h"
// sspGUIMsgList dialog

class sspGUIMsgList : public CDialog
{
	DECLARE_DYNAMIC(sspGUIMsgList)

public:
	sspGUIMsgList(CWnd* pParent = NULL);   // standard constructor
	virtual ~sspGUIMsgList();

// Dialog Data
	enum { IDD = IDD_MSGLIST_COND };
  sspCondMsgList            m_list;
  sspCondMsgList::iterator  m_pos;

	CListBox	      m_condList;
	CListBox	      m_msgList;
	int		          m_nMsgType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

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
  void updateConditional();
  void updateMessage(bool bValid);
  bool validCondition();
};

#endif
