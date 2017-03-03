// DMXselectFile.h: interface for the DMXselectFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef DMXSELECTFILE_H
#define DMXSELECTFILE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SSpMessage.h"
#include "sspScheduler.h"
#include "sspDmxDevice.h"

#include <vector>
#include <fstream>

class sspValue;
typedef std::vector<DMXFrame> DMXvector;

class DMXselectFile : public sspScheduleObject
{
private:
	std::ifstream	m_file;
	DMXvector		m_vFrames;
	sspValue*		m_pDuration;
	sspValue*		m_pIndex;
	time_t			m_startTime;
	int				m_nFrames;
	int				m_nChannels;
	int				m_nRefreshRate;

	sspDmxDevice*	m_pDevice;
	sspMsgHandler*	m_pOwner;

public:
	DMXselectFile();
	virtual ~DMXselectFile();

	bool open(LPCTSTR pFileName);
	bool readData(void);
	void close();

	void initialize(int nDevice, sspValue* pDur, sspValue* pIndex, int nRefreshRate);

	bool begin();
	bool update();
	void end();

	unsigned int getDuration() const;
	void setOwner(sspMsgHandler* pOwner);
};

#endif
