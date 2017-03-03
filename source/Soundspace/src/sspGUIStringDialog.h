#ifndef SSPGUISTRINGDIALOG_H
#define SSPGUISTRINGDIALOG_H

#include "sspDefines.h"

class sspGUIStringDialog : public CDialog
{
	DECLARE_DYNAMIC(sspGUIStringDialog)

public:
	sspGUIStringDialog(CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
	enum { IDD = IDD_LIST_STRINGS };
	CListBox	m_stringList;
	int		    m_nStringType;
	sspIntVec m_nSelection;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnReplace();
	afx_msg void OnSelectType();
	afx_msg void OnSelectString();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif
