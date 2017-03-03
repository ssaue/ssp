// SSpGUIMessageEditor.h
//

#ifndef SSPGUIMESSAGEEDITOR_H
#define SSPGUIMESSAGEEDITOR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SSpGuiMessage.h"

/////////////////////////////////////////////////////////////////////////////
// CLoadMessageDialog dialog

class CLoadMessageDialog : public CMessageDialog
{
// Construction
public:
	CLoadMessageDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoadMessageDialog)
	enum { IDD = IDD_MESSAGE_LOAD };
	int		m_nCurStream;
	int		m_nCurTask;
	//}}AFX_DATA
	enum {CURRENT, SPECIFY};


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadMessageDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoadMessageDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectStream();
	afx_msg void OnSelectTask();
	afx_msg void OnSelectValue();
	afx_msg void OnCurtask();
	afx_msg void OnStream();
	afx_msg void OnTask();
	virtual void OnOK();
	afx_msg void OnCurStream();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CSoloMessageDialog dialog

class CSoloMessageDialog : public CMessageDialog
{
// Construction
public:
	CSoloMessageDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSoloMessageDialog)
	enum { IDD = IDD_MESSAGE_SOLO };
	int		m_nCurStream;
	//}}AFX_DATA
	enum {CURRENT, SPECIFY};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoloMessageDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSoloMessageDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnStream();
	afx_msg void OnSelectStream();
	virtual void OnOK();
	afx_msg void OnCurStream();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CStartMessageDialog dialog

class CStartMessageDialog : public CMessageDialog
{
// Construction
public:
	CStartMessageDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStartMessageDialog)
	enum { IDD = IDD_MESSAGE_START };
	int		m_nCurStream;
	//}}AFX_DATA
	enum {CURRENT, SPECIFY};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartMessageDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStartMessageDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCurStream();
	afx_msg void OnStream();
	afx_msg void OnSelectStream();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CVolumeMessageDialog dialog

class CVolumeMessageDialog : public CMessageDialog
{
// Construction
public:
	CVolumeMessageDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVolumeMessageDialog)
	enum { IDD = IDD_MESSAGE_VOLUME };
	int		m_nCurStream;
	//}}AFX_DATA
	enum {CURRENT, SPECIFY};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolumeMessageDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVolumeMessageDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnCurStream();
	afx_msg void OnStream();
	afx_msg void OnSelectStream();
	afx_msg void OnSelectVolume();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif