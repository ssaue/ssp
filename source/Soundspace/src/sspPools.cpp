// sspPools.cpp: implementation of the SSpValuePool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sspPools.h"
#include "SSpBasic.h"
#include "SSpTasks.h"
#include "SSpStream.h"
#include "SSpValues.h"
#include "sspConditionals.h"
#include "sspStringObjects.h"

#include <atlsimpstr.h>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// SSpValuePool
//////////////////////////////////////////////////////////////////////

SSpValuePool::SSpValuePool()
{
}

SSpValuePool::~SSpValuePool()
{
	clearContents();
}

void SSpValuePool::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_arValues.Serialize(ar);
	m_arNames.Serialize(ar);
	// SERIALIZE FIX
	if (ar.IsLoading()) {
		for (int i=0; i<m_arValues.GetSize(); ++i) {
			m_arValues[i]->setIndex(i);
			m_arValues[i]->setName(m_arNames[i].GetBuffer());
		}
	}
}

void SSpValuePool::printASCII(sspOutStream& outStr)
{
	outStr << endl << "SSpValuePool";
	for (int i=0; i<m_arValues.GetSize(); ++i) {
		m_arValues[i]->printASCII(outStr);
	}
}

bool SSpValuePool::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (int i=0; i<m_arValues.GetSize(); ++i) {
		if (!m_arValues[i]->verify(outStr, nErrors, nWarnings)) {
			bRet = false;
		}
	}
	if (m_arValues.GetSize() == 0) {
		printError(outStr, _T("(SSpValuePool): no values defined"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpValuePool::initialize()
{
	return true;
}

void SSpValuePool::initContents()
{
	SSpBasicValue* pVal = new SSpBasicValue;
	pVal->setValue(0.0f);
	LoadValue(pVal, _T("0"));
	pVal = new SSpBasicValue;
	pVal->setValue(1.0f);
	LoadValue(pVal, _T("1"));
}

void SSpValuePool::clearContents()
{
	for (int i=0; i<m_arValues.GetSize(); i++)
		delete m_arValues[i];
	m_arValues.RemoveAll();
	m_arNames.RemoveAll();
}

int SSpValuePool::LoadValue(sspValue* pValue, LPCTSTR strName)
{
	// SERIALIZE FIX
	int nIndex = m_arValues.Add(pValue);
	pValue->setIndex(nIndex);
	pValue->setName(strName);
	return m_arNames.Add(strName);
}

void SSpValuePool::ExchangeValue(sspValue* pValue, LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	delete m_arValues[nIndex];
	m_arValues[nIndex] = pValue;
	pValue->setIndex(nIndex);
	pValue->setName(strName);
	m_arNames[nIndex] = strName;
}

void SSpValuePool::ExchangeValueName(LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	m_arNames[nIndex] = strName;
	m_arValues[nIndex]->setName(strName);
}

sspValue* SSpValuePool::getValue(int nIndex) const
{
	return m_arValues[nIndex];
}

LPCTSTR SSpValuePool::GetName(int nIndex) const
{
	return (LPCTSTR) m_arNames[nIndex];
}

unsigned int SSpValuePool::GetSize() const
{
	return m_arValues.GetSize();
}

BOOL SSpValuePool::IsLoaded() const
{
	return m_arValues.GetSize() > 0;
}

//////////////////////////////////////////////////////////////////////
// SSpConditionalPool
//////////////////////////////////////////////////////////////////////

SSpConditionalPool::SSpConditionalPool()
{
}

SSpConditionalPool::~SSpConditionalPool()
{
	clearContents();
}

void SSpConditionalPool::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_arConditionals.Serialize(ar);
	m_arNames.Serialize(ar);
	// SERIALIZE FIX
	if (ar.IsLoading()) {
		for (int i=0; i<m_arConditionals.GetSize(); ++i) {
			m_arConditionals[i]->setIndex(i);
			m_arConditionals[i]->setName(m_arNames[i].GetBuffer());
		}
	}
}

void SSpConditionalPool::printASCII(sspOutStream& outStr)
{
	outStr << endl << "SSpConditionalPool";
	for (int i=0; i<m_arConditionals.GetSize(); ++i) {
		m_arConditionals[i]->printASCII(outStr);
	}
}

bool SSpConditionalPool::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (int i=0; i<m_arConditionals.GetSize(); ++i) {
		if (!m_arConditionals[i]->verify(outStr, nErrors, nWarnings)) {
			bRet = false;
		}
	}
	if (m_arConditionals.GetSize() == 0) {
		printError(outStr, _T("(SSpConditionalPool): no conditionals defined"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool SSpConditionalPool::initialize()
{
	return true;
}

void SSpConditionalPool::initContents()
{
	SSpBoolean* pBool = new SSpBoolean;
	pBool->setValue(FALSE);
	LoadConditional(pBool, _T("False"));
	pBool = new SSpBoolean;
	pBool->setValue(TRUE);
	LoadConditional(pBool, _T("True"));
}

void SSpConditionalPool::clearContents()
{
	for (int i=0; i<m_arConditionals.GetSize(); i++)
		delete m_arConditionals[i];
	m_arConditionals.RemoveAll();
	m_arNames.RemoveAll();
}

int SSpConditionalPool::LoadConditional(sspConditional* pConditional, LPCTSTR strName)
{
	// SERIALIZE FIX
	int nIndex = m_arConditionals.Add(pConditional);
	pConditional->setIndex(nIndex);
	pConditional->setName(strName);
	return m_arNames.Add(strName);
}

void SSpConditionalPool::ExchangeConditional(sspConditional* pConditional, LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	delete m_arConditionals[nIndex];
	m_arConditionals[nIndex] = pConditional;
	pConditional->setIndex(nIndex);
	pConditional->setName(strName);
	m_arNames[nIndex] = strName;
}

void SSpConditionalPool::ExchangeConditionalName(LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	m_arConditionals[nIndex]->setName(strName);
	m_arNames[nIndex] = strName;
}

sspConditional* SSpConditionalPool::GetConditional(int nIndex) const
{
	return m_arConditionals[nIndex];
}

LPCTSTR SSpConditionalPool::GetName(int nIndex) const
{
	return m_arNames[nIndex];
}

unsigned int SSpConditionalPool::GetSize() const
{
	return m_arConditionals.GetSize();
}

BOOL SSpConditionalPool::IsLoaded() const
{
	return m_arConditionals.GetSize() > 0;
}

//////////////////////////////////////////////////////////////////////
// sspStringPool
//////////////////////////////////////////////////////////////////////

sspStringPool::sspStringPool()
{
}

sspStringPool::~sspStringPool()
{
	clearContents();
}

void sspStringPool::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_arStrings.Serialize(ar);
	m_arNames.Serialize(ar);
	// SERIALIZE FIX
	if (ar.IsLoading()) {
		for (int i=0; i<m_arStrings.GetSize(); ++i) {
			m_arStrings[i]->setIndex(i);
			m_arStrings[i]->setName(m_arNames[i].GetBuffer());
		}
	}
}

void sspStringPool::printASCII(sspOutStream& outStr)
{
	outStr << endl << "sspStringPool";
	for (int i=0; i<m_arStrings.GetSize(); ++i) {
		m_arStrings[i]->printASCII(outStr);
	}
}

bool sspStringPool::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (int i=0; i<m_arStrings.GetSize(); ++i) {
		if (!m_arStrings[i]->verify(outStr, nErrors, nWarnings)) {
			bRet = false;
		}
	}
	if (m_arStrings.GetSize() == 0) {
		printError(outStr, _T("(sspStringPool): no values defined"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool sspStringPool::initialize()
{
	return true;
}

void sspStringPool::initContents()
{
	sspSimpleString* pString = new sspSimpleString;
	pString->setString(_T("*.*"));
	LoadString(pString, _T("*.*"));
}

void sspStringPool::clearContents()
{
	for (int i=0; i<m_arStrings.GetSize(); i++)
		delete m_arStrings[i];
	m_arStrings.RemoveAll();
	m_arNames.RemoveAll();
}

int sspStringPool::LoadString(sspStringObject* pString, LPCTSTR strName)
{
	// SERIALIZE FIX
	int nIndex = m_arStrings.Add(pString);
	pString->setIndex(nIndex);
	pString->setName(strName);
	return m_arNames.Add(strName);
}

void sspStringPool::ExchangeString(sspStringObject* pString, LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	delete m_arStrings[nIndex];
	m_arStrings[nIndex] = pString;
	pString->setIndex(nIndex);
	pString->setName(strName);
	m_arNames[nIndex] = strName;
}

void sspStringPool::ExchangeStringName(LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	m_arNames[nIndex] = strName;
	m_arStrings[nIndex]->setName(strName);
}

sspStringObject* sspStringPool::getString(int nIndex) const
{
	return m_arStrings[nIndex];
}

LPCTSTR sspStringPool::GetName(int nIndex) const
{
	return (LPCTSTR) m_arNames[nIndex];
}

unsigned int sspStringPool::GetSize() const
{
	return m_arStrings.GetSize();
}

BOOL sspStringPool::IsLoaded() const
{
	return m_arStrings.GetSize() > 0;
}

//////////////////////////////////////////////////////////////////////
// SSpPlayObjectPool
//////////////////////////////////////////////////////////////////////

SSpPlayObjectPool::SSpPlayObjectPool()
{
}

SSpPlayObjectPool::~SSpPlayObjectPool()
{
	clearContents();
}

void SSpPlayObjectPool::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_arPlayObjects.Serialize(ar);
	m_arNames.Serialize(ar);
	// SERIALIZE FIX
	if (ar.IsLoading()) {
		for (int i=0; i<m_arPlayObjects.GetSize(); ++i) {
			m_arPlayObjects[i]->setIndex(i);
			m_arPlayObjects[i]->setName(m_arNames[i].GetBuffer());
		}
	}
}

void SSpPlayObjectPool::printASCII(sspOutStream& outStr)
{
	outStr << endl << "SSpPlayObjectPool";
	for (int i=0; i<m_arPlayObjects.GetSize(); ++i) {
		m_arPlayObjects[i]->printASCII(outStr);
	}
}

bool SSpPlayObjectPool::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (int i=0; i<m_arPlayObjects.GetSize(); ++i) {
		if (!m_arPlayObjects[i]->verify(outStr, nErrors, nWarnings)) {
			bRet = false;
		}
	}
	if (m_arPlayObjects.GetSize() == 0) {
		printError(outStr, _T("(SSpPlayObjectPool): no play objects defined"), nErrors);
		bRet = false;
	}
	return bRet;
}

void SSpPlayObjectPool::initContents()
{
}

void SSpPlayObjectPool::clearContents()
{
	for (int i=0; i<m_arPlayObjects.GetSize(); i++)
		delete m_arPlayObjects[i];
	m_arPlayObjects.RemoveAll();
	m_arNames.RemoveAll();
}

bool SSpPlayObjectPool::initialize()
{
	for (int i=0; i<m_arPlayObjects.GetSize(); i++) {
		if (!m_arPlayObjects[i]->initialize())
			return false;
	}
	return true;
}

int SSpPlayObjectPool::LoadPlayObject(sspPlayObject* pPlayObject, LPCTSTR strName)
{
	// SERIALIZE FIX
	int nIndex = m_arPlayObjects.Add(pPlayObject);
	pPlayObject->setIndex(nIndex);
	pPlayObject->setName(strName);
	return m_arNames.Add(strName);
}

void SSpPlayObjectPool::ExchangePlayObject(sspPlayObject* pPlayObject, LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	delete m_arPlayObjects[nIndex];
	m_arPlayObjects[nIndex] = pPlayObject;
	pPlayObject->setIndex(nIndex);
	pPlayObject->setName(strName);
	m_arNames[nIndex] = strName;
}

void SSpPlayObjectPool::ExchangeObjectName(LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	m_arPlayObjects[nIndex]->setName(strName);
	m_arNames[nIndex] = strName;
}

sspPlayObject* SSpPlayObjectPool::GetPlayObject(int nIndex) const
{
	return m_arPlayObjects[nIndex];
}

LPCTSTR SSpPlayObjectPool::GetName(int nIndex) const
{
	return m_arNames[nIndex];
}

unsigned int SSpPlayObjectPool::GetSize() const
{
	return m_arPlayObjects.GetSize();
}

BOOL SSpPlayObjectPool::IsLoaded() const
{
	return m_arPlayObjects.GetSize() > 0;
}

//////////////////////////////////////////////////////////////////////
// SSpPlayTaskPool
//////////////////////////////////////////////////////////////////////

SSpPlayTaskPool::SSpPlayTaskPool()
{
}

SSpPlayTaskPool::~SSpPlayTaskPool()
{
	clearContents();
}

void SSpPlayTaskPool::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_arPlayTasks.Serialize(ar);
	m_arNames.Serialize(ar);
	// SERIALIZE FIX
	if (ar.IsLoading()) {
		for (int i=0; i<m_arPlayTasks.GetSize(); ++i) {
			m_arPlayTasks[i]->setIndex(i);
			m_arPlayTasks[i]->setName(m_arNames[i].GetBuffer());
		}
	}
}

void SSpPlayTaskPool::printASCII(sspOutStream& outStr)
{
	outStr << endl << "SSpPlayTaskPool";
	for (int i=0; i<m_arPlayTasks.GetSize(); ++i) {
		m_arPlayTasks[i]->printASCII(outStr);
	}
}

bool SSpPlayTaskPool::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (int i=0; i<m_arPlayTasks.GetSize(); ++i) {
		if (!m_arPlayTasks[i]->verify(outStr, nErrors, nWarnings)) {
			bRet = false;
		}
	}
	if (m_arPlayTasks.GetSize() == 0) {
		printError(outStr, _T("(SSpPlayTaskPool): no tasks defined"), nErrors);
		bRet = false;
	}
	return bRet;
}

void SSpPlayTaskPool::initContents()
{
}

void SSpPlayTaskPool::clearContents()
{
	for (int i=0; i<m_arPlayTasks.GetSize(); i++)
		delete m_arPlayTasks[i];
	m_arPlayTasks.RemoveAll();
	m_arNames.RemoveAll();
}

bool SSpPlayTaskPool::initialize()
{
	for (int i=0; i<m_arPlayTasks.GetSize(); i++) {
		if (!m_arPlayTasks[i]->initialize())
			return false;
	}
	return true;
}

int SSpPlayTaskPool::LoadPlayTask(SSpPlayTask* pPlayTask, LPCTSTR strName)
{
	// SERIALIZE FIX
	int nIndex = m_arPlayTasks.Add(pPlayTask);
	pPlayTask->setIndex(nIndex);
	pPlayTask->setName(strName);
	return m_arNames.Add(strName);
}

void SSpPlayTaskPool::ExchangePlayTask(SSpPlayTask* pPlayTask, LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	delete m_arPlayTasks[nIndex];
	m_arPlayTasks[nIndex] = pPlayTask;
	pPlayTask->setIndex(nIndex);
	pPlayTask->setName(strName);
	m_arNames[nIndex] = strName;
}

void SSpPlayTaskPool::ExchangeName(LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	m_arPlayTasks[nIndex]->setName(strName);
	m_arNames[nIndex] = strName;
}

SSpPlayTask* SSpPlayTaskPool::GetPlayTask(int nIndex) const
{
	return m_arPlayTasks[nIndex];
}

LPCTSTR SSpPlayTaskPool::GetName(int nIndex) const
{
	return m_arNames[nIndex];
}

unsigned int SSpPlayTaskPool::GetSize() const
{
	return m_arPlayTasks.GetSize();
}

BOOL SSpPlayTaskPool::IsLoaded() const
{
	return m_arPlayTasks.GetSize() > 0;
}

//////////////////////////////////////////////////////////////////////
// SSpStreamPool
//////////////////////////////////////////////////////////////////////

SSpStreamPool::SSpStreamPool()
{
}

SSpStreamPool::~SSpStreamPool()
{
	clearContents();
}

void SSpStreamPool::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	m_arStreams.Serialize(ar);
	m_arNames.Serialize(ar);
	// SERIALIZE FIX
	if (ar.IsLoading()) {
		for (int i=0; i<m_arStreams.GetSize(); ++i) {
			m_arStreams[i]->setIndex(i);
			m_arStreams[i]->setName(m_arNames[i].GetBuffer());
		}
	}
}

void SSpStreamPool::printASCII(sspOutStream& outStr)
{
	outStr << endl << "SSpStreamPool";
	for (int i=0; i<m_arStreams.GetSize(); ++i) {
		m_arStreams[i]->printASCII(outStr);
	}
}

bool SSpStreamPool::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	for (int i=0; i<m_arStreams.GetSize(); ++i) {
		if (!m_arStreams[i]->verify(outStr, nErrors, nWarnings)) {
			bRet = false;
		}
	}
	if (m_arStreams.GetSize() < 2) {
		printError(outStr, _T("(SSpStreamPool): not enough streams"), nErrors);
		bRet = false;
	}
	else {
		// Checking the substream hierarchi
		sspIntVec nSubCount;
		nSubCount.resize(m_arStreams.GetSize(),0);
		for (int i=0; i<m_arStreams.GetSize(); ++i) {
			sspIntVec nsub = m_arStreams[i]->getSubStreams();
			for (unsigned int j=0; j<nsub.size(); ++j) {
				nSubCount[nsub[j]]++;
			}
		}
		if (nSubCount[0] != 0) {
			printError(outStr, _T("(SSpStreamPool): appstream cannot be a substream"), nErrors);
			bRet = false;
		}
		for (unsigned int i=1; i<nSubCount.size(); ++i) {
			if (nSubCount[i] == 0) {
				printError(outStr, _T("(SSpStreamPool): substream never accessed"), nErrors);
				bRet = false;
			}
			if (nSubCount[i] > 1) {
				printError(outStr, _T("(SSpStreamPool): substream accessed more than once"), nErrors);
				bRet = false;
			}
		}
	}
	return bRet;
}

void SSpStreamPool::initContents()
{
	SSpStream* pNew = new SSpStream;
	sspPool::Instance().streams.LoadStream(pNew, _T("MainStream"));
}

void SSpStreamPool::clearContents()
{
	for (int i=0; i<m_arStreams.GetSize(); i++)
		delete m_arStreams[i];
	m_arStreams.RemoveAll();
	m_arNames.RemoveAll();
}

bool SSpStreamPool::initialize()
{
	for (int i=0; i<m_arStreams.GetSize(); i++) {
		if (!m_arStreams[i]->initialize())
			return false;
	}
	return true;
}

void SSpStreamPool::terminate()
{
	for (int i=0; i<m_arStreams.GetSize(); i++) {
		m_arStreams[i]->terminate();
  }
}

int SSpStreamPool::LoadStream(SSpStream* pStream, LPCTSTR strName)
{
	// SERIALIZE FIX
	int nIndex = m_arStreams.Add(pStream);
	pStream->setIndex(nIndex);
	pStream->setName(strName);
	return m_arNames.Add(strName);
}

void SSpStreamPool::ExchangeStream(SSpStream* pStream, LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	delete m_arStreams[nIndex];
	m_arStreams[nIndex] = pStream;
	pStream->setIndex(nIndex);
	pStream->setName(strName);
	m_arNames[nIndex] = strName;
}

void SSpStreamPool::ExchangeStreamName(LPCTSTR strName, int nIndex)
{
	// SERIALIZE FIX
	m_arStreams[nIndex]->setName(strName);
	m_arNames[nIndex] = strName;
}

SSpStream* SSpStreamPool::GetStream(int nIndex) const
{
	return m_arStreams[nIndex];
}

LPCTSTR SSpStreamPool::GetName(int nIndex) const
{
	return m_arNames[nIndex];
}

unsigned int SSpStreamPool::GetSize() const
{
	return m_arStreams.GetSize();
}

BOOL SSpStreamPool::IsLoaded() const
{
	return m_arStreams.GetSize() > 0;
}

//////////////////////////////////////////////////////////////////////
// sspPool
//////////////////////////////////////////////////////////////////////

sspPool* sspPool::m_pInstance = NULL;
bool sspPool::m_bDestroyed = false;

sspPool::sspPool()
{
}

sspPool::sspPool(const sspPool&)
{
}

sspPool& sspPool::operator=(const sspPool&)
{
	return *this;
}

sspPool::~sspPool()
{
	m_pInstance = NULL;
	m_bDestroyed = true;
}

sspPool& sspPool::Instance()
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

void sspPool::Serialize(CArchive& ar)
{
	values.Serialize(ar);
	conditionals.Serialize(ar);
  strings.Serialize(ar);
  if (ar.IsLoading() && strings.GetSize() == 0)
    strings.initContents();
	streams.Serialize(ar);
	objects.Serialize(ar);
	tasks.Serialize(ar);
}

void sspPool::printASCII(sspOutStream& outStr)
{
	values.printASCII(outStr);
	outStr << endl;
	conditionals.printASCII(outStr);
	outStr << endl;
  strings.printASCII(outStr);
  outStr << endl;
	streams.printASCII(outStr);
	outStr << endl;
	objects.printASCII(outStr);
	outStr << endl;
	tasks.printASCII(outStr);
}

bool sspPool::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;

	if (!streams.verify(outStr, nErrors, nWarnings)) bRet = false;
	if (!objects.verify(outStr, nErrors, nWarnings)) bRet = false;
	if (!tasks.verify(outStr, nErrors, nWarnings)) bRet = false;
	if (!values.verify(outStr, nErrors, nWarnings)) bRet = false;
	if (!conditionals.verify(outStr, nErrors, nWarnings)) bRet = false;
	if (!strings.verify(outStr, nErrors, nWarnings)) bRet = false;
	return bRet;
}

