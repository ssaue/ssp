// sspParallelInput.cpp: implementation of the sspInputManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "soundspace.h"
#include "sspParallelInput.h"
#include "SSpConditionals.h"
#include "sspPools.h"

#include <inpout32drv.h>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ECP_PORT_OFFSET	0x400
#define DIRECTION_OUT	0x20

IMPLEMENT_SERIAL( sspParallelInput, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// sspParallelInput
//////////////////////////////////////////////////////////////////////

sspParallelInput::sspParallelInput() : sspInput(SSP_PARALLEL)
{
	m_nPort = DEFAULT_PORT;
	for (int i=0; i<PARALLEL_INPUTS; ++i) {
		m_nConds[i] = -1;
		m_pConds[i] = NULL;
	}
}

sspParallelInput::sspParallelInput(const sspParallelInput& inp) : sspInput(inp)
{
	m_nPort = inp.m_nPort;
	for (int i=0; i<PARALLEL_INPUTS; ++i) {
		m_nConds[i] = inp.m_nConds[i];
	}
}

sspParallelInput& sspParallelInput::operator= (const sspParallelInput& inp)
{
	sspInput::operator=(inp);
	m_nPort = inp.m_nPort;
	for (int i=0; i<PARALLEL_INPUTS; ++i) {
		m_nConds[i] = inp.m_nConds[i];
	}
	return *this;
}

sspParallelInput::~sspParallelInput()
{
	end();
}

void sspParallelInput::Serialize(CArchive& ar)
{
	sspInput::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nPort;
		for (int i=0; i<PARALLEL_INPUTS; ++i) {
			ar << m_nConds[i];
		}
	}
	else {
		ar >> m_nPort;
		for (int i=0; i<PARALLEL_INPUTS; ++i) {
			ar >> m_nConds[i];
		}
	}
}

void sspParallelInput::printASCII(sspOutStream& outStr)
{
	outStr << endl << ": sspParallelInput " << m_strName;
	sspInput::printASCII(outStr);
	outStr << endl << "   - m_nPort: " << m_nPort;
	for (int i=0; i<PARALLEL_INPUTS; ++i) {
		outStr << endl << "   - conditional " << i << ": " << sspPool::Instance().conditionals.GetName(m_nConds[i]);
		if (m_nConds[i] >= 0)
			outStr << sspPool::Instance().conditionals.GetName(m_nConds[i]);
	}
}

bool sspParallelInput::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
	if (!sspInput::verify(outStr, nErrors, nWarnings))
		bRet = false;
	if (m_nPort != DEFAULT_PORT) {
		printWarning(outStr, _T("(sspParallelInput): m_nPort is non-standard"), nWarnings);
		bRet = false;
	}
	for (int i=0; i<PARALLEL_INPUTS; ++i) {
		if (m_nConds[i] >= 0) {
			if (m_nConds[i] < 0 || m_nConds[i] > (int) sspPool::Instance().conditionals.GetSize()) {
				printError(outStr, _T("(sspParallelInput): a conditional index is not valid"), nErrors);
				bRet = false;
			}
			else if (sspPool::Instance().conditionals.GetConditional(m_nConds[i])->getType() != SSP_COND_BOOLEAN) {
				printError(outStr, _T("(sspParallelInput): conditionals must be of boolean type"), nErrors);
				bRet = false;
			}
		}
	}
	return bRet;
}

bool sspParallelInput::initialize(LPVOID pWnd)
{
	for (int i=0; i<PARALLEL_INPUTS; ++i) {
		if (m_nConds[i] >= 0) {
            m_pConds[i] = dynamic_cast<SSpBoolean*> (sspPool::Instance().conditionals.GetConditional(m_nConds[i]));
			if (!m_pConds[i]) return false;
		}
		else
			m_pConds[i] = NULL;
	}
	return sspInput::initialize(pWnd);
}

bool sspParallelInput::begin()
{
	// Assuming ECP parallel port, we first enable bidirectional dataregister (mode 001)
	unsigned int nECP = Inp32(m_nPort+ECP_PORT_OFFSET+2);
	nECP &= 0x1F;
	nECP |= 0x20;
	Out32(m_nPort+ECP_PORT_OFFSET+2, nECP);

	// Setter dataporten høy og deretter som input
	unsigned int nControl;
	nControl = Inp32(m_nPort+2);
	nControl &= ~DIRECTION_OUT;
	Out32(m_nPort+2, nControl);			// Data output
	nControl &= ~0x03;
	Out32(m_nPort+2, nControl);			// Reset control register

	unsigned int nStatus;
	nStatus = Inp32(m_nPort+1);
	nStatus &= ~0x01;					// Zero timeout bit (EPP)
	Out32(m_nPort+1, nStatus);			// Data output

	Out32(m_nPort, 0xFF);				// Set all data lines high
	nControl |= DIRECTION_OUT;
	Out32(m_nPort+2, nControl);			// Data input

	return true;
}

bool sspParallelInput::update()
{
	if (!sspInput::update())
		return false;

	// Setter dataporten høy og deretter som input
	unsigned int nControl = Inp32(m_nPort+2);
	nControl &= ~DIRECTION_OUT;
	Out32(m_nPort+2, nControl);			// Data output
	nControl &= ~0x03;
	Out32(m_nPort+2, nControl);			// Reset control register

	unsigned int nStatus = Inp32(m_nPort+1);
	nStatus &= ~0x01;					// Zero timeout bit (EPP)
	Out32(m_nPort+1, nStatus);			// Data output

	Out32(m_nPort, 0xFF);				// Set all data lines high
	nControl |= DIRECTION_OUT;
	Out32(m_nPort+2, nControl);			// Data input

	// Leser parallellporten
	unsigned int nData = Inp32(m_nPort);	// the data port
	nStatus = ((Inp32(m_nPort+1)^0x80) >> 3);	// the status port
	nData |= (nStatus << 8);								// adding together	

  bool bRtn = false;
	// Tilordner verdiene 0.0 og 1.0
	for (int i=0; i<PARALLEL_INPUTS; ++i) {
    if (m_pConds[i]) {
      bool bVal = (nData & (1 << i)) != 0;
      if (bVal ^ m_pConds[i]->isTrue()) bRtn = true;
      m_pConds[i]->setValue(bVal);
    }
	}
	return bRtn;
}
