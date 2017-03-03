// SoundSpaceDoc.h : interface of the CSoundSpaceDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDSPACEDOC_H__ED02878E_8AC6_11D3_901C_F7D8A2DF1632__INCLUDED_)
#define AFX_SOUNDSPACEDOC_H__ED02878E_8AC6_11D3_901C_F7D8A2DF1632__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"

class CSoundSpaceDoc : public CDocument
{
protected: // create from serialization only
	CSoundSpaceDoc();
	DECLARE_DYNCREATE(CSoundSpaceDoc)

// Attributes
	enum {INITIALIZE, DISPLAY, STATE};

public:
	sspIntVec		m_nDisplaySelection;

protected:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundSpaceDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSoundSpaceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

  bool initialize(LPVOID hWnd);
  void terminate();

  bool exportAscii(const CString& strFile);
  bool exportErrors(const CString& strFile, int& nErrors, int& nWarnings);

// Generated message map functions
protected:
	//{{AFX_MSG(CSoundSpaceDoc)
	afx_msg void OnStreams();
	afx_msg void OnValues();
	afx_msg void OnConditionals();
	afx_msg void OnStrings();
	afx_msg void OnPlayObjects();
	afx_msg void OnPlayTasks();
	afx_msg void OnInitialEvents();
	afx_msg void OnTriggerEvents();
	afx_msg void OnClockEvents();
	afx_msg void OnUpdateClockEvents(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInitialEvents(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlayTasks(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlayObjects(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTriggerEvents(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStreams(CCmdUI* pCmdUI);
	afx_msg void OnUpdateConditionals(CCmdUI* pCmdUI);
	afx_msg void OnUpdateResetSetting(CCmdUI* pCmdUI);
	afx_msg void OnUpdateValues(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStrings(CCmdUI* pCmdUI);
	afx_msg void OnSettingsStartupshutdown();
	afx_msg void OnResetSetting();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDevicesAudio();
	afx_msg void OnDevicesDmx();
	afx_msg void OnDevicesMidi();
	afx_msg void OnUpdateDevicesAudio(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDevicesMidi(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDevicesDmx(CCmdUI *pCmdUI);
	afx_msg void OnPreferences();
	afx_msg void OnUpdatePreferences(CCmdUI* pCmdUI);
	afx_msg void OnEditInputs();
	afx_msg void OnUpdateEditInputs(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDSPACEDOC_H__ED02878E_8AC6_11D3_901C_F7D8A2DF1632__INCLUDED_)
