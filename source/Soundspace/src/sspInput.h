// sspInput.h: interface for the sspInput class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPINPUT_H
#define SSPINPUT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspObject.h"
#include <sspTime.h>

class sspInput : public sspObject
{
protected:
	float m_fUpdateInterval;
  sspTimeUpdate m_update;

public:
	sspInput(short nType);
	sspInput(const sspInput& inp);
	sspInput& operator= (const sspInput& inp);
	virtual ~sspInput();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	virtual bool initialize(LPVOID pWnd);
	virtual bool begin() = 0;
  virtual bool update() {return m_update.update();}
	virtual bool end() = 0;
	virtual bool terminate() = 0;

	void setUpdateInterval(float fSec) {m_fUpdateInterval = fSec;}
	float getUpdateInterval() const {return m_fUpdateInterval;}
};

#endif
