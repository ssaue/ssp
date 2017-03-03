// SSpGUIMessageEditor.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "SSpGUIMessageEditor.h"
#include "sspPools.h"
#include "StreamDialog.h"
#include "ValueDialog.h"
#include "PlayTaskDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLoadMessageDialog dialog


CLoadMessageDialog::CLoadMessageDialog(CWnd* pParent /*=NULL*/)
	: CMessageDialog(CLoadMessageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadMessageDialog)
	m_nCurStream = -1;
	m_nCurTask = -1;
	//}}AFX_DATA_INIT
	m_bSpecify = FALSE;
}


void CLoadMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadMessageDialog)
	DDX_Radio(pDX, IDC_RBTN_CURSTREAM, m_nCurStream);
	DDX_Radio(pDX, IDC_RBTN_CURTASK, m_nCurTask);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoadMessageDialog, CDialog)
	//{{AFX_MSG_MAP(CLoadMessageDialog)
	ON_BN_CLICKED(IDC_SELECT_STREAM, OnSelectStream)
	ON_BN_CLICKED(IDC_SELECT_TASK, OnSelectTask)
	ON_BN_CLICKED(IDC_SELECT_VALUE, OnSelectValue)
	ON_BN_CLICKED(IDC_RBTN_CURTASK, OnCurtask)
	ON_BN_CLICKED(IDC_RBTN_STREAM, OnStream)
	ON_BN_CLICKED(IDC_RBTN_TASK, OnTask)
	ON_BN_CLICKED(IDC_RBTN_CURSTREAM, OnCurStream)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CLoadMessageDialog::OnInitDialog() 
{
	m_nCurStream = (m_bSpecify || m_msg.GetStream() != USE_SEND_PARAMETER) ? SPECIFY : CURRENT;
	m_nCurTask = (m_bSpecify || m_msg.GetMsgPara(1).iVal != USE_SEND_PARAMETER) ? SPECIFY : CURRENT;
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);

	GetDlgItem(IDC_RBTN_CURSTREAM)->EnableWindow(!m_bSpecify);
	GetDlgItem(IDC_RBTN_STREAM)->EnableWindow(!m_bSpecify);
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(m_nCurStream == SPECIFY);
	GetDlgItem(IDC_RBTN_CURTASK)->EnableWindow(!m_bSpecify);
	GetDlgItem(IDC_RBTN_TASK)->EnableWindow(!m_bSpecify);
	GetDlgItem(IDC_SELECT_TASK)->EnableWindow(m_nCurTask == SPECIFY);

	if (m_nCurStream == SPECIFY && m_msg.GetStream() >= 0)
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));
	if (m_nCurTask == SPECIFY && m_msg.GetMsgPara(1).iVal >= 0)
		GetDlgItem(IDC_SELECT_TASK)->SetWindowText(sspPool::Instance().tasks.GetName(m_msg.GetMsgPara(1).iVal));
	if (m_msg.GetMsgPara(0).iVal >= 0)
		GetDlgItem(IDC_SELECT_VALUE)->SetWindowText(sspPool::Instance().values.GetName(m_msg.GetMsgPara(0).iVal));

	return TRUE;
}

void CLoadMessageDialog::OnCurStream() 
{
	m_nCurStream = CURRENT;
	m_msg.SetStream(USE_SEND_PARAMETER);
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(FALSE);
}

void CLoadMessageDialog::OnStream() 
{
	m_nCurStream = SPECIFY;
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(TRUE);
	if (m_msg.GetStream() >= 0)
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));
}

void CLoadMessageDialog::OnSelectStream() 
{
	CStreamDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_msg.SetStream(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));
	}
}

void CLoadMessageDialog::OnSelectValue() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_msg.GetMsgPara(0).iVal = dlg.m_nSelection[0];
		GetDlgItem(IDC_SELECT_VALUE)->SetWindowText(sspPool::Instance().values.GetName(m_msg.GetMsgPara(0).iVal));
	}
}

void CLoadMessageDialog::OnCurtask() 
{
	m_nCurTask = CURRENT;
	m_msg.GetMsgPara(1).iVal = USE_SEND_PARAMETER;
	GetDlgItem(IDC_SELECT_TASK)->EnableWindow(FALSE);
}

void CLoadMessageDialog::OnTask() 
{
	m_nCurTask = SPECIFY;
	GetDlgItem(IDC_SELECT_TASK)->EnableWindow(TRUE);
	if (m_msg.GetMsgPara(1).iVal != USE_SEND_PARAMETER)
		GetDlgItem(IDC_SELECT_TASK)->SetWindowText(sspPool::Instance().tasks.GetName(m_msg.GetMsgPara(1).iVal));
}

