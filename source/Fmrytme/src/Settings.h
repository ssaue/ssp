// Settings.h: interface for the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGS_H__94654A25_585B_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_SETTINGS_H__94654A25_585B_11D2_97D2_00105A3B4608__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSettings : public CObject  
{
public:
	enum {KANALER=16};
	COLORREF m_colFarge[KANALER];
	int m_nMidiDevice;
	UINT m_nPeriode;

	CSettings();
	virtual ~CSettings() {}
};

#endif // !defined(AFX_SETTINGS_H__94654A25_585B_11D2_97D2_00105A3B4608__INCLUDED_)
