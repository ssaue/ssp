// AutostartDialog.cpp : implementation file
//

#include "stdafx.h"
#include "soundspace.h"
#include "AutostartDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutostartDialog dialog


CAutostartDialog::CAutostartDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAutostartDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutostartDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAutostartDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutostartDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutostartDialog, CDialog)
	//{{AFX_MSG_MAP(CAutostartDialog)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutostartDialog message handlers

void CAutostartDialog::OnCancel() 
{
	KillTimer(1);
	CDialog::OnCancel();
}

void CAutostartDialog::OnTimer(unsigned int nIDEvent) 
{
	KillTimer(1);
	CDialog::OnTimer(nIDEvent);
	CDialog::OnOK();
}

BOOL CAutostartDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetTimer(1, 5000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutostartDialog::OnOK() 
{
	KillTimer(1);
	CDialog::OnOK();
}
