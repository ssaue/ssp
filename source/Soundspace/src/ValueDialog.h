#if !defined(AFX_VALUEDIALOG_H__7B9D3C02_ACB0_11D3_901C_97F38983485F__INCLUDED_)
#define AFX_VALUEDIALOG_H__7B9D3C02_ACB0_11D3_901C_97F38983485F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"

// ValueDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CValueDialog dialog

class CValueDialog : public CDialog
{
// Construction
public:
	CValueDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValueDialog)
	enum { IDD = IDD_LIST_VALUES };
	CListBox	m_valueList;
	int		m_nValueType;
	//}}AFX_DATA
	sspIntVec m_nSelection;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CValueDialog)
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnReplace();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectType();
	afx_msg void OnSelectValue();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VALUEDIALOG_H__7B9D3C02_ACB0_11D3_901C_97F38983485F__INCLUDED_)