void CLoadMessageDialog::OnSelectTask() 
{
	CPlayTaskDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_msg.GetMsgPara(1).iVal = dlg.m_nSelection[0];
		GetDlgItem(IDC_SELECT_TASK)->SetWindowText(sspPool::Instance().tasks.GetName(m_msg.GetMsgPara(1).iVal));
	}
}

void CLoadMessageDialog::OnOK() 
{
	if (m_nCurStream == SPECIFY && (m_msg.GetStream() < 0 || m_msg.GetStream() > (int) sspPool::Instance().streams.GetSize())) {
		AfxMessageBox(L"Invalid stream selection.");
		return;
	}
	if (m_msg.GetMsgPara(0).iVal < 0 || m_msg.GetMsgPara(0).iVal > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid time value selection.");
		return;
	}
	if (m_nCurTask == SPECIFY && (m_msg.GetMsgPara(1).iVal < 0 || m_msg.GetMsgPara(1).iVal > (int) sspPool::Instance().tasks.GetSize())) {
		AfxMessageBox(L"Invalid task selection.");
		return;
	}
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CSoloMessageDialog dialog


CSoloMessageDialog::CSoloMessageDialog(CWnd* pParent /*=NULL*/)
	: CMessageDialog(CSoloMessageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoloMessageDialog)
	m_nCurStream = -1;
	//}}AFX_DATA_INIT
	m_bSpecify = FALSE;
}


void CSoloMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoloMessageDialog)
	DDX_Radio(pDX, IDC_RBTN_CURSTREAM, m_nCurStream);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_FADETIME, m_msg.GetMsgPara(0).iVal);
	DDV_MinMaxInt(pDX, m_msg.GetMsgPara(0).iVal, 0, 60);
}


BEGIN_MESSAGE_MAP(CSoloMessageDialog, CDialog)
	//{{AFX_MSG_MAP(CSoloMessageDialog)
	ON_BN_CLICKED(IDC_RBTN_STREAM, OnStream)
	ON_BN_CLICKED(IDC_SELECT_STREAM, OnSelectStream)
	ON_BN_CLICKED(IDC_RBTN_CURSTREAM, OnCurStream)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSoloMessageDialog::OnInitDialog() 
{
	m_nCurStream = (m_bSpecify || m_msg.GetStream() != USE_SEND_PARAMETER) ? SPECIFY : CURRENT;
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);

	GetDlgItem(IDC_RBTN_CURSTREAM)->EnableWindow(!m_bSpecify);
	GetDlgItem(IDC_RBTN_STREAM)->EnableWindow(!m_bSpecify);
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(m_nCurStream == SPECIFY);

	if (m_nCurStream == SPECIFY && m_msg.GetStream() >= 0)
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));

	return TRUE;
}

void CSoloMessageDialog::OnCurStream() 
{
	m_nCurStream = CURRENT;
	m_msg.SetStream(USE_SEND_PARAMETER);
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(FALSE);
}

void CSoloMessageDialog::OnStream() 
{
	m_nCurStream = SPECIFY;
	if (m_msg.GetStream() >= 0)
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(TRUE);
}

void CSoloMessageDialog::OnSelectStream() 
{
	CStreamDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_msg.SetStream(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));
	}
}

void CSoloMessageDialog::OnOK() 
{
	if (m_nCurStream == SPECIFY && (m_msg.GetStream() < 0 || m_msg.GetStream() > (int) sspPool::Instance().streams.GetSize())) {
		AfxMessageBox(L"Invalid stream selection.");
		return;
	}
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CStartMessageDialog dialog


CStartMessageDialog::CStartMessageDialog(CWnd* pParent /*=NULL*/)
	: CMessageDialog(CStartMessageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStartMessageDialog)
	m_nCurStream = SPECIFY;
	//}}AFX_DATA_INIT
	m_bSpecify = FALSE;
}


void CStartMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStartMessageDialog)
	DDX_Radio(pDX, IDC_RBTN_CURSTREAM, m_nCurStream);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartMessageDialog, CDialog)
	//{{AFX_MSG_MAP(CStartMessageDialog)
	ON_BN_CLICKED(IDC_RBTN_CURSTREAM, OnCurStream)
	ON_BN_CLICKED(IDC_RBTN_STREAM, OnStream)
	ON_BN_CLICKED(IDC_SELECT_STREAM, OnSelectStream)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CStartMessageDialog::OnInitDialog() 
{
	m_nCurStream = (m_bSpecify || m_msg.GetStream() != USE_SEND_PARAMETER) ? SPECIFY : CURRENT;
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);

	GetDlgItem(IDC_RBTN_CURSTREAM)->EnableWindow(!m_bSpecify);
	GetDlgItem(IDC_RBTN_STREAM)->EnableWindow(!m_bSpecify);
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(m_nCurStream == SPECIFY);

	if (m_nCurStream == SPECIFY && m_msg.GetStream() >= 0)
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));

	return TRUE;
}

