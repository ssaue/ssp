// sspMidiFileObjects.cpp: implementation of the sspMidiFile classes.
//
///////////////////////////////////////////////////////////////////////

#include "sspMidiFileObjects.h"
#include <algorithm>
#include <functional>

using namespace std;

//////////////////////////////////////////////////////////////////////
// sspMidiFileTrack
//////////////////////////////////////////////////////////////////////

class absoluteTime
{
private:
  sspMidiTimestamp m_nAccTime;

public:
  absoluteTime() : m_nAccTime(0) {}

  void operator() (sspMidiEvent& event)
  {
    m_nAccTime += event.timestamp;
    event.timestamp = m_nAccTime;
  }
};

class relativeTime
{
private:
  sspMidiTimestamp m_nAccTime;

public:
  relativeTime() : m_nAccTime(0) {}

  void operator() (sspMidiEvent& event)
  {
    event.timestamp -= m_nAccTime;
    m_nAccTime += event.timestamp;
  }
};

void sspMidiFileTrack::setAbsoluteTime()
{
  if (m_bRelativeTime) {
    for_each(begin(), end(), absoluteTime());
    m_bRelativeTime = false;
  }
}

void sspMidiFileTrack::setRelativeTime()
{
  if (!m_bRelativeTime) {
    for_each(begin(), end(), relativeTime());
    m_bRelativeTime = true;
  }
}

int sspMidiFileTrack::getDuration() const
{
  int nDuration = 0;
  sspMidiSequence::const_iterator iter;
  for (iter = begin(); iter != end(); ++iter)
    nDuration += iter->timestamp;
  return nDuration;
}


//////////////////////////////////////////////////////////////////////
// sspMidiFileSong
//////////////////////////////////////////////////////////////////////

int sspMidiFileSong::getDuration() const
{
  int nDuration = 0;
  sspMidiFileSong::const_iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
    int nTrackDuration = iter->getDuration();
    if (nTrackDuration > nDuration)
      nDuration = nTrackDuration;
  }
  return nDuration;
}

// Merge predicate
bool sortStamp(sspMidiEvent& ev1, sspMidiEvent& ev2)
{
  return (ev1.timestamp < ev2.timestamp);
}

void sspMidiFileSong::mergeTracks()
{
  if (size() < 2) return;

  front().setAbsoluteTime();
  while (size() > 1) {
    back().setAbsoluteTime();
    sspMidiFileTrack mergeTrack;
    mergeTrack.resize(front().size() + back().size());
    merge(front().begin(), front().end(), back().begin(), back().end(), mergeTrack.begin(), sortStamp);
    pop_back();
    front().swap(mergeTrack);
  }
  front().setRelativeTime();
}

class stampSplit
{
private:
  int               m_nChannel;
  sspMidiTimestamp  m_nTimeIn;
  sspMidiTimestamp  m_nTimeOut;

public:
   // Constructor initializes the value to multiply by
  stampSplit(int nChannel) : m_nChannel(nChannel), m_nTimeIn(0), m_nTimeOut(0) {}

  // The function call for the element to be multiplied
  void operator() (sspMidiEvent& event)
  {
    if (sspMidi::getChannel(event.message) == m_nChannel) {
      m_nTimeOut += event.timestamp;
      event.timestamp += m_nTimeIn;
      m_nTimeIn = 0;
    }
    else {
      m_nTimeIn += event.timestamp;
      event.timestamp += m_nTimeOut;
      m_nTimeOut = 0;
    }
  }
};

class channelDetect : public binary_function<sspMidiEvent, int, bool>
{
public:
  // The function call for the element to be multiplied
  bool operator() (const sspMidiEvent& event, const int& nChannel) const
  {
    return (sspMidi::getChannel(event.message) == nChannel);
  }
};

void sspMidiFileSong::splitTracks()
{
  if (size() != 1) return;

  sspMidiFileTrack newTrack;
  sspMidiFileTrack::iterator findEnd;
  sspMidiFileTrack origTrack = front();
  pop_back();
  do {
    int nChannel = sspMidi::getChannel(origTrack.front().message);
    // Process the timestamps to prepare a track split
    for_each(origTrack.begin(), origTrack.end(), stampSplit(nChannel));
    newTrack.resize(origTrack.size());
    // Extract a track with one channel only
    findEnd = remove_copy_if(origTrack.begin(), origTrack.end(), newTrack.begin(), not1(bind2nd(channelDetect(), nChannel)));
    newTrack.erase(findEnd, newTrack.end());
    push_back(newTrack);
    // Remove all elements of the new track from the original track
    findEnd = remove_if(origTrack.begin(), origTrack.end(), bind2nd(channelDetect(), nChannel));
    origTrack.erase(findEnd, origTrack.end());
  } while (!origTrack.empty());
}
