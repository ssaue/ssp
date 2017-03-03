// sspDevice.cpp

#include "stdafx.h"
#include "sspDevice.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// sspDeviceGroup Class
//////////////////////////////////////////////////////////////////////

sspDeviceGroup::sspDeviceGroup()
: m_bInitialized(false)
{
}

sspDeviceGroup::~sspDeviceGroup()
{
	m_nDevices.clear();
}

void sspDeviceGroup::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << m_nDevices.size();
		for (unsigned int i=0; i<m_nDevices.size(); ++i) {
			ar << m_nDevices[i];
		}
	}
	else {
		getAllDevices();
		unsigned int nBuf;
		ar >> nBuf;
		m_nDevices.resize(nBuf);
		for (unsigned int i=0; i<nBuf; i++)
			ar >> m_nDevices[i];
	}
}

void sspDeviceGroup::printASCII(sspOutStream& outStr)
{
	getAllDevices();
	outStr << endl << "- sspDeviceGroup " << m_strName;
	for (unsigned int i=0; i<m_nDevices.size(); ++i)
		outStr << endl << "  - " << getSubsetName(i);
}

bool sspDeviceGroup::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	getAllDevices();
	if (getAllDevices() < m_nDevices.size()) {
		printError(outStr, _T("(sspDeviceGroup): device does not exist"), nErrors);
		bRet = false;
	}
	if (m_nDevices.size() == 0) {
		printWarning(outStr, _T("(sspDeviceGroup): no devices selected"), nWarnings);
		bRet = false;
	}
	return bRet;
}

bool sspDeviceGroup::initialize(LPVOID hWnd)
{
  if (m_bInitialized) terminate();
  m_bInitialized = initializeImpl(hWnd);
  return m_bInitialized;
}

bool sspDeviceGroup::begin()
{
  return m_bInitialized && beginImpl();
}

sspString sspDeviceGroup::getSubsetName(int nSubsetIndex)
{
	if (nSubsetIndex < (int) m_nDevices.size())
		return getDeviceName(m_nDevices[nSubsetIndex]);
	else 
		return _T("");
}
