// SSpGUIConditionalEditor.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspPools.h"
#include "SSpGUIConditionalEditor.h"
#include "ValueDialog.h"
#include "ConditionalDialog.h"
#include "PlayObjectDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCondBooleanDialog dialog

CCondBooleanDialog::CCondBooleanDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondBooleanDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondBooleanDialog)
	m_nBool = -1;
	//}}AFX_DATA_INIT
}


void CCondBooleanDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondBooleanDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Radio(pDX, IDC_FALSE, m_nBool);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondBooleanDialog, CDialog)
	//{{AFX_MSG_MAP(CCondBooleanDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_FALSE, OnClickedBool)
	ON_BN_CLICKED(IDC_TRUE, OnClickedBool)
END_MESSAGE_MAP()


BOOL CCondBooleanDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpBoolean* pCond = dynamic_cast<SSpBoolean*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	m_nBool = m_editCond.isTrue() ? 1 : 0;
	SetWindowText(m_strTitle);
	return CDialog::OnInitDialog();
}

void CCondBooleanDialog::OnClickedBool()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty())
		m_strName = (m_nBool == 1) ? "True" : "False";
	UpdateData(FALSE);
}

void CCondBooleanDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nBool < 0) {
		AfxMessageBox(L"Please make a selection.");
		return;
	}
	CDialog::OnOK();
	m_editCond.setValue(m_nBool == 1);
	if (m_pCond)
		*((SSpBoolean*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpBoolean(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondRelationDialog dialog

CCondRelationDialog::CCondRelationDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondRelationDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondRelationDialog)
	m_strName = _T("");
	m_nRelation = -1;
	//}}AFX_DATA_INIT
}


void CCondRelationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondRelationDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Radio(pDX, IDC_EQUAL, m_nRelation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondRelationDialog, CDialog)
	//{{AFX_MSG_MAP(CCondRelationDialog)
	ON_BN_CLICKED(IDC_TESTVALUE_BTN, OnTestvalueBtn)
	ON_BN_CLICKED(IDC_THRESHOLD_BTN, OnThresholdBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCondRelationDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpValueRelation* pCond = dynamic_cast<SSpValueRelation*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	m_nRelation = m_editCond.getRelation();
	if (m_editCond.getTestValue() >= 0)
		GetDlgItem(IDC_TESTVALUE_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editCond.getTestValue()));
	if (m_editCond.getThresholdValue() >= 0)
		GetDlgItem(IDC_THRESHOLD_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editCond.getThresholdValue()));
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}

void CCondRelationDialog::OnTestvalueBtn() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editCond.setTestValue(dlg.m_nSelection[0]);
		GetDlgItem(IDC_TESTVALUE_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editCond.getTestValue()));
	}
}

void CCondRelationDialog::OnThresholdBtn() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editCond.setThresholdValue(dlg.m_nSelection[0]);
		GetDlgItem(IDC_THRESHOLD_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editCond.getThresholdValue()));
	}
}

void CCondRelationDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nRelation < 0) {
		AfxMessageBox(L"Please select a relation.");
		return;
	}
	if (m_editCond.getTestValue() < 0 || m_editCond.getTestValue() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid test value selection.");
		return;
	}
	if (m_editCond.getThresholdValue() < 0 || m_editCond.getThresholdValue() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid threshold value selection.");
		return;
	}
	CDialog::OnOK();
	m_editCond.setRelation(m_nRelation);
	if (m_pCond)
		*((SSpValueRelation*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpValueRelation(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondRangeDialog dialog


CCondRangeDialog::CCondRangeDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondRangeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondRangeDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CCondRangeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondRangeDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondRangeDialog, CDialog)
	//{{AFX_MSG_MAP(CCondRangeDialog)
	ON_BN_CLICKED(IDC_LOWERVALUE_BTN, OnLowervalueBtn)
	ON_BN_CLICKED(IDC_TESTVALUE_BTN, OnTestvalueBtn)
	ON_BN_CLICKED(IDC_UPPERVALUE_BTN, OnUppervalueBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCondRangeDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpInRange* pCond = dynamic_cast<SSpInRange*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	if (m_editCond.getRangeMin() >= 0)
		GetDlgItem(IDC_LOWERVALUE_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editCond.getRangeMin()));
	if (m_editCond.getRangeMax() >= 0)
		GetDlgItem(IDC_UPPERVALUE_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editCond.getRangeMax()));
	if (m_editCond.getTestValue() >= 0)
		GetDlgItem(IDC_TESTVALUE_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editCond.getTestValue()));
	return TRUE;
}

