/**
 * @file   sspSoundServerChannel.h
 * @author Sigurd Saue <sigurd.saue@bluezone.no>
 * @date   Sun Feb 18 2007
 * 
 * @brief  sspSoundServerChannel structure (placeholder for channel information)
 * 
 */

#ifndef SSP_SOUNDSERVERCHANNEL_H
#define SSP_SOUNDSERVERCHANNEL_H 1

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <string>
#include <vector>

class sspSoundServerChannel 
{
public:
  sspSoundServerChannel() 
    : m_expires()
    , m_item(0)
  {}

  ~sspSoundServerChannel()
  {
  }

  void play(sspSoundID item, float fDur, bool bLoop)
  {
    m_item = item;
    if (fDur > 0.0f) {      
      m_expires = boost::posix_time::second_clock::local_time() 
        + boost::posix_time::time_duration(boost::posix_time::seconds(static_cast<long>(ceil(fDur))));
    }
    else {
      m_expires = boost::posix_time::pos_infin;
    }
  }

  bool stop()
  {
    bool bReturn = false;
    if (m_item > 0 && m_expires.is_pos_infinity()) {
      m_expires = boost::posix_time::not_a_date_time;
      m_item = 0;
      bReturn = true;
    }

    return bReturn;
  }

  bool isPlaying()
  {
    if (m_item > 0) {
      if (m_expires.is_not_a_date_time() || m_expires < boost::posix_time::second_clock::local_time()) {
        m_expires = boost::posix_time::not_a_date_time;
        m_item = 0;
      }
    }
    return (m_item > 0);
  }

  sspSoundID getID() const {return m_item;}

private:
  boost::posix_time::ptime  m_expires;
  sspSoundID                m_item;
};

#endif
