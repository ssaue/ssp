/**
 * @file   sspSoundServerAPI.h
 * @author Sigurd Saue <sigurd.saue@bluezone.no>
 * @date   Sun Jan 07 2007
 * 
 * @brief  sspSoundServerAPI interface
 * 
 */

#ifndef SSP_SOUNDSERVERAPI_H
#define SSP_SOUNDSERVERAPI_H 1


#ifdef SSP_SOUNDSERVERAPI_EXPORTS
#define SSP_SOUNDSERVER_API __declspec(dllexport)
#else
#define SSP_SOUNDSERVER_API __declspec(dllimport)
#endif

#include <string>

/**
 *  The type used for sound ID's
 */
typedef unsigned int sspSoundID;
class sspSoundServerImpl;

/**
 *  The sspSoundServerAPI provides functions for communicating with the sound server
 *  NOTE: This is temporarily an interface only - all methods will return the error status
 *  sspSoundServerAPI::NOT_SUPPORTED.
 */
class SSP_SOUNDSERVER_API sspSoundServerAPI {
public:
  enum errStatus {
    OK,
    BAD_ARGUMENT,
    NOT_CONNECTED,
    NOT_AVAILABLE,
    INVALID_ID,
    ALREADY_PLAYING,
    OPERATION_FAILED,
    NOT_SUPPORTED
  };

  /**
   *  Default constructor
   */
	sspSoundServerAPI();

  /**
   *  Destructor
   */
  ~sspSoundServerAPI();

  /**
   *  Connect to sound server
   *
   *  @param host   Host address
   *  @param nPort  Port
   *
   *  @return error status
   */
  errStatus connect(std::string host, int nPort);

  /**
   *  Disconnect from sound server
   *
   *  @return error status
   */
  errStatus disconnect();

  /**
   *  Are we connected to the sound server?
   *
   *  @return true if connected, false otherwise
   */
  bool isConnected();

  /**
   *  Load a sound onto the server. Possible errors are:
   *    BAD_ARGUMENT: The id argument is a null pointer
   *  NOTE: There is no check on the validity of the file name.
   *  If the file is not found, no sound results
   *
   *  @param soundFile  File name (no path)
   *  @param id         Placeholder to receive sound ID
   *
   *  @return error status
   */
  errStatus loadSound(std::string soundFile, 
                      sspSoundID* id);

  /**
   *  Unload all sounds from the server.  Possible errors are:
   *    ILLEGAL_OPERATION: Cannot unload sounds that are playing
   *
   *  @return error status
   */
  errStatus unloadAll();

  /**
   *  Play the sound. If (fDuration <= 0.0) the sound will play for ever or until
   *  explicitly stopped with the stop command. Possible errors are:
   *    INVALID_ID: The sound ID is wrong
   *    NOT_AVAILABLE: No available sound slots (maximum 6)
   *
   *  @param id         Sound ID
   *  @param fDuration  Sound duration
   *  @param bLoop      If true, loop the sound
   *
   *  @return error status
   */
  errStatus play(sspSoundID id, 
                 float fDuration, 
                 bool bLoop);

  /**
   *  Stop the sound. Only sounds with unlimited duration (fDuration <= 0.0) can be stopped.
   *  Possible errors are:
   *    INVALID_ID: The sound ID is wrong
   *    NOT_AVAILABLE: The specified sound cannot be stopped
   *
   *  @param id   Sound ID
   *
   *  @return error status
   */
  errStatus stop(sspSoundID id);

  /**
   *  Set sound position. Position values are normalized to the range [0.0, 1.0].
   *  Possible errors are:
   *    INVALID_ID: The sound ID is wrong
   *
   *  @param id   Sound ID
   *  @param fX   X position (0.0 = far left, 1.0 = far right)
   *  @param fY   Y position (0.0 = far back, 1.0 = up close)
   *  @param fZ   Z position (0.0 = down, 1.0 = up)
   *
   *  @return error status
   */
  errStatus setPosition(sspSoundID id, 
                        float fX, 
                        float fY, 
                        float fZ);

  /**
   *  Move sound from position 1 to position 2 in the given time interval.
   *  See explanation of the positions above. Possible errors are:
   *    INVALID_ID: The sound ID is wrong
   *
   *  @param id     Sound ID
   *  @param fX1    X position 1
   *  @param fY1    Y position 1
   *  @param fZ1    Z position 1
   *  @param fX2    X position 2
   *  @param fY2    Y position 2
   *  @param fZ2    Z position 2
   *  @param fTime  Time interval
   *
   *  @return error status
   */
  errStatus movePosition(sspSoundID id, 
                         float fX1, 
                         float fY1, 
                         float fZ1, 
                         float fX2, 
                         float fY2, 
                         float fZ2, 
                         float fTime);

  /**
   *  Set sound volume. The volume is normalized to the range [0.0, 1.0].
   *  Use volume with care (e.g. for fade in/out) since it might affect
   *  the perceived sound position. Possible errors are:
   *    INVALID_ID: The sound ID is wrong
   *
   *  @param id     Sound ID
   *  @param fVol   Sound volume (0.0 = silent, 1.0 = max)
   *
   *  @return error status
   */
  errStatus setVolume(sspSoundID id, 
                      float fVol);

  /**
   *  Change sound from volume 1 to volume 2 in the given time inteval. See explanation 
   *  of sound volume above. Possible errors are:
   *    INVALID_ID: The sound ID is wrong
   *
   *  @param id     Sound ID
   *  @param fVol1  Sound volume 1
   *  @param fVol2  Sound volume 2
   *  @param fTime  Time interval
   *
   *  @return error status
   */
  errStatus changeVolume(sspSoundID id, 
                         float fVol1,
                         float fVol2,
                         float fTime);

private:
  /// Prevent copy and assignment
	sspSoundServerAPI(const sspSoundServerAPI& other);
  sspSoundServerAPI& sspSoundServerAPI::operator=(const sspSoundServerAPI& other);

  /// Private implementation hidden from external users
  class Impl;
  Impl* m_pImpl;
};

#endif
