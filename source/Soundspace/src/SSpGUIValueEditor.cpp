// SSpGUIValueEditor.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "SSpGUIValueEditor.h"
#include "sspPools.h"
#include "ValueDialog.h"
#include "ConditionalDialog.h"

#include <algorithm>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CValueFloatDialog dialog


CValueFloatDialog::CValueFloatDialog(CWnd* pParent /*=NULL*/)
	: CValueEditor(CValueFloatDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueFloatDialog)
	m_fValue = 0.0f;
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_bNameEdited = false;
}


void CValueFloatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueFloatDialog)
	DDX_Text(pDX, IDC_VALUE, m_fValue);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueFloatDialog, CDialog)
	//{{AFX_MSG_MAP(CValueFloatDialog)
	ON_EN_CHANGE(IDC_VALUE, OnChangeValue)
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_NAME, OnKillFocusName)
END_MESSAGE_MAP()

BOOL CValueFloatDialog::OnInitDialog() 
{
	if (m_pVal) {
		SSpBasicValue* pVal = dynamic_cast<SSpBasicValue*>(m_pVal);
		if (!pVal) {
			AfxMessageBox(L"Unable to assign value.");
			EndDialog(IDCANCEL);
		}
		m_editVal = *pVal;
	}
	m_fValue = m_editVal.getValue();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	m_bNameEdited = m_strName.SpanExcluding( _T("0123456789.-")).GetLength() > 0;
	return TRUE;
}

void CValueFloatDialog::OnChangeValue() 
{
	CString str;
	GetDlgItem(IDC_VALUE)->GetWindowText(str);
	if (!m_bNameEdited) {
		GetDlgItem(IDC_NAME)->SetWindowText(str);
	}
}

void CValueFloatDialog::OnKillFocusName()
{
	UpdateData(TRUE);
	if (m_strName.GetLength() > 0)
		m_bNameEdited = true;	// Protect against overwriting
}

void CValueFloatDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	CDialog::OnOK();
	m_editVal.setValue(m_fValue);
	if (m_pVal)
		*((SSpBasicValue*) m_pVal) = m_editVal;
	else
		m_pVal = new SSpBasicValue(m_editVal);
}

/////////////////////////////////////////////////////////////////////////////
// CValueInputRangeDialog dialog

CValueInputRangeDialog::CValueInputRangeDialog(CWnd* pParent /*=NULL*/)
	: CValueEditor(CValueInputRangeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueInputRangeDialog)
	m_strName = _T("");
	m_fRangeMin = 0.0f;
	m_fRangeMax = 0.0f;
	//}}AFX_DATA_INIT
}


void CValueInputRangeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueInputRangeDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_RANGE_MIN, m_fRangeMin);
	DDX_Text(pDX, IDC_RANGE_MAX, m_fRangeMax);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueInputRangeDialog, CDialog)
	//{{AFX_MSG_MAP(CValueInputRangeDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CValueInputRangeDialog::OnInitDialog() 
{
	if (m_pVal) {
		SSpInputValueRange* pVal = dynamic_cast<SSpInputValueRange*>(m_pVal);
		if (!pVal) {
			AfxMessageBox(L"Unable to assign value.");
			EndDialog(IDCANCEL);
		}
		m_editVal = *pVal;
	}
	m_fRangeMax = m_editVal.getRangeMax();
	m_fRangeMin = m_editVal.getRangeMin();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}

void CValueInputRangeDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	CDialog::OnOK();
	m_editVal.setValueRange(m_fRangeMin, m_fRangeMax);
	if (m_pVal)
		*((SSpInputValueRange*) m_pVal) = m_editVal;
	else
		m_pVal = new SSpInputValueRange(m_editVal);
}

/////////////////////////////////////////////////////////////////////////////
// CValueClockDialog dialog


CValueClockDialog::CValueClockDialog(CWnd* pParent /*=NULL*/)
	: CValueEditor(CValueClockDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueClockDialog)
	m_strName = _T("");
	m_fEndValue = 0.0f;
	m_fStartValue = 0.0f;
	m_nStartHour = 0;
	m_nEndHour = 0;
	m_nStartMinute = 0;
	m_nEndMinute = 0;
	m_nStartSecond = 0;
	m_nEndSecond = 0;
	//}}AFX_DATA_INIT
}


void CValueClockDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueClockDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_ENDVALUE, m_fEndValue);
	DDX_Text(pDX, IDC_STARTVALUE, m_fStartValue);
	DDX_Text(pDX, IDC_HOUR1, m_nStartHour);
	DDV_MinMaxInt(pDX, m_nStartHour, 0, 23);
	DDX_Text(pDX, IDC_HOUR2, m_nEndHour);
	DDV_MinMaxInt(pDX, m_nEndHour, 0, 23);
	DDX_Text(pDX, IDC_MINUTE1, m_nStartMinute);
	DDV_MinMaxInt(pDX, m_nStartMinute, 0, 59);
	DDX_Text(pDX, IDC_MINUTE2, m_nEndMinute);
	DDV_MinMaxInt(pDX, m_nEndMinute, 0, 59);
	DDX_Text(pDX, IDC_SECOND1, m_nStartSecond);
	DDV_MinMaxInt(pDX, m_nStartSecond, 0, 59);
	DDX_Text(pDX, IDC_SECOND2, m_nEndSecond);
	DDV_MinMaxInt(pDX, m_nEndSecond, 0, 59);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueClockDialog, CDialog)
	//{{AFX_MSG_MAP(CValueClockDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CValueClockDialog::OnInitDialog() 
{
	if (m_pVal) {
		SSpTimeMap* pVal = dynamic_cast<SSpTimeMap*>(m_pVal);
		if (!pVal) {
			AfxMessageBox(L"Unable to assign value.");
			EndDialog(IDCANCEL);
		}
		m_editVal = *pVal;
	}
	m_nStartHour = m_editVal.getStartClock().hour();
	m_nStartMinute = m_editVal.getStartClock().min();
	m_nStartSecond = m_editVal.getStartClock().sec();
	m_nEndHour = m_editVal.getEndClock().hour();
	m_nEndMinute = m_editVal.getEndClock().min();
	m_nEndSecond = m_editVal.getEndClock().sec();
	m_fStartValue = m_editVal.getStartValue();
	m_fEndValue = m_editVal.getEndValue();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}

void CValueClockDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	CDialog::OnOK();
	SSpClock start(m_nStartHour, m_nStartMinute, m_nStartSecond);
	SSpClock end(m_nEndHour, m_nEndMinute, m_nEndSecond);
	m_editVal.setStartClock(start);
	m_editVal.setEndClock(end);
	m_editVal.setStartValue(m_fStartValue);
	m_editVal.setEndValue(m_fEndValue);
	if (m_pVal)
		*((SSpTimeMap*) m_pVal) = m_editVal;
	else
		m_pVal = new SSpTimeMap(m_editVal);
}

/////////////////////////////////////////////////////////////////////////////
// CValueDateDialog dialog


CValueDateDialog::CValueDateDialog(CWnd* pParent /*=NULL*/)
	: CValueEditor(CValueDateDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueDateDialog)
	m_nStartDay = 1;
	m_nEndDay = 1;
	m_fEndValue = 0.0f;
	m_nStartMonth = 1;
	m_nEndMonth = 1;
	m_strName = _T("");
	m_fStartValue = 0.0f;
	//}}AFX_DATA_INIT
}


void CValueDateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueDateDialog)
	DDX_Text(pDX, IDC_DAY1, m_nStartDay);
	DDV_MinMaxInt(pDX, m_nStartDay, 1, 31);
	DDX_Text(pDX, IDC_DAY2, m_nEndDay);
	DDV_MinMaxInt(pDX, m_nEndDay, 1, 31);
	DDX_Text(pDX, IDC_ENDVALUE, m_fEndValue);
	DDX_Text(pDX, IDC_MONTH1, m_nStartMonth);
	DDV_MinMaxInt(pDX, m_nStartMonth, 1, 12);
	DDX_Text(pDX, IDC_MONTH2, m_nEndMonth);
	DDV_MinMaxInt(pDX, m_nEndMonth, 1, 12);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_STARTVALUE, m_fStartValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueDateDialog, CDialog)
	//{{AFX_MSG_MAP(CValueDateDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CValueDateDialog::OnInitDialog() 
{
	if (m_pVal) {
		SSpDateMap* pVal = dynamic_cast<SSpDateMap*>(m_pVal);
		if (!pVal) {
			AfxMessageBox(L"Unable to assign value.");
			EndDialog(IDCANCEL);
		}
		m_editVal = *pVal;
	}
	m_nStartMonth = m_editVal.getStartDate().month();
	m_nStartDay = m_editVal.getStartDate().day();
	m_nEndMonth = m_editVal.getEndDate().month();
	m_nEndDay = m_editVal.getEndDate().day();
	m_fStartValue = m_editVal.getStartValue();
	m_fEndValue = m_editVal.getEndValue();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}

void CValueDateDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	CDialog::OnOK();
	SSpDate start(m_nStartDay, m_nStartMonth);
	SSpDate end(m_nEndDay, m_nEndMonth);
	m_editVal.setStartDate(start);
	m_editVal.setEndDate(end);
	m_editVal.setStartValue(m_fStartValue);
	m_editVal.setEndValue(m_fEndValue);
	if (m_pVal)
		*((SSpDateMap*) m_pVal) = m_editVal;
	else
		m_pVal = new SSpDateMap(m_editVal);
}

/////////////////////////////////////////////////////////////////////////////
// CValueMapDialog dialog


CValueMapDialog::CValueMapDialog(CWnd* pParent /*=NULL*/)
	: CValueEditor(CValueMapDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueMapDialog)
	m_fEndValueIn = 0.0f;
	m_fEndValueOut = 0.0f;
	m_fStartValueIn = 0.0f;
	m_fStartValueOut = 0.0f;
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CValueMapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueMapDialog)
	DDX_Text(pDX, IDC_ENDVALUE1, m_fEndValueIn);
	DDX_Text(pDX, IDC_ENDVALUE2, m_fEndValueOut);
	DDX_Text(pDX, IDC_STARTVALUE1, m_fStartValueIn);
	DDX_Text(pDX, IDC_STARTVALUE2, m_fStartValueOut);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueMapDialog, CDialog)
	//{{AFX_MSG_MAP(CValueMapDialog)
	ON_BN_CLICKED(IDC_SOURCE_VALUE, OnSourceValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CValueMapDialog::OnInitDialog() 
{
	if (m_pVal) {
		SSpLinearMap* pVal = dynamic_cast<SSpLinearMap*>(m_pVal);
		if (!pVal) {
			AfxMessageBox(L"Unable to assign value.");
			EndDialog(IDCANCEL);
		}
		m_editVal = *pVal;
	}
	m_fStartValueIn = m_editVal.getInputStartValue();
	m_fEndValueIn = m_editVal.getInputEndValue();
	m_fStartValueOut = m_editVal.getOutputStartValue();
	m_fEndValueOut = m_editVal.getOutputEndValue();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	if (m_editVal.getValueIndex() >= 0)
		GetDlgItem(IDC_SOURCE_VALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editVal.getValueIndex()));
	return TRUE;
}

void CValueMapDialog::OnSourceValue() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editVal.setValueIndex(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SOURCE_VALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editVal.getValueIndex()));
	}
}

void CValueMapDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editVal.getValueIndex() < 0 || m_editVal.getValueIndex() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid value selection.");
		return;
	}
	CDialog::OnOK();
	m_editVal.setInputStartValue(m_fStartValueIn);
	m_editVal.setInputEndValue(m_fEndValueIn);
	m_editVal.setOutputStartValue(m_fStartValueOut);
	m_editVal.setOutputEndValue(m_fEndValueOut);
	if (m_pVal)
		*((SSpLinearMap*) m_pVal) = m_editVal;
	else
		m_pVal = new SSpLinearMap(m_editVal);
}

/////////////////////////////////////////////////////////////////////////////
// CValueSumDialog dialog


