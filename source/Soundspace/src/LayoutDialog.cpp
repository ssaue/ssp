// LayoutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "soundspace.h"
#include "LayoutDialog.h"
#include "sspPools.h"
#include "SSpValues.h"
#include "ValueDialog.h"

#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayoutDialog dialog


CLayoutDialog::CLayoutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLayoutDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLayoutDialog)
	//}}AFX_DATA_INIT
}


void CLayoutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayoutDialog)
	DDX_Control(pDX, IDC_VALUE_LIST, m_lbValues);
	DDX_Control(pDX, IDC_FADER_LIST, m_listFader);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayoutDialog, CDialog)
	//{{AFX_MSG_MAP(CLayoutDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_SELCHANGE(IDC_VALUE_LIST, OnSelectValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayoutDialog message handlers

BOOL CLayoutDialog::OnInitDialog() 
{
	unsigned int i;
	CDialog::OnInitDialog();
	for (i=0; i<sspPool::Instance().values.GetSize(); i++) {
		sspValue* pVal = sspPool::Instance().values.getValue(i);
		if (pVal->IsKindOf( RUNTIME_CLASS( SSpInputValueRange ) )) {
			m_nInputRangeValues.push_back(i);
			int nIndex = m_listFader.InsertString(-1, sspPool::Instance().values.GetName(i));
      m_listFader.SetSel(nIndex, find(m_nFaderSelection.begin(), m_nFaderSelection.end(), i) != m_nFaderSelection.end());
		}
	}
	m_lbValues.ResetContent();
	for (i=0; i<m_nValueSelection.size(); i++)
		m_lbValues.AddString(sspPool::Instance().values.GetName(m_nValueSelection[i]));
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
	return TRUE;
}

void CLayoutDialog::OnOK() 
{
	int nSize = m_listFader.GetSelCount();
	m_nFaderSelection.resize(nSize);
	m_listFader.GetSelItems(nSize,&m_nFaderSelection[0]);
	for (int i=0; i<nSize; i++) {
		m_nFaderSelection[i] = m_nInputRangeValues[m_nFaderSelection[i]];
	}
	CDialog::OnOK();
}

void CLayoutDialog::OnAdd() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nValueSelection.insert(m_nValueSelection.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbValues.AddString(sspPool::Instance().values.GetName(dlg.m_nSelection[i]));
		}
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CLayoutDialog::OnRemove() 
{
	int nIndex = m_lbValues.GetCurSel();
	if (nIndex >= 0 && nIndex < (int) m_nValueSelection.size()) {
		m_lbValues.DeleteString(nIndex);
		m_lbValues.SetCurSel(-1);
		m_nValueSelection.erase(m_nValueSelection.begin() + nIndex);
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CLayoutDialog::OnSelectValue() 
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_lbValues.GetCurSel() >= 0);
}
