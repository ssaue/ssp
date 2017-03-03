// sspDeviceGroups.h

#ifndef SSP_DEVICEGROUPS_H
#define SSP_DEVICEGROUPS_H

#include "sspDevice.h"
#include <vector>

class sspMidiOutDevice;
class sspMidiOutDeviceGroup : public sspDeviceGroup
{
private:
	std::vector<sspMidiOutDevice*> m_pDevices;

	friend class sspDeviceManager;
	sspMidiOutDeviceGroup();
	virtual ~sspMidiOutDeviceGroup();

public:
	// General device accessors (virtual)
	unsigned int		getAllDevices();
  unsigned int		getDeviceCount();
	sspString	getDeviceName(int nDevice);

	// Initialization and termination
	bool initializeImpl(LPVOID hWnd);
	bool beginImpl() {return true;}
	bool endImpl() {return true;}
	bool terminateImpl();
	
	// Get a pointer to MIDI device
	sspMidiOutDevice* getDevice(int nSubsetIndex);
};

class sspMidiInDevice;
class sspMidiInDeviceGroup : public sspDeviceGroup
{
private:
	std::vector<sspMidiInDevice*> m_pDevices;

	friend class sspDeviceManager;
	sspMidiInDeviceGroup();
	virtual ~sspMidiInDeviceGroup();

public:
	// General device accessors (virtual)
	unsigned int		getAllDevices();
  unsigned int		getDeviceCount();
	sspString	getDeviceName(int nDevice);

	// Initialization and termination
	bool initializeImpl(LPVOID hWnd);
	bool beginImpl();
	bool endImpl();
	bool terminateImpl();
	
	// Get a pointer to MIDI device
	sspMidiInDevice* getDevice(int nSubsetIndex);
};

#endif