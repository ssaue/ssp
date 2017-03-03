// SSpGUIValue.h : header file
//

#ifndef SSPGUIVALUE_H
#define SSPGUIVALUE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class sspValue;

// Base class for value editor dialogs
class CValueEditor : public CDialog
{
public:
	CString			m_strTitle;
	CString			m_strName;
	sspValue*		m_pVal;

	CValueEditor(unsigned int nIDTemplate, CWnd* pParentWnd); 
};

namespace SSpGUIValue {
	bool		loadValueEditor(sspValue* pVal, CString& strName);
	sspValue*	loadValueEditor(short nType, CString& strName);
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
