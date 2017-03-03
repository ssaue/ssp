// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__67E58408_5474_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_MAINFRM_H__67E58408_5474_11D2_97D2_00105A3B4608__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "mididata.h"
#include "settings.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	static const WCHAR s_settingsHeading[];
	static const WCHAR s_colorsHeading[];
	static const WCHAR s_profilMidi[];
	static const WCHAR s_profilFrekvens[];
	static const WCHAR s_profilAmpLengde[];
	static const WCHAR s_profilAmpVelocity[];
	static const WCHAR s_profilSynk[];
	static CSettings m_settings;
	CMidiPlay m_midiPlay;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	enum SEPARATOR {X_KOORD=1, Y_KOORD, LEFT, RIGHT, DIFFERANSE};
	void SetDocUpdated();
	void InitDialogBar();
	void UpdateSpor(int sporAnt, int sporNr);
	void UpdateTempo(int metronom);
	void UpdateAvstand(int spor) {}
	void UpdateKoordinat(int spor) {}
	int GetSpor();
	int GetTempo();

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	enum {DEFAULT_TEMPO = 120, MIN_TEMPO=10, MAX_TEMPO = 999};
	CToolBar    m_wndToolBar;
public:
	CStatusBar	m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGenerellmeny();
	afx_msg void OnLoop();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnUpdateStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLoop(CCmdUI* pCmdUI);
	afx_msg void OnViewStatusBar();
	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnPlayDone(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bDocUpdated;
	CDialogBar m_wndDialogBar;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__67E58408_5474_11D2_97D2_00105A3B4608__INCLUDED_)
