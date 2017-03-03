//////////////////////////////////////////
// sspWatchdogDecision.cpp

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspWatchdogDecision.h"
#include "sspWatchdogRunDlg.h"

// Time unit handling
static int nTimeDiv[] = {2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2};
static long nTimeUnit = 10;

sspWatchdogDecision::sspWatchdogDecision(void)
: m_pDlg(NULL)
, m_countDown(0L)
{
}

sspWatchdogDecision::~sspWatchdogDecision(void)
{
  delete m_pDlg;
}

bool sspWatchdogDecision::initialize(int nSecs)
{
  if (!m_pDlg) m_pDlg = new sspWatchdogRunDlg();
  if (!m_pDlg)
    return false;

  if (m_pDlg->GetSafeHwnd() == 0) m_pDlg->Create();

	// Make sure the card is disabled to begin with
  m_countDown = nSecs * nTimeDiv[nTimeUnit];
  if (m_pDlg->m_wDog.SetTimeUnit(nTimeUnit) || m_pDlg->m_wDog.SetCounter(m_countDown)) {
    terminate();
    return false;
  }
	return true;
}

bool sspWatchdogDecision::enable()
{
  if (m_pDlg->m_wDog.EnableorLoad() || m_pDlg->m_wDog.SetCounter(m_countDown))
    return false;
  else {
    RECT winRect, dogRect;
    GetWindowRect(GetDesktopWindow(), &winRect);
    m_pDlg->GetWindowRect(&dogRect);
    OffsetRect(&dogRect, (winRect.right - dogRect.right), -dogRect.top);
    m_pDlg->SetWindowPos(&CWnd::wndTopMost, dogRect.left, winRect.top, 1, 1, SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    long nout = m_pDlg->m_wDog.GetCounter();
    return true;
  }
}

void sspWatchdogDecision::refresh()
{
	long nout = m_pDlg->m_wDog.GetCounter();
	m_pDlg->m_wDog.EnableorLoad();	
}

bool sspWatchdogDecision::disable()
{
  if (m_pDlg->GetSafeHwnd() != 0)  m_pDlg->ShowWindow(SW_HIDE);  
	return !m_pDlg->m_wDog.Disable();
}

bool sspWatchdogDecision::terminate()
{
  bool bRtn = false;
  if (m_pDlg) {
    bRtn = disable();
    m_pDlg->DestroyWindow();
    delete m_pDlg;
    m_pDlg = NULL;
  }
  return bRtn;
}

int sspWatchdogDecision::counter()
{
  long nRet = m_pDlg->m_wDog.GetCounter();
  return static_cast<int>(nRet);
}
