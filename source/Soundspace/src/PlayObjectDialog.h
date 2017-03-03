#if !defined(AFX_SOUNDOBJECTDIALOG_H__66AB50A5_B2E3_11D3_901C_DE1C03342ABE__INCLUDED_)
#define AFX_SOUNDOBJECTDIALOG_H__66AB50A5_B2E3_11D3_901C_DE1C03342ABE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"

class SSpDeviceInfo;

// PlayObjectDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlayObjectDialog dialog

class CPlayObjectDialog : public CDialog
{
// Construction
public:
	CPlayObjectDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlayObjectDialog)
	enum { IDD = IDD_LIST_OBJECTS };
	CListBox	m_objectList;
	int		m_nObjectType;
	//}}AFX_DATA

	sspIntVec m_nSelection;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayObjectDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayObjectDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnReplace();
	afx_msg void OnEdit();
	afx_msg void OnSelectObject();
	afx_msg void OnSelectType();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDOBJECTDIALOG_H__66AB50A5_B2E3_11D3_901C_DE1C03342ABE__INCLUDED_)
