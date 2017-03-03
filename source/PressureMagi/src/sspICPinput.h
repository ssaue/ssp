// sspICPinput.h: interface for the sspInput class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SSPICPINPUT_H
#define SSPICPINPUT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>

class sspICPinput
{
protected:
	enum {DEFAULT_PORT=1};

  unsigned short m_command[8];

	unsigned short m_nPort;
	unsigned char	 m_nChannel;
	unsigned char	 m_nAddress;

public:
	sspICPinput();
	sspICPinput(const sspICPinput& inp);
	sspICPinput& operator= (const sspICPinput& inp);
	virtual ~sspICPinput() {}

	bool initialize();
	bool getValue(float& fValue);
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

#endif
