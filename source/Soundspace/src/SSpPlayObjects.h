// SSpPlayObjects.h: interface for the SSpPlayObject classes.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSPSOUNDOBJECTS_H__99543961_9202_11D3_901C_F98F7F995B9F__INCLUDED_)
#define AFX_SSPSOUNDOBJECTS_H__99543961_9202_11D3_901C_F98F7F995B9F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include "SSpTasks.h"
#include "sspDScontrolFile.h"

#include <vector>
#include <deque>	// For simultaneous group

class sspSilenceObject;
class DMXObject;
class sspAudioFile;

class SSpWavFile : public sspPlayObject  
{
private:
	CString m_strFileName;
	bool		m_bFolder;
  int     m_nFilterIndex;

  // Local variables
  sspString		m_previous;
  int         m_nDuration;

  DECLARE_SERIAL( SSpWavFile )

public:
	SSpWavFile();
	SSpWavFile(const SSpWavFile& obj);
	SSpWavFile& operator= (const SSpWavFile& obj);
	virtual ~SSpWavFile();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);
	bool update();
	void end();

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();

	void setFileName(LPCTSTR strName) {m_strFileName = strName;}
	void setFolder(bool bTrue) {m_bFolder = bTrue;}
  void setFilterIndex(int nIndex) {m_nFilterIndex = nIndex;}

	CString getFileName() const {return m_strFileName;}
	bool	getFolder() const {return m_bFolder;}
  int   getFilterIndex() const {return m_nFilterIndex;}
	
private:
	bool findFiles(sspStrVec& names);
};

class sspMidiScheduler;
class SSpMidiFile : public sspPlayObject  
{
private:
	CString		m_strFileName;
	int				m_nMidiDevice;
	int				m_nTempoFactor;
	bool			m_bFolder;
	int				m_nSelected;

	std::vector<sspMidiScheduler*> m_pSequences;

  DECLARE_SERIAL( SSpMidiFile )

public:
	SSpMidiFile();
	SSpMidiFile(const SSpMidiFile& obj);
	SSpMidiFile& operator= (const SSpMidiFile& obj);
	virtual ~SSpMidiFile();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();

	void setFileName(LPCTSTR strName) {m_strFileName = strName;}
	void setMidiDevice(int nDevice) {m_nMidiDevice = nDevice;}
	void setTempoFactor(int nTempoFac) {m_nTempoFactor = nTempoFac;}
	void setFolder(bool bTrue) {m_bFolder = bTrue;}

	CString getFileName() const {return m_strFileName;}
	int		getMidiDevice() const {return m_nMidiDevice;}
	int		getTempoFactor() const {return m_nTempoFactor;}
	bool	getFolder() const {return m_bFolder;}

	void clearSequences();
};

class SSpDSFile : public sspPlayObject  
{
private:
	CString			m_strFileName;
	bool			m_bFolder;
	int				m_nSelected;

	std::vector<sspDScontrolFile*> m_pFiles;

    DECLARE_SERIAL( SSpDSFile )

public:
	SSpDSFile();
	SSpDSFile(const SSpDSFile& obj);
	SSpDSFile& operator= (const SSpDSFile& obj);
	virtual ~SSpDSFile();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();

	void setFileName(LPCTSTR strName) {m_strFileName = strName;}
	void setFolder(bool bTrue) {m_bFolder = bTrue;}

	CString getFileName() const {return m_strFileName;}
	bool	getFolder() const {return m_bFolder;}

	void clearFiles();
};

class SSpMidiEventList : public sspPlayObject  
{
private:
	CString			m_strFileName;
	int				m_nMidiDevice;
	bool			m_bFolder;
	int				m_nSelected;

	std::vector<sspMidiScheduler*> m_pSequences;

	DECLARE_SERIAL( SSpMidiEventList )

public:
	SSpMidiEventList();
	SSpMidiEventList(const SSpMidiEventList& obj);
	SSpMidiEventList& operator= (const SSpMidiEventList& obj);
	virtual ~SSpMidiEventList();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();

	void setMidiDevice(int nDevice) {m_nMidiDevice = nDevice;}
	void setFileName(LPCTSTR strName) {m_strFileName = strName;}
	void setFolder(bool bTrue) {m_bFolder = bTrue;}

	CString getFileName() const {return m_strFileName;}
	int		getMidiDevice() const {return m_nMidiDevice;}
	bool	getFolder() const {return m_bFolder;}

	void clearSequences();
};

class DMXselectFile;

class SSpDmxLivePlayer : public sspPlayObject  
{
private:
	CString			m_strFileName;
	int				m_nDevice;
	int				m_nDurIndex;	// -1 is endless
	int				m_nSelectIndex;
	int				m_nRefreshRate;
	bool			m_bFolder;
	int				m_nSelected;

	std::vector<DMXselectFile*> m_pFiles;

