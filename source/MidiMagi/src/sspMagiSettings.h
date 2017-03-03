// sspMagiSettings.h: interface for the sspMagiSettings class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSP_MAGISETTINGS
#define SSP_MAGISETTINGS

class sspMagiSettings  
{
public:
	enum {NOTEONS = 10};

	static const char strProfileMap[];
	static const char strSounding[];
	static const char strDMX[];

	static const char strProfileFilter[];
	static const char strTimeWindow[];
	static const char strTimeOffset[];
	static const char strPumpStart[];
	static const char strPumpStop[];
	static const char strStartVelocity[];
	static const char strMinVelocity[];
	static const char strMaxVelocity[];
	static const char strChannel[];
	static const char strNoteon[];

public:
	// Audio settings
	static struct Timing {
    float timeWindow;
		float timeOffset;
    float pumpStart;
		float pumpStop;

		void readRegistry();
		void writeRegistry();
	} timing;

	static struct Velocity {
    int start;
    int minimum;
    int maximum;

		void readRegistry();
		void writeRegistry();
	} velocity;

	static struct MidiTrack {
    int channel;
    int noteon;

		void readRegistry();
		void writeRegistry();
	} midiTrack;

	// Slider value settings
	static int sounding[NOTEONS];
  static int dmx[NOTEONS];

	static void readMapRegistry();
	static void readAllRegistry();
	static void writeMapRegistry();
	static void writeAllRegistry();
};

#endif
