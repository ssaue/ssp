// sspDSchannelControl.cpp: implementation of the sspDSchannelControl class.
//
//////////////////////////////////////////////////////////////////////

//#include <afxtempl.h>

#include "stdafx.h"
#include "sspDSchannelControl.h"
#include <mmsystem.h>
#include <dsound.h>
#include "sspSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SSP_DSBVOLUME_DIFF (DSBVOLUME_MAX - DSBVOLUME_MIN)

//////////////////////////////////////////////////////////////////////
// sspDSchannelControl
//////////////////////////////////////////////////////////////////////

sspDSchannelControl::sspDSchannelControl() : m_volume(1.0f), m_fader(1.0f), m_pan(0.0f)
{
	m_pBuf = NULL;
	m_bDestroyed = false;
}

sspDSchannelControl::~sspDSchannelControl()
{
}

void sspDSchannelControl::create(IDirectSoundBuffer* pBuf)
{
	m_pBuf = pBuf;
	sendVolume();
	sendPan();
}

void sspDSchannelControl::setMode(SSP_PANNING_MODE nMode)
{
	m_nMode = nMode;
	switch (nMode) {
		case SSP_MONOLEFT:
			setPanRamp(-1.0f);
			break;
		case SSP_MONORIGHT:
			setPanRamp(1.0f);
			break;
		case SSP_STEREO:
			setPanRamp(0.0f);
			break;
		default:
			break;
	}
}

bool sspDSchannelControl::setVolumeRamp(float fVolume, float fTime)
{
	m_volume.Initialize(fVolume, fTime);
	if (!m_volume.bOn) sendVolume();
	return m_volume.bOn;
}

bool sspDSchannelControl::setFadeRamp(bool bUp, float fTime)
{
	m_fader.Initialize(bUp ? 1.0f : 0.0f, fTime);
	if (!m_fader.bOn) sendVolume();
	return m_fader.bOn;
}

bool sspDSchannelControl::setPanRamp(float fPan, float fTime)
{
	m_pan.Initialize(fPan, fTime);
	if (!m_pan.bOn) sendPan();
	return m_pan.bOn;
}

bool sspDSchannelControl::begin()
{
	if (m_bDestroyed) {
		m_nNextTime = 0;
		return false;
	}
	if (m_fader.bOn || m_volume.bOn || m_pan.bOn) {
		m_nNextTime = sspSetting::res.envelope;
		return true;
	}
	else {
		m_nNextTime = 0;
		return false;
	}
}

bool sspDSchannelControl::update()
{
	if (m_bDestroyed) {
		m_nNextTime = 0;
		return false;
	}
	if (m_pan.bOn) {
		sendPan();
		m_pan.Update();
	}
	if (m_volume.bOn || m_fader.bOn) {
		sendVolume();
		if (m_volume.bOn) m_volume.Update();
		if (m_fader.bOn) m_fader.Update();
	}
	m_nNextTime = sspSetting::res.envelope;
	return (m_fader.bOn || m_volume.bOn || m_pan.bOn);
}

inline bool sspDSchannelControl::sendVolume()
{
	LONG nValue = (LONG) (m_fader.fValue * m_volume.fValue * SSP_DSBVOLUME_DIFF + DSBVOLUME_MIN);
	return (m_pBuf->SetVolume(nValue) == DS_OK);
}

inline bool sspDSchannelControl::sendPan()
{
	LONG nValue = (LONG) (m_pan.fValue * DSBPAN_RIGHT);	// Presupposes symmetric DSBPAN value
	return (m_pBuf->SetPan(nValue) == DS_OK);
}

void sspDSchannelControl::end()
{
	m_nNextTime = 0;
	m_volume.Terminate();
	m_fader.Terminate();
	m_pan.Terminate();
	if (m_bDestroyed) delete this;
}

void sspDSchannelControl::destroy()
{
	if (m_fader.bOn || m_volume.bOn || m_pan.bOn)
		m_bDestroyed = true;
	else
		delete this;
}