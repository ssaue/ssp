// sspDmxDevice.h

#ifndef SSP_DMXDEVICE_H
#define SSP_DMXDEVICE_H

#include <vector>
#include <CKController.h>
#include "sspDevice.h"

class sspDmxDevice
{
private:
	DevID	m_id;
	bool	m_bOpen;

public:
	sspDmxDevice();
	virtual ~sspDmxDevice();

	bool open(DevEntry* pDev);
	bool close();
	bool isOpen() {return m_bOpen;}

	bool startPlay() const {return startLivePlay(m_id) == SUCCESS;}
	bool stopPlay() const {return stopLivePlay(m_id) == SUCCESS;}

	static void initializeData(DMXFrame& frame, int nChannels);
	bool sendData(DMXFrame* pFrame) const {return sendFrame(m_id, pFrame) == SUCCESS;}
};


class sspDmxDeviceGroup : public sspDeviceGroup
{
private:
	std::vector<DevEntry*> m_info;
	std::vector<sspDmxDevice*> m_pDevices;

	friend class sspDeviceManager;
	sspDmxDeviceGroup();
	virtual ~sspDmxDeviceGroup();

public:
	// General device accessors (virtual)
	unsigned int		getAllDevices();
	unsigned int		getDeviceCount() {return m_info.size();}
	sspString	getDeviceName(int nDevice);

	// Initialization and termination
	bool initializeImpl(LPVOID hWnd);
	bool beginImpl();
	bool endImpl();
	bool terminateImpl();
	
	// Get a pointer to DMX device
	sspDmxDevice* getDevice(int nSubsetIndex);

private:
	void clearInfo();
};


#endif