void CCondRangeDialog::OnLowervalueBtn() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editCond.setRangeMin(dlg.m_nSelection[0]);
		GetDlgItem(IDC_LOWERVALUE_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editCond.getRangeMin()));
	}
}

void CCondRangeDialog::OnTestvalueBtn() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editCond.setTestValue(dlg.m_nSelection[0]);
		GetDlgItem(IDC_TESTVALUE_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editCond.getTestValue()));
	}
}

void CCondRangeDialog::OnUppervalueBtn() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editCond.setRangeMax(dlg.m_nSelection[0]);
		GetDlgItem(IDC_UPPERVALUE_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editCond.getRangeMax()));
	}
}

void CCondRangeDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editCond.getTestValue() < 0 || m_editCond.getTestValue() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid test value selection.");
		return;
	}
	if (m_editCond.getRangeMin() < 0 || m_editCond.getRangeMin() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid lower threshold value selection.");
		return;
	}
	if (m_editCond.getRangeMax() < 0 || m_editCond.getRangeMax() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid upper threshold value selection.");
		return;
	}
	CDialog::OnOK();
	if (m_pCond)
		*((SSpInRange*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpInRange(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondTimeIntervalDialog dialog


CCondTimeIntervalDialog::CCondTimeIntervalDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondTimeIntervalDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondTimeIntervalDialog)
	m_nHourStart = 0;
	m_nHourEnd = 0;
	m_nMinuteStart = 0;
	m_nMinuteEnd = 0;
	m_strName = _T("");
	m_nSecondStart = 0;
	m_nSecondEnd = 0;
	//}}AFX_DATA_INIT
}


void CCondTimeIntervalDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondTimeIntervalDialog)
	DDX_Text(pDX, IDC_HOUR1, m_nHourStart);
	DDV_MinMaxInt(pDX, m_nHourStart, 0, 23);
	DDX_Text(pDX, IDC_HOUR2, m_nHourEnd);
	DDV_MinMaxInt(pDX, m_nHourEnd, 0, 23);
	DDX_Text(pDX, IDC_MINUTE1, m_nMinuteStart);
	DDV_MinMaxInt(pDX, m_nMinuteStart, 0, 59);
	DDX_Text(pDX, IDC_MINUTE2, m_nMinuteEnd);
	DDV_MinMaxInt(pDX, m_nMinuteEnd, 0, 59);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_SECOND1, m_nSecondStart);
	DDV_MinMaxInt(pDX, m_nSecondStart, 0, 59);
	DDX_Text(pDX, IDC_SECOND2, m_nSecondEnd);
	DDV_MinMaxInt(pDX, m_nSecondEnd, 0, 59);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondTimeIntervalDialog, CDialog)
	//{{AFX_MSG_MAP(CCondTimeIntervalDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCondTimeIntervalDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpTimeInterval* pCond = dynamic_cast<SSpTimeInterval*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	m_nHourStart = m_editCond.getStartClock().hour();
	m_nMinuteStart = m_editCond.getStartClock().min();
	m_nSecondStart = m_editCond.getStartClock().sec();
	m_nHourEnd = m_editCond.getEndClock().hour();
	m_nMinuteEnd = m_editCond.getEndClock().min();
	m_nSecondEnd = m_editCond.getEndClock().sec();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}

void CCondTimeIntervalDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	CDialog::OnOK();
	m_editCond.setStartClock(SSpClock(m_nHourStart, m_nMinuteStart, m_nSecondStart));
	m_editCond.setEndClock(SSpClock(m_nHourEnd, m_nMinuteEnd, m_nSecondEnd));
	if (m_pCond)
		*((SSpTimeInterval*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpTimeInterval(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondTimeRelationDialog dialog


CCondTimeRelationDialog::CCondTimeRelationDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondTimeRelationDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondTimeRelationDialog)
	m_nHour = 0;
	m_nMinute = 0;
	m_strName = _T("");
	m_nSecond = 0;
	m_nRelation = -1;
	//}}AFX_DATA_INIT
}


void CCondTimeRelationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondTimeRelationDialog)
	DDX_Text(pDX, IDC_HOUR, m_nHour);
	DDV_MinMaxInt(pDX, m_nHour, 0, 23);
	DDX_Text(pDX, IDC_MINUTE, m_nMinute);
	DDV_MinMaxInt(pDX, m_nMinute, 0, 59);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_SECOND, m_nSecond);
	DDV_MinMaxInt(pDX, m_nSecond, 0, 59);
	DDX_Radio(pDX, IDC_BEFORE, m_nRelation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondTimeRelationDialog, CDialog)
	//{{AFX_MSG_MAP(CCondTimeRelationDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCondTimeRelationDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpTimeRelation* pCond = dynamic_cast<SSpTimeRelation*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	m_nRelation = m_editCond.getRelation();
	m_nHour = m_editCond.getClock().hour();
	m_nMinute = m_editCond.getClock().min();
	m_nSecond = m_editCond.getClock().sec();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}

void CCondTimeRelationDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nRelation < 0) {
		AfxMessageBox(L"Please select a relation.");
		return;
	}
	CDialog::OnOK();
	SSpClock cl(m_nHour, m_nMinute, m_nSecond);
	m_editCond.setClock(cl);
	m_editCond.setRelation(m_nRelation);
	if (m_pCond)
		*((SSpTimeRelation*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpTimeRelation(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondMinuteRelationDialog dialog


CCondMinuteRelationDialog::CCondMinuteRelationDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondMinuteRelationDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondMinuteRelationDialog)
	m_nMinutes = 0;
	m_strName = _T("");
	m_nRelation = -1;
	//}}AFX_DATA_INIT
}


void CCondMinuteRelationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondMinuteRelationDialog)
	DDX_Text(pDX, IDC_MINUTES, m_nMinutes);
	DDV_MinMaxInt(pDX, m_nMinutes, 0, 59);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Radio(pDX, IDC_EQUAL, m_nRelation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondMinuteRelationDialog, CDialog)
	//{{AFX_MSG_MAP(CCondMinuteRelationDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCondMinuteRelationDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpMinuteRelation* pCond = dynamic_cast<SSpMinuteRelation*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	m_nRelation = m_editCond.getRelation();
	m_nMinutes = m_editCond.getMinute();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}

void CCondMinuteRelationDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nRelation < 0) {
		AfxMessageBox(L"Please select a relation.");
		return;
	}
	CDialog::OnOK();
	m_editCond.setMinute(m_nMinutes);
	m_editCond.setRelation(m_nRelation);
	if (m_pCond)
		*((SSpMinuteRelation*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpMinuteRelation(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondDateIntervalDialog dialog


CCondDateIntervalDialog::CCondDateIntervalDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondDateIntervalDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondDateIntervalDialog)
	m_nStartDay = 1;
	m_nEndDay = 1;
	m_nStartMonth = 1;
	m_nEndMonth = 1;
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CCondDateIntervalDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondDateIntervalDialog)
	DDX_Text(pDX, IDC_DAY1, m_nStartDay);
	DDV_MinMaxInt(pDX, m_nStartDay, 1, 31);
	DDX_Text(pDX, IDC_DAY2, m_nEndDay);
	DDV_MinMaxInt(pDX, m_nEndDay, 1, 31);
	DDX_Text(pDX, IDC_MONTH1, m_nStartMonth);
	DDV_MinMaxInt(pDX, m_nStartMonth, 1, 12);
	DDX_Text(pDX, IDC_MONTH2, m_nEndMonth);
	DDV_MinMaxInt(pDX, m_nEndMonth, 1, 12);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondDateIntervalDialog, CDialog)
	//{{AFX_MSG_MAP(CCondDateIntervalDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCondDateIntervalDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpDateInterval* pCond = dynamic_cast<SSpDateInterval*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	m_nStartDay = m_editCond.getStartDate().day();
	m_nStartMonth = m_editCond.getStartDate().month();
	m_nEndDay = m_editCond.getEndDate().day();
	m_nEndMonth = m_editCond.getEndDate().month();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}

void CCondDateIntervalDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	CDialog::OnOK();
	m_editCond.setStartDate(SSpDate(m_nStartDay, m_nStartMonth));
	m_editCond.setEndDate(SSpDate(m_nEndDay, m_nEndMonth));
	if (m_pCond)
		*((SSpDateInterval*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpDateInterval(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondDayOfWeekDialog dialog

CCondDayOfWeekDialog::CCondDayOfWeekDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondDayOfWeekDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondDateIntervalDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_nSelectCount = 0;
}


void CCondDayOfWeekDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondDayOfWeekDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondDayOfWeekDialog, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_WEEKDAY1, IDC_WEEKDAY7, OnDay )
END_MESSAGE_MAP()


BOOL CCondDayOfWeekDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpDayOfWeek* pCond = dynamic_cast<SSpDayOfWeek*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
  for (int i=sspLocalTime::SUNDAY; i <= sspLocalTime::SATURDAY; ++i) {
		CButton* pBtn = (CButton*) GetDlgItem(IDC_WEEKDAY1+i);
		if (m_editCond.getDayCheck(i)) {
            pBtn->SetCheck(BST_CHECKED);
			m_nSelectCount++;
		}
		else {
            pBtn->SetCheck(BST_UNCHECKED);
		}
	}
	SetWindowText(m_strTitle);
	return CDialog::OnInitDialog();
}

