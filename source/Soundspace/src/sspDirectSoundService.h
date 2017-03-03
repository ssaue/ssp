// sspDirectSoundService.h

#ifndef SSP_DSSERVICE_H
#define SSP_DSSERVICE_H

#include "sspDefines.h"
#include "sspScheduler.h"

#include <mmsystem.h>
#include <dsound.h>
#include <vector>

// sspDSbuffer
//
// sspDSbuffer interface class for playing WAV files using DirectSound.
// Users of this class must create DSPrimaryBuffer object before
// creating an sspDSbuffer object.

class sspAudioFile;
class sspMsgHandler;
class sspDSchannelControl;
typedef std::vector<sspDSchannelControl*> sspDSControlList;

class sspDSbuffer : public sspScheduleObject
{
public:
	enum {BOTH, LEFT, RIGHT};

  sspDSbuffer(const sspIntVec& nMonoChannels, float fVolume, float fTime);
  ~sspDSbuffer();

	bool createMono();
	bool createStereo();
  bool loadAudioFile(sspAudioFile* pFile, sspMsgHandler* pOwner);

	// Virtual methods from sspScheduleObject
  bool begin();
  bool update();
	void end();

	void terminate();
	bool isLoaded() const {return m_bLoaded;}
	bool isStereo() const {return m_bStereo;}

	size_t getSizer() const {return m_pDSBuf.size();}
	sspDSControlList getDSControl() const {return m_pDSControl;}
  void setMaxDuration(UINT nDuration) {m_nMaxDuration = nDuration * sspTimeConvert::SEC2MSEC + 1;}

private:
  unsigned int writeWaveData (unsigned int nBytes);
  unsigned int writeSilence (unsigned int nBytes);
  unsigned int getBytesToFill(int nBuffer);

	sspIntVec			m_nMonoChannels;	// List of buffers (equal to channels for mono)
	float				  m_fVolume;			  // Start volume
	float				  m_fTime;			    // Fade in time
  sspAudioFile*	m_pAudioFile;			// ptr to sspAudioFile object
	bool				  m_bStereo;			  // Is this a stereo buffer
	bool				  m_bLoaded;			  // Is there a wav file ?
  unsigned int	m_cbBufOffset;		// last write position

  sspInt64      m_nBytesRemaining;    // The remaining number of bytes to play

  // Members to control file duration
  unsigned int				m_nDuration;		// duration of wave file
  unsigned int				m_nTimeStarted;	// time (in system time) playback started
  unsigned int				m_nTimeElapsed;	// elapsed time in msec since playback started
  unsigned int        m_nMaxDuration; // If > 0, limits the playing duration (used by fade outs)

  std::vector<short*> m_primary;
  std::vector<short*> m_secondary;
  std::vector<std::pair<DWORD, DWORD> > m_readSizes;

	std::vector<LPDIRECTSOUNDBUFFER>	m_pDSBuf;
	sspDSControlList						      m_pDSControl;
	sspMsgHandler*						        m_pOwner;

	DSBUFFERDESC m_dsBufDesc;
};

#endif // SSP_DSBUFFER 