// SSpPlayObjects.cpp: implementation of the SSpWavFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "SSpPlayObjects.h"
#include "sspMidiScheduler.h"
#include "sspDirectSoundService.h"
#include "sspPools.h"
#include "sspDeviceManager.h"
#include "sspPlayManager.h"
#include "sspScheduleObjects.h"
#include "DMXselectFile.h"
#include "sspSetting.h"
#include "SSpValues.h"

#include <sspAudioFile.h>
#include <sspMidiDevice.h>
#include <sspMidiFileReader.h>
#include <sspMidiListFile.h>

#include <direct.h>
#include "debug.h"

#include <algorithm>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( SSpWavFile, CObject, 1 )
IMPLEMENT_SERIAL( SSpMidiFile, CObject, 1 )
IMPLEMENT_SERIAL( SSpDSFile, CObject, 1 )
IMPLEMENT_SERIAL( SSpMidiEventList, CObject, 1 )
IMPLEMENT_SERIAL( SSpDmxLivePlayer, CObject, 1 )
IMPLEMENT_SERIAL( SSpSilence, CObject, 1 )
IMPLEMENT_SERIAL( SSpSimultaneousGroup, CObject, 1 )
IMPLEMENT_SERIAL( SSpSequentialGroup, CObject, 1 )
IMPLEMENT_SERIAL( SSpLinearSelectGroup, CObject, 1 )
IMPLEMENT_SERIAL( SSpRandomGroup, CObject, 1 )
IMPLEMENT_SERIAL( SSpChainGroup, CObject, 1 )
IMPLEMENT_SERIAL( SSpConditionalGroup, CObject, 1 )
IMPLEMENT_SERIAL( SSpDistributionObject, CObject, 1 )
IMPLEMENT_SERIAL( SSpStateObject, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// SSpWavFile
//////////////////////////////////////////////////////////////////////

SSpWavFile::SSpWavFile() 
: sspPlayObject(SSP_PLAY_AUDIO_FILE)
, m_bFolder(false)
, m_nFilterIndex(0)
, m_nDuration(0)
{
	m_nResourceUsage[SSP_WAV] = 1;
}

SSpWavFile::SSpWavFile(const SSpWavFile& obj) 
: sspPlayObject(obj)
, m_strFileName(obj.m_strFileName)
, m_bFolder(obj.m_bFolder)
, m_nFilterIndex(obj.m_nFilterIndex)
, m_nDuration(0)
{
	m_nResourceUsage[SSP_WAV] = 1;
}

SSpWavFile& SSpWavFile::operator= (const SSpWavFile& obj)
{
	sspPlayObject::operator=(obj);
	m_bFolder = obj.m_bFolder;
	m_strFileName = obj.m_strFileName;
  m_nFilterIndex = obj.m_nFilterIndex;
	return *this;
}

SSpWavFile::~SSpWavFile()
{
}

void SSpWavFile::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_strFileName << m_bFolder << m_nFilterIndex;
	else
		ar >> m_strFileName >> m_bFolder >> m_nFilterIndex;
}

void SSpWavFile::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpWavFile " << m_strName;
	if (m_bFolder) outStr << endl << "   - is folder";
	outStr << endl << "   - m_strFileName " << (LPCTSTR)m_strFileName;
	outStr << endl << "   - m_nFilterIndex " << sspPool::Instance().strings.GetName(m_nFilterIndex);
}

bool SSpWavFile::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_strFileName.IsEmpty()) {
		printError(outStr, _T("(SSpWavFile): no file path"), nErrors);
		bRet = false;
	}
	if (m_nFilterIndex < 0 || m_nFilterIndex >= (int) sspPool::Instance().strings.GetSize()) {
		printError(outStr, _T("(SSpWavFile): m_nFilterIndex is not valid"), nErrors);
		bRet = false;
	}
	if (m_bFolder) {
		if (_wchdir((LPCTSTR)m_strFileName) != 0) {
			printError(outStr, _T("(SSpWavFile): the folder does not exist"), nErrors);
			bRet = false;
		}
	}
	else {
		ifstream test((LPCTSTR)m_strFileName);
		if (!test) {
			printError(outStr, _T("(SSpWavFile): unable to open file"), nErrors);
			bRet = false;
		}
	}
	return bRet;
}

bool SSpWavFile::initialize()
{
  m_nPlayCount = 0;
  return true;
}

bool SSpWavFile::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
  bool bRet = false;

	sspStrVec filenames;
  if (findFiles(filenames)) {
    m_pOwner = pOwner;
    if (filenames.size() == 1) {
      m_previous = filenames[0];
    }
    else {
      // Random selection
      size_t nSelect = (size_t) (filenames.size() * (rand() / (float) (RAND_MAX+1)));
      m_previous = filenames[nSelect];
    }
    sspAudioFile* pFile = new sspAudioFile(m_previous);
    if (pBuf->loadAudioFile(pFile, this) && sspPlayManager::Instance().getScheduler().addObject(pBuf)) {
			DOUT1(_T("SSpWavFile begin %s success\n"), m_previous.c_str());
      int nDuration = static_cast<int>(pFile->getDuration());
      if (m_nPlayCount) m_nDuration = std::max(nDuration, m_nDuration);
      m_nPlayCount++;
      bRet = true;
    }
    else {
			DOUT1(_T("SSpWavFile begin %s failed\n"), m_previous.c_str());
      delete pFile;
    }
  }
	return bRet;
}

bool SSpWavFile::update()
{
	DOUT1(_T("SSpWavFile update %s\n"), m_previous.c_str());
  m_nPlayCount--;
  return false;
}

void SSpWavFile::end()
{
	DOUT1(_T("SSpWavFile end %s\n"), m_previous.c_str());
  m_nPlayCount--;
}

int SSpWavFile::estimateDuration()
{
	return m_nDuration;
}

bool SSpWavFile::findFiles(sspStrVec& names)
{
  names.clear();

  if (m_strFileName.GetLength() > 0) {
		if (m_bFolder) {
			if (_wchdir((LPCTSTR)m_strFileName) == 0) {
				CFileFind finder;
        bool bFoundPrevious = false;
        const sspString strFilter = sspPool::Instance().strings.getString(m_nFilterIndex)->getString();
        BOOL bSearching = finder.FindFile(strFilter.c_str());
				while (bSearching) {
					bSearching = finder.FindNextFile();
					sspString strName = finder.GetFilePath().GetBuffer();
          if (sspAudioFile::verifyValidExtension(strName)) {
            if (strName == m_previous) // The previous file should not be selected again
              bFoundPrevious = true;
            else
              names.push_back(strName);
          }
        }
				finder.Close();
        // If the previous file is the only valid file in the folder, then select it  
        if (names.empty() && bFoundPrevious)
          names.push_back(m_previous);
			}
    }
    else {
      sspString strName = m_strFileName.GetBuffer();
      if (sspAudioFile::verifyValidExtension(strName)) {
        names.push_back(strName);
      }
    }
  }
  return !names.empty();
}

//////////////////////////////////////////////////////////////////////
// SSpMidiFile
//////////////////////////////////////////////////////////////////////

SSpMidiFile::SSpMidiFile() : sspPlayObject(SSP_PLAY_MIDI_FILE)
{
	m_bFolder = false;
	m_nSelected = 0;
	m_nMidiDevice = sspMidiOutDevice::NO_DEVICE;
	m_nTempoFactor = -1;
	m_nResourceUsage[SSP_MIDI] = 1;
}

SSpMidiFile::SSpMidiFile(const SSpMidiFile& obj) : sspPlayObject(obj)
{
	m_bFolder = obj.m_bFolder;
	m_nSelected = 0;
	m_nResourceUsage[SSP_MIDI] = 1;
	m_strFileName = obj.m_strFileName;
	m_nMidiDevice = obj.m_nMidiDevice;
	m_nTempoFactor = obj.m_nTempoFactor;
}

SSpMidiFile& SSpMidiFile::operator= (const SSpMidiFile& obj)
{
	sspPlayObject::operator=(obj);
	m_bFolder = obj.m_bFolder;
	m_nSelected = 0;
	m_strFileName = obj.m_strFileName;
	m_nMidiDevice = obj.m_nMidiDevice;
	m_nTempoFactor = obj.m_nTempoFactor;
	return *this;
}

SSpMidiFile::~SSpMidiFile()
{
	clearSequences();
}

void SSpMidiFile::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_strFileName << m_nMidiDevice << m_nTempoFactor << m_bFolder;
	else {
		ar >> m_strFileName >> m_nMidiDevice >> m_nTempoFactor >> m_bFolder;
	}
}

void SSpMidiFile::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpMidiFile " << m_strName;
	if (m_bFolder) outStr << endl << "   - is folder";
	outStr << endl << "   - m_strFileName " << (LPCTSTR)m_strFileName;
	outStr << endl << "   - m_nMidiDevice " << sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT]->getSubsetName(m_nMidiDevice);
	outStr << endl << "   - m_nTempoFactor " << sspPool::Instance().values.GetName(m_nTempoFactor);
}

