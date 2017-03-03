// StreamDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "StreamDialog.h"
#include "ValueDialog.h"
#include "sspPools.h"
#include "sspDeviceManager.h"
#include "SSpStream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStreamDialog dialog


CStreamDialog::CStreamDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStreamDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStreamDialog)
	m_nStreamType = -1;
	//}}AFX_DATA_INIT
	m_pStream = NULL;
	m_pMain = NULL;
}


void CStreamDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStreamDialog)
	DDX_Control(pDX, IDC_SELECT_STREAM, m_listStream);
	DDX_CBIndex(pDX, IDC_SELECT_TYPE, m_nStreamType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStreamDialog, CDialog)
	//{{AFX_MSG_MAP(CStreamDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_REPLACE, OnReplace)
	ON_LBN_SELCHANGE(IDC_SELECT_STREAM, OnSelectStream)
	ON_CBN_SELCHANGE(IDC_SELECT_TYPE, OnSelectType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CStreamDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPLACE)->EnableWindow(FALSE);
	m_listStream.ResetContent();
	for (unsigned int i=0; i<sspPool::Instance().streams.GetSize(); i++)
		m_listStream.InsertString(-1, sspPool::Instance().streams.GetName(i));
	return TRUE;
}

void CStreamDialog::OnAdd() 
{
	int nSelect = -1;
	CStreamBasicDialog dlg;
	switch (m_nStreamType) {
	case SSP_STREAM_TIME:
		dlg.m_pStream = new SSpStream;
		dlg.m_bAudioStream = FALSE;
		break;
	case SSP_STREAM_TASK:
		dlg.m_pStream = new SSpTaskStream;
		dlg.m_bAudioStream = FALSE;
		break;
	case SSP_STREAM_AUDIO:
		dlg.m_pStream = new SSpAudioStream;
		dlg.m_bAudioStream = TRUE;
		break;
	default:
		return;
	}
	if (dlg.DoModal() == IDOK) {
		sspPool::Instance().streams.LoadStream(dlg.m_pStream, dlg.m_strName);
		nSelect = m_listStream.InsertString(-1, dlg.m_strName);
	}
	else
		delete dlg.m_pStream;
	m_listStream.SetSel(-1, FALSE);
	m_listStream.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_listStream.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_listStream.GetSelCount() == 1);
}

void CStreamDialog::OnEdit() 
{
	if (m_listStream.GetSelCount() != 1 || m_pStream == NULL)
		return;

	int nSelect;
	m_listStream.GetSelItems(1,&nSelect);

	CStreamBasicDialog dlg;
	nSelect = m_listStream.GetCurSel();
	dlg.m_strName = sspPool::Instance().streams.GetName(nSelect);
	dlg.m_pStream = m_pStream;
	if (m_nStreamType == SSP_STREAM_AUDIO) {
		dlg.m_bAudioStream = TRUE;
	}
	else 
		dlg.m_bAudioStream = FALSE;

	if (dlg.DoModal()) {
		sspPool::Instance().streams.ExchangeStreamName(dlg.m_strName, nSelect);
		m_listStream.DeleteString(nSelect);
		nSelect = m_listStream.InsertString(nSelect, dlg.m_strName);
	}
	m_listStream.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_listStream.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_listStream.GetSelCount() == 1);

}

void CStreamDialog::OnReplace() 
{
	if (m_listStream.GetSelCount() != 1)
		return;

	int nSelect;
	m_listStream.GetSelItems(1,&nSelect);

	CStreamBasicDialog dlg;
	nSelect = m_listStream.GetCurSel();
	switch (m_nStreamType) {
	case SSP_STREAM_TIME:
		dlg.m_pStream = new SSpStream;
		dlg.m_bAudioStream = FALSE;
		break;
	case SSP_STREAM_TASK:
		dlg.m_pStream = new SSpTaskStream;
		dlg.m_bAudioStream = FALSE;
		break;
	case SSP_STREAM_AUDIO:
		dlg.m_pStream = new SSpAudioStream;
		dlg.m_bAudioStream = TRUE;
		break;
	default:
		return;
	}
	if (dlg.DoModal() == IDOK) {
		sspPool::Instance().streams.ExchangeStream(dlg.m_pStream, dlg.m_strName, nSelect);
		m_listStream.DeleteString(nSelect);
		nSelect = m_listStream.InsertString(nSelect, dlg.m_strName);
	}
	else
		delete dlg.m_pStream;
	m_listStream.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_listStream.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_listStream.GetSelCount() == 1);
}

