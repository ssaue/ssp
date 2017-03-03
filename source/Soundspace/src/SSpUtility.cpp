// SSpUtility.cpp: implementation of the SSpUtility classes
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SSpUtility.h"
#include "sspSetting.h"

//////////////////////////////////////////////////////////////////////
// SSpRamp
//////////////////////////////////////////////////////////////////////

SSpRamp::SSpRamp()
{
	fValue = 1.0f;
	fEndValue = 1.0f;
	fStep = 0.0f;
	bUp = true;
	bOn = false;
}

SSpRamp::SSpRamp(float fDefault)
{
	fValue = fDefault;
	fEndValue = fDefault;
	fStep = 0.0f;
	bUp = true;
	bOn = false;
}

void SSpRamp::Initialize(float fVolume, float fTime)
{
	fEndValue = fVolume;
	bOn = (fTime > 0.0 && fEndValue != fValue);
	if (bOn)  {
		fStep = ((fEndValue - fValue) * sspSetting::res.envelope) / (fTime * 1000);
		bUp = (fStep > 0.0);
	}
	else {
		fStep = 0.0f;
		fValue = fEndValue;
	}
}

void SSpRamp::Update()
{
	fValue += fStep;
	if (bUp && fValue >= fEndValue) {
		fValue = fEndValue;
		bOn = false;
	}
	else if (!bUp && fValue <= fEndValue) {
		fValue = fEndValue;
		bOn = false;
	}
}

void SSpRamp::Terminate()
{
	fEndValue = fValue;
	fStep = 0.0f;
	bOn = false;
}

