// sspInput.cpp : implementation file
//

#include "stdafx.h"
#include "sspInput.h"
#include "sspSetting.h"

using namespace std;

sspInput::sspInput(short nType) 
: sspObject(SSP_CLASS_INPUT, nType)
,	m_fUpdateInterval(1.0f)
, m_update(sspTimeConvert::secs2time(m_fUpdateInterval))
{
}

sspInput::sspInput(const sspInput& inp) 
: sspObject(inp)
,	m_fUpdateInterval(inp.m_fUpdateInterval)
, m_update(sspTimeConvert::secs2time(m_fUpdateInterval))
{
}

sspInput& sspInput::operator= (const sspInput& inp)
{
	sspObject::operator=(inp);
	m_fUpdateInterval = inp.m_fUpdateInterval;
	return *this;
}

sspInput::~sspInput()
{
}

void sspInput::Serialize(CArchive& ar)
{
	// The use of CString is a temporary solution (waiting for my own serialization routines)
	CObject::Serialize(ar);
	if (ar.IsStoring())
		ar << m_fUpdateInterval << CString(m_strName.c_str());
	else {
		CString str;
		ar >> m_fUpdateInterval >> str;
		m_strName = (LPCTSTR) str;
	}
}

void sspInput::printASCII(sspOutStream& outStr)
{
	outStr << endl << "   - m_fUpdateInterval: " << m_fUpdateInterval;
}

bool sspInput::verify(sspOutStream& outStr, int& nErrors, int& nWarnings)
{
	bool bRet = true;
	if (m_fUpdateInterval <= 0.0f) {
		printError(outStr, _T("(sspInput): m_fUpdateInterval is too small"), nErrors);
		bRet = false;
	}
	return bRet;
}

bool sspInput::initialize(LPVOID pWnd)
{
  m_update.setInterval(sspTimeConvert::secs2time(m_fUpdateInterval));
  m_update.initialize(false); // No increment to enforce immediate update
	return true;
}
