#pragma once
#include "afxwin.h"


// CInputDialog dialog

class CInputDialog : public CDialog
{
	DECLARE_DYNAMIC(CInputDialog)

public:
	CInputDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputDialog();

// Dialog Data
	enum { IDD = IDD_LIST_INPUTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void UpdateButtons();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelectInput();
	afx_msg void OnSelectType();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnRemove();
private:
	CListBox m_lbInputs;
public:
	int m_nInputType;
	virtual BOOL OnInitDialog();

private:
  void resetInputList();
};
