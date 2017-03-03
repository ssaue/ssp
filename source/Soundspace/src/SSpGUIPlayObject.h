// SSpGUIPlayObject.h : header file
//

#ifndef SSPGUIPLAYOBJECT_H
#define SSPGUIPLAYOBJECT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>

class sspPlayObject;
struct sspMultipleObject {
	sspPlayObject*	pObj;
	CString			strName;
};

typedef std::vector<sspMultipleObject*> sspObjVector;

// Base class for value editor dialogs
class CPlayObjectEditor : public CDialog
{
public:
	CString			m_strTitle;
	sspObjVector	m_pObjs;
	bool			m_bMultipleSelect;

	CPlayObjectEditor(unsigned int nIDTemplate, CWnd* pParentWnd); 
};

namespace SSpGUIPlayObject {
	bool			loadPlayObjectEditor(sspPlayObject* pObj, CString& strName);
	sspObjVector	loadPlayObjectEditor(short nType, bool bMultiple=false);
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
