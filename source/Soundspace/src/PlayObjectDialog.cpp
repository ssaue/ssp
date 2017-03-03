// PlayObjectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "PlayObjectDialog.h"
#include "sspPools.h"
#include "SSpTasks.h"
#include "SSpGUIPlayObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayObjectDialog dialog


CPlayObjectDialog::CPlayObjectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayObjectDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayObjectDialog)
	m_nObjectType = -1;
	//}}AFX_DATA_INIT
}


void CPlayObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayObjectDialog)
	DDX_Control(pDX, IDC_SELECT_OBJECT, m_objectList);
	DDX_CBIndex(pDX, IDC_SELECT_TYPE, m_nObjectType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlayObjectDialog, CDialog)
	//{{AFX_MSG_MAP(CPlayObjectDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REPLACE, OnReplace)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_LBN_SELCHANGE(IDC_SELECT_OBJECT, OnSelectObject)
	ON_CBN_SELCHANGE(IDC_SELECT_TYPE, OnSelectType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPlayObjectDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPLACE)->EnableWindow(FALSE);
	m_objectList.ResetContent();
	for (unsigned int i=0; i<sspPool::Instance().objects.GetSize(); i++)
		m_objectList.InsertString(-1, sspPool::Instance().objects.GetName(i));
	return TRUE;
}

void CPlayObjectDialog::OnAdd() 
{
	int nSelect = -1;
	
	sspObjVector vObj = SSpGUIPlayObject::loadPlayObjectEditor(m_nObjectType, true);
	for (unsigned int i=0; i<vObj.size(); ++i) {
		sspPool::Instance().objects.LoadPlayObject(vObj[i]->pObj, vObj[i]->strName);
		nSelect = m_objectList.AddString(vObj[i]->strName);
		delete vObj[i];
	}
	vObj.clear();
	m_objectList.SetSel(-1, FALSE);
	m_objectList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_objectList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_objectList.GetSelCount() == 1);
}

void CPlayObjectDialog::OnEdit() 
{
	int nSelect;
	if (m_objectList.GetSelCount() != 1)
		return;
	m_objectList.GetSelItems(1,&nSelect);

	CString strName = _T("");
	sspPlayObject* pObj = sspPool::Instance().objects.GetPlayObject(nSelect);
	if (pObj) {
		strName = sspPool::Instance().objects.GetName(nSelect);
		if (SSpGUIPlayObject::loadPlayObjectEditor(pObj, strName)) {
			sspPool::Instance().objects.ExchangeObjectName(strName, nSelect);
			m_objectList.DeleteString(nSelect);
			nSelect = m_objectList.InsertString(nSelect, strName);
		}
	}

	m_objectList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_objectList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_objectList.GetSelCount() == 1);
}

void CPlayObjectDialog::OnReplace() 
{
	int nSelect;
	if (m_objectList.GetSelCount() != 1)
		return;
	m_objectList.GetSelItems(1,&nSelect);

	sspObjVector vObj = SSpGUIPlayObject::loadPlayObjectEditor(m_nObjectType, false);
	if (!vObj.empty()) {
		sspPool::Instance().objects.ExchangePlayObject(vObj[0]->pObj, vObj[0]->strName, nSelect);
		m_objectList.DeleteString(nSelect);
		nSelect = m_objectList.InsertString(nSelect, vObj[0]->strName);
		delete vObj[0];
		vObj.clear();
	}

	m_objectList.SetSel(nSelect, (nSelect >= 0));
	GetDlgItem(IDC_EDIT)->EnableWindow(m_objectList.GetSelCount() == 1);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_objectList.GetSelCount() == 1);
}

void CPlayObjectDialog::OnSelectType() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_objectList.GetSelCount() == 1);
}

void CPlayObjectDialog::OnSelectObject() 
{
	m_nObjectType = -1;
	if (m_objectList.GetSelCount() == 1) {
		int nSelect;
		m_objectList.GetSelItems(1,&nSelect);
		sspPlayObject* pObj = sspPool::Instance().objects.GetPlayObject(nSelect);
		if (pObj) m_nObjectType = pObj->getType();
	}
	((CComboBox*) GetDlgItem(IDC_SELECT_TYPE))->SetCurSel(m_nObjectType);
	GetDlgItem(IDC_ADD)->EnableWindow(m_nObjectType >= 0);
	GetDlgItem(IDC_EDIT)->EnableWindow(m_nObjectType >= 0);
	GetDlgItem(IDC_REPLACE)->EnableWindow(m_nObjectType >= 0);
}

void CPlayObjectDialog::OnOK() 
{
	int nSize = m_objectList.GetSelCount();
	m_nSelection.resize(nSize);
	if (nSize > 0) m_objectList.GetSelItems(nSize,&m_nSelection[0]);
	CDialog::OnOK();
}

