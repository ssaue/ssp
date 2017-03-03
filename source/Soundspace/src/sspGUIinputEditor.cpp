// C:\Programvare\Soundspace\sspGUIinputEditor.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspGUIinputEditor.h"
#include "ConditionalDialog.h"
#include "ValueDialog.h"
#include "sspPools.h"
#include "sspDeviceManager.h"
#include "SSpBasic.h"

// CInputICPanalogDialog dialog

IMPLEMENT_DYNAMIC(CInputICPanalogDialog, CDialog)
CInputICPanalogDialog::CInputICPanalogDialog(CWnd* pParent /*=NULL*/)
	: CInputEditor(CInputICPanalogDialog::IDD, pParent)
	, m_strName(_T(""))
	, m_nAddress(0)
	, m_nChannel(0)
	, m_fUpdateInterval(0)
  , m_nPort(0)
{
}

CInputICPanalogDialog::~CInputICPanalogDialog()
{
}

void CInputICPanalogDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_NAME, m_strName);
  DDX_Text(pDX, IDC_ADDRESS, m_nAddress);
  DDX_Text(pDX, IDC_CHANNEL, m_nChannel);
  DDX_Text(pDX, IDC_UPDATEINT, m_fUpdateInterval);
  DDX_CBIndex(pDX, IDC_SELECTPORT, m_nPort);
	DDV_MinMaxInt(pDX, m_nPort, 0, 4);
}


BEGIN_MESSAGE_MAP(CInputICPanalogDialog, CDialog)
	ON_BN_CLICKED(IDC_INPUT_VALUE, OnInputValue)
END_MESSAGE_MAP()


BOOL CInputICPanalogDialog::OnInitDialog()
{
	if (m_pInput) {
		sspICPanalogInput* pInput = dynamic_cast<sspICPanalogInput*>(m_pInput);
		if (!pInput) {
			AfxMessageBox(L"Unable to assign input.");
			EndDialog(IDCANCEL);
		}
		m_editInput = *pInput;
	}
	m_strName = m_editInput.getName().c_str();
  m_nPort = m_editInput.getPort() - 1;
	m_nAddress = m_editInput.getAddress();
	m_nChannel = m_editInput.getChannel();
	m_fUpdateInterval = m_editInput.getUpdateInterval();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
  if (m_editInput.getValue() >= 0)
		GetDlgItem(IDC_INPUT_VALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editInput.getValue()));
	return TRUE;
}

void CInputICPanalogDialog::OnInputValue()
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editInput.setValue(dlg.m_nSelection[0]);
		GetDlgItem(IDC_INPUT_VALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editInput.getValue()));
	}
}

void CInputICPanalogDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editInput.getValue() < 0 || m_editInput.getValue() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid input value selection.");
		return;
	}
	else if (sspPool::Instance().values.getValue(m_editInput.getValue())->getType() != SSP_VALUE_SINGLE) {
		AfxMessageBox(L"Value must be of basic type.");
		return;
	}
	if (m_fUpdateInterval <= 0) {
		AfxMessageBox(L"Update interval is too short.");
		return;
	}

  m_editInput.setName((LPCTSTR) m_strName);
	m_editInput.setPort((unsigned short) m_nPort + 1);
	m_editInput.setAddress(m_nAddress);
	m_editInput.setChannel(m_nChannel);
	m_editInput.setUpdateInterval(m_fUpdateInterval);
	CDialog::OnOK();
	if (m_pInput)
		*((sspICPanalogInput*) m_pInput) = m_editInput;
	else
		m_pInput = new sspICPanalogInput(m_editInput);
}

/////////////////////////////
// CInputICPdigitalDialog dialog

IMPLEMENT_DYNAMIC(CInputICPdigitalDialog, CDialog)
CInputICPdigitalDialog::CInputICPdigitalDialog(CWnd* pParent /*=NULL*/)
	: CInputEditor(CInputICPdigitalDialog::IDD, pParent)
	, m_strName(_T(""))
	, m_nAddress(0)
	, m_nChannel(0)
	, m_fUpdateInterval(0)
  , m_nPort(0)
{
}

