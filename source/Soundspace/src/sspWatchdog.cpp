//////////////////////////////////////////
// sspWatchdog.h

#include "stdafx.h"
#include "sspwatchdog.h"
#include <WatchdogAPI.h>
#include <inpout32drv.h>

//////////////////////////////////////////////////////////////////////
// sspWatchdogBB_ISA
//////////////////////////////////////////////////////////////////////

inline bool sspWatchdogBB_ISA::sendCommand(char command[])
{
	int t=0;
	for (int i=0; command[i]; i++)
	{
		Out32(IOADDRESS,command[i]);
		while ((Inp32(IOADDRESS+1)&0x80)==0)
		{
			t++;
			if (t>BIGINT) return false;	/*  error */
		}
	}
	t=0;
	Out32(IOADDRESS,0x0d);		/* send out the 0x0D to WDT-01 */
	while ((Inp32(IOADDRESS+1)&0x80)==0)
	{
		t++;
		if (t>BIGINT) return false;	/*  error */
	}
	return true;
}

sspWatchdogBB_ISA::sspWatchdogBB_ISA(void)
{
}

sspWatchdogBB_ISA::~sspWatchdogBB_ISA(void)
{
}

bool sspWatchdogBB_ISA::initialize(int nSecs)
{
	// Let BIOS know that baseUART uses baudRate
	sendCommand("");	//Send a NULL command
	int nVal = Inp32(IOADDRESS+1)&0x07;
	TRACE1("Status value = %d\n", nVal);
	if (!disable())
		return false;
	char text[TIMESTRING];
	sprintf_s(text, TIMESTRING, "T=%.2f", nSecs);
	if (!sendCommand(text))
		return false;
	return true;	// Always true if not enabled
}

void sspWatchdogBB_ISA::refresh()
{
  sendCommand("S");
}

bool sspWatchdogBB_ISA::enable()
{
  return sendCommand("S");
}

bool sspWatchdogBB_ISA::disable()
{
	return sendCommand("E");
}

bool sspWatchdogBB_ISA::terminate()
{
	return disable();
}

//////////////////////////////////////////////////////////////////////
// sspWatchdogBB_PCI
//////////////////////////////////////////////////////////////////////

const double sspWatchdogBB_PCI::fTimeOuts[] = {0.5, 2.0, 6.0, 14.0, 30.0, 126.0, 254.0, 510.0};

sspWatchdogBB_PCI::sspWatchdogBB_PCI(void)
: m_handle(INVALID_HANDLE_VALUE)
, m_nTimeOut(0)
{
}

sspWatchdogBB_PCI::~sspWatchdogBB_PCI(void)
{
}

bool sspWatchdogBB_PCI::initialize(int nSecs)
{
	BYTE i;

	m_handle = BBWatchdog_Startup( 1 );
	m_nTimeOut = 1;
	if (m_handle != INVALID_HANDLE_VALUE) {
		for (i=0; i < 8 && fTimeOuts[i] < nSecs; i++);
		if (fTimeOuts[i] == nSecs || i == 0)
			m_nTimeOut += 2*i;
		else
			m_nTimeOut += 2*(i-1);
		return true;
	}
	else
		return false;
}

void sspWatchdogBB_PCI::refresh()
{
	BBWatchdog_Reset(m_handle, m_nTimeOut);
}

bool sspWatchdogBB_PCI::enable()
{
	return BBWatchdog_Reset(m_handle, m_nTimeOut) == TRUE;
}

bool sspWatchdogBB_PCI::disable()
{
	BBWatchdog_Reset(m_handle, 0);
	return BBWatchdog_Shutdown(m_handle) == TRUE;
}

bool sspWatchdogBB_PCI::terminate()
{
	disable();
	return BBWatchdog_Shutdown(m_handle) == TRUE;
}

//////////////////////////////////////////////////////////////////////
// sspWatchdogIB780
//////////////////////////////////////////////////////////////////////

const int sspWatchdogIB780::s_nTimeOuts[] = {30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0};

sspWatchdogIB780::sspWatchdogIB780(void)
{
}

sspWatchdogIB780::~sspWatchdogIB780(void)
{
}

bool sspWatchdogIB780::initialize(int nSecs)
{
  for (m_nTimeOut=15; m_nTimeOut >= 0; --m_nTimeOut) {
    if (s_nTimeOuts[m_nTimeOut] >= nSecs) return true;
  }
  return false; // Too long time out requirement
}

void sspWatchdogIB780::refresh()
{
  Out32(ENABLE_PORT, m_nTimeOut);
}

bool sspWatchdogIB780::enable()
{
  Out32(ENABLE_PORT, m_nTimeOut);
  return true;
}

bool sspWatchdogIB780::disable()
{
  Out32(DISABLE_PORT, m_nTimeOut);
  return true;
}

bool sspWatchdogIB780::terminate()
{
	return disable();
}