CValueSumDialog::CValueSumDialog(CWnd* pParent /*=NULL*/)
	: CValueEditor(CValueSumDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueSumDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

void CValueSumDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueSumDialog)
	DDX_Control(pDX, IDC_VALUES, m_lbOperands);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueSumDialog, CDialog)
	//{{AFX_MSG_MAP(CValueSumDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_SELCHANGE(IDC_VALUES, OnSelectValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CValueSumDialog::OnInitDialog()
{
	if (m_pVal) {
		SSpAddValue* pVal = dynamic_cast<SSpAddValue*>(m_pVal);
		if (!pVal) {
			AfxMessageBox(L"Unable to assign value.");
			EndDialog(IDCANCEL);
		}
		m_editVal = *pVal;
	}
	m_nOperands = m_editVal.getValueList();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
	m_lbOperands.ResetContent();
	for (unsigned int i=0; i<m_nOperands.size(); i++)
		m_lbOperands.AddString(sspPool::Instance().values.GetName(m_nOperands[i]));
	return TRUE;
}

void CValueSumDialog::OnAdd() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nOperands.insert(m_nOperands.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbOperands.AddString(sspPool::Instance().values.GetName(dlg.m_nSelection[i]));
		}
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CValueSumDialog::OnRemove() 
{
	int nIndex = m_lbOperands.GetCurSel();
	if (nIndex >= 0 && nIndex < (int) m_nOperands.size()) {
		m_lbOperands.DeleteString(nIndex);
		m_lbOperands.SetCurSel(-1);
		m_nOperands.erase(m_nOperands.begin() + nIndex);
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CValueSumDialog::OnSelectValue() 
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_lbOperands.GetCurSel() >= 0);
}

void CValueSumDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nOperands.size() < 2) {
		AfxMessageBox(L"Please select at least two values.");
		return;
	}
	CDialog::OnOK();
	m_editVal.setValueList(m_nOperands);
	if (m_pVal)
		*((SSpAddValue*) m_pVal) = m_editVal;
	else
		m_pVal = new SSpAddValue(m_editVal);
}

/////////////////////////////////////////////////////////////////////////////
// CValueProductDialog dialog


CValueProductDialog::CValueProductDialog(CWnd* pParent /*=NULL*/)
	: CValueEditor(CValueProductDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueProductDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

void CValueProductDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueProductDialog)
	DDX_Control(pDX, IDC_VALUES, m_lbOperands);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueProductDialog, CDialog)
	//{{AFX_MSG_MAP(CValueProductDialog)
	ON_LBN_SELCHANGE(IDC_VALUES, OnSelectValue)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CValueProductDialog::OnInitDialog()
{
	if (m_pVal) {
		SSpProductValue* pVal = dynamic_cast<SSpProductValue*>(m_pVal);
		if (!pVal) {
			AfxMessageBox(L"Unable to assign value.");
			EndDialog(IDCANCEL);
		}
		m_editVal = *pVal;
	}
	m_nOperands = m_editVal.getValueList();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
	m_lbOperands.ResetContent();
	for (unsigned int i=0; i<m_nOperands.size(); i++)
		m_lbOperands.AddString(sspPool::Instance().values.GetName(m_nOperands[i]));
	return TRUE;
}

void CValueProductDialog::OnAdd() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nOperands.insert(m_nOperands.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbOperands.AddString(sspPool::Instance().values.GetName(dlg.m_nSelection[i]));
		}
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CValueProductDialog::OnRemove() 
{
	int nIndex = m_lbOperands.GetCurSel();
	if (nIndex >= 0 && nIndex < (int) m_nOperands.size()) {
		m_lbOperands.DeleteString(nIndex);
		m_lbOperands.SetCurSel(-1);
		m_nOperands.erase(m_nOperands.begin() + nIndex);
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CValueProductDialog::OnSelectValue() 
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_lbOperands.GetCurSel() >= 0);
}

void CValueProductDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nOperands.size() < 2) {
		AfxMessageBox(L"Please select at least two values.");
		return;
	}
	CDialog::OnOK();
	m_editVal.setValueList(m_nOperands);
	if (m_pVal)
		*((SSpProductValue*) m_pVal) = m_editVal;
	else
		m_pVal = new SSpProductValue(m_editVal);
}

/////////////////////////////////////////////////////////////////////////////
// CValueConditionalDialog dialog


CValueConditionalDialog::CValueConditionalDialog(CWnd* pParent /*=NULL*/)
	: CValueEditor(CValueConditionalDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueConditionalDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_nSelected = -1;
}

void CValueConditionalDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueConditionalDialog)
	DDX_Control(pDX, IDC_COND_VALUE_LIST, m_lcCondValueList);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CValueConditionalDialog, CDialog)
	//{{AFX_MSG_MAP(CValueConditionalDialog)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefaultValue)
	ON_BN_CLICKED(IDC_MOVE_DOWN, OnMoveDown)
	ON_BN_CLICKED(IDC_MOVE_UP, OnMoveUp)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_NOTIFY(NM_CLICK, IDC_COND_VALUE_LIST, OnClickCondValueList)
	ON_NOTIFY(NM_DBLCLK, IDC_COND_VALUE_LIST, OnDblclkCondValueList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEFAULT_VALUE, OnDefaultValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CValueConditionalDialog::OnInitDialog() 
{
	if (m_pVal) {
		SSpConditionalValue* pVal = dynamic_cast<SSpConditionalValue*>(m_pVal);
		if (!pVal) {
			AfxMessageBox(L"Unable to assign value.");
			EndDialog(IDCANCEL);
		}
		m_editVal = *pVal;
	}
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);

	CRect ctrlRect;
	m_lcCondValueList.GetClientRect(ctrlRect);
	m_lcCondValueList.InsertColumn(0, _T("Conditional"), LVCFMT_LEFT, ctrlRect.Width() / 2, 0);
	m_lcCondValueList.InsertColumn(1, _T("Value"), LVCFMT_LEFT, ctrlRect.Width() / 2, 1);

	m_nValues = m_editVal.getValueList();
	m_nConditionals = m_editVal.getConditionals();

  m_nSize = std::min(m_nValues.size(), m_nConditionals.size());
	m_nValues.resize(m_nSize);
	m_nConditionals.resize(m_nSize);
	for (int i=0; i<m_nSize; i++) {
		m_lcCondValueList.InsertItem(LVIF_TEXT|LVIF_STATE, i, 
		   sspPool::Instance().conditionals.GetName(m_nConditionals[i]), 0, LVIS_SELECTED, 0, 0);
		m_lcCondValueList.SetItemText(i, 1, sspPool::Instance().values.GetName(m_nValues[i]));
	}

	GetDlgItem(IDC_ADD)->EnableWindow(TRUE);
	UpdateButtons();

	if (m_editVal.getDefaultValue() >= 0)
		GetDlgItem(IDC_DEFAULT_VALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editVal.getDefaultValue()));

	return TRUE;
}

void CValueConditionalDialog::OnClickCondValueList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMLV = (NMLISTVIEW*) pNMHDR;
	m_lcCondValueList.SetItemState(m_nSelected, 0, LVIS_SELECTED | LVIS_FOCUSED);
	m_nSelected = pNMLV->iItem;
	if (m_nSelected == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_lcCondValueList.SubItemHitTest(&lvhti);
		m_nSelected = lvhti.iItem;
	}
	m_lcCondValueList.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	UpdateButtons();
	*pResult = 0;
}

