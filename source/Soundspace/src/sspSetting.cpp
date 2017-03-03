// sspSetting.cpp: implementation of the sspSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sspSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////
// Static declarations
/////////////////////////////////

const wchar_t sspSetting::strProfileAudio[] = _T("Audio");
const wchar_t sspSetting::strAudioFrequency[] = _T("frequency");
const wchar_t sspSetting::strAudioBuffer[] = _T("buffer");
const wchar_t sspSetting::strAudioUpdate[] = _T("update");

const wchar_t sspSetting::strProfileResolution[] = _T("Resolution");
const wchar_t sspSetting::strResolutionTimer[] = _T("timer");
const wchar_t sspSetting::strResolutionEnvelope[] = _T("envelope");
const wchar_t sspSetting::strResolutionEvent[] = _T("event");

const wchar_t sspSetting::strProfileUpdates[] = _T("Updates");
const wchar_t sspSetting::strTimingEvent[] = _T("event");
const wchar_t sspSetting::strTimingClock[] = _T("clock");
const wchar_t sspSetting::strTimingGUI[] = _T("GUI");

const wchar_t sspSetting::strProfileFades[] = _T("Fade times");
const wchar_t sspSetting::strTimingFadeIn[] = _T("fade in");
const wchar_t sspSetting::strTimingFadeOut[] = _T("fade out");
const wchar_t sspSetting::strTimingVolume[] = _T("volume change");

const wchar_t sspSetting::strProfileSlider[] = _T("Slider");
const wchar_t sspSetting::strSlider[] = _T("value");

sspSetting::Audio		    sspSetting::audio = {44100, 2000, 500};
sspSetting::Resolution  sspSetting::res = {1, 10, 100};
sspSetting::Updates		  sspSetting::update = {1.0f, 5.0f, 10.0f};
sspSetting::FadeTimes		sspSetting::fade = {2, 5, 3};

float	sspSetting::sliderValue[] = {0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

void sspSetting::Audio::ReadRegistry()
{
	freq = AfxGetApp()->GetProfileInt(strProfileAudio, strAudioFrequency, 44100);
	bufSize = AfxGetApp()->GetProfileInt(strProfileAudio, strAudioBuffer, 2000);
	update = AfxGetApp()->GetProfileInt(strProfileAudio, strAudioUpdate, 500);
}

void sspSetting::Resolution::ReadRegistry()
{
	timer = AfxGetApp()->GetProfileInt(strProfileResolution, strResolutionTimer, 1);
	envelope = AfxGetApp()->GetProfileInt(strProfileResolution, strResolutionEnvelope, 10);
	event = AfxGetApp()->GetProfileInt(strProfileResolution, strResolutionEvent, 100);
}

void sspSetting::Updates::ReadRegistry()
{
	CString dataStr;
	dataStr = AfxGetApp()->GetProfileString(strProfileUpdates, strTimingEvent);
	swscanf_s(dataStr, _T("%f"), &event);
	dataStr = AfxGetApp()->GetProfileString(strProfileUpdates, strTimingClock);
	swscanf_s(dataStr, _T("%f"), &clock);
	dataStr = AfxGetApp()->GetProfileString(strProfileUpdates, strTimingGUI);
	swscanf_s(dataStr, _T("%f"), &gui);
}

void sspSetting::FadeTimes::ReadRegistry()
{
	fadeIn = AfxGetApp()->GetProfileInt(strProfileFades, strTimingFadeIn, 2);
	fadeOut = AfxGetApp()->GetProfileInt(strProfileFades, strTimingFadeOut, 5);
	volume = AfxGetApp()->GetProfileInt(strProfileFades, strTimingVolume, 3);
}

void sspSetting::ReadSliderRegistry()
{
	CString dataStr;
	dataStr = AfxGetApp()->GetProfileString(strProfileSlider, strSlider);
	swscanf_s(dataStr, _T("%f %f %f %f %f %f %f %f %f"), &sliderValue[0], &sliderValue[1], &sliderValue[2], &sliderValue[3],
		&sliderValue[4], &sliderValue[5], &sliderValue[6], &sliderValue[7], &sliderValue[8]);
}

void sspSetting::ReadAllRegistry()
{
	audio.ReadRegistry();
	res.ReadRegistry();
	update.ReadRegistry();
	fade.ReadRegistry();
	ReadSliderRegistry();
}

void sspSetting::Audio::WriteRegistry()
{
	AfxGetApp()->WriteProfileInt(strProfileAudio, strAudioFrequency, freq);
	AfxGetApp()->WriteProfileInt(strProfileAudio, strAudioBuffer, bufSize);
	AfxGetApp()->WriteProfileInt(strProfileAudio, strAudioUpdate, update);
}

void sspSetting::Resolution::WriteRegistry()
{
	AfxGetApp()->WriteProfileInt(strProfileResolution, strResolutionTimer, timer);
	AfxGetApp()->WriteProfileInt(strProfileResolution, strResolutionEnvelope, envelope);
	AfxGetApp()->WriteProfileInt(strProfileResolution, strResolutionEvent, event);
}

void sspSetting::Updates::WriteRegistry()
{
	CString dataStr;
	dataStr.Format(_T("%.2f"), event);
	AfxGetApp()->WriteProfileString(strProfileUpdates, strTimingEvent, dataStr);
	dataStr.Format(_T("%.2f"), clock);
	AfxGetApp()->WriteProfileString(strProfileUpdates, strTimingClock, dataStr);
	dataStr.Format(_T("%.2f"), gui);
	AfxGetApp()->WriteProfileString(strProfileUpdates, strTimingGUI, dataStr);
}

void sspSetting::FadeTimes::WriteRegistry()
{
	AfxGetApp()->WriteProfileInt(strProfileFades, strTimingFadeIn, fadeIn);
	AfxGetApp()->WriteProfileInt(strProfileFades, strTimingFadeOut, fadeOut);
	AfxGetApp()->WriteProfileInt(strProfileFades, strTimingVolume, volume);
}

void sspSetting::WriteSliderRegistry()
{
	CString dataStr;
	dataStr.Format(_T("%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f"), sliderValue[0], sliderValue[1], sliderValue[2],
		sliderValue[3], sliderValue[4], sliderValue[5], sliderValue[6], sliderValue[7], sliderValue[8]);
	AfxGetApp()->WriteProfileString(strProfileSlider, strSlider, dataStr);
}

void sspSetting::WriteAllRegistry()
{
	audio.WriteRegistry();
	res.WriteRegistry();
	update.WriteRegistry();
	fade.WriteRegistry();
	WriteSliderRegistry();
}
