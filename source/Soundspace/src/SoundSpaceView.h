// SoundSpaceView.h : interface of the CSoundSpaceView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDSPACEVIEW_H__ED028790_8AC6_11D3_901C_F7D8A2DF1632__INCLUDED_)
#define AFX_SOUNDSPACEVIEW_H__ED028790_8AC6_11D3_901C_F7D8A2DF1632__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "sspDefines.h"
#include <vector>

#define WM_PLAY_AUTOMATIC	WM_USER + 5

class CSoundSpaceView : public CFormView
{
protected: // create from serialization only
	CSoundSpaceView();
	DECLARE_DYNCREATE(CSoundSpaceView)

public:
	//{{AFX_DATA(CSoundSpaceView)
	enum { IDD = IDD_SOUNDSPACE_FORM };
	//}}AFX_DATA

// Attributes
public:
	CSoundSpaceDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundSpaceView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSoundSpaceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	enum {VALUE_MAX = 16};
	enum {SLIDER_RANGE = 100};
	UINT_PTR m_nTimer;

// Generated message map functions
protected:
	void refreshFaders();

	//{{AFX_MSG(CSoundSpaceView)
	afx_msg void OnTimer(unsigned int nIDEvent);
	afx_msg void OnVScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar);
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnInitialize();
	afx_msg void OnEditDisplay();
	afx_msg void OnUpdateEditDisplay(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnExportAsciidump();
	afx_msg void OnExportConfigerrors();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChangeExtValue(unsigned int nID);
	afx_msg LRESULT OnPlayAutomatic(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // debug version in SoundSpaceView.cpp
inline CSoundSpaceDoc* CSoundSpaceView::GetDocument()
   { return (CSoundSpaceDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDSPACEVIEW_H__ED028790_8AC6_11D3_901C_F7D8A2DF1632__INCLUDED_)