CInputICPdigitalDialog::~CInputICPdigitalDialog()
{
}

void CInputICPdigitalDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_ADDRESS, m_nAddress);
	DDX_Text(pDX, IDC_CHANNEL, m_nChannel);
	DDX_Text(pDX, IDC_UPDATEINT, m_fUpdateInterval);
  DDX_CBIndex(pDX, IDC_SELECTPORT, m_nPort);
	DDV_MinMaxInt(pDX, m_nPort, 0, 4);
}


BEGIN_MESSAGE_MAP(CInputICPdigitalDialog, CDialog)
	ON_BN_CLICKED(IDC_INPUT_CONDITIONAL, OnInputConditional)
END_MESSAGE_MAP()


BOOL CInputICPdigitalDialog::OnInitDialog()
{
	if (m_pInput) {
		sspICPdigitalInput* pInput = dynamic_cast<sspICPdigitalInput*>(m_pInput);
		if (!pInput) {
			AfxMessageBox(L"Unable to assign input.");
			EndDialog(IDCANCEL);
		}
		m_editInput = *pInput;
	}
	m_strName = m_editInput.getName().c_str();
  m_nPort = m_editInput.getPort() - 1;
	m_nAddress = m_editInput.getAddress();
	m_nChannel = m_editInput.getChannel();
	m_fUpdateInterval = m_editInput.getUpdateInterval();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	if (m_editInput.getConditional() >= 0)
		GetDlgItem(IDC_INPUT_CONDITIONAL)->SetWindowText(sspPool::Instance().conditionals.GetName(m_editInput.getConditional()));
	return TRUE;
}

void CInputICPdigitalDialog::OnInputConditional()
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editInput.setConditional(dlg.m_nSelection[0]);
		GetDlgItem(IDC_INPUT_CONDITIONAL)->SetWindowText(sspPool::Instance().conditionals.GetName(m_editInput.getConditional()));
	}
}

void CInputICPdigitalDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editInput.getConditional() < 0 || m_editInput.getConditional() > (int) sspPool::Instance().conditionals.GetSize()) {
		AfxMessageBox(L"Invalid input conditional selection.");
		return;
	}
	else if (sspPool::Instance().conditionals.GetConditional(m_editInput.getConditional())->getType() != SSP_COND_BOOLEAN) {
		AfxMessageBox(L"Conditional must be of basic boolean type.");
		return;
	}
	if (m_fUpdateInterval <= 0) {
		AfxMessageBox(L"Update interval is too short.");
		return;
	}
	m_editInput.setName((LPCTSTR) m_strName);
	m_editInput.setPort((unsigned short) m_nPort + 1);
	m_editInput.setAddress(m_nAddress);
	m_editInput.setChannel(m_nChannel);
	m_editInput.setUpdateInterval(m_fUpdateInterval);
	CDialog::OnOK();
	if (m_pInput)
		*((sspICPdigitalInput*) m_pInput) = m_editInput;
	else
		m_pInput = new sspICPdigitalInput(m_editInput);
}

// CInputParallelDialog dialog

IMPLEMENT_DYNAMIC(CInputParallelDialog, CDialog)
CInputParallelDialog::CInputParallelDialog(CWnd* pParent /*=NULL*/)
	: CInputEditor(CInputParallelDialog::IDD, pParent)
	, m_strName(_T(""))
	, m_fUpdateInterval(0)
{
}

CInputParallelDialog::~CInputParallelDialog()
{
}

void CInputParallelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_UPDATEINT, m_fUpdateInterval);
}

BEGIN_MESSAGE_MAP(CInputParallelDialog, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_ON1, IDC_ON13, OnInput )
	ON_CONTROL_RANGE(BN_CLICKED, IDC_COND1, IDC_COND13, OnSelectConditional )
END_MESSAGE_MAP()