bool SSpMidiFile::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_strFileName.IsEmpty()) {
		printError(outStr, _T("(SSpMidiFile): no file path"), nErrors);
		bRet = false;
	}
	if (m_bFolder) {
		if (_wchdir((LPCTSTR)m_strFileName) != 0) {
			printError(outStr, _T("(SSpMidiFile): the folder does not exist"), nErrors);
			bRet = false;
		}
	}
	else {
		ifstream test((LPCTSTR)m_strFileName);
		if (!test) {
			printError(outStr, _T("(SSpMidiFile): unable to open file"), nErrors);
			bRet = false;
		}
	}
	if (m_nTempoFactor < 0 || m_nTempoFactor >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpMidiFile): m_nTempoFactor is not valid"), nErrors);
		bRet = false;
	}
	if (m_nMidiDevice < 0 || m_nMidiDevice >= (int) sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT]->getSubsetSize()) {
		printError(outStr, _T("(SSpMidiFile): m_nMidiDevice is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpMidiFile::initialize()
{
  m_nPlayCount = 0;
	clearSequences();
	if (m_strFileName.GetLength() > 0) {
		if (m_bFolder) {
			if (_wchdir((LPCTSTR)m_strFileName) == 0) {
				CFileFind finder;
				BOOL bSearching = finder.FindFile(_T("*.mid"));
				while (bSearching)
				{
					bSearching = finder.FindNextFile();
					CString strName = finder.GetFilePath();

          sspMidiFileSong song;
					sspMidiFileReader file;

          if (file.open(strName) == sspMidiFileReader::ERRMSG_OK) {
            if (file.read(song) == sspMidiFileReader::ERRMSG_OK) {
              song.mergeTracks();
              if (!song.empty()) {
                sspMidiScheduler* pScheduler = new sspMidiScheduler;
                pScheduler->swapSequence(song.front());
                pScheduler->setMidiDevice(m_nMidiDevice);
						    pScheduler->setTimeFactor(sspPool::Instance().values.getValue(m_nTempoFactor));
						    pScheduler->setOwner(this);
						    m_pSequences.push_back(pScheduler);
              }
						}
            else {
              AfxMessageBox(IDS_IMPORTMIDI);
            }
				    file.close();
			    }
          else {
            AfxMessageBox(IDS_IMPORTMIDI);
          }
				}
				finder.Close();
			}
		}
		else {
      sspMidiFileSong song;
			sspMidiFileReader file;

      if (file.open(m_strFileName) == sspMidiFileReader::ERRMSG_OK) {
        if (file.read(song) == sspMidiFileReader::ERRMSG_OK) {
          song.mergeTracks();
          if (!song.empty()) {
            sspMidiScheduler* pScheduler = new sspMidiScheduler;
            pScheduler->swapSequence(song.front());
            pScheduler->setMidiDevice(m_nMidiDevice);
						pScheduler->setTimeFactor(sspPool::Instance().values.getValue(m_nTempoFactor));
						pScheduler->setOwner(this);
						m_pSequences.push_back(pScheduler);
          }
				}
        else {
          AfxMessageBox(IDS_IMPORTMIDI);
        }
				file.close();
			}
      else {
        AfxMessageBox(IDS_IMPORTMIDI);
      }
		}
	}
	if (m_pSequences.empty()) {
		CString txt;
		txt.Format(_T("There are no valid midi files in %s"), m_strFileName);
		AfxMessageBox(txt);
		return false;
	}
	else
		return true;
}

bool SSpMidiFile::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
  int nSize = (int) m_pSequences.size();
  if (nSize == 1) {
    if (m_nPlayCount > 0) return false;
    m_nSelected = 0;
  	m_pOwner = pOwner;
    m_pSequences[m_nSelected]->setOwner(this);
  	if (sspPlayManager::Instance().getScheduler().addObject(m_pSequences[m_nSelected]))
      m_nPlayCount++;
    return m_nPlayCount > 0;
  }
  else {
    m_nSelected = (int) (nSize * (rand() / (float) (RAND_MAX+1)));
    // If the file is occupied, try another
    for (int i=0; i<nSize; ++i, ++m_nSelected) {
      if (m_nSelected >= (int) nSize) m_nSelected = 0;
      if (m_pSequences[m_nSelected]->isPlaying())
        continue;
      m_pSequences[m_nSelected]->setOwner(pOwner);
	    return sspPlayManager::Instance().getScheduler().addObject(m_pSequences[m_nSelected]);
	  }
  }
	return false;
}

int SSpMidiFile::estimateDuration()
{
	return m_pSequences.empty() ? 0 : m_pSequences[m_nSelected]->getDuration();
}

void SSpMidiFile::clearSequences()
{
	for (unsigned int i=0; i<m_pSequences.size(); ++i) {
		delete m_pSequences[i];
	}
	m_pSequences.clear();
}

//////////////////////////////////////////////////////////////////////
// SSpDSFile
//////////////////////////////////////////////////////////////////////

SSpDSFile::SSpDSFile() : sspPlayObject(SSP_PLAY_DS_FILE)
{
	m_bFolder = false;
	m_nSelected = 0;
	m_nResourceUsage[SSP_DIRECTSOUND] = 1;
}

SSpDSFile::SSpDSFile(const SSpDSFile& obj) : sspPlayObject(obj)
{
	m_bFolder = obj.m_bFolder;
	m_nSelected = 0;
	m_nResourceUsage[SSP_DIRECTSOUND] = 1;
	m_strFileName = obj.m_strFileName;
}

SSpDSFile& SSpDSFile::operator= (const SSpDSFile& obj)
{
	sspPlayObject::operator=(obj);
	m_bFolder = obj.m_bFolder;
	m_nSelected = 0;
	m_strFileName = obj.m_strFileName;
	return *this;
}

SSpDSFile::~SSpDSFile()
{
	clearFiles();
}

void SSpDSFile::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_strFileName << m_bFolder;
	else
		ar >> m_strFileName >> m_bFolder;
}

void SSpDSFile::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpDSFile " << m_strName;
	if (m_bFolder) outStr << endl << "   - is folder";
	outStr << endl << "   - m_strFileName " << (LPCTSTR)m_strFileName;
}

bool SSpDSFile::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_strFileName.IsEmpty()) {
		printError(outStr, _T("(SSpDSFile): no file path"), nErrors);
		bRet = false;
	}
	if (m_bFolder) {
		if (_wchdir((LPCTSTR)m_strFileName) != 0) {
			printError(outStr, _T("(SSpDSFile): the folder does not exist"), nErrors);
			bRet = false;
		}
	}
	else {
		ifstream test((LPCTSTR)m_strFileName);
		if (!test) {
			printError(outStr, _T("(SSpDSFile): unable to open file"), nErrors);
			bRet = false;
		}
	}
	return bRet;
}

bool SSpDSFile::initialize()
{
	m_nPlayCount = 0;
	clearFiles();
	if (m_strFileName.GetLength() > 0) {
		if (m_bFolder) {
			if (_wchdir((LPCTSTR)m_strFileName) == 0) {
				CFileFind finder;
				BOOL bSearching = finder.FindFile(_T("*.dsc"));
				while (bSearching)
				{
					bSearching = finder.FindNextFile();
					CString strName = finder.GetFilePath();
					sspDScontrolFile* pFile = new sspDScontrolFile;
					if (pFile->open(strName)) {
						if (pFile->readData()) {
							pFile->setOwner(this);
							pFile->close();
							m_pFiles.push_back(pFile);
						}
						else {
							pFile->close();
							delete pFile;
						}
					}
					else
						delete pFile;
				}
				finder.Close();
			}
		}
		else {
			sspDScontrolFile* pFile = new sspDScontrolFile;
			if (pFile->open(m_strFileName)) {
				if (pFile->readData()) {
					pFile->setOwner(this);
					pFile->close();
					m_pFiles.push_back(pFile);
				}
				else {
					pFile->close();
					delete pFile;
				}
			}
			else
				delete pFile;
		}
	}
	if (m_pFiles.empty()) {
		CString txt;
		txt.Format(_T("There are no valid dsc files in %s"), m_strFileName);
		AfxMessageBox(txt);
		return false;
	}
	else
		return true;
}

bool SSpDSFile::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
  int nSize = (int) m_pFiles.size();
  if (nSize == 1) {
    if (m_nPlayCount > 0) return false;
    m_nSelected = 0;
  	m_pOwner = pOwner;
    m_pFiles[m_nSelected]->setOwner(this);
  	m_pFiles[m_nSelected]->initialize(pBuf);
  	if (sspPlayManager::Instance().getScheduler().addObject(m_pFiles[m_nSelected]))
      m_nPlayCount++;
    return m_nPlayCount > 0;
  }
  else {
    m_nSelected = (int) (nSize * (rand() / (float) (RAND_MAX+1)));
    // If the file is occupied, try another
    for (int i=0; i<nSize; ++i, ++m_nSelected) {
      if (m_nSelected >= (int) nSize) m_nSelected = 0;
      if (m_pFiles[m_nSelected]->isPlaying())
        continue;
      m_pFiles[m_nSelected]->setOwner(pOwner);
    	m_pFiles[m_nSelected]->initialize(pBuf);
	    return sspPlayManager::Instance().getScheduler().addObject(m_pFiles[m_nSelected]);
	  }
  }
	return false;
}

