#ifndef SSP_GUISTRINGEDITORS_H
#define SSP_GUISTRINGEDITORS_H

#include "sspStringObjects.h"
#include "sspGUIString.h"

#include <vector>
#include <string>

// sspSimpleStringEditor dialog

class sspSimpleStringEditor : public sspGUIStringEditor
{
	DECLARE_DYNAMIC(sspSimpleStringEditor)

public:
	sspSimpleStringEditor(CWnd* pParent = NULL);   // standard constructor
	virtual ~sspSimpleStringEditor();
  sspSimpleString m_editString;

// Dialog Data
	enum { IDD = IDD_STRING_SIMPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual BOOL OnInitDialog();
};

class sspConcatStringEditor : public sspGUIStringEditor
{
	DECLARE_DYNAMIC(sspConcatStringEditor)

public:
	sspConcatStringEditor(CWnd* pParent = NULL);   // standard constructor
	virtual ~sspConcatStringEditor();

  sspConcatString m_editString;
  CListBox	m_lbStrings;
	sspIntVec m_nStrings;

// Dialog Data
	enum { IDD = IDD_STRING_CONCAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnSelectString();
	virtual BOOL OnInitDialog();

  int m_nSelected;
	void updateButtons();
};

class sspAlphabetEditor : public sspGUIStringEditor
{
	DECLARE_DYNAMIC(sspAlphabetEditor)

public:
	sspAlphabetEditor(CWnd* pParent = NULL);   // standard constructor
	virtual ~sspAlphabetEditor();

  sspAlphabetString m_editString;
  CString m_alphabet;

// Dialog Data
	enum { IDD = IDD_STRING_ALPHABET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSourceValue();

	bool convert(const CString& cstr, sspStrVec& strVec);
	void convert(const sspStrVec& strVec, CString& cstr);
};

#endif
