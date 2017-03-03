// sspSoundServerAPI.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "sspSoundServerAPI.h"
#include "sspSoundServerImpl.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

sspSoundServerAPI::sspSoundServerAPI()
: m_pImpl(new sspSoundServerAPI::Impl)
{
}

sspSoundServerAPI::~sspSoundServerAPI()
{
  disconnect();
  delete m_pImpl;
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::connect(std::string host, int nPort)
{
  return m_pImpl->connect(host, nPort);
}

  /**
   *  Disconnect from sound server
   *
   *  @return error status
   */
sspSoundServerAPI::errStatus 
sspSoundServerAPI::disconnect()
{
  return m_pImpl->disconnect();
}

bool 
sspSoundServerAPI::isConnected()
{
  return m_pImpl->isConnected();
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::loadSound(std::string soundFile, sspSoundID* id)
{
  return m_pImpl->loadSound(soundFile, id);
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::unloadAll()
{
  return m_pImpl->unloadAll();
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::play(sspSoundID id, float fDuration, bool bLoop)
{
  return m_pImpl->play(id, fDuration, bLoop);
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::stop(sspSoundID id)
{
  return m_pImpl->stop(id);
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::setPosition(sspSoundID id, float fX, float fY, float fZ)
{
  return m_pImpl->setPosition(id, fX, fY, fZ);
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::movePosition(sspSoundID id, float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2, float fTime)
{
  return m_pImpl->movePosition(id, fX1, fY1, fZ1, fX2, fY2, fZ2, fTime);
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::setVolume(sspSoundID id, float fVol)
{
  return m_pImpl->setVolume(id, fVol);
}

sspSoundServerAPI::errStatus 
sspSoundServerAPI::changeVolume(sspSoundID id, float fVol1, float fVol2, float fTime)
{
  return m_pImpl->changeVolume(id, fVol1, fVol2, fTime);
}