void CCondDayOfWeekDialog::OnDay(unsigned int nID)
{
	int wd = nID - IDC_WEEKDAY1;
	if (wd > sspLocalTime::SATURDAY)	return;
	CButton* pBtn = (CButton*) GetDlgItem(nID);
	if (pBtn->GetCheck() == BST_CHECKED) {
        m_editCond.setDayCheck(wd, true);
		m_nSelectCount++;
	}
	else {
        m_editCond.setDayCheck(wd, false);
		m_nSelectCount--;
	}
	UpdateData(TRUE);
	if (m_nSelectCount == 1) {
		for (int i=sspLocalTime::SUNDAY; i <= sspLocalTime::SATURDAY; ++i) {
			if (m_editCond.getDayCheck(i)) {
				m_strName = sspLocalTime::s_dayNames[i];
				break;
			}
		}
	}
	UpdateData(FALSE);
}

void CCondDayOfWeekDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nSelectCount < 1) {
		AfxMessageBox(L"Please select at least one day.");
		return;
	}
	CDialog::OnOK();
	if (m_pCond)
		*((SSpDayOfWeek*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpDayOfWeek(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondAndDialog dialog


CCondAndDialog::CCondAndDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondAndDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondAndDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

CCondAndDialog::~CCondAndDialog()
{
}

void CCondAndDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondAndDialog)
	DDX_Control(pDX, IDC_CONDITIONALS, m_lbOperands);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondAndDialog, CDialog)
	//{{AFX_MSG_MAP(CCondAndDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_SELCHANGE(IDC_CONDITIONALS, OnSelectConditional)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCondAndDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpAnd* pCond = dynamic_cast<SSpAnd*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	m_nOperands = m_editCond.getConditionals();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
	m_lbOperands.ResetContent();
	for (unsigned int i=0; i<m_nOperands.size(); i++)
		m_lbOperands.AddString(sspPool::Instance().conditionals.GetName(m_nOperands[i]));
	return TRUE;
}

void CCondAndDialog::OnAdd() 
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nOperands.insert(m_nOperands.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbOperands.AddString(sspPool::Instance().conditionals.GetName(dlg.m_nSelection[i]));
		}
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CCondAndDialog::OnRemove() 
{
	int nIndex = m_lbOperands.GetCurSel();
	if (nIndex >= 0 && nIndex < (int) m_nOperands.size()) {
		m_lbOperands.DeleteString(nIndex);
		m_lbOperands.SetCurSel(-1);
		m_nOperands.erase(m_nOperands.begin() + nIndex);
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CCondAndDialog::OnSelectConditional() 
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_lbOperands.GetCurSel() >= 0);
}

void CCondAndDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nOperands.size() < 2) {
		AfxMessageBox(L"Please select at least two conditionals.");
		return;
	}
	CDialog::OnOK();
	m_editCond.setConditionals(m_nOperands);
	if (m_pCond)
		*((SSpAnd*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpAnd(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondOrDialog dialog


CCondOrDialog::CCondOrDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondOrDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondOrDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

CCondOrDialog::~CCondOrDialog()
{
}

void CCondOrDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondOrDialog)
	DDX_Control(pDX, IDC_CONDITIONALS, m_lbOperands);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondOrDialog, CDialog)
	//{{AFX_MSG_MAP(CCondOrDialog)
	ON_LBN_SELCHANGE(IDC_CONDITIONALS, OnSelectConditional)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCondOrDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpOr* pCond = dynamic_cast<SSpOr*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	m_nOperands = m_editCond.getConditionals();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
	m_lbOperands.ResetContent();
	for (unsigned int i=0; i<m_nOperands.size(); i++)
		m_lbOperands.AddString(sspPool::Instance().conditionals.GetName(m_nOperands[i]));
	return TRUE;
}

