// sspParallelOutput.h: interface for the parallel output class.
//
//////////////////////////////////////////////////////////////////////

#ifndef sspParallelOutput_H
#define sspParallelOutput_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class sspParallelOutput
{
private:
	enum {DEFAULT_PORT=0x378};
	unsigned short	m_nPort;
  unsigned int    m_nState;

public:
	sspParallelOutput();
	sspParallelOutput(const sspParallelOutput& inp);
	sspParallelOutput& operator= (const sspParallelOutput& inp);
	virtual ~sspParallelOutput();

	bool initialize();
	bool setBit(unsigned char bit, bool bOn);
	bool terminate() {return true;}

	void setPort(unsigned short nPort) {m_nPort = nPort;}
	unsigned short getPort() const {return m_nPort;}
};

#endif
