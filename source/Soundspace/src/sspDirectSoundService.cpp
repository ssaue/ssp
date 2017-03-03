// sspDirectSoundService.cpp

#define VC_EXTRALEAN

#include "stdafx.h"
#include "sspSetting.h"
#include "debug.h"
#include "sspPlayManager.h"
#include "sspDirectSoundService.h"
#include "sspDirectSoundDevice.h"
#include "sspDeviceManager.h"
#include "sspDSchannelControl.h"

#include <sspAudioFile.h>
#include <algorithm>

//
// sspDSbuffer class implementation
//
////////////////////////////////////////////////////////////

sspDSbuffer::sspDSbuffer(const sspIntVec& nMonoChannels, float fVolume, float fTime)
: m_nMonoChannels(nMonoChannels)
, m_fVolume(fVolume)
, m_fTime(fTime)
, m_pAudioFile(NULL)
, m_bStereo(false)
, m_bLoaded(false)
, m_cbBufOffset(0)
, m_nBytesRemaining(0)
, m_nDuration(0)
, m_nTimeStarted(0)
, m_nTimeElapsed(0)
, m_nMaxDuration(0)
, m_pOwner(NULL)
{
}

sspDSbuffer::~sspDSbuffer (void)
{
	terminate();
}

void sspDSbuffer::terminate()
{
	if (m_pDSBuf.size() > 0)
		end();
}

bool sspDSbuffer::createMono()
{
	sspDSDeviceGroup* pPrimary = (sspDSDeviceGroup*) sspDeviceManager::Instance()[SSP_DEVICE_DIRECTSOUND];
	if (!pPrimary || m_nMonoChannels.empty()) 
		return false;
	for (unsigned int i=0; i<m_nMonoChannels.size(); i++) {
		LPDIRECTSOUNDBUFFER pBuf;
		if (pPrimary->getPDS(m_nMonoChannels[i]/2)->CreateSoundBuffer(&m_dsBufDesc, &pBuf, NULL) == DS_OK) {
			sspDSchannelControl* pDSCO = new sspDSchannelControl;
			pDSCO->create(pBuf);

			// Initialize panning
			if (m_nMonoChannels[i] % 2)		// odd channel number
				pDSCO->setMode(sspDSchannelControl::SSP_MONORIGHT);
			else							// even channel number
				pDSCO->setMode(sspDSchannelControl::SSP_MONOLEFT);

			// Initialize volume
			if (m_fTime > 0.0f) {
				pDSCO->setVolumeRamp(0.0f);
				if (pDSCO->setVolumeRamp(m_fVolume, m_fTime))
					sspPlayManager::Instance().getScheduler().addObject(pDSCO);
			}
			else {
				pDSCO->setVolumeRamp(m_fVolume);
			}
			m_pDSControl.push_back(pDSCO);
			m_pDSBuf.push_back(pBuf);
		}
		else {
			// Error, unable to create DirectSound buffer
			DOUT (_T("ERROR: unable to create DirectSound buffer\n\r"));
			return false;
		}
	}
  m_primary.resize(m_pDSBuf.size(), NULL);
  m_secondary.resize(m_pDSBuf.size(), NULL);
  m_readSizes.resize(m_pDSBuf.size(), std::make_pair<DWORD, DWORD>(0,0));
	m_bStereo = false;
	return true;
}

