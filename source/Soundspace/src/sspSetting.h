// sspSetting.h: interface for the sspSetting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTING_H__3B806CA0_8B90_11D3_901C_C4D12E7A5D70__INCLUDED_)
#define AFX_SETTING_H__3B806CA0_8B90_11D3_901C_C4D12E7A5D70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class sspSetting  
{
public:
	enum {MAX_SLIDERS = 9, MAX_FILEPATH_SIZE = 255};

	static const wchar_t strProfileAudio[];
	static const wchar_t strAudioFrequency[];
	static const wchar_t strAudioBuffer[];
	static const wchar_t strAudioUpdate[];

	static const wchar_t strProfileResolution[];
	static const wchar_t strResolutionTimer[];
	static const wchar_t strResolutionEnvelope[];
	static const wchar_t strResolutionEvent[];

	static const wchar_t strProfileUpdates[];
	static const wchar_t strTimingEvent[];
	static const wchar_t strTimingClock[];
	static const wchar_t strTimingGUI[];

	static const wchar_t strProfileFades[];
	static const wchar_t strTimingFadeIn[];
	static const wchar_t strTimingFadeOut[];
	static const wchar_t strTimingVolume[];

	static const wchar_t strProfileSlider[];
	static const wchar_t strSlider[];

public:
	// Audio settings
	static struct Audio {
		unsigned int freq;
    unsigned int bufSize, update; 
		void ReadRegistry();
		void WriteRegistry();
	} audio;

	// Resolution in milliseconds
	static struct Resolution {
		unsigned int timer, envelope, event;
		void ReadRegistry();
		void WriteRegistry();
	} res;

	// Update intervals in seconds
	static struct Updates {
		float event;
		float clock;
    float gui;
		void ReadRegistry();
		void WriteRegistry();
	} update;

  // Timing in seconds
	static struct FadeTimes {
		unsigned int fadeIn;
		unsigned int fadeOut;
		unsigned int volume;
		void ReadRegistry();
		void WriteRegistry();
	} fade;

	// Slider value settings
	static float sliderValue[MAX_SLIDERS];

	static void ReadSliderRegistry();
	static void ReadAllRegistry();
	static void WriteSliderRegistry();
	static void WriteAllRegistry();
};

#endif // !defined(AFX_SETTING_H__3B806CA0_8B90_11D3_901C_C4D12E7A5D70__INCLUDED_)