int SSpDSFile::estimateDuration()
{
	return m_pFiles.empty() ? 0 : m_pFiles[m_nSelected]->getDuration();
}

void SSpDSFile::clearFiles()
{
	for (unsigned int i=0; i<m_pFiles.size(); ++i) {
		delete m_pFiles[i];
	}
	m_pFiles.clear();
}

//////////////////////////////////////////////////////////////////////
// SSpMidiEventList
//////////////////////////////////////////////////////////////////////

SSpMidiEventList::SSpMidiEventList() : sspPlayObject(SSP_PLAY_MIDI_LIST)
{
	m_bFolder = false;
	m_nSelected = 0;
	m_nMidiDevice = sspMidiOutDevice::NO_DEVICE;
	m_nResourceUsage[SSP_MIDI] = 1;
}

SSpMidiEventList::SSpMidiEventList(const SSpMidiEventList& obj) : sspPlayObject(obj)
{
	m_bFolder = obj.m_bFolder;
	m_nSelected = 0;
	m_nResourceUsage[SSP_MIDI] = 1;
	m_strFileName = obj.m_strFileName;
	m_nMidiDevice = obj.m_nMidiDevice;
}

SSpMidiEventList& SSpMidiEventList::operator= (const SSpMidiEventList& obj)
{
	sspPlayObject::operator=(obj);
	m_bFolder = obj.m_bFolder;
	m_nSelected = 0;
	m_strFileName = obj.m_strFileName;
	m_nMidiDevice = obj.m_nMidiDevice;
	return *this;
}

SSpMidiEventList::~SSpMidiEventList()
{
	clearSequences();
}

void SSpMidiEventList::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_strFileName << m_nMidiDevice << m_bFolder;
	else
		ar >> m_strFileName >> m_nMidiDevice >> m_bFolder;
}

void SSpMidiEventList::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpMidiEventList " << m_strName;
	if (m_bFolder) outStr << endl << "   - is folder";
	outStr << endl << "   - m_strFileName " << (LPCTSTR)m_strFileName;
	outStr << endl << "   - m_nMidiDevice " << sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT]->getSubsetName(m_nMidiDevice);
}

bool SSpMidiEventList::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_strFileName.IsEmpty()) {
		printError(outStr, _T("(SSpMidiEventList): no file path"), nErrors);
		bRet = false;
	}
	if (m_bFolder) {
		if (_wchdir((LPCTSTR)m_strFileName) != 0) {
			printError(outStr, _T("(SSpMidiEventList): the folder does not exist"), nErrors);
			bRet = false;
		}
	}
	else {
		ifstream test((LPCTSTR)m_strFileName);
		if (!test) {
			printError(outStr, _T("(SSpMidiEventList): unable to open file"), nErrors);
			bRet = false;
		}
	}
	if (m_nMidiDevice < 0 || m_nMidiDevice >= (int) sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT]->getSubsetSize()) {
		printError(outStr, _T("(SSpMidiEventList): m_nMidiDevice is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpMidiEventList::initialize()
{
	m_nPlayCount = 0;
	clearSequences();
	if (m_strFileName.GetLength() > 0) {
		if (m_bFolder) {
			if (_wchdir((LPCTSTR)m_strFileName) == 0) {
				CFileFind finder;
				BOOL bSearching = finder.FindFile(_T("*.evt"));
				while (bSearching)
				{
					bSearching = finder.FindNextFile();
					CString strName = finder.GetFilePath();

          sspMidiSequence seq;
			    sspMidiListFile file;

          if (file.open(strName)) {
            if (file.read(seq)) {
              sspMidiScheduler* pScheduler = new sspMidiScheduler;
              pScheduler->swapSequence(seq);
              pScheduler->setMidiDevice(m_nMidiDevice);
					    pScheduler->setOwner(this);
					    m_pSequences.push_back(pScheduler);
				    }
            else {
           		AfxMessageBox(IDS_FAIL_READ_MIDILIST);
            }
				    file.close();
			    }
          else {
         		AfxMessageBox(IDS_FAIL_OPEN_MIDILIST);
          }
				}
				finder.Close();
			}
		}
		else {
      sspMidiSequence seq;
			sspMidiListFile file;

      if (file.open(m_strFileName)) {
        if (file.read(seq)) {
          sspMidiScheduler* pScheduler = new sspMidiScheduler;
          pScheduler->swapSequence(seq);
          pScheduler->setMidiDevice(m_nMidiDevice);
					pScheduler->setOwner(this);
					m_pSequences.push_back(pScheduler);
				}
        else {
          AfxMessageBox(IDS_FAIL_READ_MIDILIST);
        }
				file.close();
			}
      else {
        AfxMessageBox(IDS_FAIL_OPEN_MIDILIST);
      }
		}
	}
	if (m_pSequences.empty()) {
		CString txt;
		txt.Format(_T("There are no valid midi event files in %s"), m_strFileName);
		AfxMessageBox(txt);
		return false;
	}
	else
		return true;
}

bool SSpMidiEventList::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
  int nSize = (int) m_pSequences.size();
  if (nSize == 1) {
    if (m_nPlayCount > 0) return false;
    m_nSelected = 0;
  	m_pOwner = pOwner;
    m_pSequences[m_nSelected]->setOwner(this);
  	if (sspPlayManager::Instance().getScheduler().addObject(m_pSequences[m_nSelected]))
      m_nPlayCount++;
    return m_nPlayCount > 0;
  }
  else {
    m_nSelected = (int) (nSize * (rand() / (float) (RAND_MAX+1)));
    // If the file is occupied, try another
    for (int i=0; i<nSize; ++i, ++m_nSelected) {
      if (m_nSelected >= (int) nSize) m_nSelected = 0;
      if (m_pSequences[m_nSelected]->isPlaying())
        continue;
      m_pSequences[m_nSelected]->setOwner(pOwner);
	    return sspPlayManager::Instance().getScheduler().addObject(m_pSequences[m_nSelected]);
	  }
  }
	return false;
}

int SSpMidiEventList::estimateDuration()
{
	return m_pSequences.empty() ? 0 : m_pSequences[m_nSelected]->getDuration();
}

void SSpMidiEventList::clearSequences()
{
	for (unsigned int i=0; i<m_pSequences.size(); ++i) {
		delete m_pSequences[i];
	}
	m_pSequences.clear();
}

//////////////////////////////////////////////////////////////////////
// SSpDmxLivePlayer
//////////////////////////////////////////////////////////////////////

SSpDmxLivePlayer::SSpDmxLivePlayer() : sspPlayObject(SSP_PLAY_DMX_FILE)
{
	m_bFolder = false;
	m_nSelected = 0;
	m_nDevice = 0;
	m_nDurIndex = -1;
	m_nSelectIndex = -1;
	m_nRefreshRate = 10;
}

SSpDmxLivePlayer::SSpDmxLivePlayer(const SSpDmxLivePlayer& obj) : sspPlayObject(obj)
{
	m_bFolder = obj.m_bFolder;
	m_nSelected = 0;
	m_strFileName = obj.m_strFileName;
	m_nDevice = obj.m_nDevice;
	m_nDurIndex = obj.m_nDurIndex;
	m_nSelectIndex = obj.m_nSelectIndex;
	m_nRefreshRate = obj.m_nRefreshRate;
}

SSpDmxLivePlayer& SSpDmxLivePlayer::operator= (const SSpDmxLivePlayer& obj)
{
	sspPlayObject::operator=(obj);
	m_bFolder = obj.m_bFolder;
	m_nSelected = 0;
	m_strFileName = obj.m_strFileName;
	m_nDevice = obj.m_nDevice;
	m_nDurIndex = obj.m_nDurIndex;
	m_nSelectIndex = obj.m_nSelectIndex;
	m_nRefreshRate = obj.m_nRefreshRate;
	return *this;
}

SSpDmxLivePlayer::~SSpDmxLivePlayer()
{
	clearFiles();
}

void SSpDmxLivePlayer::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_strFileName << m_nDevice << m_nRefreshRate;
		ar << m_nDurIndex << m_nSelectIndex << m_bFolder;
	}
	else {
		ar >> m_strFileName >> m_nDevice >> m_nRefreshRate;
		ar >> m_nDurIndex >> m_nSelectIndex >> m_bFolder;
	}
}

void SSpDmxLivePlayer::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpDmxLivePlayer " << m_strName;
	if (m_bFolder) outStr << endl << "   - is folder";
	outStr << endl << "   - m_strFileName " << (LPCTSTR)m_strFileName << ", m_nRefreshRate: " << m_nRefreshRate;
	outStr << endl << "   - m_nDevice " << sspDeviceManager::Instance()[SSP_DEVICE_DMX]->getSubsetName(m_nDevice);
	outStr << endl << "   - m_nDurIndex " << sspPool::Instance().values.GetName(m_nDurIndex);
	outStr << endl << "   - m_nSelectIndex " << sspPool::Instance().values.GetName(m_nSelectIndex);
}

