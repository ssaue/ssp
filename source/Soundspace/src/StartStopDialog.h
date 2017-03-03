#include "atltime.h"
#if !defined(AFX_STARTSTOPDIALOG_H__6A204602_4776_11D4_8D73_FF294CFF140B__INCLUDED_)
#define AFX_STARTSTOPDIALOG_H__6A204602_4776_11D4_8D73_FF294CFF140B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StartStopDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStartStopDialog dialog

class CStartStopDialog : public CDialog
{
// Construction
public:
	CStartStopDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStartStopDialog)
	enum { IDD = IDD_SETTINGS_STARTSTOP };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartStopDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStartStopDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
  afx_msg void OnEnableInterval();
  CTime m_startTime;
  CTime m_stopTime;
  int m_emptyAction;
  BOOL m_bEnableInterval;
  int m_nStartAction;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTSTOPDIALOG_H__6A204602_4776_11D4_8D73_FF294CFF140B__INCLUDED_)
