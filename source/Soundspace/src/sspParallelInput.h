// sspParallelInput.h: interface for the parallel input class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPPARALLELINPUT_H
#define SSPPARALLELINPUT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspInput.h"
#include <vector>

class SSpBoolean;

class sspParallelInput : public sspInput
{
public:
	enum {PARALLEL_INPUTS=13};

protected:
    DECLARE_SERIAL(sspParallelInput)
	unsigned short	m_nPort;
	int				m_nConds[PARALLEL_INPUTS];
	SSpBoolean*		m_pConds[PARALLEL_INPUTS];

	enum {DEFAULT_PORT=0x378};

public:
	sspParallelInput();
	sspParallelInput(const sspParallelInput& inp);
	sspParallelInput& operator= (const sspParallelInput& inp);
	virtual ~sspParallelInput();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	bool initialize(LPVOID pWnd);
	bool begin();
	bool update();
	bool end() {return true;}
	bool terminate() {return true;}

	void setPort(unsigned short nPort) {m_nPort = nPort;}
	void setConditional(int nInput, int nCond) {if (nInput < PARALLEL_INPUTS) m_nConds[nInput] = nCond;}

	unsigned short getPort() const {return m_nPort;}
	int getConditional(int nInput) const {return (nInput < PARALLEL_INPUTS) ? m_nConds[nInput] : -1;}
};

#endif
