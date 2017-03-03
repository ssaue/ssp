// sspMidiFileObjects.h: interface for the sspMidiFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPMIDIFILEOBJECTS_H
#define SSPMIDIFILEOBJECTS_H

#include "sspMidi.h"
#include <string>

class sspMidiFileTime
{
protected:
  bool m_bSMPTE;
  int m_nTimeBase;
  int m_nFrames;
  int m_nSubFrames;
  int m_nTempo;
  double m_dTimeFactor;

public:
 	enum {DEFAULT_TEMPO=500000, DEFAULT_TIMEBASE=384, DEFAULT_FRAMES=25, DEFAULT_SUBFRAMES=40};

  sspMidiFileTime() 
    : m_bSMPTE(true)
    , m_nTimeBase(DEFAULT_TIMEBASE)
    , m_nFrames(DEFAULT_FRAMES)
    , m_nSubFrames(DEFAULT_SUBFRAMES)
    , m_nTempo(DEFAULT_TEMPO)
  {
    computeTimeFactor();
  }
  ~sspMidiFileTime() {}

  void setTimeBase(int nTimeBase) 
    {m_bSMPTE = false; m_nTimeBase = nTimeBase; computeTimeFactor();}
  void setSMPTEBase(int nFrames, int nSubFrames) 
    {m_bSMPTE = true; m_nFrames = nFrames; m_nSubFrames = nSubFrames; computeTimeFactor();}
  void setTempo(int nTempo) {m_nTempo = nTempo; computeTimeFactor();}

  bool isSMPTE() const {return m_bSMPTE;}
  int getTimeBase() const {return m_nTimeBase;}
  int getFrames() const {return m_nFrames;}
  int getSubFrames() const {return m_nSubFrames;}
  int getTempo() const {return m_nTempo;}
  int getMetronome() const {return (int) (60000000L / m_nTempo);}
  double getTimeFactor() const {return m_dTimeFactor;}

private:
  void computeTimeFactor() 
    {m_dTimeFactor = m_bSMPTE ? (1000.0 / (m_nFrames * m_nSubFrames)) : (m_nTempo / (1000.0 * m_nTimeBase));}
};

class sspMidiFileTrack : public sspMidiSequence
{
protected:
	int   m_nChannel;
	BYTE  m_nProgram;
  std::string m_strTitle;
  bool  m_bRelativeTime;

public:
  sspMidiFileTrack() 
    : m_nChannel(0)
    , m_nProgram(0) 
    , m_bRelativeTime(true)
  {}
  ~sspMidiFileTrack() {}

  void setTitle(std::string strTitle) {m_strTitle = strTitle;}
  const std::string& getTitle() const {return m_strTitle;}

  // Set all time stamps as absolute time from the beginning
  void setAbsoluteTime(); 

  // Set all time stamps as relative time (default)
  void setRelativeTime();

  int getDuration() const;
};

class sspMidiFileSong : public std::vector<sspMidiFileTrack>
{
protected:
 	enum {DEFAULT_NUMERATOR=4, DEFAULT_DENOMINATOR=4};

  int m_nTimeSignNum;
  int m_nTimeSignDenom;
  sspMidiFileTime m_time;
  std::string m_strTitle;

public:
  sspMidiFileSong() 
    : m_nTimeSignNum(DEFAULT_NUMERATOR)
    , m_nTimeSignDenom(DEFAULT_DENOMINATOR)
  {}
  ~sspMidiFileSong() {}

  void mergeTracks();   // Merge all tracks into one (relevant for playing Format 1 files)
  void splitTracks();   // Split events into tracks based on different MIDI channels (relevant for editing Format 0 files)

  void setTimeSignature(int nNum, int nDenom) {m_nTimeSignNum = nNum; m_nTimeSignDenom = nDenom;}
  void setTitle(std::string strTitle) {m_strTitle = strTitle;}

  void getTimeSignature(int& nNum, int& nDenom) const {nNum = m_nTimeSignNum; nDenom = m_nTimeSignDenom;}
  const std::string& getTitle() const {return m_strTitle;}

  sspMidiFileTime& getFileTime() {return m_time;}
  const sspMidiFileTime& getFileTime() const {return m_time;}

  int getDuration() const;
};

#endif