void CStreamDialog::OnSelectStream() 
{
	m_nStreamType = -1;
	if (m_listStream.GetSelCount() == 1) {
		int nSelect;
		m_listStream.GetSelItems(1,&nSelect);
		m_pStream = sspPool::Instance().streams.GetStream(nSelect);
		if (m_pStream->IsKindOf( RUNTIME_CLASS( SSpAudioStream ) ))
			m_nStreamType = SSP_STREAM_AUDIO;
		else if (m_pStream->IsKindOf( RUNTIME_CLASS( SSpTaskStream ) ))
			m_nStreamType = SSP_STREAM_TASK;
		else if (m_pStream->IsKindOf( RUNTIME_CLASS( SSpStream ) ))
			m_nStreamType = SSP_STREAM_TIME;
		else
			m_nStreamType = -1;
	}
	((CComboBox*) GetDlgItem(IDC_SELECT_TYPE))->SetCurSel(m_nStreamType);
	GetDlgItem(IDC_EDIT)->EnableWindow(m_nStreamType >= 0);
	GetDlgItem(IDC_ADD)->EnableWindow(m_nStreamType >= 0 && m_pStream != m_pMain);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_pStream != m_pMain && m_nStreamType >= 0);
}

void CStreamDialog::OnSelectType() 
{
	m_nStreamType = ((CComboBox*) GetDlgItem(IDC_SELECT_TYPE))->GetCurSel();
	GetDlgItem(IDC_ADD)->EnableWindow(m_nStreamType >= 0);
}

void CStreamDialog::OnOK() 
{
	int nSize = m_listStream.GetSelCount();
	m_nSelection.resize(nSize);
	if (nSize > 0) m_listStream.GetSelItems(nSize,&m_nSelection[0]);
	CDialog::OnOK();
}


/////////////////////////////////////////////////////////////////////////////
// CStreamBasicDialog dialog


CStreamBasicDialog::CStreamBasicDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStreamBasicDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStreamBasicDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_pStream = NULL;
	m_bAudioStream = FALSE;
	m_nSelected = -1;
	m_nFactor = -1;
}


void CStreamBasicDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStreamBasicDialog)
	DDX_Text(pDX, IDC_STREAM_NAME, m_strName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SUBSTREAM, m_lbStreams);
}


BEGIN_MESSAGE_MAP(CStreamBasicDialog, CDialog)
	//{{AFX_MSG_MAP(CStreamBasicDialog)
	ON_BN_CLICKED(IDC_LOAD_DEVICE, OnLoadDevice)
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_SUBSTREAM, OnSelectSubstream)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_SELECT_FACTOR, OnSelectFactor)
END_MESSAGE_MAP()

BOOL CStreamBasicDialog::OnInitDialog() 
{
	unsigned int i;
	CDialog::OnInitDialog();
	GetDlgItem(IDC_LOAD_DEVICE)->EnableWindow(m_bAudioStream);
	m_streams = m_pStream->getSubStreams();
	m_lbStreams.ResetContent();
	for (i=0; i<m_streams.size(); ++i) {
		m_lbStreams.InsertString(-1, sspPool::Instance().streams.GetName(m_streams[i]));
	}
	m_nFactor = m_pStream->getTimeFactor();
	if (m_nFactor >= 0)
		GetDlgItem(IDC_SELECT_FACTOR)->SetWindowText(sspPool::Instance().values.GetName(m_nFactor));
	return true;
}

void CStreamBasicDialog::OnAdd()
{
	CStreamDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_streams.insert(m_streams.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbStreams.AddString(sspPool::Instance().streams.GetName(dlg.m_nSelection[i]));
		}
		m_nSelected = m_streams.size()-1;
		m_lbStreams.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CStreamBasicDialog::OnRemove()
{
	if (m_nSelected >= 0 && m_nSelected < (int) m_streams.size()) {
		m_streams.erase(m_streams.begin() + m_nSelected);
		m_lbStreams.DeleteString(m_nSelected);
		if (m_nSelected == (int) m_streams.size()) m_nSelected--;
		m_lbStreams.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CStreamBasicDialog::OnSelectFactor()
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_nFactor = dlg.m_nSelection[0];
		GetDlgItem(IDC_SELECT_FACTOR)->SetWindowText(sspPool::Instance().values.GetName(m_nFactor));
	}
}

void CStreamBasicDialog::OnSelectSubstream()
{
	m_nSelected = m_lbStreams.GetCurSel();
	UpdateButtons();
}

void CStreamBasicDialog::OnLoadDevice() 
{
	CStreamDeviceDialog dlg;
	dlg.m_pStream = dynamic_cast<SSpAudioStream*>(m_pStream);
	if (dlg.m_pStream != NULL)
		dlg.DoModal();
}

void CStreamBasicDialog::UpdateButtons()
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_nSelected >= 0);
}

