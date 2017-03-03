// ConditionalDialog.h : header file
//

#if !defined(AFX_CONDITIONALDIALOG_H__E81C3466_B21B_11D3_901C_A9B71D1C1BB3__INCLUDED_)
#define AFX_CONDITIONALDIALOG_H__E81C3466_B21B_11D3_901C_A9B71D1C1BB3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"

/////////////////////////////////////////////////////////////////////////////
// CConditionalDialog dialog

class CConditionalDialog : public CDialog
{
// Construction
public:
	CConditionalDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConditionalDialog)
	enum { IDD = IDD_LIST_CONDITIONALS };
	CListBox	m_condList;
	int		m_nConditionalType;
	//}}AFX_DATA
	sspIntVec m_nSelection;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionalDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConditionalDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnReplace();
	afx_msg void OnSelectType();
	afx_msg void OnSelectConditional();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITIONALDIALOG_H__E81C3466_B21B_11D3_901C_A9B71D1C1BB3__INCLUDED_)