bool sspDSbuffer::createStereo()
{
	sspDSDeviceGroup* pPrimary = (sspDSDeviceGroup*) sspDeviceManager::Instance()[SSP_DEVICE_DIRECTSOUND];
	if (!pPrimary || m_nMonoChannels.empty()) 
		return false;
	for (unsigned int i=0; i<m_nMonoChannels.size(); i++) {
		LPDIRECTSOUNDBUFFER pBuf;
		if (pPrimary->getPDS(m_nMonoChannels[i]/2)->CreateSoundBuffer(&m_dsBufDesc, &pBuf, NULL) == DS_OK) {
			sspDSchannelControl* pDSCO = new sspDSchannelControl;
			pDSCO->create(pBuf);

			// Initialize panning
			if (m_nMonoChannels[i] % 2)		// odd channel number
				pDSCO->setMode(sspDSchannelControl::SSP_MONORIGHT);
			else if (i == m_nMonoChannels.size()-1 || (m_nMonoChannels[i+1] > m_nMonoChannels[i]+1))
				pDSCO->setMode(sspDSchannelControl::SSP_MONOLEFT);
			else {
				pDSCO->setMode(sspDSchannelControl::SSP_STEREO);
				i++;		// The next is already taken care of
			}

			// Initialize volume
			if (m_fTime > 0.0f) {
				pDSCO->setVolumeRamp(0.0f);
				if (pDSCO->setVolumeRamp(m_fVolume, m_fTime))
					sspPlayManager::Instance().getScheduler().addObject(pDSCO);
			}
			else {
				pDSCO->setVolumeRamp(m_fVolume);
			}
			m_pDSControl.push_back(pDSCO);
			m_pDSBuf.push_back(pBuf);
		}
		else {
			// Error, unable to create DirectSound buffer
			DOUT(_T("ERROR: unable to create DirectSound buffer\n\r"));
			return false;
		}
	}
  m_primary.resize(m_pDSBuf.size(), NULL);
  m_secondary.resize(m_pDSBuf.size(), NULL);
  m_readSizes.resize(m_pDSBuf.size(), std::make_pair<DWORD, DWORD>(0,0));
	m_bStereo = true;
	return true;
}

bool sspDSbuffer::loadAudioFile(sspAudioFile* pFile, sspMsgHandler* pOwner)
{
	ASSERT (pFile);
	if (!m_pAudioFile && pFile && pFile->open() == SSP_AUDIO_ERR_NO_ERROR) {
		m_pAudioFile = pFile;
		m_pOwner = pOwner;

    sspAudioInfo bufInfo;
    const sspAudioInfo& fileInfo = m_pAudioFile->getFileInfo();
    bufInfo.channels = (fileInfo.channels % 2) ? 1 : 2;
    bufInfo.samplerate = sspSetting::audio.freq;
    bufInfo.frames = (sspSetting::audio.bufSize * bufInfo.samplerate) / 1000;
    int nErrorNo = m_pAudioFile->initialize(bufInfo);
    if (nErrorNo == SSP_AUDIO_ERR_NO_ERROR) {

  		// Configure buffers
      memset(&m_dsBufDesc, 0, sizeof(DSBUFFERDESC)); // Zero it out.
		  m_dsBufDesc.dwSize = sizeof(DSBUFFERDESC);
		  m_dsBufDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_GLOBALFOCUS;
      m_dsBufDesc.dwBufferBytes = static_cast<DWORD>(bufInfo.frames * bufInfo.channels * sizeof(short));
      
      m_dsBufDesc.lpwfxFormat = new WAVEFORMATEX;
      m_dsBufDesc.lpwfxFormat->wFormatTag = WAVE_FORMAT_PCM;
      m_dsBufDesc.lpwfxFormat->nChannels = bufInfo.channels;
		  m_dsBufDesc.lpwfxFormat->cbSize = 0;
      m_dsBufDesc.lpwfxFormat->wBitsPerSample = 16;
		  m_dsBufDesc.lpwfxFormat->nChannels = bufInfo.channels;
		  m_dsBufDesc.lpwfxFormat->nSamplesPerSec = bufInfo.samplerate;
      m_dsBufDesc.lpwfxFormat->nBlockAlign = bufInfo.channels * sizeof(short);
      m_dsBufDesc.lpwfxFormat->nAvgBytesPerSec = m_dsBufDesc.lpwfxFormat->nBlockAlign * m_dsBufDesc.lpwfxFormat->nSamplesPerSec ;

		  m_bLoaded  = true;

		  if (bufInfo.channels == 1)
			  return createMono();
		  else if (bufInfo.channels == 2)
			  return createStereo();
    }
    else {
			DOUT1(_T("sspDSbuffer::loadAudioFile: %s\n"), sspAudioFile::getErrorMessage(nErrorNo).c_str());
      m_pAudioFile->close();
    }
	}
  else {
    if (m_pAudioFile) {
			DOUT(_T("sspDSbuffer::loadAudioFile failed: Audio file exists\n"));
    }
    else if (!pFile) {
			DOUT(_T("sspDSbuffer::loadAudioFile failed: empty file parameter\n"));
    }
    else {
			DOUT(_T("sspDSbuffer::loadAudioFile failed: no given reason\n"));
    }
  }
	m_bLoaded  = false;
	m_pAudioFile = NULL;
  return false;
}

