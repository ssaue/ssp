// sspWavDevice.h

#ifndef SSP_WAVDEVICE_H
#define SSP_WAVDEVICE_H

#include <mmsystem.h>
#include "sspDevice.h"

class sspWavOutDeviceGroup : public sspDeviceGroup
{
private:
	friend class sspDeviceManager;
	sspWavOutDeviceGroup();
	virtual ~sspWavOutDeviceGroup();

public:
	// General device accessors (virtual)
	unsigned int		getAllDevices() {return waveOutGetNumDevs();}
	unsigned int		getDeviceCount() {return waveOutGetNumDevs();}
	sspString	getDeviceName(int nDevice);

	// Initialization and termination (virtual overrides)
	bool initializeImpl(LPVOID hWnd) {return true;}
	bool beginImpl() {return true;}
	bool endImpl() {return true;}
	bool terminateImpl() {return true;}
};

#endif