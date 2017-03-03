#pragma once
#include "afxwin.h"

// CDeviceAudioDialog dialog

class CDeviceAudioDialog : public CDialog
{
	DECLARE_DYNAMIC(CDeviceAudioDialog)

public:
	CDeviceAudioDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceAudioDialog();

// Dialog Data
	enum { IDD = IDD_DEVICES_AUDIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_lbDSDevice;
	CListBox m_lbWavDevice;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
#pragma once


// CDeviceDmxDialog dialog

class CDeviceDmxDialog : public CDialog
{
	DECLARE_DYNAMIC(CDeviceDmxDialog)

public:
	CDeviceDmxDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceDmxDialog();

// Dialog Data
	enum { IDD = IDD_DEVICES_DMX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_lbDevices;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
#pragma once


// CDeviceMidiDialog dialog

class CDeviceMidiDialog : public CDialog
{
	DECLARE_DYNAMIC(CDeviceMidiDialog)

public:
	CDeviceMidiDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceMidiDialog();

// Dialog Data
	enum { IDD = IDD_DEVICES_MIDI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_lbOutDevice;
	CListBox m_lbInDevice;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