bool sspDSbuffer::begin()
{
  m_nNextTime = sspSetting::audio.update;

  if (m_pAudioFile == NULL) {
		DOUT(_T("sspDSbuffer::begin failed (audiofile is NULL)\n"));
		m_cbBufOffset = 0;
		// Reset DirectSound buffer
		for (unsigned int i=0; i<m_pDSBuf.size(); i++)
			m_pDSBuf[i]->SetCurrentPosition (0);
		writeSilence(m_dsBufDesc.dwBufferBytes);
		return false;
	}
	else {
		// Reset buffer ptr
		m_cbBufOffset = 0;
    m_nDuration = static_cast<unsigned int>(m_pAudioFile->getDuration() * 1000);

		// Reset file ptr, etc
		m_pAudioFile->cue();
    m_nBytesRemaining = m_pAudioFile->getFileInfo().frames * m_dsBufDesc.lpwfxFormat->nChannels * sizeof(short);
		DOUT1(_T("sspDSbuffer::begin - remaining %d bytes\n"), m_nBytesRemaining);

		// Reset DirectSound buffer
		for (unsigned int i=0; i<m_pDSBuf.size(); i++)
			m_pDSBuf[i]->SetCurrentPosition (0);

		// Fill buffer with wave data
    if (m_dsBufDesc.dwBufferBytes > m_nBytesRemaining) {
  		unsigned int nWritten = writeWaveData(static_cast<unsigned int>(m_nBytesRemaining));        
	    writeSilence(m_dsBufDesc.dwBufferBytes - nWritten);
      m_nBytesRemaining = 0;
    }
    else {
      m_nBytesRemaining -= writeWaveData(m_dsBufDesc.dwBufferBytes);
    }

		for (unsigned int i=0; i<m_pDSBuf.size(); i++)  {
			HRESULT nResult = m_pDSBuf[i]->Play (0, 0, DSBPLAY_LOOPING);
			if (nResult != DS_OK) {
				end();
				DOUT(_T("Error, play failed\n\r"));
				return false;
			}
		}
		m_nTimeStarted = timeGetTime();
		return true;
	}
}

// update
//
// Routine to service buffer requests initiated by periodic timer.
//
// Returns true if buffer serviced normally; otherwise returns false.

