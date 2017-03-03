// SSpGUIUtilityEditor.h : header file
//

#ifndef SSPGUIUTILITYEDITOR_H
#define SSPGUIUTILITYEDITOR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "afxcmn.h"
#include "atltime.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CEditValue dialog

class CEditValue : public CDialog
{
	DECLARE_DYNAMIC(CEditValue)

public:
	CEditValue(CRect pos, CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditValue();

// Dialog Data
	enum { IDD = IDD_EDIT_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_nValue;
  CRect m_pos;
};

/////////////////////////////////////////////////////////////////////////////
// CEditCombo dialog

class CEditCombo : public CDialog
{
	DECLARE_DYNAMIC(CEditCombo)

public:
	CEditCombo(CRect pos, CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditCombo();

// Dialog Data
	enum { IDD = IDD_EDIT_COMBO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
  CComboBox m_lbChoices;
  int m_nChoice;
  CRect m_pos;
  CStringArray strChoices;

  void addChoice(int i, CString txt);
};

/////////////////////////////////////////////////////////////////////////////
// CEditClock dialog

class CEditClock : public CDialog
{
	DECLARE_DYNAMIC(CEditClock)

public:
	CEditClock(CRect pos, CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditClock();

// Dialog Data
	enum { IDD = IDD_EDIT_CLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
  CTime m_time;
  CRect m_pos;
};

/////////////////////////////////////////////////////////////////////////////
// sspGUIfileDialogs namespace

namespace sspGUIfileDialogs
{
	enum {MAXBUFFER = 2562};	// Strange buffer limitation (possible Win BUG)

	bool SelectFolder(HWND hOwner, const wchar_t* strTitle, CString& strFolder);
	bool SelectFile(HWND hOwner, const wchar_t* strTitle, const wchar_t* strFilter, CString& strFile);
	bool SelectMultipleFiles(HWND hOwner, const wchar_t* strTitle, const wchar_t* strFilter, CStringArray& strNames);
};

/////////////////////////////////////////////////////////////////////////////
// sspSaveDialog dialog


class sspSaveDialog : public CFileDialog
{
// Construction
public:
	sspSaveDialog(LPCTSTR lpszFileName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SonBrowseDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SonBrowseDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
