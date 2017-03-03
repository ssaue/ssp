#if !defined(AFX_DIALOG_H__94654A20_585B_11D2_97D2_00105A3B4608__INCLUDED_)
#define AFX_DIALOG_H__94654A20_585B_11D2_97D2_00105A3B4608__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Dialog.h : header file
//

#include "settings.h"
#include "FMData.h"

#define WM_OPPSETT				WM_USER + 5
#define WM_OPPDATER_MODULATOR	WM_USER + 6

/////////////////////////////////////////////////////////////////////////////
// CFMDialog dialog
// Dette er en klasse som kun fungerer som base for de tre etterfølgende

class CFMDialog : public CDialog
{
protected:
	CView*	m_pView;

public:
	enum {ALLE=1, OPPSETT_MSG=8};	// Konstanter ved parameter-overføring i PostMessage

	CFMDialog(CView* pView);
	virtual BOOL Create() = 0;
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnOppsett();
	afx_msg void OnEditChange(UINT nID);
	afx_msg void OnChangeGrunnpuls();
};

/////////////////////////////////////////////////////////////////////////////
// CFMSimpleDialog dialog

class CFMSimpleDialog : public CFMDialog
{
// Construction
public:
	CFMSimpleDialog(CView* pView, CFMSekvens* pSekv);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFMSimpleDialog)
	enum { IDD = IDD_SIMPLEFM };
	double	m_fPulsforhold;
	int		m_nGrunnpuls;
	double	m_dModFaktor;
	double	m_dModPuls;
	int		m_nFunksjon;
	double	m_dModFase;
	int		m_nModPotens;
	//}}AFX_DATA
	CFMSekvens* m_pSekv;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFMSimpleDialog)
	public:
	virtual BOOL Create();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFMSimpleDialog)
	afx_msg void OnBruk();
	afx_msg void OnBrukalle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFMParallellDialog dialog

class CFMParallellDialog : public CFMDialog
{
// Construction
public:
	CFMParallellDialog(CView* pView, CFMSekvens* pSekv);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFMParallellDialog)
	enum { IDD = IDD_PARALLELFM };
	int		m_nGrunnpuls;
	int		m_nFunksjon1;
	int		m_nFunksjon2;
	double	m_dModFaktor1;
	double	m_dModFaktor2;
	double	m_dModPuls1;
	double	m_dModPuls2;
	double	m_dModFase1;
	double	m_dModFase2;
	int		m_nModPotens1;
	int		m_nModPotens2;
	//}}AFX_DATA
	CFMSekvens* m_pSekv;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFMParallellDialog)
	public:
	virtual BOOL Create();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFMParallellDialog)
	afx_msg void OnBrukalle();
	afx_msg void OnBruk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CFMSerieDialog dialog

class CFMSerieDialog : public CFMDialog
{
// Construction
public:
	CFMSerieDialog(CView* pView, CFMSekvens* pSekv);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFMSerieDialog)
	enum { IDD = IDD_SERIEFM };
	int		m_nGrunnpuls;
	int		m_nFunksjon1;
	int		m_nFunksjon2;
	double	m_dModFaktor2;
	double	m_dModFaktor1;
	double	m_dModPuls1;
	double	m_dModPuls2;
	double	m_dModFase1;
	double	m_dModFase2;
	int		m_nModPotens1;
	int		m_nModPotens2;
	//}}AFX_DATA
	CFMSekvens* m_pSekv;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFMSerieDialog)
	public:
	virtual BOOL Create();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFMSerieDialog)
	afx_msg void OnBruk();
	afx_msg void OnBrukalle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CFargeDialog dialog

class CFargeDialog : public CDialog
{
// Construction
public:
	CFargeDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFargeDialog)
	enum { IDD = IDD_FARGER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	COLORREF m_colObjects[CSettings::KANALER];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFargeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFargeDialog)
	afx_msg void OnPaint();
	afx_msg void OnButtonClicked(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DrawColorBoxes();
};
/////////////////////////////////////////////////////////////////////////////
// CFMOppsettDialog dialog

class CFMOppsettDialog : public CDialog
{
// Construction
public:
	enum {ENKEL, PARALLELL, SERIE};

	CFMOppsettDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFMOppsettDialog)
	enum { IDD = IDD_FMOPPSETT };
	int		m_nOppsett;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFMOppsettDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFMOppsettDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CGenerellDialog dialog

class CGenerellDialog : public CDialog
{
// Construction
public:
	CGenerellDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGenerellDialog)
	enum { IDD = IDD_GENERELL };
	int		m_nDeviceNr;
	UINT	m_nPeriode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenerellDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenerellDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nAntall;
};
/////////////////////////////////////////////////////////////////////////////
// CMinsteTidDialog dialog

class CMinsteTidDialog : public CDialog
{
// Construction
public:
	CMinsteTidDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMinsteTidDialog)
	enum { IDD = IDD_MINSTETID };
	int		m_nTidsenhet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMinsteTidDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMinsteTidDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CNyStemmeDialog dialog

class CNyStemmeDialog : public CDialog
{
// Construction
public:
	CNyStemmeDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNyStemmeDialog)
	enum { IDD = IDD_NYSTEMME };
	double	m_dLengde;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNyStemmeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNyStemmeDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CStemme1Dialog dialog

class CStemme1Dialog : public CDialog
{
// Construction
public:
	CStemme1Dialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStemme1Dialog)
	enum { IDD = IDD_STEMME1 };
	double	m_dLengde;
	int		m_nMetronom;
	int		m_nNevner;
	int		m_nTeller;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStemme1Dialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStemme1Dialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CPlotDialog dialog

class CPlotDialog : public CDialog
{
// Construction
public:
	int m_nAntallSpor;
	CPlotDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlotDialog)
	enum { IDD = IDD_PLOTINFO };
	double	m_dAmpLengde;
	double	m_dAmpVelocity;
	int		m_nStemme;
	int		m_nSynk;
	int		m_nTaktSlag;
	BOOL	m_bOppdaterPuls;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlotDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSynkron();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_H__94654A20_585B_11D2_97D2_00105A3B4608__INCLUDED_)