    DECLARE_SERIAL( SSpDmxLivePlayer )

public:
	SSpDmxLivePlayer();
	SSpDmxLivePlayer(const SSpDmxLivePlayer& obj);
	SSpDmxLivePlayer& operator= (const SSpDmxLivePlayer& obj);
	virtual ~SSpDmxLivePlayer();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);
	void end();

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();

	void setFileName(LPCTSTR strName) {m_strFileName = strName;}
	void setDmxDevice(int nDevice) {m_nDevice = nDevice;}
	void setRefreshRate(int nRate) {m_nRefreshRate = nRate;}
	void setDuration(int nIndex) {m_nDurIndex = nIndex;}
	void setSelector(int nIndex) {m_nSelectIndex = nIndex;}
	void setFolder(bool bTrue) {m_bFolder = bTrue;}

	CString getFileName() const {return m_strFileName;}
	int		getDevice() const {return m_nDevice;}
	int		getDuration() const {return m_nDurIndex;}
	int		getSelector() const {return m_nSelectIndex;}
	int		getRefreshRate() const {return m_nRefreshRate;}
	bool	getFolder() const {return m_bFolder;}

	void clearFiles();
};


class SSpSilence : public sspPlayObject  
{
private:
	int				m_nValueIndex;
	sspSilenceObject*	m_pSilence;

    DECLARE_SERIAL( SSpSilence )

public:
	SSpSilence();
	SSpSilence(const SSpSilence& obj);
	SSpSilence& operator= (const SSpSilence& obj);
	virtual ~SSpSilence();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();

	void setLengthValue(int nIndex) {m_nValueIndex = nIndex;}
	int	 getLengthValue() const {return m_nValueIndex;}
};

class SSpSimultaneousGroup : public sspPlayObject  
{
private:
	sspIntVec m_nObjects;
	int	 m_nDuration;

	std::deque<sspPlayObject*> m_pObjects;	// sorted objects

    DECLARE_SERIAL( SSpSimultaneousGroup )

public:
	SSpSimultaneousGroup();
	SSpSimultaneousGroup(const SSpSimultaneousGroup& obj);
	SSpSimultaneousGroup& operator= (const SSpSimultaneousGroup& obj);
	virtual ~SSpSimultaneousGroup();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);
	bool update();
	void end();

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();

	void setPlayObjects(const sspIntVec& nObjects) {m_nObjects = nObjects;}
	const sspIntVec& getPlayObjects() const {return m_nObjects;}
};

class SSpSequentialGroup : public sspPlayObject  
{
private:
	sspIntVec			            m_nObjects;
  sspIntVec::const_iterator m_current;

	sspDSbuffer*	m_pBuffer;
	int					  m_nDuration;

  DECLARE_SERIAL( SSpSequentialGroup )

public:
	SSpSequentialGroup();
	SSpSequentialGroup(const SSpSequentialGroup& obj);
	SSpSequentialGroup& operator= (const SSpSequentialGroup& obj);
	virtual ~SSpSequentialGroup();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);
	bool update();
	void end();

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();

	void setPlayObjects(const sspIntVec& nObjects) {m_nObjects = nObjects;}
	const sspIntVec& getPlayObjects() const {return m_nObjects;}
};

class SSpLinearSelectGroup : public sspPlayObject  
{
private:
	sspIntVec m_nObjects;
	int	m_nValue;
	unsigned int m_nSelected;

    DECLARE_SERIAL( SSpLinearSelectGroup )

public:
	SSpLinearSelectGroup();
	SSpLinearSelectGroup(const SSpLinearSelectGroup& obj);
	SSpLinearSelectGroup& operator= (const SSpLinearSelectGroup& obj);
	virtual ~SSpLinearSelectGroup();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);
	void end();

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int	 estimateDuration();
	bool isPlaying();

	void setPlayObjects(const sspIntVec& nObjects) {m_nObjects = nObjects;}
	const sspIntVec& getPlayObjects() const {return m_nObjects;}

	void setValue(int nIndex) {m_nValue = nIndex;}
	int	 getValue() const {return m_nValue;}
};

class SSpRandomGroup : public sspPlayObject  
{
private:
	sspIntVec m_nObjects;
	sspIntVec m_nConstWeight;
	sspIntVec m_nVarWeight;

	int	m_nTotalWeight;
	int	m_nSelected;
	int m_nElementCount;

    DECLARE_SERIAL( SSpRandomGroup )

public:
	SSpRandomGroup();
	SSpRandomGroup(const SSpRandomGroup& obj);
	SSpRandomGroup& operator= (const SSpRandomGroup& obj);
	virtual ~SSpRandomGroup();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);
	void end();

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();
	bool isPlaying();

	void setPlayObjects(const sspIntVec& nObjects) {m_nObjects = nObjects;}
	void setConstWeight(const sspIntVec& nWeights) {m_nConstWeight = nWeights;}
	void setVariableWeight(const sspIntVec& nWeights) {m_nVarWeight = nWeights;}

	const sspIntVec& getPlayObjects() const {return m_nObjects;}
	const sspIntVec& getConstWeight() const {return m_nConstWeight;}
	const sspIntVec& getVariableWeight() const {return m_nVarWeight;}
};

class SSpChainGroup : public sspPlayObject  
{
private:
	sspIntVec m_nObjects;
	sspIntVec m_nFollowers;

