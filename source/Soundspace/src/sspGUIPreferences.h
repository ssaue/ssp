// sspGUIPreferences.h: interface for the sspSetting class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

// sspGUIPrefPageAudio dialog

class sspGUIPrefPageAudio : public CPropertyPage
{
	DECLARE_DYNAMIC(sspGUIPrefPageAudio)

protected:
	static const unsigned int m_nAudioFrequency[];
	static const unsigned int m_nFreqCount;

	int		m_nFreqIndex;

public:
	sspGUIPrefPageAudio();
	virtual ~sspGUIPrefPageAudio();

	int		m_nFrequency;
	unsigned int	m_nBufferLength;
	unsigned int	m_nBufferUpdate;

// Dialog Data
	enum { IDD = IDD_PREFPAGE_AUDIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};
#pragma once


// sspGUIPrefPageFade dialog

class sspGUIPrefPageFade : public CPropertyPage
{
	DECLARE_DYNAMIC(sspGUIPrefPageFade)

public:
	sspGUIPrefPageFade();
	virtual ~sspGUIPrefPageFade();

 	unsigned int	m_nFadeIn;
	unsigned int	m_nFadeOut;
	unsigned int	m_nVolume;

// Dialog Data
	enum { IDD = IDD_PREFPAGE_FADETIMES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


// sspGUIPrefPageResolution dialog

class sspGUIPrefPageResolution : public CPropertyPage
{
	DECLARE_DYNAMIC(sspGUIPrefPageResolution)

public:
	sspGUIPrefPageResolution();
	virtual ~sspGUIPrefPageResolution();

	unsigned int	m_nTimer;
	unsigned int	m_nEnvelope;
	unsigned int	m_nEvent;

// Dialog Data
	enum { IDD = IDD_PREFPAGE_RESOLUTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
#pragma once


// sspGUIPrefPageUpdates dialog

class sspGUIPrefPageUpdates : public CPropertyPage
{
	DECLARE_DYNAMIC(sspGUIPrefPageUpdates)

public:
	sspGUIPrefPageUpdates();
	virtual ~sspGUIPrefPageUpdates();

	float m_fEvent;
	float m_fClock;
	float m_fGUI;

// Dialog Data
	enum { IDD = IDD_PREFPAGE_UPDATES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

// sspGUIPreferences

class sspGUIPreferences : public CPropertySheet
{
	DECLARE_DYNAMIC(sspGUIPreferences)

public:
	sspGUIPreferences(unsigned int nIDCaption, CWnd* pParentWnd = NULL, unsigned int iSelectPage = 0);
	sspGUIPreferences(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, unsigned int iSelectPage = 0);
	virtual ~sspGUIPreferences();

  sspGUIPrefPageAudio       m_pageAudio;
  sspGUIPrefPageFade        m_pageFade;
  sspGUIPrefPageResolution  m_pageResolution;
  sspGUIPrefPageUpdates     m_pageUpdates;

protected:
	DECLARE_MESSAGE_MAP()
};


