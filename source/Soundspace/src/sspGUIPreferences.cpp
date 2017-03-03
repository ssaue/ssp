// sspGUIPreferences.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspGUIPreferences.h"

// sspGUIPrefPageAudio dialog

const unsigned int sspGUIPrefPageAudio::m_nAudioFrequency[] = {11025, 22050, 44100, 48000, 96000};
const unsigned int sspGUIPrefPageAudio::m_nFreqCount = 5;		// Number of alternative sampling frequencies

IMPLEMENT_DYNAMIC(sspGUIPrefPageAudio, CPropertyPage)
sspGUIPrefPageAudio::sspGUIPrefPageAudio()
	: CPropertyPage(sspGUIPrefPageAudio::IDD)
{
	m_nFreqIndex = -1;
	m_nFrequency = 44100;
	m_nBufferLength = 0;
	m_nBufferUpdate = 0;
}

sspGUIPrefPageAudio::~sspGUIPrefPageAudio()
{
}

void sspGUIPrefPageAudio::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_FREQUENCY, m_nFreqIndex);
	DDX_Text(pDX, IDC_BUFFER_LENGTH, m_nBufferLength);
	DDV_MinMaxUInt(pDX, m_nBufferLength, 1000, 5000);
	DDX_Text(pDX, IDC_BUFFER_UPDATE, m_nBufferUpdate);
	DDV_MinMaxUInt(pDX, m_nBufferUpdate, 500, 1000);
}

BEGIN_MESSAGE_MAP(sspGUIPrefPageAudio, CPropertyPage)
END_MESSAGE_MAP()

BOOL sspGUIPrefPageAudio::OnInitDialog()
{
  unsigned int i;
	for (i=0; i<m_nFreqCount; i++) {
		if (m_nFrequency == m_nAudioFrequency[i]) {
			m_nFreqIndex = i;
			break;
		}
	}
	return CPropertyPage::OnInitDialog();
}

void sspGUIPrefPageAudio::OnOK()
{
	if (m_nFreqIndex >= 0) m_nFrequency = m_nAudioFrequency[m_nFreqIndex];
	CPropertyPage::OnOK();
}

// sspGUIPrefPageFade dialog

IMPLEMENT_DYNAMIC(sspGUIPrefPageFade, CPropertyPage)
sspGUIPrefPageFade::sspGUIPrefPageFade()
	: CPropertyPage(sspGUIPrefPageFade::IDD)
{
	m_nFadeIn = 0;
	m_nFadeOut = 0;
	m_nVolume = 0;
}

sspGUIPrefPageFade::~sspGUIPrefPageFade()
{
}

void sspGUIPrefPageFade::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FADE_IN, m_nFadeIn);
	DDV_MinMaxUInt(pDX, m_nFadeIn, 0, 99);
	DDX_Text(pDX, IDC_FADE_OUT, m_nFadeOut);
	DDV_MinMaxUInt(pDX, m_nFadeOut, 0, 99);
	DDX_Text(pDX, IDC_VOLUME_CHANGE, m_nVolume);
	DDV_MinMaxUInt(pDX, m_nVolume, 0, 99);
}


BEGIN_MESSAGE_MAP(sspGUIPrefPageFade, CPropertyPage)
END_MESSAGE_MAP()


// sspGUIPrefPageResolution dialog

IMPLEMENT_DYNAMIC(sspGUIPrefPageResolution, CPropertyPage)
sspGUIPrefPageResolution::sspGUIPrefPageResolution()
	: CPropertyPage(sspGUIPrefPageResolution::IDD)
{
}

sspGUIPrefPageResolution::~sspGUIPrefPageResolution()
{
}

void sspGUIPrefPageResolution::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIMER_RES, m_nTimer);
	DDV_MinMaxUInt(pDX, m_nTimer, 1, 100);
	DDX_Text(pDX, IDC_ENVELOPE_RES, m_nEnvelope);
	DDV_MinMaxUInt(pDX, m_nEnvelope, 1, 1000);
	DDX_Text(pDX, IDC_EVENT_RES, m_nEvent);
	DDV_MinMaxUInt(pDX, m_nEvent, 10, 1000);
}


BEGIN_MESSAGE_MAP(sspGUIPrefPageResolution, CPropertyPage)
END_MESSAGE_MAP()


// sspGUIPrefPageUpdates dialog

IMPLEMENT_DYNAMIC(sspGUIPrefPageUpdates, CPropertyPage)
sspGUIPrefPageUpdates::sspGUIPrefPageUpdates()
	: CPropertyPage(sspGUIPrefPageUpdates::IDD)
{
	m_fEvent = 1.0f;
	m_fClock = 10.0f;
	m_fGUI = 10.0f;
}

sspGUIPrefPageUpdates::~sspGUIPrefPageUpdates()
{
}

void sspGUIPrefPageUpdates::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EVENTSTEP, m_fEvent);
	DDV_MinMaxFloat(pDX, m_fEvent, 0.01f, 60.0f);
	DDX_Text(pDX, IDC_CLOCKSTEP, m_fClock);
	DDV_MinMaxFloat(pDX, m_fClock, 1.0f, 60.0f);
	DDX_Text(pDX, IDC_GUISTEP, m_fGUI);
	DDV_MinMaxFloat(pDX, m_fGUI, 1.0f, 60.0f);
}


BEGIN_MESSAGE_MAP(sspGUIPrefPageUpdates, CPropertyPage)
END_MESSAGE_MAP()


// sspGUIPreferences

IMPLEMENT_DYNAMIC(sspGUIPreferences, CPropertySheet)
sspGUIPreferences::sspGUIPreferences(unsigned int nIDCaption, CWnd* pParentWnd, unsigned int iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

sspGUIPreferences::sspGUIPreferences(LPCTSTR pszCaption, CWnd* pParentWnd, unsigned int iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
  AddPage(&m_pageAudio);
  AddPage(&m_pageUpdates);
  AddPage(&m_pageResolution);
  AddPage(&m_pageFade);
}

sspGUIPreferences::~sspGUIPreferences()
{
}


BEGIN_MESSAGE_MAP(sspGUIPreferences, CPropertySheet)
END_MESSAGE_MAP()


// sspGUIPreferences message handlers