void sspPool::initContents()
{
	streams.initContents();
	values.initContents();
	conditionals.initContents();
  strings.initContents();
	objects.initContents();
	tasks.initContents();
}

void sspPool::clearContents()
{
	streams.clearContents();
	values.clearContents();
	conditionals.clearContents();
  strings.clearContents();
	objects.clearContents();
	tasks.clearContents();
}

bool sspPool::initialize()
{
	if (!streams.initialize()) {
		AfxMessageBox(L"Error in initializing streams.");
		return false;
	}
	if (!objects.initialize()) {
		AfxMessageBox(L"Error in initializing objects.");
		return false;
	}
	if (!tasks.initialize()) {
		AfxMessageBox(L"Error in initializing tasks.");
		return false;
	}
	if (!values.initialize()) {
		AfxMessageBox(L"Error in initializing values.");
		return false;
	}
	if (!conditionals.initialize()) {
		AfxMessageBox(L"Error in initializing conditionals.");
		return false;
	}
	if (!strings.initialize()) {
		AfxMessageBox(L"Error in initializing string objects.");
		return false;
	}
	return true;
}

void sspPool::terminate()
{
	streams.terminate();
}

void sspPool::create()
{
	static sspPool theInstance;
	m_pInstance = &theInstance;
}

void sspPool::onDeadReference()
{
	throw std::runtime_error("Dead reference to object pool detected");
}