void CStartMessageDialog::OnCurStream() 
{
	m_nCurStream = CURRENT;
	m_msg.SetStream(USE_SEND_PARAMETER);
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(FALSE);
}

void CStartMessageDialog::OnStream() 
{
	m_nCurStream = SPECIFY;
	if (m_msg.GetStream() >= 0)
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(TRUE);
}

void CStartMessageDialog::OnSelectStream() 
{
	CStreamDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_msg.SetStream(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));
	}
}

void CStartMessageDialog::OnOK() 
{
	if (m_nCurStream == SPECIFY && (m_msg.GetStream() < 0 || m_msg.GetStream() > (int) sspPool::Instance().streams.GetSize())) {
		AfxMessageBox(L"Invalid stream selection.");
		return;
	}
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CVolumeMessageDialog dialog


CVolumeMessageDialog::CVolumeMessageDialog(CWnd* pParent /*=NULL*/)
	: CMessageDialog(CVolumeMessageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolumeMessageDialog)
	m_nCurStream = -1;
	//}}AFX_DATA_INIT
	m_bSpecify = FALSE;
}


void CVolumeMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolumeMessageDialog)
	DDX_Radio(pDX, IDC_RBTN_CURSTREAM, m_nCurStream);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_FADETIME, m_msg.GetMsgPara(1).iVal);
}


BEGIN_MESSAGE_MAP(CVolumeMessageDialog, CDialog)
	//{{AFX_MSG_MAP(CVolumeMessageDialog)
	ON_BN_CLICKED(IDC_RBTN_CURSTREAM, OnCurStream)
	ON_BN_CLICKED(IDC_RBTN_STREAM, OnStream)
	ON_BN_CLICKED(IDC_SELECT_STREAM, OnSelectStream)
	ON_BN_CLICKED(IDC_SELECT_VOLUME, OnSelectVolume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeMessageDialog message handlers

BOOL CVolumeMessageDialog::OnInitDialog() 
{
	m_nCurStream = (m_bSpecify || m_msg.GetStream() != USE_SEND_PARAMETER) ? SPECIFY : CURRENT;
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);

	GetDlgItem(IDC_RBTN_CURSTREAM)->EnableWindow(!m_bSpecify);
	GetDlgItem(IDC_RBTN_STREAM)->EnableWindow(!m_bSpecify);
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(m_nCurStream == SPECIFY);

	if (m_nCurStream == SPECIFY && m_msg.GetStream() >= 0)
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));
	if (m_msg.GetMsgPara(0).iVal >= 0)
		GetDlgItem(IDC_SELECT_VOLUME)->SetWindowText(sspPool::Instance().values.GetName(m_msg.GetMsgPara(0).iVal));

	return TRUE;
}

void CVolumeMessageDialog::OnCurStream() 
{
	m_nCurStream = CURRENT;
	m_msg.SetStream(USE_SEND_PARAMETER);
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(FALSE);
}

void CVolumeMessageDialog::OnStream() 
{
	m_nCurStream = SPECIFY;
	if (m_msg.GetStream() >= 0)
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));
	GetDlgItem(IDC_SELECT_STREAM)->EnableWindow(TRUE);
}

void CVolumeMessageDialog::OnSelectStream() 
{
	CStreamDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_msg.SetStream(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_STREAM)->SetWindowText(sspPool::Instance().streams.GetName(m_msg.GetStream()));
	}
}

void CVolumeMessageDialog::OnSelectVolume() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_msg.GetMsgPara(0).iVal = dlg.m_nSelection[0];
		GetDlgItem(IDC_SELECT_VOLUME)->SetWindowText(sspPool::Instance().values.GetName(m_msg.GetMsgPara(0).iVal));
	}
}

void CVolumeMessageDialog::OnOK() 
{
	if (m_nCurStream == SPECIFY && (m_msg.GetStream() < 0 || m_msg.GetStream() > (int) sspPool::Instance().streams.GetSize())) {
		AfxMessageBox(L"Invalid stream selection.");
		return;
	}
	if (m_msg.GetMsgPara(0).iVal < 0 || m_msg.GetMsgPara(0).iVal > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid volume value selection.");
		return;
	}
	CDialog::OnOK();
}
