// sspWatchdogRunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspWatchdogRunDlg.h"


// sspWatchdogRunDlg dialog

IMPLEMENT_DYNAMIC(sspWatchdogRunDlg, CDialog)
sspWatchdogRunDlg::sspWatchdogRunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(sspWatchdogRunDlg::IDD, pParent)
{
}

sspWatchdogRunDlg::~sspWatchdogRunDlg()
{
}

void sspWatchdogRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WATCHDOGOCXCTRL1, m_wDog);
}


BEGIN_MESSAGE_MAP(sspWatchdogRunDlg, CDialog)
END_MESSAGE_MAP()

BOOL sspWatchdogRunDlg::Create()
{
  return CDialog::Create(sspWatchdogRunDlg::IDD);
}
