//////////////////////////////////////////
// sspWatchdog.h

#ifndef SSP_WATCHDOG_H
#define SSP_WATCHDOG_H

#include "sspResetManager.h"

class sspWatchdogBB_ISA : public sspWatchdog
{
protected:
	enum {CHOICES=4, TIMESTRING=12, BIGINT=327600};
	enum {COMPORT=2, BAUDRATE=4, IOADDRESS=0x378};
	bool sendCommand(char command[]);

public:
	sspWatchdogBB_ISA(void);
	~sspWatchdogBB_ISA(void);

	bool initialize(int nSecs);
	bool enable();
	void refresh();
	bool disable();
	bool terminate();
};

class sspWatchdogBB_PCI : public sspWatchdog
{
protected:
	static const double	fTimeOuts[];
	BYTE		            m_nTimeOut;
	HANDLE              m_handle;

public:
	sspWatchdogBB_PCI(void);
	~sspWatchdogBB_PCI(void);

	bool initialize(int nSecs);
	bool enable();
	void refresh();
	bool disable();
	bool terminate();
};

class sspWatchdogIB780 : public sspWatchdog
{
protected:
	static const int s_nTimeOuts[];
	BYTE		         m_nTimeOut;

  enum {ENABLE_PORT=0x0443, DISABLE_PORT=0x0441};

public:
	sspWatchdogIB780(void);
	~sspWatchdogIB780(void);

	bool initialize(int nSecs);
	bool enable();
	void refresh();
	bool disable();
	bool terminate();
};

#endif