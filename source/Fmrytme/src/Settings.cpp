// Settings.cpp: implementation of the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fmrytme.h"
#include "mmsystem.h"
#include "Settings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSettings::CSettings()
{
	m_nPeriode = 1000;
	m_nMidiDevice = 1;
	for (int i=0; i<KANALER; i++)
		m_colFarge[i] = RGB(255,0,0);
}
