// sspDSchannelControl.h: interface for the sspDSchannelControl classes.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPDSCHANNELCONTROL_H
#define SSPDSCHANNELCONTROL_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspScheduleObjects.h"
#include "SSpUtility.h"

/*
** sspDSchannelControl 
** The volume is represented as a float value between 0.0 (min) and 1.0 (max)
*/

struct IDirectSoundBuffer;

class sspDSchannelControl : public sspScheduleObject  
{
public:
	enum SSP_PANNING_MODE {SSP_MONOLEFT, SSP_MONORIGHT, SSP_STEREO};

private:
	enum {MAX_WAV_VOLUME = 0xFFFFU};

	IDirectSoundBuffer* m_pBuf;
	SSP_PANNING_MODE	m_nMode;

	SSpRamp	m_fader;
	SSpRamp	m_volume;
	SSpRamp m_pan;

	bool m_bDestroyed;

	bool sendVolume();
	bool sendPan();

public:
	sspDSchannelControl();
	virtual ~sspDSchannelControl();

	void create(IDirectSoundBuffer* pBuf);
	void setMode(SSP_PANNING_MODE nMode);

	// The ramp functions return true if scheduling is needed
	bool setVolumeRamp(float fVolume, float fTime = 0.0f);
	bool setFadeRamp(bool bUp, float fTime = 0.0f);
	bool setPanRamp(float fPan, float fTime = 0.0f);

	float getVolume() const {return m_volume.fEndValue;}
	float getPan() const {return m_pan.fEndValue;}
	SSP_PANNING_MODE getMode() const {return m_nMode;}

	bool begin();
	bool update();
	void end();

	void destroy();
	bool isDestroyed() const {return m_bDestroyed;}
};

#endif 
