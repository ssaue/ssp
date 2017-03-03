// SSpGUIinput.h : header file
//

#ifndef SSPGUIINPUT_H
#define SSPGUIINPUT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class sspInput;

// Base class for conditional editor dialogs
class CInputEditor : public CDialog
{
public:
	CString		m_strTitle;
	sspInput*	m_pInput;

	CInputEditor(unsigned int nIDTemplate, CWnd* pParentWnd); 
};

namespace sspGUIinput {
	bool		loadInputEditor(sspInput* pInput);
	sspInput*	loadInputEditor(short nType);
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
