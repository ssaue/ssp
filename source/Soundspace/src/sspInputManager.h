// sspInputManager.h: interface for the sspInputManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSP_INPUTPOOL_H
#define SSP_INPUTPOOL_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include "sspInput.h"
#include <vector>

class SSpInputValueRange;

class sspInputManager : public sspIObase
{
protected:
	std::vector<sspInput*>	m_inputs;
	sspIntVec               m_nFaders;
  bool m_bTriggered;
  bool m_bUpdated;

private:
	static sspInputManager* m_pInstance;
	static bool m_bDestroyed;

public:
	static sspInputManager& Instance();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	bool initialize(LPVOID pWnd);
	bool start();
	bool update();
	bool stop();
	bool terminate();
	void clearContents();

	int setInput(sspInput* pInput);
	void removeInput(int nIndex);
	sspInput* getInput(int nIndex) const;

  void setFaderSelection(const sspIntVec& nFaders);
  void setFaderValue(int nIndex, float fVal);
  size_t getFaderSize() const {return m_nFaders.size();}
  const sspIntVec& getFaderSelection() const {return m_nFaders;}
  const SSpInputValueRange* getFader(int nIndex) const;

	const sspString& getName(int nIndex) const;
	size_t getSize() const;
  float getMinUpdateInterval() const;

	bool isLoaded() const;
  bool isUpdated() const {return m_bUpdated;}
  void setTriggered(bool bTrig = true) {m_bTriggered = bTrig;}

private:
	sspInputManager();
	sspInputManager(const sspInputManager&);
	sspInputManager& operator=(const sspInputManager&);
	virtual ~sspInputManager();

	static void create();
	static void onDeadReference();
};

#endif // !defined(AFX_SSPINPUTINTERFACE_H__81A5E0A0_3D8A_11D4_8D73_C9C4792DFF72__INCLUDED_)
