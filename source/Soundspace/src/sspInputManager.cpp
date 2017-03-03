// sspInputManager.cpp: implementation of the sspInputManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "soundspace.h"
#include "sspInputManager.h"
#include "sspPools.h"
#include "sspValues.h"
#include "sspSetting.h"
#include <limits>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//IMPLEMENT_SERIAL( sspInputManager, CObject, 1 )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sspInputManager* sspInputManager::m_pInstance = NULL;
bool sspInputManager::m_bDestroyed = false;

sspInputManager::sspInputManager()
{
  m_bTriggered = false;
}

sspInputManager::sspInputManager(const sspInputManager&)
{
  m_bTriggered = false;
}

sspInputManager& sspInputManager::operator=(const sspInputManager&)
{
	return *this;
}

sspInputManager::~sspInputManager()
{
	m_pInstance = NULL;
	m_bDestroyed = true;
	clearContents();
}

sspInputManager& sspInputManager::Instance()
{
	if (!m_pInstance) {
		if (m_bDestroyed) {
			onDeadReference();
		}
		else {
			create();
		}
	}
	return *m_pInstance;
}

void sspInputManager::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		ar << m_inputs.size();
		for (size_t i=0; i<m_inputs.size(); ++i) {
			ar << m_inputs[i];
		}
		ar << m_nFaders.size();
		for (size_t i=0; i<m_nFaders.size(); i++) {
			ar << m_nFaders[i];
		}
	}
	else {
		size_t nSize;
		ar >> nSize;
		m_inputs.resize(nSize);
		for (size_t i=0; i<nSize; i++) {
			CObject* pObj;
			ar >> pObj;
			m_inputs[i] = (sspInput*) pObj;
		}
		ar >> nSize;
		m_nFaders.resize(nSize);
		for (size_t i=0; i<nSize; i++) {
			ar >> m_nFaders[i];
		}
    if (m_nFaders.size() > sspSetting::MAX_SLIDERS)
      m_nFaders.resize(sspSetting::MAX_SLIDERS);
    for (size_t i=0; i < m_nFaders.size(); ++i) {
		  SSpInputValueRange* pVal = dynamic_cast<SSpInputValueRange*>(sspPool::Instance().values.getValue(m_nFaders[i]));
		  if (pVal != NULL) pVal->setRelativeValue(sspSetting::sliderValue[i]);
    }
	}
}

void sspInputManager::printASCII(sspOutStream& outStr)
{
	outStr << endl << "sspInputManager";
	for (size_t i=0; i<m_inputs.size(); ++i)
		m_inputs[i]->printASCII(outStr);
}

bool sspInputManager::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (size_t i=0; i<m_inputs.size(); ++i) {
		if (!m_inputs[i]->verify(outStr, nErrors, nWarnings)) {
			bRet = false;
		}
	}
	return bRet;
}

bool sspInputManager::initialize(LPVOID pWnd)
{
	for (size_t i=0; i<m_inputs.size(); ++i) {
		if (!m_inputs[i]->initialize(pWnd))
			return false;
	}
	return true;
}

bool sspInputManager::start()
{
	for (size_t i=0; i<m_inputs.size(); ++i) {
		if (!m_inputs[i]->begin())
			return false;
	}
  m_bTriggered = false;
	return true;
}

bool sspInputManager::update()
{
  m_bUpdated = m_bTriggered;
  m_bTriggered = false;
	for (size_t i=0; i<m_inputs.size(); ++i) {
		if (m_inputs[i]->update()) 
			m_bUpdated = true;	// At least one input is updated
	}
	return m_bUpdated;
}

bool sspInputManager::stop()
{
	for (size_t i=0; i<m_inputs.size(); ++i) {
		if (!m_inputs[i]->end())
			return false;
	}
	return true;
}

bool sspInputManager::terminate()
{
	for (size_t i=0; i<m_inputs.size(); ++i) {
		if (!m_inputs[i]->terminate())
			return false;
	}
	return true;
}

void sspInputManager::clearContents()
{
	terminate();
	for (size_t i=0; i<m_inputs.size(); ++i)
		delete m_inputs[i];
	m_inputs.clear();
  m_nFaders.clear();
}

int sspInputManager::setInput(sspInput* pInput)
{
	m_inputs.push_back(pInput);
	return m_inputs.size() - 1;
}

void sspInputManager::removeInput(int nIndex)
{
	delete m_inputs[nIndex];
  m_inputs.erase(m_inputs.begin() + nIndex);
}

sspInput* sspInputManager::getInput(int nIndex) const
{
	return m_inputs[nIndex];
}

void sspInputManager::setFaderSelection(const sspIntVec& nFaders)
{
  m_nFaders = nFaders;
  if (m_nFaders.size() > sspSetting::MAX_SLIDERS)
    m_nFaders.resize(sspSetting::MAX_SLIDERS);
  for (size_t i=0; i < m_nFaders.size(); ++i) {
		SSpInputValueRange* pVal = dynamic_cast<SSpInputValueRange*>(sspPool::Instance().values.getValue(m_nFaders[i]));
		if (pVal != NULL) pVal->setRelativeValue(sspSetting::sliderValue[i]);
  }
}

void sspInputManager::setFaderValue(int nIndex, float fVal)
{
  if (nIndex < 0 || nIndex > (int) m_nFaders.size()) return;
	SSpInputValueRange* pVal = dynamic_cast<SSpInputValueRange*>(sspPool::Instance().values.getValue(m_nFaders[nIndex]));
  if (pVal != NULL) {
    pVal->setRelativeValue(fVal);
    sspSetting::sliderValue[nIndex] = fVal;
    m_bTriggered = true;
  }
}

const SSpInputValueRange* sspInputManager::getFader(int nIndex) const
{
  return nIndex < (int) m_nFaders.size() 
    ?  dynamic_cast<SSpInputValueRange*>(sspPool::Instance().values.getValue(m_nFaders[nIndex]))
    : NULL;
}

const sspString& sspInputManager::getName(int nIndex) const
{
	return m_inputs[nIndex]->getName();
}

size_t sspInputManager::getSize() const
{
	return m_inputs.size();
}

float sspInputManager::getMinUpdateInterval() const
{
  float fMin = numeric_limits<float>::max();
	for (size_t i=0; i<m_inputs.size(); ++i) {
    float fVal = m_inputs[i]->getUpdateInterval();
    if (fVal < fMin) fMin = fVal;
	}
	return fMin;
}

bool sspInputManager::isLoaded() const
{
	return (!m_inputs.empty());
}

void sspInputManager::create()
{
	static sspInputManager theInstance;
	m_pInstance = &theInstance;
}

void sspInputManager::onDeadReference()
{
	throw std::runtime_error("Dead reference to input pool detected");
}
