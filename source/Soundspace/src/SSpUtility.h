// SSpUtility.h: various utility functions and classes
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPUTILITY_H
#define SSPUTILITY_H

struct SSpRamp {
	float	fValue;
	float	fStep;
	float	fEndValue;
	bool	bUp;
	bool	bOn;

	SSpRamp();
	SSpRamp(float fDefault);

	void Initialize(float fVolume, float fTime);
	void Update();
	void Terminate();
};

#endif