// PressureMagiDlg.h : header file
//

#pragma once
#include "afxwin.h"

// CPressureMagiDlg dialog
class CPressureMagiDlg : public CDialog
{
// Construction
public:
	CPressureMagiDlg(CWnd* pParent = NULL);	// standard constructor
  void updateMonitor();

// Dialog Data
	enum { IDD = IDD_PRESSUREMAGI_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
  afx_msg void OnOK();
  afx_msg void OnCancel();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnStart();
	afx_msg void OnStop();
  afx_msg LRESULT OnPlayAutomatic(WPARAM wParam, LPARAM lParam);

private:
  void enableEdits(bool bEnable = true);

protected:
  CComboBox m_cbInputDevice;
  CComboBox m_cbOutputDevice;
  CEdit m_editInputVelocity;
  CEdit m_editReadVelocity;
  CEdit m_editOutputVelocity;
  CEdit m_editMeasuredCurrent;
  CButton m_btnOutputValve;

public:
  bool m_bAutoStart;
};
