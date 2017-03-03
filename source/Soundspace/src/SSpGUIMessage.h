// SSpGUIMessage.h
//

#ifndef SSPGUIMESSAGE_H
#define SSPGUIMESSAGE_H

#include "SSpMessage.h"

class CMessageDialog : public CDialog
{
public:
	CString m_strTitle;
	BOOL m_bSpecify;
	SSpStreamMsg m_msg;

	CMessageDialog(unsigned int nIDTemplate, CWnd* pParentWnd); 
};

namespace SSpGUIMessage {
	CString GetMessageText(SSpStreamMsg* pMessage);
	bool LoadMessageEditor(SSpStreamMsg* pMessage, BOOL bSpecify = FALSE);
	void FillMessageData(SSpStreamMsg* pMessage, int nType);
}


#endif