bool SSpDmxLivePlayer::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_strFileName.IsEmpty()) {
		printError(outStr, _T("(SSpDmxLivePlayer): no file path"), nErrors);
		bRet = false;
	}
	if (m_bFolder) {
		if (_wchdir((LPCTSTR)m_strFileName) != 0) {
			printError(outStr, _T("(SSpDmxLivePlayer): the folder does not exist"), nErrors);
			bRet = false;
		}
	}
	else {
		ifstream test((LPCTSTR)m_strFileName);
		if (!test) {
			printError(outStr, _T("(SSpDmxLivePlayer): unable to open file"), nErrors);
			bRet = false;
		}
	}
	if (m_nDevice < 0 || m_nDevice >= (int) sspDeviceManager::Instance()[SSP_DEVICE_DMX]->getSubsetSize()) {
		printError(outStr, _T("(SSpDmxLivePlayer): m_nDevice is not valid"), nErrors);
		bRet = false;
	}
	if (m_nDurIndex < 0 || m_nDurIndex >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpDmxLivePlayer): m_nDurIndex is not valid"), nErrors);
		bRet = false;
	}
	if (m_nSelectIndex < 0 || m_nSelectIndex >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpDmxLivePlayer): m_nSelectIndex is not valid"), nErrors);
		bRet = false;
	}
	if (m_nRefreshRate <= 0 ) {
		printError(outStr, _T("(SSpDmxLivePlayer): m_nRefreshRate is not valid"), nErrors);
		bRet = false;
	}
	else if (m_nRefreshRate < 10 || m_nRefreshRate > 1000 ) {
		printWarning(outStr, _T("(SSpDmxLivePlayer): m_nRefreshRate is outside recommended range"), nWarnings);
		bRet = false;
	}
	return bRet;
}

bool SSpDmxLivePlayer::initialize()
{
	m_nPlayCount = 0;
	clearFiles();
	if (m_strFileName.GetLength() > 0) {
		if (m_bFolder) {
			if (_wchdir((LPCTSTR)m_strFileName) == 0) {
				CFileFind finder;
				BOOL bSearching = finder.FindFile(_T("*.dmx"));
				while (bSearching)
				{
					bSearching = finder.FindNextFile();
					CString strName = finder.GetFilePath();
					DMXselectFile* pFile = new DMXselectFile;
					if (pFile->open(strName)) {
						if (pFile->readData()) {
							pFile->setOwner(this);
							sspValue* pDur = (m_nDurIndex < 0) ? NULL : sspPool::Instance().values.getValue(m_nDurIndex);
							sspValue* pSel = sspPool::Instance().values.getValue(m_nSelectIndex);
							pFile->initialize(m_nDevice, pDur, pSel, m_nRefreshRate);
							pFile->close();
							m_pFiles.push_back(pFile);
						}
						else {
							pFile->close();
							delete pFile;
						}
					}
					else
						delete pFile;
				}
				finder.Close();
			}
		}
		else {
			DMXselectFile* pFile = new DMXselectFile;
			if (pFile->open(m_strFileName)) {
				if (pFile->readData()) {
					pFile->setOwner(this);
					sspValue* pDur = (m_nDurIndex < 0) ? NULL : sspPool::Instance().values.getValue(m_nDurIndex);
					sspValue* pSel = sspPool::Instance().values.getValue(m_nSelectIndex);
					pFile->initialize(m_nDevice, pDur, pSel, m_nRefreshRate);
					pFile->close();
					m_pFiles.push_back(pFile);
				}
				else {
					pFile->close();
					delete pFile;
				}
			}
			else
				delete pFile;
		}
	}
	if (m_pFiles.empty()) {
		CString txt;
		txt.Format(_T("There are no valid dmx files in %s"), m_strFileName);
		AfxMessageBox(txt);
		return false;
	}
	else
		return true;
}

bool SSpDmxLivePlayer::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
  int nSize = (int) m_pFiles.size();
  if (nSize == 1) {
    if (m_nPlayCount > 0) return false;
    m_nSelected = 0;
  	m_pOwner = pOwner;
    m_pFiles[m_nSelected]->setOwner(this);
  	if (sspPlayManager::Instance().getScheduler().addObject(m_pFiles[m_nSelected]))
      m_nPlayCount++;
    return m_nPlayCount > 0;
  }
  else {
    m_nSelected = (int) (nSize * (rand() / (float) (RAND_MAX+1)));
    // If the file is occupied, try another
    for (int i=0; i<nSize; ++i, ++m_nSelected) {
      if (m_nSelected >= (int) nSize) m_nSelected = 0;
      if (m_pFiles[m_nSelected]->isPlaying())
        continue;
      m_pFiles[m_nSelected]->setOwner(pOwner);
	    return sspPlayManager::Instance().getScheduler().addObject(m_pFiles[m_nSelected]);
	  }
  }
	return false;
}

void SSpDmxLivePlayer::end()
{
	if (m_nPlayCount > 0)
		m_pFiles[m_nSelected]->end();
	m_nPlayCount = 0;
}

int SSpDmxLivePlayer::estimateDuration()
{
	return (m_nDurIndex < 0) ? 0 : (int) sspPool::Instance().values.getValue(m_nDurIndex);
}

void SSpDmxLivePlayer::clearFiles()
{
	for (unsigned int i=0; i<m_pFiles.size(); ++i) {
		delete m_pFiles[i];
	}
	m_pFiles.clear();
}


//////////////////////////////////////////////////////////////////////
// SSpSilence
//////////////////////////////////////////////////////////////////////

SSpSilence::SSpSilence() : sspPlayObject(SSP_PLAY_SILENCE)
{
	m_nValueIndex = -1;
	m_pSilence = NULL;
}

SSpSilence::SSpSilence(const SSpSilence& obj) : sspPlayObject(obj)
{
	m_pSilence = NULL;
	m_nValueIndex = obj.m_nValueIndex;
}

SSpSilence& SSpSilence::operator= (const SSpSilence& obj)
{
	sspPlayObject::operator=(obj);
	m_nValueIndex = obj.m_nValueIndex;
	return *this;
}

SSpSilence::~SSpSilence()
{
	delete m_pSilence;
}

void SSpSilence::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nValueIndex;
	else
		ar >> m_nValueIndex;
}

void SSpSilence::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpSilence " << m_strName;
	outStr << endl << "   - m_nValueIndex " << sspPool::Instance().values.GetName(m_nValueIndex);
}

bool SSpSilence::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nValueIndex < 0 || m_nValueIndex >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpSilence): m_nValueIndex is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpSilence::initialize()
{
	m_nPlayCount = 0;
	delete m_pSilence;
	m_pSilence = new sspSilenceObject(sspPool::Instance().values.getValue(m_nValueIndex));
	if (m_pSilence != NULL) {
		m_pSilence->setOwner(this);
		return true;
	}
	else
		return false;
}

bool SSpSilence::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
	if (m_nPlayCount > 0)
		return false;
	m_pOwner = pOwner;
	if (sspPlayManager::Instance().getScheduler().addObject(m_pSilence))
    m_nPlayCount++;
	return m_nPlayCount > 0;
}

int SSpSilence::estimateDuration()
{
	if (m_pSilence != NULL)
		return (int) sspPool::Instance().values.getValue(m_nValueIndex); 
	else
		return 0;
}

//////////////////////////////////////////////////////////////////////
// SSpSimultaneousGroup
//////////////////////////////////////////////////////////////////////

SSpSimultaneousGroup::SSpSimultaneousGroup() : sspPlayObject(SSP_PLAY_SIMULTANEOUS)
{
	m_nDuration = 0;
}

SSpSimultaneousGroup::SSpSimultaneousGroup(const SSpSimultaneousGroup& obj) : sspPlayObject(obj)
{
	m_nDuration = 0;
	m_nObjects = obj.m_nObjects;
}

SSpSimultaneousGroup& SSpSimultaneousGroup::operator= (const SSpSimultaneousGroup& obj)
{
	sspPlayObject::operator=(obj);
	m_nObjects = obj.m_nObjects;
	return *this;
}

SSpSimultaneousGroup::~SSpSimultaneousGroup()
{
}

void SSpSimultaneousGroup::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nObjects.size();
		for (unsigned int i=0; i<m_nObjects.size(); ++i) {
			ar << m_nObjects[i];
		}
	}
	else {
		int nBuf;
		ar >> nBuf;
		m_nObjects.resize(nBuf);
		for (int i=0; i<nBuf; i++)
			ar >> m_nObjects[i];
	}
}

void SSpSimultaneousGroup::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpSimultaneousGroup " << m_strName;
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		outStr << endl << "   - play object " << i << ": " << sspPool::Instance().objects.GetName(m_nObjects[i]);
	}
}

