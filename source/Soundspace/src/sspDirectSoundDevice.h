// sspDirectSoundDevice.h

#ifndef SSP_DIRECTSOUNDDEVICE_H
#define SSP_DIRECTSOUNDDEVICE_H

#include <mmsystem.h>
#include <dsound.h>
#include "sspDevice.h"

/****************************************************************************
** Direct Sound Device class
****************************************************************************/

class sspDSDeviceGroup : public sspDeviceGroup
{
private:
	struct dsInfoStruct {
		LPGUID		lpGuid;
		sspString	strDesc;
	};
	std::vector<dsInfoStruct*> m_dsInfo;

	std::vector<LPDIRECTSOUND> m_pDS;
	std::vector<LPDIRECTSOUNDBUFFER> m_pDSBuf;

    HWND m_hApp;
	DSBUFFERDESC m_dsBufDesc;
	WAVEFORMATEX m_pcmWf;

	friend class sspDeviceManager;
	sspDSDeviceGroup();
	virtual ~sspDSDeviceGroup();

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

    LPDIRECTSOUND getPDS(int nDevice);

private:
	void setBufferFormat(int nChannels);

	// Getting device information
	void addInfo(LPGUID lpGuid, LPCWSTR lpDesc);
	void clearInfo();
	static BOOL CALLBACK enumCallback(LPGUID lpGuid, LPCWSTR lpDesc, LPCWSTR lpModule, LPVOID lpContext);
};

#endif