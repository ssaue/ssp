// MidiMagiDlg.h : header file
//

#pragma once

#include <sspMidiFileObjects.h>
#include "afxcmn.h"

// CMidiMagiDlg dialog
class CMidiMagiDlg : public CDialog
{
// Construction
public:
	CMidiMagiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MIDIMAGI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
  sspMidiFileSong m_song;
  
  void resetDialog();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
  CString m_strFilename;
  int m_nTracks;
  double m_dDuration;
  afx_msg void OnOpenfile();
  afx_msg void OnProcess();
  CListCtrl m_lcNoteMap;
  afx_msg void OnNMDblclkNoteonMap(NMHDR *pNMHDR, LRESULT *pResult);
};