bool SSpSimultaneousGroup::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nObjects.size(); ++i) {
		if (m_nObjects[i] == m_nIndex || m_nObjects[i] < 0 
			|| m_nObjects[i] >= (int) sspPool::Instance().objects.GetSize()) {
			printError(outStr, _T("(SSpSimultaneousGroup): a play object index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nObjects.size() == 0) {
		printError(outStr, _T("(SSpSimultaneousGroup): there are no play objects"), nErrors);
		bRet = false;
	}
	if (m_nObjects.size() == 1) {
		printWarning(outStr, _T("(SSpSimultaneousGroup): there is only a single play object"), nWarnings);
		bRet = false;
	}
	return bRet;
}

bool SSpSimultaneousGroup::initialize()
{
	m_nPlayCount= 0;
	m_pObjects.clear();
	for (unsigned int i=0; i<m_nObjects.size(); ++i) {
		sspPlayObject* pObj = sspPool::Instance().objects.GetPlayObject(m_nObjects[i]);
		
		// Make sure that any Wav-files are first in the list !
		if (pObj->getResourceUsage(SSP_WAV) > 0)
			m_pObjects.push_front(pObj);
		else
			m_pObjects.push_back(pObj);

		int nDur = pObj->estimateDuration();
		if (nDur > m_nDuration) m_nDuration = nDur;
		for (int j=0; j<SSP_TYPES; j++)
			m_nResourceUsage[j] += pObj->getResourceUsage(j);
	}
	return true;
}

bool SSpSimultaneousGroup::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
  if (m_nPlayCount > 0) 
    return false;

	for (unsigned int i=0; i<m_pObjects.size(); i++) {
		if (m_pObjects[i]->begin(pBuf, this)) 
      m_nPlayCount++;
	}

  if (m_nPlayCount > 0) {
		m_pOwner = pOwner;
    return true;
	}
  else
    return false;
}

bool SSpSimultaneousGroup::update()
{
	m_nPlayCount--;
	return (m_nPlayCount > 0);
}

void SSpSimultaneousGroup::end()
{
	for (unsigned int i=0; i<m_pObjects.size(); i++)
		m_pObjects[i]->end();
	m_nPlayCount = 0;
}

int SSpSimultaneousGroup::estimateDuration()
{
	return m_nDuration;
}

//////////////////////////////////////////////////////////////////////
// SSpSequentialGroup
//////////////////////////////////////////////////////////////////////

SSpSequentialGroup::SSpSequentialGroup() : sspPlayObject(SSP_PLAY_SEQUENTIAL)
{
	m_nDuration = 0;
	m_pBuffer = NULL;
}

SSpSequentialGroup::SSpSequentialGroup(const SSpSequentialGroup& obj) : sspPlayObject(obj)
{
	m_nDuration = 0;
	m_pBuffer = NULL;
	m_nObjects = obj.m_nObjects;
}

SSpSequentialGroup& SSpSequentialGroup::operator= (const SSpSequentialGroup& obj)
{
	sspPlayObject::operator=(obj);
	m_nObjects = obj.m_nObjects;
	return *this;
}

SSpSequentialGroup::~SSpSequentialGroup()
{
}

void SSpSequentialGroup::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nObjects.size();
		for (unsigned int i=0; i<m_nObjects.size(); ++i) {
			ar << m_nObjects[i];
		}
	}
	else {
		int nBuf;
		ar >> nBuf;
		m_nObjects.resize(nBuf);
		for (int i=0; i<nBuf; i++)
			ar >> m_nObjects[i];
	}
}

void SSpSequentialGroup::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpSequentialGroup " << m_strName;
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		outStr << endl << "   - play object " << i << ": " << sspPool::Instance().objects.GetName(m_nObjects[i]);
	}
}

bool SSpSequentialGroup::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nObjects.size(); ++i) {
		if (m_nObjects[i] == m_nIndex || m_nObjects[i] < 0 
			|| m_nObjects[i] >= (int) sspPool::Instance().objects.GetSize()) {
			printError(outStr, _T("(SSpSequentialGroup): a play object index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nObjects.size() == 0) {
		printError(outStr, _T("(SSpSequentialGroup): there are no play objects"), nErrors);
		bRet = false;
	}
	if (m_nObjects.size() == 1) {
		printWarning(outStr, _T("(SSpSequentialGroup): there is only a single play object"), nWarnings);
		bRet = false;
	}
	return bRet;
}

bool SSpSequentialGroup::initialize()
{
	m_nPlayCount= 0;
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		m_nDuration += sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->estimateDuration();
		for (int j=0; j<SSP_TYPES; j++) {
			short nCount = sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->getResourceUsage(j);
			if (nCount > m_nResourceUsage[j]) m_nResourceUsage[j] = nCount;
		}
	}
	return true;
}

bool SSpSequentialGroup::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
  if (m_nPlayCount > 0) 
    return false;

  m_current = m_nObjects.begin();
	if (sspPool::Instance().objects.GetPlayObject(*m_current)->begin(pBuf, this)) {
		m_nPlayCount= 1;
		m_pBuffer = pBuf;
		m_pOwner = pOwner;
	}
	return m_nPlayCount > 0;
}

bool SSpSequentialGroup::update()
{
  m_current++;
	if (m_current != m_nObjects.end() 
    && sspPool::Instance().objects.GetPlayObject(*m_current)->begin(m_pBuffer, this)) {
      return true;
  }
  else {
    m_nPlayCount = 0;
		return false;
  }
}

void SSpSequentialGroup::end()
{
	sspPool::Instance().objects.GetPlayObject(*m_current)->end();
  m_current = m_nObjects.end();
	m_nPlayCount = 0;
}

int SSpSequentialGroup::estimateDuration()
{
	return m_nDuration;
}

//////////////////////////////////////////////////////////////////////
// SSpLinearSelectGroup
//////////////////////////////////////////////////////////////////////

SSpLinearSelectGroup::SSpLinearSelectGroup() : sspPlayObject(SSP_PLAY_LINEAR_SELECT)
{
	m_nValue = -1;
	m_nSelected = 0;
}

SSpLinearSelectGroup::SSpLinearSelectGroup(const SSpLinearSelectGroup& obj) : sspPlayObject(obj)
{
	m_nSelected = 0;
	m_nObjects = obj.m_nObjects;
	m_nValue = obj.m_nValue;
}

SSpLinearSelectGroup& SSpLinearSelectGroup::operator= (const SSpLinearSelectGroup& obj)
{
	sspPlayObject::operator=(obj);
	m_nObjects = obj.m_nObjects;
	m_nValue = obj.m_nValue;
	return *this;
}

SSpLinearSelectGroup::~SSpLinearSelectGroup()
{
}

void SSpLinearSelectGroup::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nObjects.size() << m_nValue;
		for (unsigned int i=0; i<m_nObjects.size(); ++i) {
			ar << m_nObjects[i];
		}
	}
	else {
		int nBuf;
		ar >> nBuf >> m_nValue;
		m_nObjects.resize(nBuf);
		for (int i=0; i<nBuf; i++)
			ar >> m_nObjects[i];
	}
}

void SSpLinearSelectGroup::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpLinearSelectGroup " << m_strName;
	outStr << endl << "   - m_nValue: " << sspPool::Instance().values.GetName(m_nValue);
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		outStr << endl << "   - play object " << i << ": " << sspPool::Instance().objects.GetName(m_nObjects[i]);
	}
}

bool SSpLinearSelectGroup::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nObjects.size(); ++i) {
		if (m_nObjects[i] == m_nIndex || m_nObjects[i] < 0 
			|| m_nObjects[i] >= (int) sspPool::Instance().objects.GetSize()) {
			printError(outStr, _T("(SSpLinearSelectGroup): a play object index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nObjects.size() == 0) {
		printError(outStr, _T("(SSpLinearSelectGroup): there are no play objects"), nErrors);
		bRet = false;
	}
	if (m_nObjects.size() == 1) {
		printWarning(outStr, _T("(SSpLinearSelectGroup): there is only a single play object"), nWarnings);
		bRet = false;
	}
	if (m_nValue < 0 || m_nValue >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpLinearSelectGroup): m_nValue is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpLinearSelectGroup::initialize()
{
	m_nPlayCount= 0;
	int nMaxDur = 0;
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		int nDur = sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->estimateDuration();
		if (nDur > nMaxDur) {
			nMaxDur = nDur;
			m_nSelected = i;
		}
		for (int j=0; j<SSP_TYPES; j++) {
			short nCount = sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->getResourceUsage(j);
			if (nCount > m_nResourceUsage[j]) m_nResourceUsage[j] = nCount;
		}
	}
	if (m_nObjects.empty()) {
		CString txt;
		txt.Format(_T("There are no elements in %s"), m_strName);
		AfxMessageBox(txt);
		return false;
	}
	else
		return true;
}

bool SSpLinearSelectGroup::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
	float fVal = sspPool::Instance().values.getValue(m_nValue)->getValue() + 0.5f;	// rounding
	m_nSelected = fVal < 0 ? 0 : (unsigned int) fVal; 
	if (m_nSelected >= m_nObjects.size()) m_nSelected = m_nObjects.size() - 1;

	return sspPool::Instance().objects.GetPlayObject(m_nObjects[m_nSelected])->begin(pBuf, pOwner);
}

void SSpLinearSelectGroup::end()
{
	sspPool::Instance().objects.GetPlayObject(m_nObjects[m_nSelected])->end();
	m_nPlayCount= 0;
}

int SSpLinearSelectGroup::estimateDuration()
{
	return sspPool::Instance().objects.GetPlayObject(m_nObjects[m_nSelected])->estimateDuration();
}

bool SSpLinearSelectGroup::isPlaying()
{
	return sspPool::Instance().objects.GetPlayObject(m_nObjects[m_nSelected])->isPlaying();
}


//////////////////////////////////////////////////////////////////////
// SSpRandomGroup
//////////////////////////////////////////////////////////////////////

SSpRandomGroup::SSpRandomGroup() : sspPlayObject(SSP_PLAY_RANDOM)
{
	m_nTotalWeight = 0;
	m_nSelected = 0;
}

SSpRandomGroup::SSpRandomGroup(const SSpRandomGroup& obj) : sspPlayObject(obj)
{
	m_nTotalWeight = 0;
	m_nSelected = 0;
	m_nObjects = obj.m_nObjects;
	m_nConstWeight = obj.m_nConstWeight;
	m_nVarWeight = obj.m_nVarWeight;
}

SSpRandomGroup& SSpRandomGroup::operator= (const SSpRandomGroup& obj)
{
	sspPlayObject::operator=(obj);
	m_nObjects = obj.m_nObjects;
	m_nConstWeight = obj.m_nConstWeight;
	m_nVarWeight = obj.m_nVarWeight;
	return *this;
}

SSpRandomGroup::~SSpRandomGroup()
{
}

void SSpRandomGroup::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		int nSize = min(m_nObjects.size(), min(m_nConstWeight.size(), m_nVarWeight.size()));
		ar << nSize;
		for (int i=0; i<nSize; ++i) {
			ar << m_nObjects[i] << m_nConstWeight[i] << m_nVarWeight[i];
		}
	}
	else {
		int nBuf;
		ar >> nBuf;
		m_nObjects.resize(nBuf);
		m_nConstWeight.resize(nBuf);
		m_nVarWeight.resize(nBuf);
		for (int i=0; i<nBuf; i++) {
			ar >> m_nObjects[i] >> m_nConstWeight[i] >> m_nVarWeight[i];
		}
	}
}

