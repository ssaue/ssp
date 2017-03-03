// sspICPinput.h: interface for the sspInput class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPICPINPUT_H
#define SSPICPINPUT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspInput.h"
#include <vector>

class SSpBoolean;
class SSpBasicValue;

class sspICPinput : public sspInput
{
protected:
	enum {DEFAULT_PORT=1};

	static std::vector<unsigned short> s_ports;
  unsigned short m_command[8];

	unsigned short m_nPort;
	unsigned char	m_nChannel;
	unsigned char	m_nAddress;

public:
	sspICPinput(short nType);
	sspICPinput(const sspICPinput& inp);
	sspICPinput& operator= (const sspICPinput& inp);
	virtual ~sspICPinput() {}
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	bool initialize(LPVOID pWnd);
	bool begin() {return true;}
	bool end() {return true;}
	bool terminate();

	void setPort(unsigned short nPort) {m_nPort = nPort;}
	void setChannel(unsigned char nChannel) {m_nChannel = nChannel;}
	void setAddress(unsigned char nAddress) {m_nAddress = nAddress;}

	unsigned short getPort() const {return m_nPort;}
	unsigned char getChannel() const {return m_nChannel;}
	unsigned char getAddress() const {return m_nAddress;}

private:
	bool initCom();
};

class sspICPanalogInput : public sspICPinput
{
protected:
    DECLARE_SERIAL(sspICPanalogInput)
	int				m_nValueIndex;
	SSpBasicValue*	m_pValue;

public:
	sspICPanalogInput();
	sspICPanalogInput(const sspICPanalogInput& inp);
	sspICPanalogInput& operator= (const sspICPanalogInput& inp);
	virtual ~sspICPanalogInput();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	bool initialize(LPVOID pWnd);
	bool update();

	void setValue(int nValue) {m_nValueIndex = nValue;}
	int getValue() const {return m_nValueIndex;}
};


class sspICPdigitalInput : public sspICPinput
{
protected:
    DECLARE_SERIAL(sspICPdigitalInput)
	int			m_nCondIndex;
	SSpBoolean*	m_pCond;

public:
	sspICPdigitalInput();
	sspICPdigitalInput(const sspICPdigitalInput& inp);
	sspICPdigitalInput& operator= (const sspICPdigitalInput& inp);
	virtual ~sspICPdigitalInput();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	bool initialize(LPVOID pWnd);
	bool update();

	void setConditional(int nCond) {m_nCondIndex = nCond;}
	int getConditional() const {return m_nCondIndex;}
};

#endif
