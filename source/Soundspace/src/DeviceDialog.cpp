// C:\Programvare\Soundspace\DeviceDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "devicedialog.h"
#include "sspDeviceManager.h"
#include <vector>

// CDeviceAudioDialog dialog

IMPLEMENT_DYNAMIC(CDeviceAudioDialog, CDialog)
CDeviceAudioDialog::CDeviceAudioDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceAudioDialog::IDD, pParent)
{
}

CDeviceAudioDialog::~CDeviceAudioDialog()
{
}

void CDeviceAudioDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DS_DEVICE, m_lbDSDevice);
	DDX_Control(pDX, IDC_WAV_DEVICE, m_lbWavDevice);
}

BEGIN_MESSAGE_MAP(CDeviceAudioDialog, CDialog)
END_MESSAGE_MAP()

// CDeviceAudioDialog message handlers

BOOL CDeviceAudioDialog::OnInitDialog()
{
	unsigned int i, nCount;
	sspIntVec subset;
	sspDeviceGroup* pGroup;

	CDialog::OnInitDialog();

	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_DIRECTSOUND];
	nCount = pGroup->getAllDevices();
	m_lbDSDevice.ResetContent();
	for (i=0; i<nCount; ++i)
		m_lbDSDevice.InsertString(i, pGroup->getDeviceName(i).c_str());
	subset = pGroup->getSubset();
	for (i=0; i<subset.size(); ++i) {
		if (subset[i] < (int)nCount)
			m_lbDSDevice.SetSel(subset[i]);
	}
	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_WAV];
	nCount = pGroup->getAllDevices();
	m_lbWavDevice.ResetContent();
	for (i=0; i<nCount; ++i)
		m_lbWavDevice.InsertString(i, pGroup->getDeviceName(i).c_str());
	subset = pGroup->getSubset();
	for (i=0; i<subset.size(); ++i) {
		if (subset[i] < (int)nCount)
			m_lbWavDevice.SetSel(subset[i]);
	}
	return TRUE;
}

void CDeviceAudioDialog::OnOK()
{
	unsigned int nCount;
	sspIntVec subset;
	sspDeviceGroup* pGroup;

	// Get DirectSound selection
	nCount = m_lbDSDevice.GetSelCount();
  subset.resize(nCount);
  if (nCount > 0) {
  	m_lbDSDevice.GetSelItems(nCount, &subset[0]);
  }
	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_DIRECTSOUND];
	pGroup->setSubset(subset);

	// Get MME wav selection
	nCount = m_lbWavDevice.GetSelCount();
  subset.resize(nCount);
  if (nCount > 0) {
  	m_lbWavDevice.GetSelItems(nCount, &subset[0]);
  }
	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_WAV];
	pGroup->setSubset(subset);

	CDialog::OnOK();
}

// CDeviceDmxDialog dialog

IMPLEMENT_DYNAMIC(CDeviceDmxDialog, CDialog)
CDeviceDmxDialog::CDeviceDmxDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceDmxDialog::IDD, pParent)
{
}

CDeviceDmxDialog::~CDeviceDmxDialog()
{
}

void CDeviceDmxDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DMX_DEVICES, m_lbDevices);
}


BEGIN_MESSAGE_MAP(CDeviceDmxDialog, CDialog)
END_MESSAGE_MAP()


// CDeviceDmxDialog message handlers

BOOL CDeviceDmxDialog::OnInitDialog()
{
	unsigned int i, nCount;
	sspIntVec subset;
	sspDeviceGroup* pGroup;

	CDialog::OnInitDialog();

	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_DMX];
	nCount = pGroup->getAllDevices();
	m_lbDevices.ResetContent();
	for (i=0; i<nCount; ++i)
		m_lbDevices.InsertString(i, pGroup->getDeviceName(i).c_str());
	subset = pGroup->getSubset();
	for (i=0; i<subset.size(); ++i) {
		if (subset[i] < (int)nCount)
			m_lbDevices.SetSel(subset[i]);
	}
	return TRUE;
}

void CDeviceDmxDialog::OnOK()
{
	unsigned int nCount;
	sspIntVec subset;
	sspDeviceGroup* pGroup;

	// Get DirectSound selection
	nCount = m_lbDevices.GetSelCount();
  subset.resize(nCount);
  if (nCount > 0) {
  	m_lbDevices.GetSelItems(nCount, &subset[0]);
  }
	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_DMX];
	pGroup->setSubset(subset);

	CDialog::OnOK();
}

// CDeviceMidiDialog dialog

IMPLEMENT_DYNAMIC(CDeviceMidiDialog, CDialog)
CDeviceMidiDialog::CDeviceMidiDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceMidiDialog::IDD, pParent)
{
}

CDeviceMidiDialog::~CDeviceMidiDialog()
{
}

void CDeviceMidiDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MIDIOUT_DEVICE, m_lbOutDevice);
	DDX_Control(pDX, IDC_MIDIIN_DEVICE, m_lbInDevice);
}


BEGIN_MESSAGE_MAP(CDeviceMidiDialog, CDialog)
END_MESSAGE_MAP()


// CDeviceMidiDialog message handlers

BOOL CDeviceMidiDialog::OnInitDialog()
{
	unsigned int i, nCount;
	sspIntVec subset;
	sspDeviceGroup* pGroup;

	CDialog::OnInitDialog();

	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT];
	nCount = pGroup->getAllDevices();
	m_lbOutDevice.ResetContent();
	for (i=0; i<nCount; ++i)
		m_lbOutDevice.InsertString(i, pGroup->getDeviceName(i).c_str());
	subset = pGroup->getSubset();
	for (i=0; i<subset.size(); ++i) {
		if (subset[i] < (int)nCount)
			m_lbOutDevice.SetSel(subset[i]);
	}
	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_MIDIIN];
	nCount = pGroup->getAllDevices();
	m_lbInDevice.ResetContent();
	for (i=0; i<nCount; ++i)
		m_lbInDevice.InsertString(i, pGroup->getDeviceName(i).c_str());
	subset = pGroup->getSubset();
	for (i=0; i<subset.size(); ++i) {
		if (subset[i] < (int)nCount)
			m_lbInDevice.SetSel(subset[i]);
	}
	return TRUE;
}

void CDeviceMidiDialog::OnOK()
{
	unsigned int nCount;
	sspIntVec subset;
	sspDeviceGroup* pGroup;

	// Get MIDI OUT selection
	nCount = m_lbOutDevice.GetSelCount();
  subset.resize(nCount);
  if (nCount > 0) {
	  m_lbOutDevice.GetSelItems(nCount, &subset[0]);
  }
	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT];
	pGroup->setSubset(subset);

	// Get MIDI IN selection
	nCount = m_lbInDevice.GetSelCount();
  subset.resize(nCount);
  if (nCount > 0) {
  	m_lbInDevice.GetSelItems(nCount, &subset[0]);
  }
	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_MIDIIN];
	pGroup->setSubset(subset);

	CDialog::OnOK();
}
