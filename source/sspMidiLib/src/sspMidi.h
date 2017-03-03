// sspMidi.h: basic MIDI definitions and utilities 
// Using the portMIDI library
//
/////////////////////////////////////////////////////

#ifndef SSPMIDI_H
#define SSPMIDI_H

#include <vector>
#include <portmidi.h>

typedef PmMessage                 sspMidiMessage;
typedef PmTimestamp               sspMidiTimestamp;
typedef PmEvent                   sspMidiEvent;
typedef std::vector<sspMidiEvent> sspMidiSequence;

#ifndef BYTE
  typedef unsigned char BYTE;
#endif

namespace sspMidi {
	enum {NOTEOFF=0x80, NOTEON=0x90, CONTROL=0xB0, PROGRAM=0xC0, PITCH=0xE0, SYSTEM=0xF0, SYSEX=0XF0, EO_SYSEX=0XF7};

  inline sspMidiMessage createMessage(int nStatus, int nData1, int nData2) {return Pm_Message(nStatus, nData1, nData2);}

	inline BYTE getStatus(const sspMidiMessage& msg) {return (BYTE) Pm_MessageStatus(msg);}
	inline BYTE getData1(const sspMidiMessage& msg) {return (BYTE) Pm_MessageData1(msg);}
	inline BYTE getData2(const sspMidiMessage& msg) {return (BYTE) Pm_MessageData2(msg);}
	inline BYTE getType(const sspMidiMessage& msg) {return getStatus(msg) & 0xF0;}
	inline BYTE getChannel(const sspMidiMessage& msg) {return getStatus(msg) & 0x0F;}

	inline bool isNoteOff(const sspMidiMessage& msg) {return (getType(msg) == NOTEOFF) || ((getType(msg) == NOTEON) && (getData2(msg) == 0));} 
	inline bool isNoteOn(const sspMidiMessage& msg) {return (getType(msg) == NOTEON) && (getData2(msg) > 0);}
	inline bool isProgramChange(const sspMidiMessage& msg) {return (getType(msg) == PROGRAM);}
	inline bool isControl(const sspMidiMessage& msg) {return (getType(msg) == CONTROL);}
	inline bool isSystemMessage(const sspMidiMessage& msg) {return (getType(msg) == SYSTEM);}

	inline BYTE getProgram(const sspMidiMessage& msg) {return (isProgramChange(msg) ? getData1(msg) : 0);}
	inline BYTE getVelocity(const sspMidiMessage& msg) {return (isNoteOn(msg) ? getData2(msg) : 0);}

  inline void setStatus(sspMidiMessage& msg, BYTE status) {msg = (msg & 0xFFFF00) | (status & 0xFF);}
  inline void setData1(sspMidiMessage& msg, BYTE data) {msg = (msg & 0xFF00FF) | ((data << 8) & 0xFF00);}
  inline void setData2(sspMidiMessage& msg, BYTE data) {msg = (msg & 0xFFFF) | ((data << 16) & 0xFF0000);}
	inline void setChannel(sspMidiMessage& msg, int nChannel) {setStatus(msg, getStatus(msg) | (nChannel & 0x0f));}
	inline void setProgram(sspMidiMessage& msg, BYTE program) {if (isProgramChange(msg)) setData1(msg, program);}
}

#endif