BOOL CInputParallelDialog::OnInitDialog()
{
	if (m_pInput) {
		sspParallelInput* pInput = dynamic_cast<sspParallelInput*>(m_pInput);
		if (!pInput) {
			AfxMessageBox(L"Unable to assign input.");
			EndDialog(IDCANCEL);
		}
		m_editInput = *pInput;
	}
	m_strName = m_editInput.getName().c_str();
	m_fUpdateInterval = m_editInput.getUpdateInterval();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	CString txt;
	txt.Format(_T("0x%x"), m_editInput.getPort());	// Hex notation
	GetDlgItem(IDC_PORT)->SetWindowText(txt);
	for (int i=0; i<sspParallelInput::PARALLEL_INPUTS; ++i) {
		CButton* pOnBtn = (CButton*) GetDlgItem(IDC_ON1+i);
		CButton* pCondBtn = (CButton*) GetDlgItem(IDC_COND1+i);
		if (m_editInput.getConditional(i) >= 0) {
			pOnBtn->SetCheck(BST_CHECKED);
			pCondBtn->SetWindowText(sspPool::Instance().conditionals.GetName(m_editInput.getConditional(i)));
		}
		else {
			pOnBtn->SetCheck(BST_UNCHECKED);
			pCondBtn->SetWindowText(_T("<< Select conditional >>"));
			pCondBtn->EnableWindow(FALSE);
		}
	}			
	return TRUE;
}

void CInputParallelDialog::OnInput(unsigned int nID)
{
	unsigned int nInput = nID - IDC_ON1;
	if (nInput >= sspParallelInput::PARALLEL_INPUTS)
		return;
	CButton* pOnBtn = (CButton*) GetDlgItem(nID);
	if (pOnBtn->GetCheck() == BST_CHECKED) {
		GetDlgItem(IDC_COND1+nInput)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_COND1 + nInput)->SetWindowText(_T("<< Select conditional >>"));
		GetDlgItem(IDC_COND1+nInput)->EnableWindow(FALSE);
		m_editInput.setConditional(nInput, -1);
	}
}

void CInputParallelDialog::OnSelectConditional(unsigned int nID)
{
	unsigned int nInput = nID - IDC_COND1;
	if (nInput >= sspParallelInput::PARALLEL_INPUTS)
		return;

	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editInput.setConditional(nInput, dlg.m_nSelection[0]);
		GetDlgItem(nID)->SetWindowText(sspPool::Instance().conditionals.GetName(m_editInput.getConditional(nInput)));
	}
}

void CInputParallelDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	for (int i=0; i<sspParallelInput::PARALLEL_INPUTS; ++i) {
		if (m_editInput.getConditional(i) > (int) sspPool::Instance().conditionals.GetSize()) {
			AfxMessageBox(L"Invalid input conditional selection.");
			return;
		}
		else if (m_editInput.getConditional(i) >= 0 && sspPool::Instance().conditionals.GetConditional(m_editInput.getConditional(i))->getType() != SSP_COND_BOOLEAN) {
			AfxMessageBox(L"Conditional must be of basic boolean type.");
			return;
		}
	}
	if (m_fUpdateInterval <= 0) {
		AfxMessageBox(L"Update interval is too short.");
		return;
	}
	m_editInput.setName((LPCTSTR) m_strName);

	// Reading port as hex number
	wchar_t buffer[10];
	GetDlgItem(IDC_PORT)->GetWindowText(buffer, 10);
	wchar_t* stop;
	unsigned short nPort = (unsigned short) wcstoul(buffer, &stop, 0);
	m_editInput.setPort(nPort);

	m_editInput.setUpdateInterval(m_fUpdateInterval);
	CDialog::OnOK();
	if (m_pInput)
		*((sspParallelInput*) m_pInput) = m_editInput;
	else
		m_pInput = new sspParallelInput(m_editInput);
}


// CInputMIDIeventDialog dialog

IMPLEMENT_DYNAMIC(CInputMIDIeventDialog, CDialog)
CInputMIDIeventDialog::CInputMIDIeventDialog(CWnd* pParent /*=NULL*/)
	: CInputEditor(CInputMIDIeventDialog::IDD, pParent)
	, m_strName(_T(""))
	, m_fUpdateInterval(0)
	, m_nMsgType(0)
	, m_bChannel(FALSE)
	, m_nChannelLow(-1)
	, m_nChannelHigh(-1)
	, m_bData1(FALSE)
	, m_nData1Low(-1)
	, m_nData1High(-1)
	, m_bData2(FALSE)
	, m_nData2Low(-1)
	, m_nData2High(-1)
	, m_nDevice(-1)
{
}

