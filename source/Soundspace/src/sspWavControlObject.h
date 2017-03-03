// sspWavControlObject.h: interface for the sspWavControlObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVCONTROLOBJECT_H__20DD38C2_00B0_11D4_901C_D87E90D4EBA6__INCLUDED_)
#define AFX_WAVCONTROLOBJECT_H__20DD38C2_00B0_11D4_901C_D87E90D4EBA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspScheduleObjects.h"
#include "SSpUtility.h"
#include <vector>

/*
** sspWavControlObject 
** The volume is represented as a float value between 0.0 (min) and 1.0 (max)
*/

class sspWavControlObject : public sspScheduleObject  
{
private:
	enum PANNING {LEFT, RIGHT, BOTH};
	enum {MAX_WAV_VOLUME = 0xFFFFU};

	sspIntVec m_nVolume;
	sspIntVec m_nPan;

	SSpRamp	m_fader;
	SSpRamp	m_volume;

	void sendVolume();

public:
	sspWavControlObject();
	virtual ~sspWavControlObject();

	void create(const sspIntVec& nFaders);

	// The ramp functions return true if scheduling is needed
	bool setVolumeRamp(float fVolume, float fTime);
	bool setFadeRamp(bool bUp, float fTime);

	float getVolume() const {return m_volume.fEndValue;}

	bool begin();
	bool update();
	void end();
};

#endif // !defined(AFX_WAVCONTROLOBJECT_H__20DD38C2_00B0_11D4_901C_D87E90D4EBA6__INCLUDED_)
