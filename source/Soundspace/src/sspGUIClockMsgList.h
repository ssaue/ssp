// SSpGUIClockMsgList.h
//

#ifndef SSPGUICLOCKMSGLIST_H
#define SSPGUICLOCKMSGLIST_H

#include "sspMsgList.h"

// sspGUIClockMsgList dialog

class sspGUIClockMsgList : public CDialog
{
	DECLARE_DYNAMIC(sspGUIClockMsgList)

public:
	sspGUIClockMsgList(CWnd* pParent = NULL);   // standard constructor
	virtual ~sspGUIClockMsgList();

// Dialog Data
	enum { IDD = IDD_MSGLIST_CLOCK };
  sspClockMsgList            m_list;
  sspClockMsgList::iterator  m_pos;
  sspCondMsgList::iterator   m_posCond;

  CListBox        m_clockList;
	CListBox	      m_condList;
	CListBox	      m_msgList;
	int		          m_nMsgType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

  afx_msg void OnAddClock();
	afx_msg void OnEditClock();
	afx_msg void OnRemoveClock();
	afx_msg void OnSelectClock();

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
  void updateClock();
  void updateConditional();
  void updateMessage(bool bValid);
  bool validClock();
  bool validCondition();
};

#endif