	int	m_nCircleLength;
	int	m_nCircleCount;
	int	m_nSelectIndex;

    DECLARE_SERIAL( SSpChainGroup )

public:
	SSpChainGroup();
	SSpChainGroup(const SSpChainGroup& obj);
	SSpChainGroup& operator= (const SSpChainGroup& obj);
	virtual ~SSpChainGroup();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);
	void end();

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();
	bool isPlaying();

	void setPlayObjects(const sspIntVec& nObjects) {m_nObjects = nObjects;}
	void setFollowers(const sspIntVec& nObjects) {m_nFollowers = nObjects;}
	void setChainLength(int nLength) {m_nCircleLength = nLength;}

	const sspIntVec& getPlayObjects() const {return m_nObjects;}
	const sspIntVec& getFollowers() const {return m_nFollowers;}
	int  getChainLength() {return m_nCircleLength;}
};

class SSpConditionalGroup : public sspPlayObject  
{
private:
	sspIntVec m_nObjects;
	sspIntVec m_nConditionals;

	int m_nDefaultObject;
	int	m_nSelected;

    DECLARE_SERIAL( SSpConditionalGroup )

public:
	SSpConditionalGroup();
	SSpConditionalGroup(const SSpConditionalGroup& obj);
	SSpConditionalGroup& operator= (const SSpConditionalGroup& obj);
	virtual ~SSpConditionalGroup();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);
	void end();

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int  estimateDuration();
	bool isPlaying();

	void setPlayObjects(const sspIntVec& nObjects) {m_nObjects = nObjects;}
	void setConditionals(const sspIntVec& nConds) {m_nConditionals = nConds;}
	void setDefaultObject(int nIndex) {m_nDefaultObject = nIndex;}

	const sspIntVec& getPlayObjects() const {return m_nObjects;}
	const sspIntVec& getConditionals() const {return m_nConditionals;}
	int	 getDefaultObject() {return m_nDefaultObject;}
};

class SSpDistributionObject : public sspPlayObject  
{
private:
	int	m_nObject;
	int	m_nStartInterval;
	int	m_nEndInterval;
	int	m_nLoopCondition;
	int m_nLoopMode;	
	int	m_nLoopDuration;	// Depending on mode: count or time

	sspSilenceObject*		m_silence;
	bool				m_bSilence;
	time_t				m_tStart;
	sspDSbuffer*	m_pBuffer;
	int					m_nLoopCounter;

    DECLARE_SERIAL( SSpDistributionObject )

public:
	enum {LOOP_DURATION, LOOP_COUNT, LOOP_CONDITION};

	SSpDistributionObject();
	SSpDistributionObject(const SSpDistributionObject& obj);
	SSpDistributionObject& operator= (const SSpDistributionObject& obj);
	virtual ~SSpDistributionObject();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);
	bool update();
	void end();

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int estimateDuration();

	void setPlayObject(int nObject) {m_nObject = nObject;}
	void setStartInterval(int nStart) {m_nStartInterval = nStart;}
	void setEndInterval(int nEnd) {m_nEndInterval = (nEnd >= 0) ? nEnd : m_nStartInterval;}
	void SetLoopMode(int nMode) {m_nLoopMode = nMode;}
	void setLoopCondition(int nCondition) {m_nLoopCondition = nCondition;}
	void setLoopDuration(int nDurCount) {m_nLoopDuration = nDurCount;}

	int getPlayObject() const {return m_nObject;}
	int getStartInterval() const {return m_nStartInterval;}
	int getEndInterval() const {return m_nEndInterval;}
	int getLoopMode() const {return m_nLoopMode;}
	int getLoopCondition() const {return m_nLoopCondition;}
	int getLoopDuration() const {return m_nLoopDuration;}
};

class SSpStateObject : public sspPlayObject  
{
private:
	int m_nGlobalState;
	int	m_nInputValue;
  int m_nMode;

  DECLARE_SERIAL( SSpStateObject )

public:
  enum {STATE_TYPE_INCREMENT, STATE_TYPE_DECREMENT, STATE_TYPE_SETVALUE};

	SSpStateObject();
	SSpStateObject(const SSpStateObject& obj);
	SSpStateObject& operator= (const SSpStateObject& obj);
	virtual ~SSpStateObject();
	void Serialize(CArchive& ar);

	bool initialize();
	bool begin(sspDSbuffer* pBuf, sspMsgHandler* pOwner);

	// Virtual methods
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	int	 estimateDuration();

	void setMode(int nMode) {m_nMode = nMode;}
	int	 getMode() const {return m_nMode;}

	void setGlobalState(int nIndex) {m_nGlobalState = nIndex;}
	int	 getGlobalState() const {return m_nGlobalState;}

	void setInputValue(int nIndex) {m_nInputValue = nIndex;}
	int	 getInputValue() const {return m_nInputValue;}
};

#endif // !defined(AFX_SSPSOUNDOBJECTS_H__99543961_9202_11D3_901C_F98F7F995B9F__INCLUDED_)
