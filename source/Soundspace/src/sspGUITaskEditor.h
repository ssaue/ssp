// sspGUITaskEditor.h : header file
//

#ifndef SSP_GUI_TASKEDITOR_H
#define SSP_GUI_TASKEDITOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SSpTasks.h"

/////////////////////////////////////////////////////////////////////////////
// sspGUITaskEditor dialog

class sspGUITaskEditor : public CDialog
{
// Construction
public:
	sspGUITaskEditor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(sspGUITaskEditor)
	enum { IDD = IDD_TASK_EDITOR };
	CListBox	m_msgList;
	CListBox	m_condList;
	CString	m_strName;
	int		m_nMsgType;
	//}}AFX_DATA
	SSpPlayTask	m_task;

// Temporary pointers
  sspCondMsgList*           m_pList;
  sspCondMsgList::iterator  m_pos;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(sspGUITaskEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(sspGUITaskEditor)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectObject();
	afx_msg void OnSelectFactor();
	afx_msg void OnSelectCondition();

	afx_msg void OnAddCondition();
  afx_msg void OnEditCondition();
 	afx_msg void OnRemoveCondition();
	afx_msg void OnSelectMessageCondition();

  afx_msg void OnAddMessage();
	afx_msg void OnEditMessage();
	afx_msg void OnRemoveMessage();
	afx_msg void OnSelectMessage();
	afx_msg void OnSelectMessageType();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnSelectEvent(unsigned int nID);
	DECLARE_MESSAGE_MAP()

  void updateList();
  void updateConditional();
  void updateMessage(bool bValid);
  bool validCondition();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKEDITORDLG_H__F2037784_B93F_11D6_8B69_444553540000__INCLUDED_)
