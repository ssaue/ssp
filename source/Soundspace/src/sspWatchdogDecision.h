//////////////////////////////////////////
// sspWatchdogDecision.h

#ifndef SSP_WATCHDOGDECISION_H
#define SSP_WATCHDOGDECISION_H

#include "sspResetManager.h"

class sspWatchdogRunDlg;

class sspWatchdogDecision : public sspWatchdog
{
protected:
	sspWatchdogRunDlg*	m_pDlg;
  long                m_countDown;

public:
	sspWatchdogDecision(void);
	~sspWatchdogDecision(void);

	bool initialize(int nSecs);
	bool enable();
	void refresh();
	bool disable();
	bool terminate();

  int counter();
};


#endif