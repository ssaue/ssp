// PlayTaskDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspGUITaskEditor.h"
#include "PlayTaskDialog.h"
#include "sspPools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayTaskDialog dialog


CPlayTaskDialog::CPlayTaskDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayTaskDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayTaskDialog)
	//}}AFX_DATA_INIT
}


void CPlayTaskDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayTaskDialog)
	DDX_Control(pDX, IDC_SELECT_TASK, m_taskList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayTaskDialog, CDialog)
	//{{AFX_MSG_MAP(CPlayTaskDialog)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_LBN_SELCHANGE(IDC_SELECT_TASK, OnSelectTask)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPlayTaskDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	m_taskList.ResetContent();
	for (unsigned int i=0; i<sspPool::Instance().tasks.GetSize(); i++)
		m_taskList.InsertString(-1, sspPool::Instance().tasks.GetName(i));
	return TRUE;
}

void CPlayTaskDialog::OnEdit() 
{
	if (m_taskList.GetSelCount() != 1)
		return;

	int nSelect;
	m_taskList.GetSelItems(1,&nSelect);

	SSpPlayTask* pTask = sspPool::Instance().tasks.GetPlayTask(nSelect);
	if (pTask != NULL) {
		sspGUITaskEditor dlg;
		dlg.m_task = *pTask;
		dlg.m_strName = sspPool::Instance().tasks.GetName(nSelect);
		if (dlg.DoModal() == IDOK) {
			sspPool::Instance().tasks.ExchangePlayTask(new SSpPlayTask(dlg.m_task), dlg.m_strName, nSelect);
			m_taskList.DeleteString(nSelect);
			nSelect = m_taskList.InsertString(nSelect, dlg.m_strName);
		}
	}
	m_taskList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_taskList.GetSelCount() == 1);
}

void CPlayTaskDialog::OnAdd() 
{
	int nSelect = -1;
	sspGUITaskEditor dlg;
	if (dlg.DoModal() == IDOK) {
		sspPool::Instance().tasks.LoadPlayTask(new SSpPlayTask(dlg.m_task), dlg.m_strName);
		nSelect = m_taskList.InsertString(nSelect, dlg.m_strName);
	}
	m_taskList.SetSel(-1, FALSE);
	m_taskList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_taskList.GetSelCount() == 1);
}

void CPlayTaskDialog::OnSelectTask() 
{
	GetDlgItem(IDC_EDIT)->EnableWindow(m_taskList.GetSelCount() == 1);
}

void CPlayTaskDialog::OnOK() 
{
	int nSize = m_taskList.GetSelCount();
	m_nSelection.resize(nSize);
	if (nSize > 0) m_taskList.GetSelItems(nSize,&m_nSelection[0]);
	CDialog::OnOK();
}

