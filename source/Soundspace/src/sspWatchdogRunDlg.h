//////////////////////////////////////////
// sspWatchdogRunDlg.h

#pragma once

#include "watchdog_ocx.h"

// sspWatchdogRunDlg dialog

class sspWatchdogRunDlg : public CDialog
{
	DECLARE_DYNAMIC(sspWatchdogRunDlg)

public:
	sspWatchdogRunDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~sspWatchdogRunDlg();

  BOOL Create();

 	CWatchDog_OCX	m_wDog;

// Dialog Data
	enum { IDD = IDD_WATCHDOG_RUNDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
