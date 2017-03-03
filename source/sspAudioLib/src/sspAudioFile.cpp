// sspAudioFile.cpp

#include "sspAudioFile.h"
#include <tchar.h>
#include <algorithm>
using namespace std;

#define MAX_BUFFER 260

sspAudioFile::sspAudioFile(wstring strFilename)
: m_strName(strFilename)
, m_sndFile(NULL)
{
  m_fileInfo.channels = 0;
  m_playInfo.channels = 0;
}

// Destructor
sspAudioFile::~sspAudioFile (void)
{
  close();
}

int sspAudioFile::open(int nMode)
{
	close();
  if (m_strName.empty()) 
    return false;

	// Convert Unicode string to multibyte string
	size_t converted;
	char str[MAX_BUFFER];
	wcstombs_s(&converted, str, MAX_BUFFER, m_strName.c_str(), _TRUNCATE);
	if (converted < m_strName.size())
		return false;

  m_sndFile = sf_open(str, nMode, &m_fileInfo);
  return m_sndFile == NULL ? sf_error(NULL) : 0;
}

int sspAudioFile::close()
{
  int nRet = 0;
  if (m_sndFile != NULL) {
    nRet = sf_close(m_sndFile);
    m_sndFile = NULL;
  }
  return nRet;    
}

inline bool sspAudioFile::cue() const
{
  return (m_sndFile != NULL && sf_seek(m_sndFile, 0, SEEK_SET) >= 0);
}

int sspAudioFile::initialize(const sspAudioInfo& bufInfo)
{
  if (bufInfo.channels == 0 || bufInfo.frames == 0)
    return SSP_AUDIO_ERR_INVALID_PLAYINFO;

  // Only mono and stereo buffers are supported (for now)
  if (bufInfo.channels > 2)
    return SSP_AUDIO_ERR_UNSUPPORTED_PLAYFORMAT;

  // The samplerates of file and buffer must correspond
  if (bufInfo.samplerate != m_fileInfo.samplerate)
    return SSP_AUDIO_ERR_INCOMPATIBLE_FORMATS;

  // The number of file channels must be dividable by the number of buffer channels
  if (m_fileInfo.channels % bufInfo.channels != 0)
    return SSP_AUDIO_ERR_INCOMPATIBLE_FORMATS;

  m_playInfo = bufInfo;

  if (m_playInfo.channels != m_fileInfo.channels)
    m_workBuffer.resize(static_cast<size_t>(m_playInfo.frames * m_fileInfo.channels));

  m_bInitialized = cue();
  return m_bInitialized ? SSP_AUDIO_ERR_NO_ERROR : SSP_AUDIO_ERR_UNKNOWN_FILE_ERROR;
}

unsigned int sspAudioFile::read(std::vector<short*>& bufs, unsigned int nSamples)
{
  unsigned int nRead = 0;

  if (m_bInitialized) {

    if (m_playInfo.channels == m_fileInfo.channels) {
      nRead = static_cast<unsigned int>(sf_read_short(m_sndFile, bufs[0], nSamples));
      for (size_t i=1; i < bufs.size(); ++i) {
        memcpy(bufs[i], bufs[0], nRead * sizeof(short));
      }
    }
    else {
      if (nSamples <= m_playInfo.frames * m_fileInfo.channels) {  // Verify that the work buffer is large enough
        const size_t nFac = m_fileInfo.channels / m_playInfo.channels;
        short* pSource = &m_workBuffer[0];
        nRead = static_cast<unsigned int>(sf_read_short(m_sndFile, pSource, nSamples * nFac) / nFac);
        int nOffset = 0;
        size_t nNumBufs = min(bufs.size(), nFac);
        for (unsigned int i=0; i < nRead / m_playInfo.channels; ++i) {
          for (size_t j=0; j < nNumBufs; ++j) {
            memcpy(bufs[j]+nOffset, pSource + j * m_playInfo.channels, m_playInfo.channels * sizeof(short));
          }
          nOffset += m_playInfo.channels;
          pSource += m_fileInfo.channels;
        }

        // Fill the remaining play buffers (if any)
        size_t nSourceBuf = 0;
        while (nNumBufs < bufs.size()) {
          memcpy(bufs[nNumBufs++], bufs[nSourceBuf++], nRead * sizeof(short));
        }
      }
    }
  }

  return nRead;
}

double sspAudioFile::getDuration() const
{
  double dDuration = 0.0;
  if (m_fileInfo.samplerate > 0) {
    dDuration = static_cast<double>(m_fileInfo.frames) / m_fileInfo.samplerate;
  }
  return dDuration;
}

std::wstring sspAudioFile::getErrorMessage(int nErrorNo)
{
	if (nErrorNo <= SSP_AUDIO_ERR_MAX_SNDFILE_ERROR) { // libsndfile error

		// Convert Unicode string to multibyte string
		const char* error = sf_error_number(nErrorNo);
		size_t converted;
		wchar_t str[MAX_BUFFER];
		mbstowcs_s(&converted, str, MAX_BUFFER, error, _TRUNCATE);
		return str;
	}

  switch (nErrorNo) {
    case SSP_AUDIO_ERR_INVALID_PLAYINFO:
      return _T("Invalid play information");
      break;
    case SSP_AUDIO_ERR_INCOMPATIBLE_FORMATS:
      return _T("File and play information are incompatible");
      break;
    case SSP_AUDIO_ERR_UNKNOWN_FILE_ERROR:
      return _T("Unspecified error in file access");
      break;
    default:
      return _T("Invalid error number");
      break;
  }
}

std::vector<std::wstring> sspAudioFile::s_extensions;

bool sspAudioFile::verifyValidExtension(const std::wstring& strFilename)
{
  if (s_extensions.empty()) {
    int nMajorCount;
	  sf_command (NULL, SFC_GET_FORMAT_MAJOR_COUNT, &nMajorCount, sizeof (int));

    SF_FORMAT_INFO info ;
    for (int i=0; i<nMajorCount; ++i) {
      info.format = i;
  		sf_command (NULL, SFC_GET_FORMAT_MAJOR, &info, sizeof (info));

			// Concert to Unibyte string
			size_t converted;
			wchar_t str[10];
			mbstowcs_s(&converted, str, 10, info.extension, _TRUNCATE);
      s_extensions.push_back(str);

      if (strcmp(info.extension, "aiff") == 0) s_extensions.push_back(_T("aif"));  // Hack to support Windows short form
    }
  }

  size_t nPos = strFilename.find_last_of(L".");
  std::wstring ext = strFilename.substr(nPos+1, strFilename.length() - nPos);
  std::vector<std::wstring>::iterator finder = find(s_extensions.begin(), s_extensions.end(), ext);
  return finder != s_extensions.end();
}