void SSpRandomGroup::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpRandomGroup " << m_strName;
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		outStr << endl << "   - play object " << i << ": " << sspPool::Instance().objects.GetName(m_nObjects[i]);
		outStr << " , weight (const): " << m_nConstWeight[i];
		if (m_nVarWeight[i] >= 0)
			outStr << " , weight (variable): " << sspPool::Instance().values.GetName(m_nVarWeight[i]);
		else
			outStr << " , weight (variable): none";
	}
}

bool SSpRandomGroup::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nObjects.size(); ++i) {
		if (m_nObjects[i] == m_nIndex || m_nObjects[i] < 0 
			|| m_nObjects[i] >= (int) sspPool::Instance().objects.GetSize()) {
			printError(outStr, _T("(SSpRandomGroup): a play object index is not valid"), nErrors);
			bRet = false;
		}
		if (m_nVarWeight[i] >= (int) sspPool::Instance().values.GetSize()) {
			printError(outStr, _T("(SSpRandomGroup): a value index is not valid"), nErrors);
			bRet = false;
		}
		if (m_nConstWeight[i] < 0) {
			printError(outStr, _T("(SSpRandomGroup): const weight should be positive"), nErrors);
			bRet = false;
		}
	}
	if (m_nObjects.size() == 0) {
		printError(outStr, _T("(SSpRandomGroup): there are no play objects"), nErrors);
		bRet = false;
	}
	if (m_nObjects.size() == 1) {
		printWarning(outStr, _T("(SSpRandomGroup): there is only a single play object"), nWarnings);
		bRet = false;
	}
	if (m_nObjects.size() != m_nConstWeight.size() || m_nConstWeight.size() != m_nVarWeight.size()) {
		printError(outStr, _T("(SSpRandomGroup): the object and weight sizes do not match"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpRandomGroup::initialize()
{
	m_nPlayCount= 0;
	int nMaxDur = 0;

	m_nElementCount = min(m_nObjects.size(), min(m_nConstWeight.size(), m_nVarWeight.size()));
	for (int i=0; i<m_nElementCount; i++) {
		int nDur = sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->estimateDuration();
		if (nDur > nMaxDur) {
			nMaxDur = nDur;
			m_nSelected = m_nObjects[i];
		}
		for (int j=0; j<SSP_TYPES; j++) {
			short nCount = sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->getResourceUsage(j);
			if (nCount > m_nResourceUsage[j]) m_nResourceUsage[j] = nCount;
		}
		m_nTotalWeight += m_nConstWeight[i];
	}
	return true;
}

bool SSpRandomGroup::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
	int i;
	double fTotWeight = m_nTotalWeight;
	for (i=0; i<m_nElementCount; i++) {
		if (m_nVarWeight[i] >= 0)
			fTotWeight += sspPool::Instance().values.getValue(m_nVarWeight[i])->getValue();
	}
	double fVal = (fTotWeight * (rand() - 1.0)) / RAND_MAX;
	for (i=0; i<m_nElementCount; i++) {
		fVal -= m_nConstWeight[i];
		if (m_nVarWeight[i] >= 0)
			fVal -= sspPool::Instance().values.getValue(m_nVarWeight[i])->getValue();
		if (fVal < 0) {
			m_nSelected = m_nObjects[i];
			break;
		}
	}
	return sspPool::Instance().objects.GetPlayObject(m_nSelected)->begin(pBuf, pOwner);
}

void SSpRandomGroup::end()
{
	sspPool::Instance().objects.GetPlayObject(m_nSelected)->end();
	m_nPlayCount= 0;
}

int SSpRandomGroup::estimateDuration()
{
	return sspPool::Instance().objects.GetPlayObject(m_nSelected)->estimateDuration();
}

bool SSpRandomGroup::isPlaying()
{
	return sspPool::Instance().objects.GetPlayObject(m_nSelected)->isPlaying();
}

//////////////////////////////////////////////////////////////////////
// SSpChainGroup
//////////////////////////////////////////////////////////////////////

SSpChainGroup::SSpChainGroup() : sspPlayObject(SSP_PLAY_CHAIN)
{
	m_nSelectIndex = 0;
	m_nCircleLength = 0;
	m_nCircleCount = 0;
}

SSpChainGroup::SSpChainGroup(const SSpChainGroup& obj) : sspPlayObject(obj)
{
	m_nSelectIndex = 0;
	m_nCircleCount = 0;
	m_nObjects = obj.m_nObjects;
	m_nFollowers = obj.m_nFollowers;
	m_nCircleLength = obj.m_nCircleLength;
}

SSpChainGroup& SSpChainGroup::operator= (const SSpChainGroup& obj)
{
	sspPlayObject::operator=(obj);
	m_nObjects = obj.m_nObjects;
	m_nFollowers = obj.m_nFollowers;
	m_nCircleLength = obj.m_nCircleLength;
	return *this;
}

SSpChainGroup::~SSpChainGroup()
{
}

void SSpChainGroup::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		int nSize = min(m_nObjects.size(), m_nFollowers.size());
		ar << nSize << m_nCircleLength;
		for (int i=0; i<nSize; ++i) {
			ar << m_nObjects[i] << m_nFollowers[i];
		}
	}
	else {
		int nBuf;
		ar >> nBuf >> m_nCircleLength;
		m_nObjects.resize(nBuf);
		m_nFollowers.resize(nBuf);
		for (int i=0; i<nBuf; i++)
			ar >> m_nObjects[i] >> m_nFollowers[i];
	}
}

void SSpChainGroup::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpChainGroup " << m_strName;
	outStr << endl << "   - m_nCircleLength: " << m_nCircleLength;
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		outStr << endl << "   - play object " << i << ": " << sspPool::Instance().objects.GetName(m_nObjects[i]);
		outStr << " ,follower: " << sspPool::Instance().objects.GetName(m_nFollowers[i]);
	}
}

