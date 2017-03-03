// sspSoundServerAPI::Impl.cpp : 
//

#include "stdafx.h"
#include "sspSoundServerImpl.h"

#include <sstream>
#include <boost/regex.hpp>

using boost::asio::ip::tcp;

using namespace std;

enum {SUPPORTED_CHANNELS = 6};

sspSoundServerAPI::Impl::Impl()
: m_service()
, m_socket(m_service)
, m_bIsConnected(false)
, m_items(1, "Invalid") // Item 0 is not legal
, m_channels(SUPPORTED_CHANNELS+1) // The supported channels will start on index 1
, m_assign()
{
}

sspSoundServerAPI::Impl::~Impl()
{
  unloadAll();
  disconnect();
}

inline sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::sendMessage(std::string text)
{
  sspSoundServerAPI::errStatus err = sspSoundServerAPI::OK;
  try {
    boost::asio::write(m_socket, boost::asio::buffer(text));

#if defined(_DEBUG)
    cout << "Send message: " << text << endl;
#endif

    boost::asio::streambuf response;
    boost::asio::read_until(m_socket, response, boost::regex("/n"));

#if defined(_DEBUG)
    std::istream is(&response);
    std::string input;
    getline(is, input);
    cout << "Receive message: " << input << endl;
#endif

  }
  catch (std::exception& e) {
    cerr << "Send/receive error: " << e.what() << endl;
    err = sspSoundServerAPI::OPERATION_FAILED;
  }
  return err;
}

