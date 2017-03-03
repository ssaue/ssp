#pragma once
#include "atltime.h"


// CSettingsResetDlg dialog

class CSettingsResetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingsResetDlg)

public:
	CSettingsResetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettingsResetDlg();

// Dialog Data
	enum { IDD = IDD_SETTINGS_RESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

 	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnWatchdogEnable();
	afx_msg void OnSelectCondition();
  afx_msg void onSelectTimeUnit();
  afx_msg void OnSoftReboot();
  afx_msg void OnHardReboot();

  void updateControls();

	DECLARE_MESSAGE_MAP()
public:
  BOOL m_bEnable;
  int m_nWatchdogType;
  int m_nTimeout;
  int m_nInterval;
  CTime m_timeOfDay;
	int	m_nConditionIndex;
  int m_nTimeUnit;
};