bool SSpChainGroup::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nObjects.size(); ++i) {
		if (m_nObjects[i] == m_nIndex || m_nObjects[i] < 0 
			|| m_nObjects[i] >= (int) sspPool::Instance().objects.GetSize()) {
			printError(outStr, _T("(SSpChainGroup): a play object index is not valid"), nErrors);
			bRet = false;
		}
		if (m_nFollowers[i] == m_nIndex || m_nFollowers[i] < 0 
			|| m_nFollowers[i] >= (int) sspPool::Instance().objects.GetSize()) {
			printError(outStr, _T("(SSpChainGroup): a follower index is not valid"), nErrors);
			bRet = false;
		}
		if (m_nObjects[i] == m_nFollowers[i]) {
			printError(outStr, _T("(SSpChainGroup): play object and follower is identical"), nErrors);
			bRet = false;
		}
	}
	if (m_nCircleLength <= 1) {
		printError(outStr, _T("(SSpChainGroup): the chain has too few elements"), nErrors);
		bRet = false;
	}
	if (m_nObjects.size() == 1) {
		printWarning(outStr, _T("(SSpChainGroup): there is only a single play object"), nWarnings);
		bRet = false;
	}
	if (m_nObjects.size() == 0) {
		printError(outStr, _T("(SSpChainGroup): there are no play objects"), nErrors);
		bRet = false;
	}
	if (m_nObjects.size() != m_nFollowers.size()) {
		printError(outStr, _T("(SSpChainGroup): the object and follower sizes do not match"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpChainGroup::initialize()
{
	m_nPlayCount= 0;
	m_nCircleCount = m_nCircleLength;
	int nMaxDur = 0;
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		int nDur = sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->estimateDuration();
		if (nDur > nMaxDur) {
			nMaxDur = nDur;
		}
		for (int j=0; j<SSP_TYPES; j++) {
			short nCount = sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->getResourceUsage(j);
			if (nCount > m_nResourceUsage[j]) m_nResourceUsage[j] = nCount;
		}
	}
	return true;
}

bool SSpChainGroup::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
	if (m_nCircleCount == m_nCircleLength) {
		m_nSelectIndex = (int) (m_nObjects.size() * (rand() / (float) (RAND_MAX+1)));
		m_nCircleCount = 1;
	}
	else {
		m_nSelectIndex = m_nFollowers[m_nSelectIndex];
		m_nCircleCount++;
	}
	return sspPool::Instance().objects.GetPlayObject(m_nObjects[m_nSelectIndex])->begin(pBuf, pOwner);
}

void SSpChainGroup::end()
{
	sspPool::Instance().objects.GetPlayObject(m_nObjects[m_nSelectIndex])->end();
	m_nPlayCount= 0;
	m_nCircleCount = m_nCircleLength;
}

int SSpChainGroup::estimateDuration()
{
	return sspPool::Instance().objects.GetPlayObject(m_nObjects[m_nSelectIndex])->estimateDuration();
}

bool SSpChainGroup::isPlaying()
{
	return sspPool::Instance().objects.GetPlayObject(m_nObjects[m_nSelectIndex])->isPlaying();
}

//////////////////////////////////////////////////////////////////////
// SSpConditionalGroup
//////////////////////////////////////////////////////////////////////

SSpConditionalGroup::SSpConditionalGroup() : sspPlayObject(SSP_PLAY_CONDITIONAL)
{
	m_nSelected = 0;
}

SSpConditionalGroup::SSpConditionalGroup(const SSpConditionalGroup& obj) : sspPlayObject(obj)
{
	m_nSelected = 0;
	m_nObjects = obj.m_nObjects;
	m_nConditionals = obj.m_nConditionals;
	m_nDefaultObject = obj.m_nDefaultObject;
}

SSpConditionalGroup& SSpConditionalGroup::operator= (const SSpConditionalGroup& obj)
{
	sspPlayObject::operator=(obj);
	m_nObjects = obj.m_nObjects;
	m_nConditionals = obj.m_nConditionals;
	m_nDefaultObject = obj.m_nDefaultObject;
	return *this;
}

SSpConditionalGroup::~SSpConditionalGroup()
{
}

void SSpConditionalGroup::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		int nSize = min(m_nObjects.size(), m_nConditionals.size());
		ar << nSize;
		for (int i=0; i<nSize; ++i) {
			ar << m_nObjects[i] << m_nConditionals[i];
		} 
		ar << m_nDefaultObject; 
	}
	else {
		int nBuf;
		ar >> nBuf;
		m_nObjects.resize(nBuf);
		m_nConditionals.resize(nBuf);
		for (int i=0; i<nBuf; i++)
			ar >> m_nObjects[i] >> m_nConditionals[i];
		ar >> m_nDefaultObject; 
	}
}

void SSpConditionalGroup::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpConditionalGroup " << m_strName;
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		outStr << endl << "   - play object " << i << ": " << sspPool::Instance().objects.GetName(m_nObjects[i]);
		outStr << " , with conditional: " << sspPool::Instance().conditionals.GetName(m_nConditionals[i]);
	}
	if (m_nDefaultObject >= 0)
		outStr << endl << "   - m_nDefaultObject: " << sspPool::Instance().objects.GetName(m_nDefaultObject);
	else
		outStr << endl << "   - m_nDefaultObject: none";
}

bool SSpConditionalGroup::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (unsigned int i=0; i<m_nObjects.size(); ++i) {
		if (m_nObjects[i] == m_nIndex || m_nObjects[i] < 0 
			|| m_nObjects[i] >= (int) sspPool::Instance().objects.GetSize()) {
			printError(outStr, _T("(SSpConditionalGroup): a play object index is not valid"), nErrors);
			bRet = false;
		}
		if (m_nConditionals[i] < 0 || m_nConditionals[i] >= (int) sspPool::Instance().conditionals.GetSize()) {
			printError(outStr, _T("(SSpConditionalGroup): a conditional index is not valid"), nErrors);
			bRet = false;
		}
	}
	if (m_nObjects.size() == 1 && m_nDefaultObject < 0) {
		printWarning(outStr, _T("(SSpConditionalGroup): there is only a single play object"), nWarnings);
		bRet = false;
	}
	if (m_nObjects.size() == 0) {
		printError(outStr, _T("(SSpConditionalGroup): there are no play objects"), nErrors);
		bRet = false;
	}
	if (m_nObjects.size() != m_nConditionals.size()) {
		printError(outStr, _T("(SSpConditionalGroup): the object and conditional sizes do not match"), nErrors);
		bRet = false;
	}
	if (m_nDefaultObject < 0) {
		printWarning(outStr, _T("(SSpConditionalGroup): there is no m_nDefaultObject"), nErrors);
		bRet = false;
	}
	else if (m_nDefaultObject == m_nIndex || m_nDefaultObject >= (int) sspPool::Instance().objects.GetSize()) {
		printError(outStr, _T("(SSpDistributionObject): m_nDefaultObject is not valid"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpConditionalGroup::initialize()
{
	m_nPlayCount= 0;
	int nMaxDur = 0;
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		int nDur = sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->estimateDuration();
		if (nDur > nMaxDur) {
			nMaxDur = nDur;
			m_nSelected = m_nObjects[i];
		}
		for (int j=0; j<SSP_TYPES; j++) {
			short nCount = sspPool::Instance().objects.GetPlayObject(m_nObjects[i])->getResourceUsage(j);
			if (nCount > m_nResourceUsage[j]) m_nResourceUsage[j] = nCount;
		}
	}
	return true;
}

bool SSpConditionalGroup::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
	m_nSelected = m_nDefaultObject;
	for (unsigned int i=0; i<m_nObjects.size(); i++) {
		if (sspPool::Instance().conditionals.GetConditional(m_nConditionals[i])->isTrue()) {
			m_nSelected = m_nObjects[i];
			break;
		}
	}
	if (m_nSelected >= 0)
		return sspPool::Instance().objects.GetPlayObject(m_nSelected)->begin(pBuf, pOwner);
	else
		return false;
}

void SSpConditionalGroup::end()
{
	sspPool::Instance().objects.GetPlayObject(m_nSelected)->end();
	m_nPlayCount= 0;
}

int SSpConditionalGroup::estimateDuration()
{
	return sspPool::Instance().objects.GetPlayObject(m_nSelected)->estimateDuration();
}

bool SSpConditionalGroup::isPlaying()
{
	return sspPool::Instance().objects.GetPlayObject(m_nSelected)->isPlaying();
}

//////////////////////////////////////////////////////////////////////
// SSpDistributionObject
//////////////////////////////////////////////////////////////////////

SSpDistributionObject::SSpDistributionObject() : sspPlayObject(SSP_PLAY_DISTRIBUTION)
{
	m_nObject = -1;
	m_nStartInterval = -1;
	m_nEndInterval = -1;
	m_nLoopCondition = -1;
	m_nLoopMode = LOOP_DURATION;
	m_nLoopDuration = -1;
	m_pBuffer = NULL;
	m_silence = new sspSilenceObject(NULL);
	m_bSilence = false;
}

SSpDistributionObject::SSpDistributionObject(const SSpDistributionObject& obj) : sspPlayObject(obj)
{
	m_pBuffer = NULL;
	m_silence = new sspSilenceObject(NULL);
	m_bSilence = false;
	m_nObject = obj.m_nObject;
	m_nStartInterval = obj.m_nStartInterval;
	m_nEndInterval = obj.m_nEndInterval;
	m_nLoopCondition = obj.m_nLoopCondition;
	m_nLoopMode = obj.m_nLoopMode;
	m_nLoopDuration = obj.m_nLoopDuration;
}

SSpDistributionObject& SSpDistributionObject::operator= (const SSpDistributionObject& obj)
{
	sspPlayObject::operator=(obj);
	m_nObject = obj.m_nObject;
	m_nStartInterval = obj.m_nStartInterval;
	m_nEndInterval = obj.m_nEndInterval;
	m_nLoopCondition = obj.m_nLoopCondition;
	m_nLoopMode = obj.m_nLoopMode;
	m_nLoopDuration = obj.m_nLoopDuration;
	return *this;
}

SSpDistributionObject::~SSpDistributionObject()
{
	delete m_silence;
}

void SSpDistributionObject::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nObject << m_nStartInterval << m_nEndInterval << m_nLoopCondition << m_nLoopMode << m_nLoopDuration;
	}
	else {
		ar >> m_nObject >> m_nStartInterval >> m_nEndInterval >> m_nLoopCondition >> m_nLoopMode >> m_nLoopDuration;
	}
}

