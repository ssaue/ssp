//////////////////////////////////////////
// sspExecutiveManager.h

#ifndef SSP_EXECUTIVEMANAGER_H
#define SSP_EXECUTIVEMANAGER_H

#include "SSpObject.h"
#include "sspPlayManager.h"
#include <sspTimerInterface.h>

class sspTimer;

// Class sspExecutiveManager is implemented as a Singleton pattern
class sspExecutiveManager : public sspIObase, public sspTimerObject
{
protected:
  // Configurable member variables
  tShutdownProc m_nStopProc;
  tStartupProc  m_nStartProc;

  // Internal variables
  sspTimer*     m_pTimer;
  bool          m_bRunning;
  tErrorNumber  m_nError;

private:
	static sspExecutiveManager* m_pInstance;
	static bool m_bDestroyed;

public:
	static sspExecutiveManager& Instance();

 	void Serialize(CArchive& ar);
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
  void clearContents();

	bool initialize(LPVOID hWnd);
	bool start();
	bool stop();
	bool terminate();

  // Virtual methods from sspTimerObject
	bool begin(sspTimerType& nMSecs);
	bool update(sspTimerType& nMSecs);
	void end();

  void setShutdownProcedure(tShutdownProc nProc) {m_nStopProc = nProc;}
  void setStartupProcedure(tStartupProc nProc) {m_nStartProc = nProc;}

  tShutdownProc getShutdownProcedure() const {return m_nStopProc;}
  tStartupProc getStartupProcedure() const {return m_nStartProc;}
  tErrorNumber getErrorNumber() const {return m_nError;}

  bool isRunning() const {return m_bRunning;}
  bool isPlaying() const {return sspPlayManager::Instance().isPlaying();}

private:
	sspExecutiveManager();
	sspExecutiveManager(const sspExecutiveManager&);
	sspExecutiveManager& operator=(const sspExecutiveManager&);
	~sspExecutiveManager();

	static void create();
	static void onDeadReference();
};

#endif