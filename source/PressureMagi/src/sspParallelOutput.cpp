// sspParallelOutput.cpp: implementation of the sspInputManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sspParallelOutput.h"

#include <inpout32drv.h>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ECP_PORT_OFFSET	0x400
#define DIRECTION_OUT	0x20

//////////////////////////////////////////////////////////////////////
// sspParallelOutput
//////////////////////////////////////////////////////////////////////

sspParallelOutput::sspParallelOutput()
: m_nPort(DEFAULT_PORT)
, m_nState(0)
{
}

sspParallelOutput::sspParallelOutput(const sspParallelOutput& inp) 
: m_nPort(inp.m_nPort)
, m_nState(inp.m_nState)
{
}

sspParallelOutput& sspParallelOutput::operator= (const sspParallelOutput& inp)
{
	m_nPort = inp.m_nPort;
	m_nState = inp.m_nState;
	return *this;
}

sspParallelOutput::~sspParallelOutput()
{
}

bool sspParallelOutput::initialize()
{
	// Assuming ECP parallel port, we first disable bidirectional dataregister
	unsigned int nECP = Inp32(m_nPort+ECP_PORT_OFFSET+2);
	nECP &= 0x1F; // Bits 5-7 are set to zero
	Out32(m_nPort+ECP_PORT_OFFSET+2, nECP);

	// Disable bidirectional port
	unsigned int nControl;
	nControl = Inp32(m_nPort+2);
	nControl &= ~DIRECTION_OUT;
	Out32(m_nPort+2, nControl);			// Disable bi-directional
	Out32(m_nPort, 0x00);				    // Set all data lines low
	Out32(m_nPort, 0x01);				    // Set all data lines low
	Out32(m_nPort, 0x00);				    // Set all data lines low
  m_nState = 0;

	return true;
}

bool sspParallelOutput::setBit(unsigned char bit, bool bOn)
{
  if (bit > 7)
    return false;

  unsigned int nMask = 0x01 << bit;
  unsigned int nNewState = bOn ? (m_nState | nMask) : (m_nState & ~nMask);
  if (nNewState != m_nState) {
	  Out32(m_nPort, nNewState);
    m_nState = nNewState;
  }
  return true;
}