void CStreamBasicDialog::OnOK() 
{
	if (m_nFactor < 0 || m_nFactor > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid time factor selection.");
		return;
	}
	CDialog::OnOK();
	m_pStream->setSubStreams(m_streams);
	m_pStream->setTimeFactor(m_nFactor);
}

/////////////////////////////////////////////////////////////////////////////
// CStreamDeviceDialog dialog

CStreamDeviceDialog::CStreamDeviceDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStreamDeviceDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStreamDeviceDialog)
	m_nWaitBuffers = 0;
	m_nPlayBuffers = 0;
	m_nStereo = MONO;
	m_bFader = FALSE;
	//}}AFX_DATA_INIT
	m_pStream = NULL;
}

void CStreamDeviceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStreamDeviceDialog)
	DDX_Text(pDX, IDC_WAIT_BUFFERS, m_nWaitBuffers);
	DDV_MinMaxUInt(pDX, m_nWaitBuffers, 0, 10);
	DDX_Text(pDX, IDC_PLAY_BUFFERS, m_nPlayBuffers);
	DDX_Radio(pDX, IDC_STEREO, m_nStereo);
	DDX_Check(pDX, IDC_ENABLE_FADER, m_bFader);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_AUDIO_DEVICE, m_lbDSlist);
	DDX_Control(pDX, IDC_VOLUME_DEVICE, m_lbWAVlist);
}

BEGIN_MESSAGE_MAP(CStreamDeviceDialog, CDialog)
	//{{AFX_MSG_MAP(CStreamDeviceDialog)
	ON_BN_CLICKED(IDC_MONO, OnMono)
	ON_BN_CLICKED(IDC_STEREO, OnStereo)
	ON_BN_CLICKED(IDC_BTN_FADERVALUE, OnSelectValue)
	ON_BN_CLICKED(IDC_ENABLE_FADER, OnEnableFader)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CStreamDeviceDialog::OnInitDialog() 
{
	m_nFaderValue = m_pStream->getFaderValue();
	m_bFader = (m_nFaderValue >= 0);
	if (m_bFader) {
		GetDlgItem(IDC_BTN_FADERVALUE)->SetWindowText(sspPool::Instance().values.GetName(m_nFaderValue));
	}
	GetDlgItem(IDC_VOLUME_DEVICE)->EnableWindow(m_bFader);
	GetDlgItem(IDC_BTN_FADERVALUE)->EnableWindow(m_bFader);

	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_PBUFFER_SPIN);
	pSpin->SetRange(1,10);
	pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_WBUFFER_SPIN);
	pSpin->SetRange(0,10);
	m_nPlayBuffers = m_pStream->getPlayBuffers();
	m_nWaitBuffers = m_pStream->getWaitBuffers();

	m_nStereo = MONO;
	CDialog::OnInitDialog();	
	displayMono();
	sspIntVec nDevices = m_pStream->getDSDevices();
	for (unsigned int i=0; i<nDevices.size(); i++)
		m_lbDSlist.SetSel(nDevices[i]);
  if (m_bFader) {
    nDevices = m_pStream->getWavDevices();
	  for (unsigned int i=0; i<nDevices.size(); i++)
		  m_lbWAVlist.SetSel(nDevices[i]);
  }
	return TRUE;	
}

void CStreamDeviceDialog::OnMono() 
{
	sspIntVec nDSvec = getDSselection();
	sspIntVec nWAVvec = getWAVselection();
	UpdateData(TRUE);
	m_lbDSlist.ResetContent();
	m_lbWAVlist.ResetContent();
	displayMono();
	for (unsigned int i=0; i<nDSvec.size(); i++)
		m_lbDSlist.SetSel(nDSvec[i]);
	for (unsigned int i=0; i<nWAVvec.size(); i++)
		m_lbWAVlist.SetSel(nWAVvec[i]);
}

void CStreamDeviceDialog::OnStereo() 
{
	sspIntVec nDSvec = getDSselection();
	sspIntVec nWAVvec = getWAVselection();
	UpdateData(TRUE);
	m_lbDSlist.ResetContent();
	m_lbWAVlist.ResetContent();
	displayStereo();
	for (unsigned int i=0; i<nDSvec.size(); i++)
		m_lbDSlist.SetSel(nDSvec[i]/2);
	for (unsigned int i=0; i<nWAVvec.size(); i++)
		m_lbWAVlist.SetSel(nWAVvec[i]/2);
}

