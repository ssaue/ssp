// sspWavControlObject.cpp: implementation of the sspWavControlObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sspWavControlObject.h"
#include "mmsystem.h"
#include "sspSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// sspWavControlObject
//////////////////////////////////////////////////////////////////////

sspWavControlObject::sspWavControlObject() : m_volume(1.0f), m_fader(1.0f)
{
}

sspWavControlObject::~sspWavControlObject()
{
}

void sspWavControlObject::create(const sspIntVec& nFaders)
{
	for (unsigned int i=0; i<nFaders.size(); i++) {
		m_nVolume.push_back(nFaders[i]>>1);
		if (nFaders[i] % 2)
			m_nPan.push_back(RIGHT);
		else if (i == nFaders.size()-1 || (nFaders[i+1] > nFaders[i]+1))
			m_nPan.push_back(LEFT);
		else {
			m_nPan.push_back(BOTH);
			i++;		// The next is already taken care of
		}
	}
	sendVolume();
}

bool sspWavControlObject::setVolumeRamp(float fVolume, float fTime)
{
	m_volume.Initialize(fVolume, fTime);
	if (!m_volume.bOn) sendVolume();
	return m_volume.bOn;
}

bool sspWavControlObject::setFadeRamp(bool bUp, float fTime)
{
	m_fader.Initialize(bUp ? 1.0f : 0.0f, fTime);
	if (!m_fader.bOn) sendVolume();
	return m_fader.bOn;
}

bool sspWavControlObject::begin()
{
	if (m_fader.bOn || m_volume.bOn) {
		m_nNextTime = sspSetting::res.envelope;
		return true;
	}
	else {
		m_nNextTime = 0;
		return false;
	}
}

bool sspWavControlObject::update()
{
	sendVolume();
	if (m_volume.bOn) m_volume.Update();
	if (m_fader.bOn) m_fader.Update();
	m_nNextTime = sspSetting::res.envelope;
	return (m_fader.bOn || m_volume.bOn);
}

inline void sspWavControlObject::sendVolume()
{
	unsigned short nValue = (unsigned short) (m_fader.fValue * m_volume.fValue * MAX_WAV_VOLUME);
	union {unsigned long stereo; unsigned short mono[2];} nSetVolume;
	for (unsigned int i=0; i<m_nPan.size(); ++i) {
		switch(m_nPan[i]) {
		case LEFT:
			waveOutGetVolume((HWAVEOUT) m_nVolume[i], &nSetVolume.stereo);
			nSetVolume.mono[LEFT] = nValue;
			waveOutSetVolume((HWAVEOUT) m_nVolume[i], nSetVolume.stereo);
			break;
		case RIGHT:
			waveOutGetVolume((HWAVEOUT) m_nVolume[i], &nSetVolume.stereo);
			nSetVolume.mono[RIGHT] = nValue;
			waveOutSetVolume((HWAVEOUT) m_nVolume[i], nSetVolume.stereo);
			break;
		case BOTH:
			nSetVolume.mono[LEFT] = nValue;
			nSetVolume.mono[RIGHT] = nValue;
			waveOutSetVolume((HWAVEOUT) m_nVolume[i], nSetVolume.stereo);
			break;
		default:
			break;
		}
	}
}

void sspWavControlObject::end()
{
	m_nNextTime = 0;
	m_volume.Terminate();
	m_fader.Terminate();
}

