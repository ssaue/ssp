// SSpGUIConditional.h : header file
//

#ifndef SSPGUICONDITIONAL_H
#define SSPGUICONDITIONAL_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class sspConditional;

// Base class for conditional editor dialogs
class CConditionalEditor : public CDialog
{
public:
	CString			m_strTitle;
	CString			m_strName;
	sspConditional* m_pCond;

	CConditionalEditor(unsigned int nIDTemplate, CWnd* pParentWnd); 
};

namespace SSpGUIConditional {
	bool			loadConditionalEditor(sspConditional* pCond, CString& strName);
	sspConditional*	loadConditionalEditor(short nType, CString& strName);
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
