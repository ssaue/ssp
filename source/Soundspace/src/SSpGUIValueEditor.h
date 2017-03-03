// SSPGUIValueEditor.h : header file
//

#ifndef SSPGUIVALUEEDITOR_H
#define SSPGUIVALUEEDITOR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include "SSpGUIValue.h"
#include "SSpValues.h"

/////////////////////////////////////////////////////////////////////////////
// CValueFloatDialog dialog

class CValueFloatDialog : public CValueEditor
{
// Construction
public:
	CValueFloatDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValueFloatDialog)
	enum { IDD = IDD_VALUE_FLOAT };
	float	m_fValue;
	//}}AFX_DATA
	SSpBasicValue m_editVal;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueFloatDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool	m_bNameEdited;

	// Generated message map functions
	//{{AFX_MSG(CValueFloatDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKillFocusName();
};

/////////////////////////////////////////////////////////////////////////////
// CValueInputRangeDialog dialog

class CValueInputRangeDialog : public CValueEditor
{
// Construction
public:
	CValueInputRangeDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValueInputRangeDialog)
	enum { IDD = IDD_VALUE_INPUTRANGE };
	float	m_fRangeMin;
	float	m_fRangeMax;
	//}}AFX_DATA
	SSpInputValueRange m_editVal;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueInputRangeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CValueInputRangeDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CValueClockDialog dialog

class CValueClockDialog : public CValueEditor
{
// Construction
public:
	CValueClockDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValueClockDialog)
	enum { IDD = IDD_VALUE_CLOCK };
	float	m_fEndValue;
	float	m_fStartValue;
	int		m_nStartHour;
	int		m_nEndHour;
	int		m_nStartMinute;
	int		m_nEndMinute;
	int		m_nStartSecond;
	int		m_nEndSecond;
	//}}AFX_DATA
	SSpTimeMap m_editVal;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueClockDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CValueClockDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CValueDateDialog dialog

class CValueDateDialog : public CValueEditor
{
// Construction
public:
	CValueDateDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValueDateDialog)
	enum { IDD = IDD_VALUE_DATE };
	int		m_nStartDay;
	int		m_nEndDay;
	float	m_fEndValue;
	int		m_nStartMonth;
	int		m_nEndMonth;
	float	m_fStartValue;
	//}}AFX_DATA
	SSpDateMap m_editVal;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueDateDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CValueDateDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CValueMapDialog dialog

class CValueMapDialog : public CValueEditor
{
// Construction
public:
	CValueMapDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValueMapDialog)
	enum { IDD = IDD_VALUE_MAP };
	float	m_fEndValueIn;
	float	m_fEndValueOut;
	float	m_fStartValueIn;
	float	m_fStartValueOut;
	//}}AFX_DATA
	SSpLinearMap m_editVal;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueMapDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CValueMapDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSourceValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CValueSumDialog dialog

class CValueSumDialog : public CValueEditor
{
// Construction
public:
	CValueSumDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValueSumDialog)
	enum { IDD = IDD_VALUE_OPERATION };
	CListBox	m_lbOperands;
	//}}AFX_DATA
	SSpAddValue m_editVal;
	sspIntVec m_nOperands;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueSumDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CValueSumDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnSelectValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CValueProductDialog dialog

class CValueProductDialog : public CValueEditor
{
// Construction
public:
	CValueProductDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValueProductDialog)
	enum { IDD = IDD_VALUE_OPERATION };
	CListBox	m_lbOperands;
	//}}AFX_DATA
	SSpProductValue m_editVal;
	sspIntVec m_nOperands;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueProductDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CValueProductDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectValue();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CValueConditionalDialog dialog

class CValueConditionalDialog : public CValueEditor
{
// Construction
public:
	CValueConditionalDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValueConditionalDialog)
	enum { IDD = IDD_VALUE_CONDITIONAL };
	CListCtrl	m_lcCondValueList;
	//}}AFX_DATA
	SSpConditionalValue m_editVal;
	sspIntVec m_nConditionals;
	sspIntVec m_nValues;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueConditionalDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nSize;
	int m_nSelected;

	void UpdateButtons();

	// Generated message map functions
	//{{AFX_MSG(CValueConditionalDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefaultValue();
	afx_msg void OnMoveDown();
	afx_msg void OnMoveUp();
	afx_msg void OnRemove();
	afx_msg void OnClickCondValueList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCondValueList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CValueRandomDialog dialog

class CValueRandomDialog : public CValueEditor
{
// Construction
public:
	CValueRandomDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValueRandomDialog)
	enum { IDD = IDD_VALUE_RANDOM };
	//}}AFX_DATA
	SSpRandomValue m_editVal;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueRandomDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CValueRandomDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnLowerLimit();
	afx_msg void OnUpperLimit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VALUEDIALOG_H__7B9D3C02_ACB0_11D3_901C_97F38983485F__INCLUDED_)
