// SSpGUIPlayObjectEditor.h : header file
//

#ifndef SSPGUIOBJECTEDITOR_H
#define SSPGUIOBJECTEDITOR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include "SSpGUIPlayObject.h"
#include "SSpPlayObjects.h"

/////////////////////////////////////////////////////////////////////////////
// CObjectWavDialog dialog

class CObjectWavDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectWavDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectWavDialog)
	enum { IDD = IDD_OBJECT_WAVFILE };
	//}}AFX_DATA
	SSpWavFile		m_editObj;
	CStringArray	m_strFileName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectWavDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectWavDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

	afx_msg void OnFileFilter();
	afx_msg void OnCheckFolder();

  BOOL  m_bFolder;
};

/////////////////////////////////////////////////////////////////////////////
// CObjectMidiDialog dialog

class CObjectMidiDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectMidiDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectMidiDialog)
	enum { IDD = IDD_OBJECT_MIDIFILE };
	//}}AFX_DATA
	CStringArray	m_strFileName;
	SSpMidiFile		m_editObj;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectMidiDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectMidiDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
private:
	CListBox m_lbDevice;
	int m_nDevice;
public:
	afx_msg void OnTempofactor();
	afx_msg void OnCheckFolder();
protected:
	BOOL m_bFolder;
};

/////////////////////////////////////////////////////////////////////////////
// CObjectDSControlDialog dialog

class CObjectDSControlDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectDSControlDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectDSControlDialog)
	enum { IDD = IDD_OBJECT_DSFILE };
	//}}AFX_DATA
	CStringArray	m_strFileName;
	SSpDSFile		m_editObj;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectDSControlDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectDSControlDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg void OnCheckFolder();
	BOOL m_bFolder;
};

/////////////////////////////////////////////////////////////////////////////
// CObjectMidiListDialog dialog

class CObjectMidiListDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectMidiListDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectMidiListDialog)
	enum { IDD = IDD_OBJECT_MIDILIST };
	int		m_nDevice;
	//}}AFX_DATA
	CStringArray		m_strFileName;
	SSpMidiEventList	m_editObj;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectMidiListDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectMidiListDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CListBox m_lbDevice;
	afx_msg void OnCheckFolder();
protected:
	BOOL m_bFolder;
};

/////////////////////////////////////////////////////////////////////////////
// CObjectDMXDialog dialog

class CObjectDMXDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectDMXDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectDMXDialog)
	enum { IDD = IDD_OBJECT_DMXSELECTFILE };
	int		m_nRefreshInterval;
	int		m_nDevice;
	//}}AFX_DATA
	CStringArray		m_strFileName;
	SSpDmxLivePlayer	m_editObj;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectDMXDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectDMXDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	afx_msg void OnDuration();
	afx_msg void OnSelectindex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
private:
	CListBox m_lbDevice;
public:
	afx_msg void OnCheckFolder();
protected:
	BOOL m_bFolder;
};

/////////////////////////////////////////////////////////////////////////////
// CObjectSilenceDialog dialog

class CObjectSilenceDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectSilenceDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectSilenceDialog)
	enum { IDD = IDD_OBJECT_SILENCE };
	//}}AFX_DATA
	SSpSilence m_editObj;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectSilenceDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CObjectSilenceDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelectDuration();
protected:
	virtual void OnOK();
private:
	CString m_strName;
};

/////////////////////////////////////////////////////////////////////////////
// CObjectSimultaneousDialog dialog

class CObjectSimultaneousDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectSimultaneousDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CObjectSimultaneousDialog();

// Dialog Data
	//{{AFX_DATA(CObjectSimultaneousDialog)
	enum { IDD = IDD_OBJECT_SIMULTANEOUS };
	CString	m_strName;
	//}}AFX_DATA
	SSpSimultaneousGroup m_editObj;
	sspIntVec m_nObjects;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectSimultaneousDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nSelected;

	void UpdateButtons();
	// Generated message map functions
	//{{AFX_MSG(CObjectSimultaneousDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelectObject();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
private:
	CListBox m_lbObjects;
};

/////////////////////////////////////////////////////////////////////////////
// CObjectSequentialDialog dialog

class CObjectSequentialDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectSequentialDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectSequentialDialog)
	enum { IDD = IDD_OBJECT_SEQUENTIAL };
	CString	m_strName;
	//}}AFX_DATA
	SSpSequentialGroup m_editObj;
	sspIntVec m_nObjects;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectSequentialDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nSelected;

	void UpdateButtons();

	// Generated message map functions
	//{{AFX_MSG(CObjectSequentialDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg void OnSelectObject();
	afx_msg void OnUp();
	afx_msg void OnDown();
	CListBox m_lbObjects;
};