bool sspDSbuffer::update()
{
	// Maintain elapsed time count
	m_nTimeElapsed = timeGetTime () - m_nTimeStarted;
  if (m_nMaxDuration) {
    m_nDuration = std::min(m_nTimeElapsed + m_nMaxDuration, m_nDuration);
    m_nMaxDuration = 0;
  }

	// Stop if all of sound has played
	if (m_nTimeElapsed >= m_nDuration) 	{
		return false;
	}
  else {
    m_nNextTime = std::min(sspSetting::audio.update, m_nDuration - m_nTimeElapsed);
  }

  // Determine free space in sound buffer (smallest among buffers)
  unsigned int nWritten = 0;
	unsigned int nFreeBytes = getBytesToFill(0);

	for (unsigned int i=1; i<m_pDSBuf.size(); i++) {
		unsigned int nBytes = getBytesToFill(i);
		if (nBytes < nFreeBytes) nFreeBytes = nBytes;
	}

  if (nFreeBytes) {

		// See how much wave data remains to be sent to buffer
		if (m_nBytesRemaining == 0) {

	    // All wave data has been sent to buffer - fill free space with silence
      if (writeSilence(nFreeBytes) < nFreeBytes) {
	      // Error writing silence data
			  ASSERT (0);
				DOUT(_T("writeSilence failed\n\r"));
			}
		}
		else if (m_nBytesRemaining >= nFreeBytes) {
	    // Enough wave data remains to fill free space in buffer
		  // Fill free space in buffer with wave data
      if ((nWritten = writeWaveData(nFreeBytes)) < nFreeBytes) {
	      // Error writing silence data
			  ASSERT (0);
				DOUT(_T("writeWaveData failed\n\r"));
			}
		}
		else {
	    // Some wave data remains, but not enough to fill free space
			// Send wave data to buffer, fill remainder of free space with silence
      if ((nWritten = writeWaveData(static_cast<unsigned int>(m_nBytesRemaining))) ==  m_nBytesRemaining) {

        unsigned int nRest = nFreeBytes - nWritten;
        if (writeSilence(nRest) != nRest) {
          // Error writing silence data
					ASSERT (0);
					DOUT(_T("writeSilence failed\n\r"));
				}
			}
			else {
        // Error writing wave data
				ASSERT (0);
				DOUT(_T("writeWaveData failed\n\r"));
			}
		}
	}
  m_nBytesRemaining -= nWritten;
	return true;
}

void sspDSbuffer::end()
{
	m_bLoaded  = false;
  m_nBytesRemaining = 0;

	DOUT(_T("sspDSbuffer::end\n"));
	for (size_t i=0; i<m_pDSBuf.size(); ++i) {
		m_pDSBuf[i]->Stop();
		m_pDSBuf[i]->Release();
	}
	m_pDSBuf.clear();

	for (size_t i=0; i<m_pDSControl.size(); ++i) {
		m_pDSControl[i]->destroy();
	}
	m_pDSControl.clear();

  delete m_pAudioFile;
	m_pAudioFile = NULL;
  delete m_dsBufDesc.lpwfxFormat;

	if (m_pOwner != NULL) {
		SSpMessage msg;
		msg.SetMsgType(SSP_IS_DONE);
		m_pOwner->handleMessage(msg);
	}
}

unsigned int sspDSbuffer::writeWaveData(unsigned int nBytes)
{
  HRESULT hr;
  unsigned int nReturn = 0;

  unsigned int nSamplesToRead = 0;
  unsigned int nSamplesRead1 = 0;
  unsigned int nSamplesRead2 = 0;

//  DOUT1 ("sspDSbuffer::WriteWaveData to read %d bytes\n", nBytes);   

  // Lock the sound buffers
  for (size_t i=0; i<m_pDSBuf.size(); ++i) {
	  hr = m_pDSBuf[i]->Lock(m_cbBufOffset, nBytes, (LPVOID*)&m_primary[i], &m_readSizes[i].first, (LPVOID*)&m_secondary[i], &m_readSizes[i].second, 0);
	  if (hr != DS_OK) {
		  // Error locking sound buffer
			DOUT1(_T("Error, unable to lock sound buffer %d\n\r"), i);
		  return 0;
	  }
  }

  // Write data to sound buffer. Because the sound buffer is circular, we may have to
  // do two write operations if locked portion of buffer wraps around to start of buffer.
  nSamplesToRead = m_readSizes[0].first / sizeof(short);
  if ((nSamplesRead1 = m_pAudioFile->read(m_primary, nSamplesToRead)) == nSamplesToRead) {

    nReturn += nSamplesRead1 * sizeof(short);
    if (m_secondary[0]) {    // Second write required?
      nSamplesToRead = m_readSizes[0].second / sizeof(short);
      if ((nSamplesRead2 = m_pAudioFile->read(m_secondary, nSamplesToRead)) == nSamplesToRead) {
        nReturn += nSamplesRead2 * sizeof(short);
      }
      else {

        // Error, didn't read wave data completely
				DOUT1(_T("Error, read failure - only %d\n\r"), nSamplesRead2);
        nReturn = 0;
      }
    }
  }
  else {
    // Error, didn't read wave data completely
		DOUT1(_T("Error, read failure - only %d\n\r"), nSamplesRead1);
    nReturn = 0;
  }

  // Unlock the sound buffers
  for (size_t i=0; i<m_pDSBuf.size(); ++i) {
	  m_pDSBuf[i]->Unlock((LPVOID)m_primary[i], m_readSizes[i].first, (LPVOID)m_secondary[i], m_readSizes[i].second);
  }

  // Update our buffer offset
	m_cbBufOffset = (m_cbBufOffset + nReturn) % m_dsBufDesc.dwBufferBytes;
 // DOUT1 ("sspDSbuffer::WriteWaveData have read %d bytes\n", nReturn);   
  return nReturn;
}