void SSpDistributionObject::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpDistributionObject " << m_strName;
	outStr << endl << "   - m_nObject: " << sspPool::Instance().objects.GetName(m_nObject);
	outStr << endl << "   - m_nStartInterval: " << sspPool::Instance().values.GetName(m_nStartInterval);
	outStr << " , m_nEndInterval: " << sspPool::Instance().values.GetName(m_nEndInterval);
	outStr << endl << "   - m_nLoopCondition: " << sspPool::Instance().conditionals.GetName(m_nLoopCondition);
	outStr << " , m_nLoopMode: ";
  switch (m_nLoopMode) {
    case LOOP_DURATION:
    	outStr << "On duration";
      break;
    case LOOP_COUNT:
    	outStr << "On count";
      break;
    case LOOP_CONDITION:
    	outStr << "On condition";
      break;
    default:
      break;
  }
	if (m_nLoopMode == LOOP_COUNT || m_nLoopMode == LOOP_DURATION) {
		outStr << " , m_nLoopDuration: " << sspPool::Instance().values.GetName(m_nLoopDuration);
	}
}

bool SSpDistributionObject::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nObject == m_nIndex || m_nObject < 0 || m_nObject >= (int) sspPool::Instance().objects.GetSize()) {
		printError(outStr, _T("(SSpDistributionObject): m_nObject is not valid"), nErrors);
		bRet = false;
	}
	if (m_nStartInterval < 0 || m_nStartInterval >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpDistributionObject): m_nStartInterval is not valid"), nErrors);
		bRet = false;
	}
	if (m_nEndInterval >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpDistributionObject): m_nEndInterval is not valid"), nErrors);
		bRet = false;
	}
	if (m_nLoopCondition < 0 || m_nLoopCondition >= (int) sspPool::Instance().conditionals.GetSize()) {
		printError(outStr, _T("(SSpDistributionObject): m_nLoopCondition is not valid"), nErrors);
		bRet = false;
	}
	if (m_nLoopMode == LOOP_COUNT || m_nLoopMode == LOOP_DURATION) {
		if (m_nLoopDuration < 0 || m_nLoopDuration > (int) sspPool::Instance().values.GetSize()) {
			printError(outStr, _T("(SSpDistributionObject): m_nLoopDuration is not valid"), nErrors);
			bRet = false;
		}
	}
	return bRet;
}

bool SSpDistributionObject::initialize()
{
	m_nPlayCount= 0;
	m_silence->setOwner(this);
	for (int i=0; i<SSP_TYPES; i++)
		m_nResourceUsage[i] = sspPool::Instance().objects.GetPlayObject(m_nObject)->getResourceUsage(i);
	if (m_nEndInterval < 0) m_nEndInterval = m_nStartInterval;
	return true;
}

bool SSpDistributionObject::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
  if (m_nPlayCount > 0)
    return false;

	if (sspPool::Instance().objects.GetPlayObject(m_nObject)->begin(pBuf, this)) {
		m_pOwner = pOwner;
		m_pBuffer = pBuf;
		m_bSilence = false;
		m_nLoopCounter = 0;
    m_nPlayCount++;
		if (m_nLoopMode == LOOP_DURATION) time(&m_tStart);
	}
	return isPlaying();
}

bool SSpDistributionObject::update()
{
	float fTimeFac = sspPool::Instance().conditionals.GetConditional(m_nLoopCondition)->isTrue() ? 0.0f : 1.0f;
	switch (m_nLoopMode) {
	case LOOP_DURATION:
		if (fTimeFac < 1.0) {
			fTimeFac = (float) difftime(time(NULL), m_tStart) / sspPool::Instance().values.getValue(m_nLoopDuration)->getValue();
		}
		break;
	case LOOP_COUNT:
		if (fTimeFac < 1.0 && !m_bSilence) {
			m_nLoopCounter++;
			fTimeFac = m_nLoopCounter / sspPool::Instance().values.getValue(m_nLoopDuration)->getValue();
		}
		break;
	default:
		break;
	}

	if (fTimeFac < 1.0) {
		if (m_bSilence) {
			if (sspPool::Instance().objects.GetPlayObject(m_nObject)->begin(m_pBuffer, this)) {
				m_bSilence = false;
				return true;
			}
		}
		else {
			if (fTimeFac == 0.0f) 
				m_silence->setDuration(sspPool::Instance().values.getValue(m_nStartInterval)->getValue());
			else {
				float fStart = sspPool::Instance().values.getValue(m_nStartInterval)->getValue();
				float fEnd = sspPool::Instance().values.getValue(m_nEndInterval)->getValue();
				m_silence->setDuration(fStart + fTimeFac * (fEnd - fStart));
			}
			if (sspPlayManager::Instance().getScheduler().addObject(m_silence)) {
				m_bSilence = true;
				return true;
			}
		}
	}
  m_nPlayCount = 0;
	return false;
}

void SSpDistributionObject::end()
{
	sspPool::Instance().objects.GetPlayObject(m_nObject)->end();
  m_silence->setPlaying(false);
	m_nPlayCount= 0;
}

int SSpDistributionObject::estimateDuration()
{
	switch (m_nLoopMode) {
	case LOOP_DURATION:
		return (int) sspPool::Instance().values.getValue(m_nLoopDuration)->getValue();
	case LOOP_COUNT:
		return (int) (sspPool::Instance().values.getValue(m_nLoopDuration)->getValue() 
			* sspPool::Instance().objects.GetPlayObject(m_nObject)->estimateDuration());
	case LOOP_CONDITION:
	default:
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////
// SSpStateObject
//////////////////////////////////////////////////////////////////////

SSpStateObject::SSpStateObject() 
  : sspPlayObject(SSP_PLAY_STATE)
  , m_nGlobalState(-1)
  , m_nInputValue(-1)
  , m_nMode(STATE_TYPE_INCREMENT)
{
}

SSpStateObject::SSpStateObject(const SSpStateObject& obj) 
  : sspPlayObject(obj)
  , m_nGlobalState(obj.m_nGlobalState)
  , m_nInputValue(obj.m_nInputValue)
  , m_nMode(obj.m_nMode)
{
}

SSpStateObject& SSpStateObject::operator= (const SSpStateObject& obj)
{
	sspPlayObject::operator=(obj);
  m_nGlobalState = obj.m_nGlobalState;
  m_nInputValue = obj.m_nInputValue;
  m_nMode = obj.m_nMode;
	return *this;
}

SSpStateObject::~SSpStateObject()
{
}

void SSpStateObject::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nGlobalState << m_nInputValue << m_nMode;
	else
		ar >> m_nGlobalState >> m_nInputValue >> m_nMode;
}

void SSpStateObject::printASCII(sspOutStream& outStr)
{
	outStr << endl << m_nIndex << ": SSpStateObject " << m_strName;
  outStr << "m_nGlobalState: " << sspPool::Instance().values.GetName(m_nGlobalState);
  switch (m_nMode) {
    case STATE_TYPE_INCREMENT:
      outStr << ", increment.";
      break;
    case STATE_TYPE_DECREMENT:
      outStr << ", decrement.";
      break;
    case STATE_TYPE_SETVALUE:
      outStr << ", set value: " << sspPool::Instance().values.GetName(m_nInputValue);
      break;
    default:
      outStr << ", ILLEGAL MODE!";
      break;
  }
}

bool SSpStateObject::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (m_nGlobalState < 0 || m_nGlobalState >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(SSpStateObject): m_nGlobalState is not valid"), nErrors);
		bRet = false;
	}

	if (m_nMode == STATE_TYPE_SETVALUE && 
    (m_nInputValue < 0 || m_nInputValue >= (int) sspPool::Instance().values.GetSize())) {
		printError(outStr, _T("(SSpStateObject): m_nInputValue is not valid"), nErrors);
		bRet = false;
	}

  if (m_nMode != STATE_TYPE_INCREMENT && m_nMode != STATE_TYPE_DECREMENT && m_nMode != STATE_TYPE_SETVALUE) {
		printError(outStr, _T("(SSpStateObject): m_nMode is not valid"), nErrors);
		bRet = false;
	}

  return bRet;
}

bool SSpStateObject::initialize() 
{
  SSpBasicValue* pState = dynamic_cast<SSpBasicValue*>(sspPool::Instance().values.getValue(m_nGlobalState));
  if (!pState) {
		AfxMessageBox(L"The state variable must be a simple value.");
		return false;
	}
	else
		return true;
}

bool SSpStateObject::begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner)
{
  SSpBasicValue* pState = dynamic_cast<SSpBasicValue*>(sspPool::Instance().values.getValue(m_nGlobalState));
  if (!pState) return false;

  switch (m_nMode) {
    case STATE_TYPE_INCREMENT:
      pState->setValue(pState->getValue() + 1.0f);
      break;
    case STATE_TYPE_DECREMENT:
      pState->setValue(pState->getValue() - 1.0f);
      break;
    case STATE_TYPE_SETVALUE:
      pState->setValue(sspPool::Instance().values.getValue(m_nInputValue)->getValue());
      break;
    default:
      break;
  }

  return false;
}

int SSpStateObject::estimateDuration()
{
	return 0;
}