inline bool
sspSoundServerAPI::Impl::isPlaying(sspSoundID id)
{
  bool bReturn = false;
  if (m_assign.find(id) != m_assign.end()) {
    sspSoundServerChannel& curChannel = m_channels[m_assign[id]];
    if (curChannel.getID() == id && curChannel.isPlaying())
      bReturn = true;
    else
      m_assign.erase(id);
  }

  return bReturn;
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::connect(string host, int nPort)
{
  boost::system::error_code error = boost::asio::error::host_not_found;

  ostringstream ps;
  ps << nPort;

  tcp::resolver resolver(m_service);
  tcp::resolver::query query(tcp::v4(), host, ps.str());
  tcp::resolver::iterator iterator = resolver.resolve(query);
  tcp::endpoint newend = *iterator;

  try {
    if (m_bIsConnected) { // Already connected

      tcp::endpoint oldend = m_socket.remote_endpoint(error);

      if (!error && oldend == newend) {
        // Already connected to the right endpoint
        m_bIsConnected = true;
        return sspSoundServerAPI::OK;
      }
      // Not connected to the right endpoint
      disconnect();
    }

    m_socket.connect(newend, error);
    if (error)
      throw boost::system::system_error(error);
    else
      m_bIsConnected = true;
  }
  catch (std::exception& e) {
    cerr << "Connect error: " << e.what() << endl;
  }
  return m_bIsConnected ? sspSoundServerAPI::OK : sspSoundServerAPI::NOT_CONNECTED;  
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::disconnect()
{
  if (m_bIsConnected) {
    sendMessage("stop /n");
    m_bIsConnected = false;
    m_socket.close();
    return sspSoundServerAPI::OK;
  }
  else {
    return sspSoundServerAPI::NOT_CONNECTED;
  }
}

bool 
sspSoundServerAPI::Impl::isConnected()
{
  return m_bIsConnected;
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::loadSound(string soundFile, sspSoundID* id)
{
  if (!m_bIsConnected)
    return sspSoundServerAPI::NOT_CONNECTED;

  if (id == NULL || soundFile.empty())
    return sspSoundServerAPI::BAD_ARGUMENT;

  *id = static_cast<sspSoundID>(m_items.size());
  m_items.push_back(soundFile);
  return sspSoundServerAPI::OK;
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::unloadAll()
{
  sspSoundServerAPI::errStatus retval = sspSoundServerAPI::OK;

  // Check whether any sounds are playing
  for (unsigned int i=1; i<=SUPPORTED_CHANNELS; ++i) {  // Do not use index 0
    if (m_channels[i].isPlaying()) {
      retval = sspSoundServerAPI::OPERATION_FAILED;
      break;
    }
  }

  if (retval == sspSoundServerAPI::OK) {
    m_assign.clear();
    m_items.resize(1);  // Keep one item
  }

  return retval;
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::play(sspSoundID id, float fDuration, bool bLoop)
{
  if (!m_bIsConnected)
    return sspSoundServerAPI::NOT_CONNECTED;

  if (id <= 0 || id >= m_items.size())
    return sspSoundServerAPI::INVALID_ID;

  if (isPlaying(id))
    return sspSoundServerAPI::ALREADY_PLAYING;

  for (unsigned int i=1; i<=SUPPORTED_CHANNELS; ++i) {  // Do not use index 0
    if (!m_channels[i].isPlaying()) {
  
      // Found an unoccupied channel
      sspSoundID oldID = m_channels[i].getID();
      if (oldID > 0) m_assign.erase(oldID);

      if (fDuration <= 0.0f) fDuration = -1.0f; // -1 indicates "for ever"
      m_channels[i].play(id, fDuration, bLoop);
      m_assign[id] = i;

      ostringstream output;
      output << "csoundCommand i" << (100 + i) << " 0 " << fDuration;
      output << " \"" << m_items[id] << "\" ";
      output << (bLoop ? 1 : 0) << " /n";
      return sendMessage(output.str());
    }
  }

  return sspSoundServerAPI::NOT_AVAILABLE;
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::stop(sspSoundID id)
{
  if (!m_bIsConnected)
    return sspSoundServerAPI::NOT_CONNECTED;

  if (!isPlaying(id))
    return sspSoundServerAPI::INVALID_ID;

  int nChannel = m_assign[id];

  if (!m_channels[nChannel].stop())
    return sspSoundServerAPI::NOT_AVAILABLE;
  else
    m_assign.erase(id);

  ostringstream output;
  output << "csoundCommand i -" << (100 + nChannel) << " 0 1 /n";
  return sendMessage(output.str());
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::setPosition(sspSoundID id, float fX, float fY, float fZ)
{
  if (!m_bIsConnected)
    return sspSoundServerAPI::NOT_CONNECTED;

  if (!isPlaying(id))
    return sspSoundServerAPI::INVALID_ID;

  int nChannel = m_assign[id];

  ostringstream output;
  output << "csoundCommand i" << (200 + nChannel) << " 0 1 " << fX << " " << fY << " " << fZ << " /n";
  return sendMessage(output.str());
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::movePosition(sspSoundID id, float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2, float fTime)
{
  if (!m_bIsConnected)
    return sspSoundServerAPI::NOT_CONNECTED;

  if (!isPlaying(id))
    return sspSoundServerAPI::INVALID_ID;

  int nChannel = m_assign[id];

  ostringstream output;
  output << "csoundCommand i" << (250 + nChannel) << " 0 " << fTime << " ";
  output << fX1 << " " << fY1 << " " << fZ1 << " ";
  output << fX2 << " " << fY2 << " " << fZ2 << " /n";
  return sendMessage(output.str());
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::setVolume(sspSoundID id, float fVol)
{
  if (!m_bIsConnected)
    return sspSoundServerAPI::NOT_CONNECTED;

  if (!isPlaying(id))
    return sspSoundServerAPI::INVALID_ID;

  int nChannel = m_assign[id];

  ostringstream output;
  output << "csoundCommand i" << (150 + nChannel) << " 0 1 " << fVol << " " << fVol << " /n";
  return sendMessage(output.str());
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::Impl::changeVolume(sspSoundID id, float fVol1, float fVol2, float fTime)
{
  if (!m_bIsConnected)
    return sspSoundServerAPI::NOT_CONNECTED;

  if (!isPlaying(id))
    return sspSoundServerAPI::INVALID_ID;

  int nChannel = m_assign[id];

  ostringstream output;
  output << "csoundCommand i" << (150 + nChannel) << " 0 " << fTime << " ";
  output << fVol1 << " " << fVol2 << " /n";;
  return sendMessage(output.str());
}
