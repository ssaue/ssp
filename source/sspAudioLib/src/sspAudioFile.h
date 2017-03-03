// sspAudioFile.h: interface for the sspAudioFile class.
//
////////////////////////////////////////////////////////

#ifndef SSP_AUDIOFILE_H
#define SSP_AUDIOFILE_H

#include "sspAudio.h"

#include <string>
#include <vector>

class sspAudioFile
{
public:
  /*
   * Constructor
   *
   * @param strFilename File name of audio file
   */
  sspAudioFile(std::wstring strFilename);

  /*
   * Destructor
   */
  ~sspAudioFile();

  /*
   * Open the audio file
   *
   * @param nMode The file opening mode (SFM_READ, SFM_WRITE or SFM_RDWR)
   *
   * @return Error message (SSP_AUDIO_ERR_NO_ERROR = 0 if successful)
   */
  int open(int nMode = SFM_READ);

  /*
   * Close the audio file
   *
   * @return Error message (SSP_AUDIO_ERR_NO_ERROR = 0 if successful)
   */
  int close();

  /*
   * Initialize file operations with play buffer information:
   *    Verify that file and buffer sample rates match
   *    Allocate work buffers if necessary (buf.size = bufInfo.frames * fileInfo.channels)
   *    Reset the file pointer to the beginning
   *
   * @param bufInfo A description of the play buffers
   *
   * @return Error message (SSP_AUDIO_ERR_NO_ERROR = 0 if successful)
   */
  int initialize(const sspAudioInfo& bufInfo);

  /*
   * Reset the file pointer to the beginning
   *
   * @return True if successful
   */
  bool cue() const;

  /*
   * Fill input buffers with audio data
   *
   * @param bufs The input buffers
   * @param nSamples The number of samples to fill
   *
   * @return The number of samples actually filled
   */
  unsigned int read(std::vector<short*>& bufs, unsigned int nSamples);

  /*
   * Get the audio file information
   *
   * @return File information structure
   */
  const sspAudioInfo& getFileInfo() const {return m_fileInfo;}

  /*
   * Is the file opened?
   *
   * @return True if file is open
   */
  bool isOpen() const {return m_sndFile != NULL;}

  /*
   * Get file duration
   *
   * @return The duration in seconds
   */
  double getDuration() const;

  /*
   * Get the character string explaining the error return from open and close
   *
   * @param nErrorNo The error number
   *
   * @return Error text string
   */
  static std::wstring getErrorMessage(int nErrorNo);

  /*
   * Verify that a filename has a valid extension (supported by the library)
   *
   * @param strFilename The file name
   *
   * @return True if the extension is valid and supported
   */
  static bool verifyValidExtension(const std::wstring& strFilename);

protected:
  std::wstring		m_strName;
	SNDFILE*        m_sndFile;
	sspAudioInfo 	  m_fileInfo;
	sspAudioInfo 	  m_playInfo;
  bool            m_bInitialized;

private:
  std::vector<short>  m_workBuffer;
  static std::vector<std::wstring> s_extensions;
};

#endif

  //UINT GetNumBytesRemaining (void) { return (m_nDataSize - m_nBytesPlayed); }
  //UINT GetAvgDataRate (void) { return (m_nAvgDataRate); }
  //UINT GetDataSize (void) { return (m_nDataSize); }
  //UINT GetNumBytesPlayed (void) { return (m_nBytesPlayed); }

