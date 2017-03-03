#pragma once

#include "sspGUIInput.h"
#include "sspICPinput.h"
#include "sspParallelInput.h"
#include "sspMIDIinput.h"
#include "afxwin.h"

// CInputICPanalogDialog dialog

class CInputICPanalogDialog : public CInputEditor
{
	DECLARE_DYNAMIC(CInputICPanalogDialog)

public:
	CInputICPanalogDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputICPanalogDialog();

// Dialog Data
	enum { IDD = IDD_INPUT_ICPANALOG };
	sspICPanalogInput m_editInput;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_strName;
	unsigned char m_nAddress;
	unsigned char m_nChannel;
	float m_fUpdateInterval;
  int m_nPort;

public:
	afx_msg void OnInputValue();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};

// CInputICPdigitalDialog dialog

class CInputICPdigitalDialog : public CInputEditor
{
	DECLARE_DYNAMIC(CInputICPdigitalDialog)

public:
	CInputICPdigitalDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputICPdigitalDialog();

// Dialog Data
	enum { IDD = IDD_INPUT_ICPDIGITAL };
	sspICPdigitalInput m_editInput;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_strName;
	unsigned char m_nAddress;
	unsigned char m_nChannel;
	float m_fUpdateInterval;
  int m_nPort;

public:
	afx_msg void OnInputConditional();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
#pragma once


// CInputParallelDialog dialog

class CInputParallelDialog : public CInputEditor
{
	DECLARE_DYNAMIC(CInputParallelDialog)

public:
	CInputParallelDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputParallelDialog();

// Dialog Data
	enum { IDD = IDD_INPUT_PARALLEL };
	sspParallelInput m_editInput;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_strName;
	float m_fUpdateInterval;

public:
	afx_msg void OnInput(unsigned int nID);
	afx_msg void OnSelectConditional(unsigned int nID);
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
#pragma once


// CInputMIDIeventDialog dialog

class CInputMIDIeventDialog : public CInputEditor
{
	DECLARE_DYNAMIC(CInputMIDIeventDialog)

public:
	CInputMIDIeventDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputMIDIeventDialog();

// Dialog Data
	enum { IDD = IDD_INPUT_MIDIEVENT };
	sspMIDIeventInput m_editInput;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_strName;
	int m_nMsgType;
	float m_fUpdateInterval;
	int m_nDevice;
	CListBox m_lbDevice;
	BOOL m_bChannel;
	int m_nChannelLow;
	int m_nChannelHigh;
	BOOL m_bData1;
	int m_nData1Low;
	int m_nData1High;
	BOOL m_bData2;
	int m_nData2Low;
	int m_nData2High;

public:
	afx_msg void OnInputConditional();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedChannel();
	afx_msg void OnBnClickedData1();
	afx_msg void OnBnClickedData2();
	afx_msg void OnEnChangeChannelLow();
	afx_msg void OnEnChangeData1Low();
	afx_msg void OnEnChangeData2Low();

private:
	void updateControls();
};