void CValueConditionalDialog::OnDblclkCondValueList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMLV = (NMLISTVIEW*) pNMHDR;
	m_lcCondValueList.SetItemState(m_nSelected, 0, LVIS_SELECTED | LVIS_FOCUSED);
	m_nSelected = pNMLV->iItem;
	if (m_nSelected == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_lcCondValueList.SubItemHitTest(&lvhti);
		m_nSelected = lvhti.iItem;
	}
	m_lcCondValueList.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	UpdateButtons();
	*pResult = 0;

	if (m_nSelected < 0)
		return;

	if (pNMLV->iSubItem == 0) {
		CConditionalDialog dlg;
		if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
			m_nConditionals[m_nSelected] = dlg.m_nSelection[0]; 
			m_lcCondValueList.SetItemText(m_nSelected, 0, sspPool::Instance().conditionals.GetName(m_nConditionals[m_nSelected]));
		}
	}
	else if (pNMLV->iSubItem == 1) {
		CValueDialog dlg;
		if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
			m_nValues[m_nSelected] = dlg.m_nSelection[0]; 
			m_lcCondValueList.SetItemText(m_nSelected, 1, sspPool::Instance().values.GetName(m_nValues[m_nSelected]));
		}
	}
}

void CValueConditionalDialog::OnAdd() 
{
	m_nConditionals.push_back(-1);
	m_nValues.push_back(-1);

	m_lcCondValueList.SetItemState(m_nSelected, 0, LVIS_SELECTED|LVIS_FOCUSED);
	m_lcCondValueList.InsertItem(LVIF_TEXT | LVIF_STATE, m_nSize, _T("<< Select conditional >>"),
		LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
	m_lcCondValueList.SetItemText(m_nSize, 1, _T("<< Select value >>"));

	m_nSelected = m_nSize;
	m_nSize++;
	UpdateButtons();
}

void CValueConditionalDialog::OnMoveDown() 
{
	CString text;
	if (m_nSelected < m_nSize-1) {
		int nCond = m_nConditionals[m_nSelected];
		m_nConditionals[m_nSelected] = m_nConditionals[m_nSelected+1];
		m_nConditionals[m_nSelected+1] = nCond;

		text = 	m_lcCondValueList.GetItemText(m_nSelected, 0);
		m_lcCondValueList.SetItemText(m_nSelected, 0, m_lcCondValueList.GetItemText(m_nSelected+1, 0));
		m_lcCondValueList.SetItemText(m_nSelected+1, 0, text);

		int nVal = m_nValues[m_nSelected];
		m_nValues[m_nSelected] = m_nValues[m_nSelected+1];
		m_nValues[m_nSelected+1] = nVal;

		text = 	m_lcCondValueList.GetItemText(m_nSelected, 1);
		m_lcCondValueList.SetItemText(m_nSelected, 1, m_lcCondValueList.GetItemText(m_nSelected+1, 1));
		m_lcCondValueList.SetItemText(m_nSelected+1, 1, text);

		m_lcCondValueList.SetItemState(m_nSelected, 0, LVIS_SELECTED|LVIS_FOCUSED);
		m_nSelected++;
		m_lcCondValueList.SetItemState(m_nSelected, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		UpdateButtons();
	}
}

void CValueConditionalDialog::OnMoveUp() 
{
	CString text;
	if (m_nSelected > 0) {
		int nCond = m_nConditionals[m_nSelected];
		m_nConditionals[m_nSelected] = m_nConditionals[m_nSelected-1];
		m_nConditionals[m_nSelected-1] = nCond;

		text = 	m_lcCondValueList.GetItemText(m_nSelected, 0);
		m_lcCondValueList.SetItemText(m_nSelected, 0, m_lcCondValueList.GetItemText(m_nSelected-1, 0));
		m_lcCondValueList.SetItemText(m_nSelected-1, 0, text);

		int nVal = m_nValues[m_nSelected];
		m_nValues[m_nSelected] = m_nValues[m_nSelected-1];
		m_nValues[m_nSelected-1] = nVal;

		text = 	m_lcCondValueList.GetItemText(m_nSelected, 1);
		m_lcCondValueList.SetItemText(m_nSelected, 1, m_lcCondValueList.GetItemText(m_nSelected-1, 1));
		m_lcCondValueList.SetItemText(m_nSelected-1, 1, text);

		m_lcCondValueList.SetItemState(m_nSelected, 0, LVIS_SELECTED|LVIS_FOCUSED);
		m_nSelected--;
		m_lcCondValueList.SetItemState(m_nSelected, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		UpdateButtons();
	}
}

void CValueConditionalDialog::OnRemove() 
{
	if (m_nSelected >= 0 && m_nSelected < m_nSize) {
		m_nConditionals.erase(m_nConditionals.begin() + m_nSelected);
		m_nValues.erase(m_nValues.begin() + m_nSelected);
		m_lcCondValueList.DeleteItem(m_nSelected);
		m_nSize--;
		if (m_nSelected == m_nSize)	m_nSelected--;
		m_lcCondValueList.SetItemState(m_nSelected, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		UpdateButtons();
	}
}	

void CValueConditionalDialog::OnDefaultValue() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editVal.setDefaultValue(dlg.m_nSelection[0]);
		GetDlgItem(IDC_DEFAULT_VALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editVal.getDefaultValue()));
	}
}

void CValueConditionalDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editVal.getDefaultValue() < 0 || m_editVal.getDefaultValue() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Please enter a default value.");
		return;
	}
	for (int i=0; i<m_nSize; i++) {
		if (m_nConditionals[i] == -1) {
			AfxMessageBox(L"Missing conditional in list.");
			return;
		}
		if (m_nValues[i] == -1) {
			AfxMessageBox(L"Missing value in list.");
			return;
		}
	}
	CDialog::OnOK();
	m_editVal.setConditionals(m_nConditionals);
	m_editVal.setValueList(m_nValues);
	if (m_pVal)
		*((SSpConditionalValue*) m_pVal) = m_editVal;
	else
		m_pVal = new SSpConditionalValue(m_editVal);
}

