// sspICPinput.cpp: implementation of the sspICPinput classes.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "soundspace.h"
#include "sspICPinput.h"
#include "SSpValues.h"
#include "SSpConditionals.h"
#include "sspPools.h"

#include <i7000.h>
#include <uart.h>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( sspICPanalogInput, CObject, 1 )
IMPLEMENT_SERIAL( sspICPdigitalInput, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// sspICPinput
//////////////////////////////////////////////////////////////////////

vector<unsigned short> sspICPinput::s_ports;

// Privately scoped enumeration
enum {BAUDRATE=9600L, DATABITS=8, STOPBIT=0, PARITY=0, ESC_KEY=27, TIMEOUT_DELAY=100};
enum IpcCommandSlots {IPC_COMPORT, IPC_ADDRESS, IPC_ID, IPC_CHECKSUM, IPC_TIMEOUT, IPC_CHANNEL, IPC_DEBUGINFO};

sspICPinput::sspICPinput(short nType) 
: sspInput(nType)
, m_nPort(DEFAULT_PORT)
, m_nAddress(1)
, m_nChannel(0)
{
  memset(m_command, 0, sizeof(m_command));
  m_command[IPC_TIMEOUT] = TIMEOUT_DELAY;
}

bool sspICPinput::initCom()
{
  m_command[IPC_COMPORT] = m_nPort;
  m_command[IPC_ADDRESS] = m_nAddress;
  m_command[IPC_CHANNEL] = m_nChannel;

	for (unsigned int i=0; i<s_ports.size(); ++i) {
		if (m_nPort == s_ports[i])
			return true;              // This port is already initialized
	}
	s_ports.push_back(m_nPort);		// Add the port to the initialized list

  unsigned char nPort = (unsigned char) m_nPort;
  return (Open_Com(nPort,BAUDRATE,DATABITS,PARITY,STOPBIT) == NoError);
}

sspICPinput::sspICPinput(const sspICPinput& inp) : sspInput(inp)
{
	m_nPort = inp.m_nPort;
	m_nAddress = inp.m_nAddress;
	m_nChannel = inp.m_nChannel;
}

sspICPinput& sspICPinput::operator= (const sspICPinput& inp)
{
	sspInput::operator=(inp);
	m_nPort = inp.m_nPort;
	m_nAddress = inp.m_nAddress;
	m_nChannel = inp.m_nChannel;
	return *this;
}

void sspICPinput::Serialize(CArchive& ar)
{
	sspInput::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nPort << m_nAddress << m_nChannel;
  else {
		ar >> m_nPort >> m_nAddress >> m_nChannel;
    if (m_nPort > 5) m_nPort = 1; // Converting from address to COM-number
  }
}

void sspICPinput::printASCII(sspOutStream& outStr)
{
	sspInput::printASCII(outStr);
	outStr << endl << "   - m_nPort: " << m_nPort << ", m_nAddress: " << m_nAddress << ", m_nChannel: " << m_nChannel;
}

bool sspICPinput::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
	if (!sspInput::verify(outStr, nErrors, nWarnings))
		bRet = false;
	if (m_nPort != DEFAULT_PORT) {
		printWarning(outStr, _T("(sspICPinput): m_nPort is non-standard"), nWarnings);
		bRet = false;
	}
	if (m_nChannel > 15) {
		printError(outStr, _T("(sspICPinput): m_nChannel is larger than 15"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool sspICPinput::initialize(LPVOID pWnd)
{
  return initCom() ? sspInput::initialize(pWnd) : false;
}

bool sspICPinput::terminate()
{
  unsigned char nPort = (unsigned char) m_nPort;
  return (Close_Com(nPort) == NoError);
}


//////////////////////////////////////////////////////////////////////
// sspICPanalogInput
//////////////////////////////////////////////////////////////////////

sspICPanalogInput::sspICPanalogInput() : sspICPinput(SSP_ICPCON_ANALOG)
{
	m_nValueIndex = -1;
  m_command[IPC_ID] = 0x7017; // Analog module
}

sspICPanalogInput::sspICPanalogInput(const sspICPanalogInput& inp) : sspICPinput(inp)
{
	m_nValueIndex = inp.m_nValueIndex;
  m_command[IPC_ID] = 0x7017; // Analog module
}

sspICPanalogInput& sspICPanalogInput::operator= (const sspICPanalogInput& inp)
{
	sspICPinput::operator=(inp);
	m_nValueIndex = inp.m_nValueIndex;
	return *this;
}

sspICPanalogInput::~sspICPanalogInput()
{
	end();
}

void sspICPanalogInput::Serialize(CArchive& ar)
{
	sspICPinput::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nValueIndex;
	else
		ar >> m_nValueIndex;
}

void sspICPanalogInput::printASCII(sspOutStream& outStr)
{
	outStr << endl << ": sspICPanalogInput " << m_strName;
	sspICPinput::printASCII(outStr);
	outStr << endl << "   - m_nValueIndex: " << sspPool::Instance().values.GetName(m_nValueIndex);
}

bool sspICPanalogInput::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
	if (!sspICPinput::verify(outStr, nErrors, nWarnings))
		bRet = false;
	if (m_nValueIndex < 0 || m_nValueIndex >= (int) sspPool::Instance().values.GetSize()) {
		printError(outStr, _T("(sspICPanalogInput): m_nValueIndex is not valid"), nErrors);
		bRet = false;
	}
	else if (sspPool::Instance().values.getValue(m_nValueIndex)->getType() != SSP_VALUE_SINGLE) {
		printError(outStr, _T("(sspICPanalogInput): m_nValueIndex must be of basic type"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool sspICPanalogInput::initialize(LPVOID pWnd)
{
	m_pValue = dynamic_cast<SSpBasicValue*> (sspPool::Instance().values.getValue(m_nValueIndex));
	if (!m_pValue) return false;
	return sspICPinput::initialize(pWnd);
}

bool sspICPanalogInput::update()
{
	if (!sspInput::update())
		return false;

  float fVal;
  if (AnalogIn(m_command, &fVal, NULL, NULL) != NoError)
    return false;

  bool bRtn = fVal != m_pValue->getValue();
	m_pValue->setValue(fVal);
	return bRtn;
}

//////////////////////////////////////////////////////////////////////
// sspICPdigitalInput
//////////////////////////////////////////////////////////////////////

sspICPdigitalInput::sspICPdigitalInput() : sspICPinput(SSP_ICPCON_DIGITAL)
{
	m_nCondIndex = -1;
  m_command[IPC_ID] = 0x7052; // Digital module
}

sspICPdigitalInput::sspICPdigitalInput(const sspICPdigitalInput& inp) : sspICPinput(inp)
{
	m_nCondIndex = inp.m_nCondIndex;
  m_command[IPC_ID] = 0x7052; // Digital module
}

sspICPdigitalInput& sspICPdigitalInput::operator= (const sspICPdigitalInput& inp)
{
	sspICPinput::operator=(inp);
	m_nCondIndex = inp.m_nCondIndex;
	return *this;
}

sspICPdigitalInput::~sspICPdigitalInput()
{
	end();
}

void sspICPdigitalInput::Serialize(CArchive& ar)
{
	sspICPinput::Serialize(ar);
	if (ar.IsStoring())
		ar << m_nCondIndex;
	else
		ar >> m_nCondIndex;
}

void sspICPdigitalInput::printASCII(sspOutStream& outStr)
{
	outStr << endl << ": sspICPdigitalInput " << m_strName;
	sspICPinput::printASCII(outStr);
	outStr << endl << "   - m_nCondIndex: " << sspPool::Instance().conditionals.GetName(m_nCondIndex);
}

bool sspICPdigitalInput::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
	if (!sspICPinput::verify(outStr, nErrors, nWarnings))
		bRet = false;
	if (m_nCondIndex < 0 || m_nCondIndex >= (int) sspPool::Instance().conditionals.GetSize()) {
		printError(outStr, _T("(sspICPdigitalInput): m_nCondIndex is not valid"), nErrors);
		bRet = false;
	}
	else if (sspPool::Instance().conditionals.GetConditional(m_nCondIndex)->getType() != SSP_COND_BOOLEAN) {
		printError(outStr, _T("(sspICPdigitalInput): m_nCondIndex must be of basic type"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool sspICPdigitalInput::initialize(LPVOID pWnd)
{
	m_pCond = dynamic_cast<SSpBoolean*> (sspPool::Instance().conditionals.GetConditional(m_nCondIndex));
	if (!m_pCond) return false;
	return sspICPinput::initialize(pWnd);
}

bool sspICPdigitalInput::update()
{
	if (!sspInput::update())
		return false;

  if (DigitalIn(m_command, NULL, NULL, NULL) != NoError)
    return false;

  bool bVal = (m_command[5] & (1 << m_nChannel)) != 0;
  bool bRtn = bVal ^ m_pCond->isTrue();
  m_pCond->setValue(bVal);
  return bRtn;
}
