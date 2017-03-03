#pragma once


// sspGUIMapDialog dialog

class sspGUIMapDialog : public CDialog
{
	DECLARE_DYNAMIC(sspGUIMapDialog)

public:
	sspGUIMapDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~sspGUIMapDialog();

// Dialog Data
	enum { IDD = IDD_MAPDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  int m_nPitch;
  int m_nDMX;
};