void CValueConditionalDialog::UpdateButtons()
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_nSelected >= 0);
	GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(m_nSelected >= 0 && m_nSelected < m_nSize-1);
	GetDlgItem(IDC_MOVE_UP)->EnableWindow(m_nSelected > 0);
}

/////////////////////////////////////////////////////////////////////////////
// CValueRandomDialog dialog


CValueRandomDialog::CValueRandomDialog(CWnd* pParent /*=NULL*/)
	: CValueEditor(CValueRandomDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueRandomDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CValueRandomDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueRandomDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueRandomDialog, CDialog)
	//{{AFX_MSG_MAP(CValueRandomDialog)
	ON_BN_CLICKED(IDC_LOWER_LIMIT, OnLowerLimit)
	ON_BN_CLICKED(IDC_UPPER_LIMIT, OnUpperLimit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CValueRandomDialog::OnInitDialog() 
{
	if (m_pVal) {
		SSpRandomValue* pVal = dynamic_cast<SSpRandomValue*>(m_pVal);
		if (!pVal) {
			AfxMessageBox(L"Unable to assign value.");
			EndDialog(IDCANCEL);
		}
		m_editVal = *pVal;
	}
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);

	if (m_editVal.getValueLow() >= 0)
		GetDlgItem(IDC_LOWER_LIMIT)->SetWindowText(sspPool::Instance().values.GetName(m_editVal.getValueLow()));
	if (m_editVal.getValueHigh() >= 0)
		GetDlgItem(IDC_UPPER_LIMIT)->SetWindowText(sspPool::Instance().values.GetName(m_editVal.getValueHigh()));

	return TRUE;
}

void CValueRandomDialog::OnLowerLimit() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editVal.setValueLow(dlg.m_nSelection[0]);
		GetDlgItem(IDC_LOWER_LIMIT)->SetWindowText(sspPool::Instance().values.GetName(m_editVal.getValueLow()));
	}
}

void CValueRandomDialog::OnUpperLimit() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editVal.setValueHigh(dlg.m_nSelection[0]);
		GetDlgItem(IDC_UPPER_LIMIT)->SetWindowText(sspPool::Instance().values.GetName(m_editVal.getValueHigh()));
	}
}

void CValueRandomDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editVal.getValueLow() < 0 || m_editVal.getValueLow() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid lower value selection.");
		return;
	}
	if (m_editVal.getValueHigh() < 0 || m_editVal.getValueHigh() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid upper value selection.");
		return;
	}

	CDialog::OnOK();
	if (m_pVal)
		*((SSpRandomValue*) m_pVal) = m_editVal;
	else
		m_pVal = new SSpRandomValue(m_editVal);
}
