// sspDirectSoundDevice.cpp

#include "stdafx.h"
#include "sspDirectSoundDevice.h"
#include "sspSetting.h"
#include "debug.h"

// sspDSDeviceGroup class implementation
//
////////////////////////////////////////////////////////////

sspDSDeviceGroup::sspDSDeviceGroup()
{
	m_strName = _T("Direct Sound");
    // Initialize member data
    m_hApp = NULL;
}

// Destructor
sspDSDeviceGroup::~sspDSDeviceGroup()
{
	terminate();
	for (unsigned int i=0; i<m_dsInfo.size(); ++i) {
		delete m_dsInfo[i]->lpGuid;
		delete m_dsInfo[i];
	}
	m_dsInfo.clear();
}

unsigned int sspDSDeviceGroup::getAllDevices()
{
	if (m_dsInfo.empty())
		DirectSoundEnumerate(enumCallback, this);
	return m_dsInfo.size();
}

unsigned int sspDSDeviceGroup::getDeviceCount()
{
	return m_dsInfo.size();
}

sspString sspDSDeviceGroup::getDeviceName(int nDevice)
{
	return (nDevice < (int) m_dsInfo.size()) ? m_dsInfo[nDevice]->strDesc : _T("");
}

// initialize
bool sspDSDeviceGroup::initializeImpl(LPVOID hWnd)
{
	if (m_pDS.size() > 0) return TRUE;	// Already initialized

    if (hWnd == NULL)
    {
        // Error, invalid hwnd
        DOUT (_T("ERROR: Invalid parameters, unable to initialize services\n\r"));
        return FALSE;
    }
	m_hApp = (HWND) hWnd;
    setBufferFormat(2);
	m_pDS.reserve(m_nDevices.size());
	m_pDSBuf.reserve(m_nDevices.size());
	for (unsigned int i=0; i<m_nDevices.size(); i++) {
		// Create DirectSound object
		LPDIRECTSOUND ds;
		HRESULT nResult = DirectSoundCreate(m_dsInfo[m_nDevices[i]]->lpGuid, &ds, NULL);
		if (nResult == DS_OK) {
			nResult = ds->SetCooperativeLevel(m_hApp, DSSCL_PRIORITY);
			if (nResult == DS_OK) {
				LPDIRECTSOUNDBUFFER dsbuf;
				nResult = ds->CreateSoundBuffer(&m_dsBufDesc, &dsbuf, NULL);
				if (nResult == DS_OK) {
					nResult = dsbuf->SetFormat(&m_pcmWf);
					if (nResult == DS_OK) {
						DOUT (_T("SUCCESS: DirectSound created and formatted\n\r"));
					}
					else {
						DOUT(_T("ERROR: Unable to set DirectSound format\n\r"));
						return FALSE;
					}
				m_pDSBuf.push_back(dsbuf);
				}
				else {
					DOUT(_T("ERROR: Unable to create DirectSound buffer\n\r"));
					return FALSE;
				}
			}
			else {
				DOUT(_T("ERROR: Unable to set DirectSound cooperative level\n\r"));
				return FALSE;
			}
			m_pDS.push_back(ds);
		}
		else {
			// Error
			DOUT(_T("ERROR: Unable to create DirectSound object\n\r"));
			return FALSE;
		}
	}
    return TRUE;
}

bool sspDSDeviceGroup::beginImpl()
{
	bool bRet = true;
	for (unsigned int i=0; i<m_pDSBuf.size(); i++)  {
		HRESULT nResult = m_pDSBuf[i]->Play (0, 0, DSBPLAY_LOOPING);
		if (nResult != DS_OK) {
			DOUT(_T("Error, play failed\n\r"));
			bRet = false;
		}
	}
	return bRet;
}

bool sspDSDeviceGroup::endImpl()
{
	for (unsigned int i=0; i<m_pDSBuf.size(); i++) {
		if (m_pDSBuf[i] != NULL)
			m_pDSBuf[i]->Stop();
	}
	return true;
}

bool sspDSDeviceGroup::terminateImpl()
{
	end();
	for (unsigned int i=0; i<m_pDS.size(); ++i) {
		if (m_pDS[i]) m_pDS[i]->Release();
	}
	m_pDS.clear();
	m_pDSBuf.clear();
	return true;
}

LPDIRECTSOUND sspDSDeviceGroup::getPDS(int nDevice)
{
	return (nDevice < (int) m_pDS.size()) ? m_pDS[nDevice] : NULL;
}

void sspDSDeviceGroup::setBufferFormat(int nChannels)
{
	memset(&m_pcmWf, 0, sizeof(PCMWAVEFORMAT));
	m_pcmWf.wFormatTag = WAVE_FORMAT_PCM;
	m_pcmWf.nChannels = nChannels;
	m_pcmWf.nSamplesPerSec = sspSetting::audio.freq;
	m_pcmWf.wBitsPerSample = 16;
	m_pcmWf.nBlockAlign = m_pcmWf.nChannels * (m_pcmWf.wBitsPerSample/8);
	m_pcmWf.nAvgBytesPerSec = m_pcmWf.nSamplesPerSec * m_pcmWf.nBlockAlign;
 	// Set up DSBUFFERDESC structure.
	memset(&m_dsBufDesc, 0, sizeof(DSBUFFERDESC)); // Zero it out.
	m_dsBufDesc.dwSize = sizeof(DSBUFFERDESC);
	m_dsBufDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
}

void sspDSDeviceGroup::addInfo(LPGUID lpGuid, LPCWSTR lpDesc)
{
	dsInfoStruct* pInfo = new dsInfoStruct;
	if (lpGuid != NULL) {
		pInfo->lpGuid = new GUID;
		memcpy(pInfo->lpGuid, lpGuid, sizeof(GUID));
	}
	else
		pInfo->lpGuid = NULL;
	pInfo->strDesc = lpDesc;
	m_dsInfo.push_back(pInfo);
}

void sspDSDeviceGroup::clearInfo()
{
	for (unsigned int i=0; i<m_dsInfo.size(); ++i) {
		delete m_dsInfo[i]->lpGuid;
		delete m_dsInfo[i];
	}
	m_dsInfo.clear();
}

BOOL CALLBACK sspDSDeviceGroup::enumCallback(LPGUID lpGuid, LPCWSTR lpDesc, LPCWSTR lpModule, LPVOID lpContext)
{	
	sspDSDeviceGroup* pDSDG = (sspDSDeviceGroup*) lpContext;
	pDSDG->addInfo(lpGuid, lpDesc);
	return TRUE;
}
