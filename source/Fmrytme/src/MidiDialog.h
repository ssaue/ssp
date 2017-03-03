#if !defined(AFX_MIDIDIALOG_H__E99FC722_5C4F_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_MIDIDIALOG_H__E99FC722_5C4F_11D2_97D2_00105A3B4608__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MidiDialog.h : header file
//

#include "settings.h"

/////////////////////////////////////////////////////////////////////////////
// CSangDialog dialog

class CSangDialog : public CDialog
{
// Construction
public:
	int m_nStemmer;
	CSangDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSangDialog)
	enum { IDD = IDD_SANGDIALOG };
	CString	m_sSangTittel;
	//}}AFX_DATA
	int m_nLydklasse[CSettings::KANALER];
	int m_nLyd[CSettings::KANALER];
	int m_nProgram[CSettings::KANALER];
	int m_nKanal[CSettings::KANALER];
	CString m_sTittel[CSettings::KANALER];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSangDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSangDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnKlasseChange(UINT nID);
	afx_msg void OnLydChange(UINT nID);
	afx_msg void OnProgramChange(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDIDIALOG_H__E99FC722_5C4F_11D2_97D2_00105A3B4608__INCLUDED_)