void CStreamDeviceDialog::OnSelectValue() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_nFaderValue = dlg.m_nSelection[0];
		GetDlgItem(IDC_BTN_FADERVALUE)->SetWindowText(sspPool::Instance().values.GetName(m_nFaderValue));
	}
}

void CStreamDeviceDialog::OnEnableFader() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_VOLUME_DEVICE)->EnableWindow(m_bFader);
	GetDlgItem(IDC_BTN_FADERVALUE)->EnableWindow(m_bFader);
}

void CStreamDeviceDialog::OnOK() 
{
	CDialog::OnOK();

	m_pStream->setPlayBuffers(m_nPlayBuffers);
	m_pStream->setWaitBuffers(m_nWaitBuffers);
	m_pStream->setFaderValue(m_nFaderValue);

	m_pStream->setDSDevices(getDSselection());
	m_pStream->setWavDevices(getWAVselection());
}

void CStreamDeviceDialog::displayMono() 
{
	unsigned int i;
	m_lbDSlist.ResetContent();
	sspDeviceGroup* pGroup = sspDeviceManager::Instance()[SSP_DEVICE_DIRECTSOUND];
	for (i=0; i<pGroup->getSubsetSize(); i++) {
		CString txt = pGroup->getSubsetName(i).c_str();
		m_lbDSlist.InsertString(-1, txt + _T(" - L"));
		m_lbDSlist.InsertString(-1, txt + _T(" - R"));
	}
	m_lbWAVlist.ResetContent();
	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_WAV];
	for (i=0; i<pGroup->getSubsetSize(); i++) {
		CString txt = pGroup->getSubsetName(i).c_str();
		m_lbWAVlist.InsertString(-1, txt + _T(" - L"));
		m_lbWAVlist.InsertString(-1, txt + _T(" - R"));
	}
}

void CStreamDeviceDialog::displayStereo() 
{
	unsigned int i;
	m_lbDSlist.ResetContent();
	sspDeviceGroup* pGroup = sspDeviceManager::Instance()[SSP_DEVICE_DIRECTSOUND];
	for (i=0; i<pGroup->getSubsetSize(); i++)
		m_lbDSlist.InsertString(-1, pGroup->getSubsetName(i).c_str());
	m_lbWAVlist.ResetContent();
	pGroup = sspDeviceManager::Instance()[SSP_DEVICE_WAV];
	for (i=0; i<pGroup->getSubsetSize(); i++)
		m_lbWAVlist.InsertString(-1, pGroup->getSubsetName(i).c_str());
}

sspIntVec CStreamDeviceDialog::getDSselection()
{
	sspIntVec subset;
  unsigned int nCount = m_lbDSlist.GetSelCount();
  if (nCount > 0) {
	  if (m_nStereo == MONO) {
		  subset.resize(nCount);
		  m_lbDSlist.GetSelItems(nCount, &subset[0]);
	  }
	  else if (m_nStereo == STEREO) {
		  sspIntVec subsetHalf;
		  subsetHalf.resize(nCount);
		  m_lbDSlist.GetSelItems(nCount, &subsetHalf[0]);
		  subset.resize(nCount * 2);
		  for (size_t i=0; i<nCount; ++i) {
			  subset[2*i] = 2*subsetHalf[i];
			  subset[2*i+1] = 2*subsetHalf[i]+1;
		  }
	  }
  }
	return subset;
}

sspIntVec CStreamDeviceDialog::getWAVselection()
{
	sspIntVec subset;
	unsigned int nCount = m_lbWAVlist.GetSelCount();
  if (nCount > 0) {
	  if (m_nStereo == MONO) {
		  subset.resize(nCount);
		  if (nCount > 0) m_lbWAVlist.GetSelItems(nCount, &subset[0]);
	  }
	  else if (m_nStereo == STEREO) {
		  sspIntVec subsetHalf;
		  subsetHalf.resize(nCount);
		  if (nCount > 0) m_lbWAVlist.GetSelItems(nCount, &subsetHalf[0]);
		  subset.resize(nCount * 2);
		  for (size_t i=0; i<nCount; ++i) {
			  subset[2*i] = 2*subsetHalf[i];
			  subset[2*i+1] = 2*subsetHalf[i]+1;
		  }
	  }
  }
	return subset;
}