CInputMIDIeventDialog::~CInputMIDIeventDialog()
{
}

void CInputMIDIeventDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_UPDATEINT, m_fUpdateInterval);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nMsgType);
	DDX_Text(pDX, IDC_CHANNEL_LOW, m_nChannelLow);
	DDX_Text(pDX, IDC_CHANNEL_HIGH, m_nChannelHigh);
	DDX_Check(pDX, IDC_CHANNEL, m_bChannel);
	DDX_Check(pDX, IDC_DATA1, m_bData1);
	DDX_Text(pDX, IDC_DATA1_LOW, m_nData1Low);
	DDX_Text(pDX, IDC_DATA1_HIGH, m_nData1High);
	DDX_Check(pDX, IDC_DATA2, m_bData2);
	DDX_Text(pDX, IDC_DATA2_LOW, m_nData2Low);
	DDX_Text(pDX, IDC_DATA2_HIGH, m_nData2High);
	DDX_LBIndex(pDX, IDC_DEVICES, m_nDevice);
	DDX_Control(pDX, IDC_DEVICES, m_lbDevice);
}


BEGIN_MESSAGE_MAP(CInputMIDIeventDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHANNEL, OnBnClickedChannel)
	ON_BN_CLICKED(IDC_DATA1, OnBnClickedData1)
	ON_BN_CLICKED(IDC_DATA2, OnBnClickedData2)
	ON_EN_CHANGE(IDC_CHANNEL_LOW, OnEnChangeChannelLow)
	ON_EN_CHANGE(IDC_DATA1_LOW, OnEnChangeData1Low)
	ON_EN_CHANGE(IDC_DATA2_LOW, OnEnChangeData2Low)
	ON_BN_CLICKED(IDC_INPUT_CONDITIONAL, OnInputConditional)
END_MESSAGE_MAP()


BOOL CInputMIDIeventDialog::OnInitDialog()
{
	if (m_pInput) {
		sspMIDIeventInput* pInput = dynamic_cast<sspMIDIeventInput*>(m_pInput);
		if (!pInput) {
			AfxMessageBox(L"Unable to assign input.");
			EndDialog(IDCANCEL);
		}
		m_editInput = *pInput;
	}
	m_strName = m_editInput.getName().c_str();
	m_nDevice = m_editInput.getMidiDevice();
	m_nMsgType = m_editInput.getMsgType();
	m_fUpdateInterval = m_editInput.getUpdateInterval();
	m_editInput.getMsgChannel(m_nChannelLow, m_nChannelHigh);
	m_bChannel = (m_nChannelLow >= 0);
	if (m_bChannel) {
		m_nChannelLow++;
		m_nChannelHigh++;
	}
	m_editInput.getMsgData1(m_nData1Low, m_nData1High);
	m_bData1 = (m_nData1Low >= 0);
	m_editInput.getMsgData2(m_nData2Low, m_nData2High);
	m_bData2 = (m_nData2Low >= 0);
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	m_lbDevice.ResetContent();
	sspDeviceGroup* pGroup = sspDeviceManager::Instance()[SSP_DEVICE_MIDIIN];
	for (unsigned int i=0; i<pGroup->getSubsetSize(); i++)
		m_lbDevice.InsertString(-1, pGroup->getSubsetName(i).c_str());
	m_lbDevice.SetCurSel(m_nDevice);
	if (m_editInput.getConditional() >= 0)
		GetDlgItem(IDC_INPUT_CONDITIONAL)->SetWindowText(sspPool::Instance().conditionals.GetName(m_editInput.getConditional()));
	updateControls();
	return TRUE;
}

void CInputMIDIeventDialog::OnCbnSelchangeCombo1()
{
	UpdateData(TRUE);
	m_bData1 &= (m_nMsgType != sspMIDIeventInput::SSP_MI_ANY_MSG);
	m_bData2 &= (m_nMsgType == sspMIDIeventInput::SSP_MI_CONTROL_MSG);
	updateControls();
}

