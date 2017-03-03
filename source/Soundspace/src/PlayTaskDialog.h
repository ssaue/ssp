#if !defined(AFX_SOUNDEVENTDIALOG_H__78688424_B6F3_11D3_901C_B9916F3FEBA7__INCLUDED_)
#define AFX_SOUNDEVENTDIALOG_H__78688424_B6F3_11D3_901C_B9916F3FEBA7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include "SSpMessage.h"
#include "SSpTasks.h"

// PlayTaskDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlayTaskDialog dialog

class CPlayTaskDialog : public CDialog
{
// Construction
public:
	CPlayTaskDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlayTaskDialog)
	enum { IDD = IDD_LIST_TASKS };
	CListBox	m_taskList;
	//}}AFX_DATA
	sspIntVec m_nSelection;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayTaskDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayTaskDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnEdit();
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnSelectTask();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDEVENTDIALOG_H__78688424_B6F3_11D3_901C_B9916F3FEBA7__INCLUDED_)
