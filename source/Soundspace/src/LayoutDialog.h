#if !defined(AFX_LAYOUTDIALOG_H__042571E0_C875_11D6_8D0A_0002B3A77CED__INCLUDED_)
#define AFX_LAYOUTDIALOG_H__042571E0_C875_11D6_8D0A_0002B3A77CED__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LayoutDialog.h : header file
//

#include "sspDefines.h"

/////////////////////////////////////////////////////////////////////////////
// CLayoutDialog dialog

class CLayoutDialog : public CDialog
{
// Construction
public:
	CLayoutDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLayoutDialog)
	enum { IDD = IDD_LAYOUT_DIALOG };
	CListBox	m_lbValues;
	CListBox	m_listFader;
	//}}AFX_DATA
	sspIntVec m_nValueSelection;
	sspIntVec m_nFaderSelection;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayoutDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	sspIntVec m_nInputRangeValues;

	// Generated message map functions
	//{{AFX_MSG(CLayoutDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnSelectValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYOUTDIALOG_H__042571E0_C875_11D6_8D0A_0002B3A77CED__INCLUDED_)