/////////////////////////////////////////////////////////////////////////////
// CObjectLinearSelectDialog dialog

class CObjectLinearSelectDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectLinearSelectDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectLinearSelectDialog)
	enum { IDD = IDD_OBJECT_LINEARSELECT };
	CListBox	m_lbObjects;
	CString	m_strName;
	//}}AFX_DATA
	SSpLinearSelectGroup m_editObj;
	sspIntVec	m_nObjects;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectLinearSelectDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nSelected;

	void UpdateButtons();

	// Generated message map functions
	//{{AFX_MSG(CObjectLinearSelectDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnInputvalue();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnSelectObject();
	afx_msg void OnUp();
	afx_msg void OnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};

/////////////////////////////////////////////////////////////////////////////
// CObjectRandomDialog dialog

class CObjectRandomDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectRandomDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectRandomDialog)
	enum { IDD = IDD_OBJECT_RANDOM };
	CString	m_strName;
	//}}AFX_DATA
	SSpRandomGroup	m_editObj;
	sspIntVec		m_nObjects;
	sspIntVec		m_nConstWeight;
	sspIntVec		m_nVarWeight;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectRandomDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int		m_nSelected;
	unsigned int	m_nSize;

	void UpdateButtons();
	// Generated message map functions
	//{{AFX_MSG(CObjectRandomDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	//}}AFX_MSG
	afx_msg void OnNMClickWeightList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkWeightList(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
private:
	CListCtrl m_lbWeights;
};

/////////////////////////////////////////////////////////////////////////////
// CObjectChainDialog dialog

class CObjectChainDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectChainDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectChainDialog)
	enum { IDD = IDD_OBJECT_CHAIN };
	CString	m_strName;
	int		m_nChainLength;
	//}}AFX_DATA
	SSpChainGroup m_editObj;
	sspIntVec	m_nObjects;
	sspIntVec	m_nFollowers;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectChainDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	unsigned int	m_nSize;
	int		m_nSelected;

	void UpdateButtons();
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	DECLARE_MESSAGE_MAP()
public:
protected:
	virtual void OnOK();
public:
	CListCtrl m_lcChainList;
	afx_msg void OnClickChainList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkChainList(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////
// CConditionalObjectDialog dialog

class CObjectConditionalDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectConditionalDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectConditionalDialog)
	enum { IDD = IDD_OBJECT_CONDITIONAL };
	CString	m_strName;
	//}}AFX_DATA
	SSpConditionalGroup m_editObj;
	sspIntVec	m_nObjects;
	sspIntVec	m_nConds;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectConditionalDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int		m_nSelected;
	unsigned int	m_nSize;

	void UpdateButtons();

	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDefault();
	afx_msg void OnDown();
	afx_msg void OnUp();
	afx_msg void OnRemove();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CListCtrl m_lcCondObject;
	afx_msg void OnClickCondObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkCondObjectList(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////
// CObjectDistributionDialog dialog

class CObjectDistributionDialog : public CPlayObjectEditor
{
// Construction
public:
	CObjectDistributionDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectDistributionDialog)
	enum { IDD = IDD_OBJECT_DISTRIBUTION };
	CString	m_strName;
	BOOL	m_bSpecifyEnd;
	int		m_nLoopMode;
	//}}AFX_DATA
	SSpDistributionObject m_editObj;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectDistributionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

  void updateEndButton();
	void updateModeButtons();

	// Generated message map functions
	//{{AFX_MSG(CObjectDistributionDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectObject();
	afx_msg void OnSelectCondition();
	afx_msg void OnSelectStart();
	afx_msg void OnSelectEnd();
	afx_msg void OnSelectDuration();
	afx_msg void OnSelectCount();
	afx_msg void OnChangeMode();
	afx_msg void OnEndDistribution();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDOBJECTDIALOG_H__66AB50A5_B2E3_11D3_901C_DE1C03342ABE__INCLUDED_)
#pragma once


// CObjectSetState dialog

class CObjectSetState : public CPlayObjectEditor
{
	DECLARE_DYNAMIC(CObjectSetState)

public:
	CObjectSetState(CWnd* pParent = NULL);   // standard constructor
	virtual ~CObjectSetState();

// Dialog Data
	SSpStateObject m_editObj;
	CString	m_strName;
  int m_nMode;

	enum { IDD = IDD_OBJECT_SETSTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
  afx_msg void OnBtnStatevalue();
  afx_msg void OnBtnInputvalue();
	virtual void OnOK();
};
