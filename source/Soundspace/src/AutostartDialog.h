#if !defined(AFX_AUTOSTARTDIALOG_H__8D818861_F4DB_11D6_8B6A_444553540000__INCLUDED_)
#define AFX_AUTOSTARTDIALOG_H__8D818861_F4DB_11D6_8B6A_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutostartDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutostartDialog dialog

class CAutostartDialog : public CDialog
{
// Construction
public:
	CAutostartDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutostartDialog)
	enum { IDD = IDD_AUTOSTART_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutostartDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutostartDialog)
	virtual void OnCancel();
	afx_msg void OnTimer(unsigned int nIDEvent);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOSTARTDIALOG_H__8D818861_F4DB_11D6_8B6A_444553540000__INCLUDED_)
