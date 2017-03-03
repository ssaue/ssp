/**
 * @file   sspSoundServerImpl.h
 * @author Sigurd Saue <sigurd.saue@bluezone.no>
 * @date   Sun Feb 18 2007
 * 
 * @brief  sspSoundServerImpl implementation
 * 
 */

#ifndef SSP_SOUNDSERVERIMPL_H
#define SSP_SOUNDSERVERIMPL_H 1

#include "sspSoundServerAPI.h"
#include "sspSoundServerChannel.h"

#pragma warning( push )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4996 )
#include <boost/asio.hpp>
#pragma warning( pop )

#include <string>
#include <vector>
#include <map>

class sspSoundServerAPI::Impl {
public:

	Impl();
  ~Impl();

  sspSoundServerAPI::errStatus 
  connect(std::string host, int nPort);

  sspSoundServerAPI::errStatus 
  disconnect();

  bool isConnected();

  sspSoundServerAPI::errStatus 
  loadSound(std::string soundFile, sspSoundID* id);

  sspSoundServerAPI::errStatus 
  unloadAll();

  sspSoundServerAPI::errStatus 
  play(sspSoundID id, float fDuration, bool bLoop);

  sspSoundServerAPI::errStatus 
  stop(sspSoundID id);

  sspSoundServerAPI::errStatus 
  setPosition(sspSoundID id, float fX, float fY, float fZ);

  sspSoundServerAPI::errStatus 
  movePosition(sspSoundID id, float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2, float fTime);

  sspSoundServerAPI::errStatus 
  setVolume(sspSoundID id, float fVol);

  sspSoundServerAPI::errStatus 
  changeVolume(sspSoundID id, float fVol1, float fVol2, float fTime);

private:
  // Class members
  boost::asio::io_service       m_service;
  boost::asio::ip::tcp::socket  m_socket;

  bool  m_bIsConnected;

  typedef std::string sspSoundServerItem;
  std::vector< sspSoundServerItem > m_items;
  std::vector< sspSoundServerChannel > m_channels;
  std::map< sspSoundID, int > m_assign;

  /// Prevent copy and assignment
	Impl(const Impl& other);
  Impl& Impl::operator=(const Impl& other);

  // Helper functions
  bool isPlaying(sspSoundID id);
  sspSoundServerAPI::errStatus sendMessage(std::string text);
};

#endif
