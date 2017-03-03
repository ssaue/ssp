// SettingsResetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "SettingsResetDlg.h"
#include "sspResetManager.h"
#include "ConditionalDialog.h"
#include "sspPools.h"
#include ".\settingsresetdlg.h"

// CSettingsResetDlg dialog

IMPLEMENT_DYNAMIC(CSettingsResetDlg, CDialog)
CSettingsResetDlg::CSettingsResetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsResetDlg::IDD, pParent)
  , m_bEnable(FALSE)
  , m_nWatchdogType(0)
  , m_nTimeout(0)
  , m_nInterval(0)
  , m_timeOfDay(0)
  , m_nTimeUnit(0)
{
}

CSettingsResetDlg::~CSettingsResetDlg()
{
}

void CSettingsResetDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Check(pDX, IDC_WATCHDOG_ENABLE, m_bEnable);
  DDX_CBIndex(pDX, IDC_WATCHDOG_TYPE, m_nWatchdogType);
  DDX_Text(pDX, IDC_WATCHDOG_TIME, m_nTimeout);
  DDV_MinMaxInt(pDX, m_nTimeout, 1, 100);
  DDX_Text(pDX, IDC_RESET_INTERVAL, m_nInterval);
  DDV_MinMaxInt(pDX, m_nInterval, 0, 100);
  DDX_CBIndex(pDX, IDC_RESET_TIMEUNIT, m_nTimeUnit);
  DDX_DateTimeCtrl(pDX, IDC_TIMEOFDAY, m_timeOfDay);
}


BEGIN_MESSAGE_MAP(CSettingsResetDlg, CDialog)
	ON_BN_CLICKED(IDC_WATCHDOG_ENABLE, OnWatchdogEnable)
	ON_BN_CLICKED(IDC_SELECT_CONDITION, OnSelectCondition)
  ON_CBN_SELCHANGE(IDC_RESET_TIMEUNIT, onSelectTimeUnit)
  ON_BN_CLICKED(IDC_HARDREBOOT, OnHardReboot)
  ON_BN_CLICKED(IDC_SOFTREBOOT, OnSoftReboot)
END_MESSAGE_MAP()

BOOL CSettingsResetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
  updateControls();
	if (m_nConditionIndex >= 0)
		GetDlgItem(IDC_SELECT_CONDITION)->SetWindowText(sspPool::Instance().conditionals.GetName(m_nConditionIndex));
	return true;
}

void CSettingsResetDlg::OnSelectCondition() 
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_nConditionIndex = dlg.m_nSelection[0];
		GetDlgItem(IDC_SELECT_CONDITION)->SetWindowText(sspPool::Instance().conditionals.GetName(m_nConditionIndex));
	}
}

void CSettingsResetDlg::OnWatchdogEnable() 
{
  UpdateData(TRUE);
  updateControls();
}

void CSettingsResetDlg::updateControls()
{
	GetDlgItem(IDC_WATCHDOG_TYPE)->EnableWindow(m_bEnable);
	GetDlgItem(IDC_WATCHDOG_TIME)->EnableWindow(m_bEnable);
	GetDlgItem(IDC_HARDREBOOT)->EnableWindow(m_bEnable);
  GetDlgItem(IDC_TIMEOFDAY)->EnableWindow(m_nTimeUnit == sspResetManager::TUNIT_DAYS);
}

void CSettingsResetDlg::onSelectTimeUnit()
{
  UpdateData(TRUE);
  updateControls();
}

void CSettingsResetDlg::OnOK()
{
  UpdateData(TRUE);
  if (m_bEnable) {
    sspWatchdog* pDog = sspWatchdog::create(m_nWatchdogType);
    bool bOK = pDog && pDog->initialize(10);
    if (bOK) pDog->terminate();
    delete pDog;
    if (!bOK) {
      AfxMessageBox(L"The selected watchdog is not accessible. Please disable or try another type!");
      return;
    }
  }
 	CDialog::OnOK();
}

void CSettingsResetDlg::OnSoftReboot()
{
  if (IDYES == AfxMessageBox(L"This operation will reboot your computer! Are you sure you want to continue?", MB_YESNO|MB_ICONEXCLAMATION))
    sspResetManager::softReboot();
}

void CSettingsResetDlg::OnHardReboot()
{
  UpdateData(TRUE);
  if (m_bEnable) {
    sspWatchdog* pDog = sspWatchdog::create(m_nWatchdogType);
    if (pDog && pDog->initialize(m_nTimeout)) {
      if (IDYES == AfxMessageBox(L"This operation will reboot your computer! Are you sure you want to continue?", MB_YESNO|MB_ICONEXCLAMATION) && pDog->enable())
      {
        bool bRefresh = true;
        while (bRefresh) {
          wchar_t warning[80];
          swprintf_s(warning, 80, _T("Reboot in %d seconds! Press ABORT to disable or RETRY to refresh."), static_cast<int>(pDog->counter()) / 2);
          int nResponse = AfxMessageBox(warning, MB_ABORTRETRYIGNORE|MB_ICONEXCLAMATION);
          switch (nResponse) {
            case IDABORT: 
              pDog->disable();
              pDog->terminate();
              bRefresh = false;
              break;
            case IDRETRY:
              pDog->refresh();
              break;
            case IDIGNORE:
            default:
              bRefresh = false;
              break;
          }
        }
      }
      else
        pDog->terminate();
    }
    else {
      AfxMessageBox(L"The selected watchdog is not accessible. Please disable or try another type!");
    }
    delete pDog;
    return;
  }
}
