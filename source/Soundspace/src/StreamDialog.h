// StreamDialog.h : header file
//

#if !defined(AFX_STREAMDIALOG_H__542D7800_A8FB_11D3_901C_E4F76D2F7EAF__INCLUDED_)
#define AFX_STREAMDIALOG_H__542D7800_A8FB_11D3_901C_E4F76D2F7EAF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include "afxwin.h"

class SSpStream;
class SSpAppStream;
class SSpAudioStream;

/////////////////////////////////////////////////////////////////////////////
// CStreamDialog dialog

class CStreamDialog : public CDialog
{
// Construction
public:
	CStreamDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStreamDialog)
	enum { IDD = IDD_LIST_STREAMS };
	CListBox	m_listStream;
	int		m_nStreamType;
	//}}AFX_DATA
	SSpStream*	m_pStream;
	SSpStream*	m_pMain;
	sspIntVec		m_nSelection;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStreamDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStreamDialog)
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnReplace();
	afx_msg void OnSelectStream();
	afx_msg void OnSelectType();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CStreamBasicDialog dialog

class CStreamBasicDialog : public CDialog
{
// Construction
public:
	CStreamBasicDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStreamBasicDialog)
	enum { IDD = IDD_STREAM_BASIC };
	CString	m_strName;
	//}}AFX_DATA
	SSpStream*		m_pStream;
	BOOL			m_bAudioStream;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStreamBasicDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListBox	m_lbStreams;
	int			m_nSelected;
	// For the time being, these are kept outside the Stream-object
	sspIntVec	m_streams;
	int			m_nFactor;

	void UpdateButtons();
	// Generated message map functions
	//{{AFX_MSG(CStreamBasicDialog)
	afx_msg void OnLoadDevice();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelectSubstream();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnSelectFactor();
};
/////////////////////////////////////////////////////////////////////////////
// CStreamDeviceDialog dialog

class CStreamDeviceDialog : public CDialog
{
// Construction
public:
	CStreamDeviceDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStreamDeviceDialog)
	enum { IDD = IDD_STREAM_DEVICES };
	unsigned int	m_nWaitBuffers;
	unsigned int	m_nPlayBuffers;
	int		m_nStereo;
	BOOL	m_bFader;
	//}}AFX_DATA
	enum {STEREO, MONO};

	SSpAudioStream*	m_pStream;
	int				m_nFaderValue;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStreamDeviceDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void displayMono();
	void displayStereo();

	sspIntVec getDSselection();
	sspIntVec getWAVselection();

	// Generated message map functions
	//{{AFX_MSG(CStreamDeviceDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnMono();
	afx_msg void OnStereo();
	afx_msg void OnSelectValue();
	afx_msg void OnEnableFader();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CListBox m_lbDSlist;
	CListBox m_lbWAVlist;
};

#endif // !defined(AFX_STREAMDIALOG_H__542D7800_A8FB_11D3_901C_E4F76D2F7EAF__INCLUDED_)
