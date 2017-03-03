// SSpGUIConditionalEditor.h : header file
//

#ifndef SSPGUICONDITIONALEDITOR_H
#define SSPGUICONDITIONALEDITOR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include "SSpGUIConditional.h"
#include "SSpConditionals.h"

/////////////////////////////////////////////////////////////////////////////
// CCondBooleanDialog dialog

class CCondBooleanDialog : public CConditionalEditor
{
// Construction
public:
	CCondBooleanDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCondBooleanDialog)
	enum { IDD = IDD_COND_BOOLEAN };
	int		m_nBool;
	//}}AFX_DATA
	SSpBoolean m_editCond;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondBooleanDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondBooleanDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedBool();
};

/////////////////////////////////////////////////////////////////////////////
// CCondRelationDialog dialog

class CCondRelationDialog : public CConditionalEditor
{
// Construction
public:
	CCondRelationDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCondRelationDialog)
	enum { IDD = IDD_COND_RELATION };
	int		m_nRelation;
	//}}AFX_DATA
	SSpValueRelation m_editCond;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondRelationDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondRelationDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnTestvalueBtn();
	afx_msg void OnThresholdBtn();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCondRangeDialog dialog

class CCondRangeDialog : public CConditionalEditor
{
// Construction
public:
	CCondRangeDialog(CWnd* pParent = NULL);   // standard constructor
	SSpInRange m_editCond;

// Dialog Data
	//{{AFX_DATA(CCondRangeDialog)
	enum { IDD = IDD_COND_RANGE };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondRangeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondRangeDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnLowervalueBtn();
	afx_msg void OnTestvalueBtn();
	afx_msg void OnUppervalueBtn();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCondTimeIntervalDialog dialog

class CCondTimeIntervalDialog : public CConditionalEditor
{
// Construction
public:
	CCondTimeIntervalDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCondTimeIntervalDialog)
	enum { IDD = IDD_COND_TIMEINT };
	int		m_nHourStart;
	int		m_nHourEnd;
	int		m_nMinuteStart;
	int		m_nMinuteEnd;
	int		m_nSecondStart;
	int		m_nSecondEnd;
	//}}AFX_DATA
	SSpTimeInterval m_editCond;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondTimeIntervalDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondTimeIntervalDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCondTimeRelationDialog dialog

class CCondTimeRelationDialog : public CConditionalEditor
{
// Construction
public:
	CCondTimeRelationDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCondTimeRelationDialog)
	enum { IDD = IDD_COND_TIMERELATION };
	int		m_nHour;
	int		m_nMinute;
	int		m_nSecond;
	int		m_nRelation;
	//}}AFX_DATA
	SSpTimeRelation m_editCond;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondTimeRelationDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondTimeRelationDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCondMinuteRelationDialog dialog

class CCondMinuteRelationDialog : public CConditionalEditor
{
// Construction
public:
	CCondMinuteRelationDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCondMinuteRelationDialog)
	enum { IDD = IDD_COND_MINUTES };
	int		m_nMinutes;
	int		m_nRelation;
	//}}AFX_DATA
	SSpMinuteRelation m_editCond;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondMinuteRelationDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondMinuteRelationDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCondDateIntervalDialog dialog

class CCondDateIntervalDialog : public CConditionalEditor
{
// Construction
public:
	CCondDateIntervalDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCondDateIntervalDialog)
	enum { IDD = IDD_COND_DATEINT };
	int		m_nStartDay;
	int		m_nEndDay;
	int		m_nStartMonth;
	int		m_nEndMonth;
	//}}AFX_DATA
	SSpDateInterval m_editCond;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondDateIntervalDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondDateIntervalDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCondDayOfWeekDialog dialog

class CCondDayOfWeekDialog : public CConditionalEditor
{
// Construction
public:
	CCondDayOfWeekDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCondDayOfWeekDialog)
	enum { IDD = IDD_COND_DAYOFWEEK };
	int		m_nBool;
	//}}AFX_DATA
	SSpDayOfWeek m_editCond;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondDayOfWeekDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int	m_nSelectCount;

	// Generated message map functions
	//{{AFX_MSG(CCondDayOfWeekDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDay(unsigned int nID);
};

/////////////////////////////////////////////////////////////////////////////
// CCondAndDialog dialog

class CCondAndDialog : public CConditionalEditor
{
// Construction
public:
	CCondAndDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCondAndDialog();

// Dialog Data
	//{{AFX_DATA(CCondAndDialog)
	enum { IDD = IDD_COND_OPERATION };
	CListBox	m_lbOperands;
	//}}AFX_DATA
	SSpAnd m_editCond;
	sspIntVec m_nOperands;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondAndDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondAndDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnSelectConditional();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCondOrDialog dialog

class CCondOrDialog : public CConditionalEditor
{
// Construction
public:
	CCondOrDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCondOrDialog();

// Dialog Data
	//{{AFX_DATA(CCondOrDialog)
	enum { IDD = IDD_COND_OPERATION };
	CListBox	m_lbOperands;
	//}}AFX_DATA
	SSpOr m_editCond;
	sspIntVec m_nOperands;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondOrDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondOrDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectConditional();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCondNotDialog dialog

class CCondNotDialog : public CConditionalEditor
{
// Construction
public:
	CCondNotDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCondNotDialog)
	enum { IDD = IDD_COND_NOT };
	//}}AFX_DATA
	SSpNot m_editCond;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondNotDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondNotDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnConditional();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCondPlayingDialog dialog

class CCondPlayingDialog : public CConditionalEditor
{
// Construction
public:
	CCondPlayingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCondPlayingDialog();

// Dialog Data
	//{{AFX_DATA(CCondPlayingDialog)
	enum { IDD = IDD_COND_PLAYING };
	CListBox	m_lbOperands;
	//}}AFX_DATA
	SSpIsPlaying m_editCond;
	sspIntVec m_nOperands;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCondPlayingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCondPlayingDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectPlayobjects();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CONDITIONALDIALOG_H__E81C3466_B21B_11D3_901C_A9B71D1C1BB3__INCLUDED_)