// WriteSilence
//
// Writes silence to sound buffer. This is a helper method used by
// ServiceBuffer; it's not exposed to users of the sspDSbuffer class.
inline unsigned int sspDSbuffer::writeSilence(unsigned int nBytes)
{
  HRESULT hr;
  unsigned int nReturn = 0;

  //DOUT1 ("sspDSbuffer::WriteSilence: %d bytes\n", nBytes);

  for (unsigned int i=0; i<m_pDSBuf.size(); i++) {

    // Lock the sound buffer
		hr = m_pDSBuf[i]->Lock(m_cbBufOffset, nBytes, (LPVOID*)&m_primary[i], &m_readSizes[i].first, (LPVOID*)&m_secondary[i], &m_readSizes[i].second, 0);
		if (hr == DS_OK) {
			// Write data to sound buffer. Because the sound buffer is circular, we may have to
			// do two write operations if locked portion of buffer wraps around to start of buffer.
			ASSERT (m_primary[i]);
			memset (m_primary[i], 0, m_readSizes[i].first);
        
			// Second write required?
			if (m_secondary[i]) {
				memset (m_secondary[i], 0, m_readSizes[i].second);
			}
			// Unlock sound buffer
			m_pDSBuf[i]->Unlock((LPVOID)m_primary[i], m_readSizes[i].first, (LPVOID)m_secondary[i], m_readSizes[i].second);
      nReturn = m_readSizes[i].first + m_readSizes[i].second;
		}
		else {
			// Error locking sound buffer
			DOUT1(_T("Error, unable to lock sound buffer %d\n\r"), i);
			return 0;
		}
	}       

  // Update our buffer offset
  m_cbBufOffset = (m_cbBufOffset + nReturn) % m_dsBufDesc.dwBufferBytes;
  return nReturn;
}


// GetMaxWriteSize
//
// Helper function to calculate max size of sound buffer write operation, i.e. how much
// free space there is in buffer.
unsigned int sspDSbuffer::getBytesToFill(int nBuffer)
{
  DWORD dwWriteCursor, dwPlayCursor, dwMaxSize;

  // Get current play position
  if (m_pDSBuf[nBuffer]->GetCurrentPosition (&dwPlayCursor, &dwWriteCursor) == DS_OK) {
    if (m_cbBufOffset <= dwPlayCursor) {
      // Our write position trails play cursor
      dwMaxSize = dwPlayCursor - m_cbBufOffset;
    }
    else { // (m_cbBufOffset > dwPlayCursor)
      // Play cursor has wrapped
      dwMaxSize = m_dsBufDesc.dwBufferBytes - m_cbBufOffset + dwPlayCursor;
    }
  }
  else {
    // GetCurrentPosition call failed
    ASSERT (0);
    dwMaxSize = 0;
  }

  return dwMaxSize;  // Return the number of bytes
}