void CInputMIDIeventDialog::OnBnClickedChannel()
{
	UpdateData(TRUE);
	updateControls();
}

void CInputMIDIeventDialog::OnBnClickedData1()
{
	UpdateData(TRUE);
	updateControls();
}

void CInputMIDIeventDialog::OnBnClickedData2()
{
	UpdateData(TRUE);
	updateControls();
}

void CInputMIDIeventDialog::OnEnChangeChannelLow()
{
	UpdateData(TRUE);
	if (m_nChannelHigh < m_nChannelLow) {
		m_nChannelHigh = m_nChannelLow;
		UpdateData(FALSE);
	}
}

void CInputMIDIeventDialog::OnEnChangeData1Low()
{
	UpdateData(TRUE);
	if (m_nData1High < m_nData1Low) {
		m_nData1High = m_nData1Low;
		UpdateData(FALSE);
	}
}

void CInputMIDIeventDialog::OnEnChangeData2Low()
{
	UpdateData(TRUE);
	if (m_nData2High < m_nData2Low) {
		m_nData2High = m_nData2Low;
		UpdateData(FALSE);
	}
}

void CInputMIDIeventDialog::OnInputConditional()
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editInput.setConditional(dlg.m_nSelection[0]);
		GetDlgItem(IDC_INPUT_CONDITIONAL)->SetWindowText(sspPool::Instance().conditionals.GetName(m_editInput.getConditional()));
	}
}

void CInputMIDIeventDialog::updateControls()
{
	GetDlgItem(IDC_DATA1)->EnableWindow(m_nMsgType != sspMIDIeventInput::SSP_MI_ANY_MSG);
	GetDlgItem(IDC_DATA2)->EnableWindow(m_nMsgType == sspMIDIeventInput::SSP_MI_CONTROL_MSG);
	GetDlgItem(IDC_CHANNEL_LOW)->EnableWindow(m_bChannel);
	GetDlgItem(IDC_CHANNEL_HIGH)->EnableWindow(m_bChannel);
	GetDlgItem(IDC_DATA1_LOW)->EnableWindow(m_bData1);
	GetDlgItem(IDC_DATA1_HIGH)->EnableWindow(m_bData1);
	GetDlgItem(IDC_DATA2_LOW)->EnableWindow(m_bData2);
	GetDlgItem(IDC_DATA2_HIGH)->EnableWindow(m_bData2);
}

void CInputMIDIeventDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editInput.getConditional() < 0 || m_editInput.getConditional() > (int) sspPool::Instance().conditionals.GetSize()) {
		AfxMessageBox(L"Invalid input conditional selection.");
		return;
	}
	else if (sspPool::Instance().conditionals.GetConditional(m_editInput.getConditional())->getType() != SSP_COND_BOOLEAN) {
		AfxMessageBox(L"Conditional must be of basic boolean type.");
		return;
	}
	if (m_nDevice < 0 || m_nDevice > (int)sspDeviceManager::Instance()[SSP_DEVICE_MIDIIN]->getSubsetSize()) {
		AfxMessageBox(L"Invalid MIDI device selection.");
		return;
	}
	if (m_fUpdateInterval <= 0) {
		AfxMessageBox(L"Update interval is too short.");
		return;
	}
	m_editInput.setName((LPCTSTR) m_strName);
	m_editInput.setMidiDevice(m_nDevice);
	m_editInput.setMsgType(m_nMsgType);
	if (m_bChannel)
		m_editInput.setMsgChannel(m_nChannelLow-1, m_nChannelHigh-1);
	else
		m_editInput.setMsgChannel(-1,-1);
	if (m_bData1)
		m_editInput.setMsgData1(m_nData1Low, m_nData1High);
	else
		m_editInput.setMsgData1(-1,-1);
	if (m_bData2)
		m_editInput.setMsgData2(m_nData2Low, m_nData2High);
	else
		m_editInput.setMsgData2(-1,-1);
	m_editInput.setUpdateInterval(m_fUpdateInterval);
	CDialog::OnOK();
	if (m_pInput)
		*((sspMIDIeventInput*) m_pInput) = m_editInput;
	else
		m_pInput = new sspMIDIeventInput(m_editInput);
}
