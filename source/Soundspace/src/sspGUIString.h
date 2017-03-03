// SSpGUIString.h : header file
//

#ifndef SSPGUISTRING_H
#define SSPGUISTRING_H

class sspStringObject;

// Base class for string object editor dialogs
class sspGUIStringEditor : public CDialog
{
public:
	CString			      m_strTitle;
	CString			      m_strName;
	sspStringObject*  m_pString;

	sspGUIStringEditor(unsigned int nIDTemplate, CWnd* pParentWnd); 
};

namespace sspGUIString {
	bool			        loadStringEditor(sspStringObject* pString, CString& strName);
	sspStringObject*	loadStringEditor(short nType, CString& strName);
}

#endif