void CCondOrDialog::OnAdd() 
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nOperands.insert(m_nOperands.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbOperands.AddString(sspPool::Instance().conditionals.GetName(dlg.m_nSelection[i]));
		}
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CCondOrDialog::OnRemove() 
{
	int nIndex = m_lbOperands.GetCurSel();
	if (nIndex >= 0 && nIndex < (int) m_nOperands.size()) {
		m_lbOperands.DeleteString(nIndex);
		m_lbOperands.SetCurSel(-1);
		m_nOperands.erase(m_nOperands.begin() + nIndex);
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CCondOrDialog::OnSelectConditional() 
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_lbOperands.GetCurSel() >= 0);
}

void CCondOrDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nOperands.size() < 2) {
		AfxMessageBox(L"Please select at least two conditionals.");
		return;
	}
	CDialog::OnOK();
	m_editCond.setConditionals(m_nOperands);
	if (m_pCond)
		*((SSpOr*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpOr(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondNotDialog dialog


CCondNotDialog::CCondNotDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondNotDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondNotDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CCondNotDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondNotDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCondNotDialog, CDialog)
	//{{AFX_MSG_MAP(CCondNotDialog)
	ON_BN_CLICKED(IDC_CONDITIONAL, OnConditional)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCondNotDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpNot* pCond = dynamic_cast<SSpNot*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	if (m_editCond.getConditional() >= 0)
		GetDlgItem(IDC_CONDITIONAL)->SetWindowText(sspPool::Instance().conditionals.GetName(m_editCond.getConditional()));
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}

void CCondNotDialog::OnConditional() 
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editCond.setConditional(dlg.m_nSelection[0]);
		GetDlgItem(IDC_CONDITIONAL)->SetWindowText(sspPool::Instance().conditionals.GetName(m_editCond.getConditional()));
	}
}

void CCondNotDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editCond.getConditional() < 0 || m_editCond.getConditional() > (int) sspPool::Instance().conditionals.GetSize()) {
		AfxMessageBox(L"Invalid conditional selection.");
		return;
	}
	CDialog::OnOK();
	if (m_pCond)
		*((SSpNot*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpNot(m_editCond);
}

/////////////////////////////////////////////////////////////////////////////
// CCondPlayingDialog dialog


CCondPlayingDialog::CCondPlayingDialog(CWnd* pParent /*=NULL*/)
	: CConditionalEditor(CCondPlayingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCondPlayingDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

CCondPlayingDialog::~CCondPlayingDialog()
{
}


void CCondPlayingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCondPlayingDialog)
	DDX_Control(pDX, IDC_PLAYOBJECTS, m_lbOperands);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCondPlayingDialog, CDialog)
	//{{AFX_MSG_MAP(CCondPlayingDialog)
	ON_LBN_SELCHANGE(IDC_PLAYOBJECTS, OnSelectPlayobjects)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCondPlayingDialog::OnInitDialog() 
{
	if (m_pCond) {
		SSpIsPlaying* pCond = dynamic_cast<SSpIsPlaying*>(m_pCond);
		if (!pCond) {
			AfxMessageBox(L"Unable to assign conditional.");
			EndDialog(IDCANCEL);
		}
		m_editCond = *pCond;
	}
	m_nOperands = m_editCond.getObjects();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
	m_lbOperands.ResetContent();
	for (unsigned int i=0; i<m_nOperands.size(); i++)
		m_lbOperands.AddString(sspPool::Instance().objects.GetName(m_nOperands[i]));
	return TRUE;
}

void CCondPlayingDialog::OnAdd() 
{
	CPlayObjectDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nOperands.insert(m_nOperands.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbOperands.AddString(sspPool::Instance().objects.GetName(dlg.m_nSelection[i]));
		}
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CCondPlayingDialog::OnRemove() 
{
	int nIndex = m_lbOperands.GetCurSel();
	if (nIndex >= 0 && nIndex < (int) m_nOperands.size()) {
		m_lbOperands.DeleteString(nIndex);
		m_lbOperands.SetCurSel(-1);
		m_nOperands.erase(m_nOperands.begin() + nIndex);
	}
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CCondPlayingDialog::OnSelectPlayobjects() 
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_lbOperands.GetCurSel() >= 0);
}

void CCondPlayingDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nOperands.size() < 1) {
		AfxMessageBox(L"Please select at least one play object.");
		return;
	}
	CDialog::OnOK();
	m_editCond.setObjects(m_nOperands);
	if (m_pCond)
		*((SSpIsPlaying*) m_pCond) = m_editCond;
	else
		m_pCond = new SSpIsPlaying(m_editCond);
}
