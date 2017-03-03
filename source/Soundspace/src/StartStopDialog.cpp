// StartStopDialog.cpp : implementation file
//

#include "stdafx.h"
#include "soundspace.h"
#include "StartStopDialog.h"
#include ".\startstopdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartStopDialog dialog


CStartStopDialog::CStartStopDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStartStopDialog::IDD, pParent)
  , m_startTime(0)
  , m_stopTime(0)
  , m_emptyAction(0)
  , m_bEnableInterval(FALSE)
  , m_nStartAction(0)
{
}


void CStartStopDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_DateTimeCtrl(pDX, IDC_STARTTIME, m_startTime);
  DDX_DateTimeCtrl(pDX, IDC_STOPTIME, m_stopTime);
  DDX_CBIndex(pDX, IDC_EMPTYACTION, m_emptyAction);
  DDX_Check(pDX, IDC_ENABLE_INTERVAL, m_bEnableInterval);
  DDX_CBIndex(pDX, IDC_STARTACTION, m_nStartAction);
}


BEGIN_MESSAGE_MAP(CStartStopDialog, CDialog)
	//{{AFX_MSG_MAP(CStartStopDialog)
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_ENABLE_INTERVAL, OnEnableInterval)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartStopDialog message handlers

BOOL CStartStopDialog::OnInitDialog() 
{
  GetDlgItem(IDC_STARTTIME)->EnableWindow(m_bEnableInterval);
  GetDlgItem(IDC_STOPTIME)->EnableWindow(m_bEnableInterval);
	CDialog::OnInitDialog();
	return true;
}

void CStartStopDialog::OnEnableInterval()
{
  UpdateData(TRUE);
  GetDlgItem(IDC_STARTTIME)->EnableWindow(m_bEnableInterval);
  GetDlgItem(IDC_STOPTIME)->EnableWindow(m_bEnableInterval);
}
