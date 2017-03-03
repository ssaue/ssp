// sspGUIMapDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MidiMagi.h"
#include "sspGUIMapDialog.h"


// sspGUIMapDialog dialog

IMPLEMENT_DYNAMIC(sspGUIMapDialog, CDialog)
sspGUIMapDialog::sspGUIMapDialog(CWnd* pParent /*=NULL*/)
	: CDialog(sspGUIMapDialog::IDD, pParent)
  , m_nPitch(0)
  , m_nDMX(0)
{
}

sspGUIMapDialog::~sspGUIMapDialog()
{
}

void sspGUIMapDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_PITCH, m_nPitch);
  DDV_MinMaxInt(pDX, m_nPitch, 0, 127);
  DDX_Text(pDX, IDC_DMX, m_nDMX);
	DDV_MinMaxInt(pDX, m_nDMX, 0, 127);
}


BEGIN_MESSAGE_MAP(sspGUIMapDialog, CDialog)
END_MESSAGE_MAP()


// sspGUIMapDialog message handlers
