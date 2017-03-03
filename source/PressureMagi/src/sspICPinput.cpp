// sspICPinput.cpp: implementation of the sspICPinput classes.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sspICPinput.h"

#include <i7000.h>
#include <uart.h>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// sspICPinput
//////////////////////////////////////////////////////////////////////

// Privately scoped enumeration
enum {BAUDRATE=9600L, DATABITS=8, STOPBIT=0, PARITY=0, ESC_KEY=27, TIMEOUT_DELAY=100};
enum IpcCommandSlots {IPC_COMPORT, IPC_ADDRESS, IPC_ID, IPC_CHECKSUM, IPC_TIMEOUT, IPC_CHANNEL, IPC_DEBUGINFO};

sspICPinput::sspICPinput() 
: m_nPort(DEFAULT_PORT)
, m_nAddress(1)
, m_nChannel(0)
{
  memset(m_command, 0, sizeof(m_command));
  m_command[IPC_ID] = 0x7017;	// Analog module
  m_command[IPC_TIMEOUT] = TIMEOUT_DELAY;
}

sspICPinput::sspICPinput(const sspICPinput& inp)
{
	m_nPort = inp.m_nPort;
	m_nAddress = inp.m_nAddress;
	m_nChannel = inp.m_nChannel;
}

sspICPinput& sspICPinput::operator= (const sspICPinput& inp)
{
	m_nPort = inp.m_nPort;
	m_nAddress = inp.m_nAddress;
	m_nChannel = inp.m_nChannel;
	return *this;
}

bool sspICPinput::initCom()
{
  m_command[IPC_COMPORT] = m_nPort;
  m_command[IPC_ADDRESS] = m_nAddress;
  m_command[IPC_CHANNEL] = m_nChannel;

  unsigned char nPort = (unsigned char) m_nPort;
  // Multiple tries
  for (int i=0; i < 10; i++) {
    if (Open_Com(nPort,BAUDRATE,DATABITS,PARITY,STOPBIT) == NoError)
      return true;
    Sleep(100);
  }
  return false;
}

bool sspICPinput::initialize()
{
  return initCom();
}

bool sspICPinput::getValue(float& fValue)
{
  return (AnalogIn(m_command, &fValue, NULL, NULL) == NoError);
}

bool sspICPinput::terminate()
{
  unsigned char nPort = (unsigned char) m_nPort;
  return (Close_Com(nPort) == NoError);
}
