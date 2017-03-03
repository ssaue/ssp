// SSpGUIUtilityEditor.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "SSpGUIUtilityEditor.h"
#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditValue dialog

IMPLEMENT_DYNAMIC(CEditValue, CDialog)
CEditValue::CEditValue(CRect pos, CWnd* pParent /*=NULL*/)
	: CDialog(CEditValue::IDD, pParent)
  , m_pos(pos)
	, m_nValue(0)
{
}

CEditValue::~CEditValue()
{
}

void CEditValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VALUE, m_nValue);
}

BEGIN_MESSAGE_MAP(CEditValue, CDialog)
END_MESSAGE_MAP()

BOOL CEditValue::OnInitDialog() 
{
	CDialog::OnInitDialog();
  CRect rDlg;
  GetWindowRect(&rDlg);
  CRect newRect(m_pos.TopLeft(), rDlg.Size());
  MoveWindow(newRect);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CEditCombo dialog

IMPLEMENT_DYNAMIC(CEditCombo, CDialog)
CEditCombo::CEditCombo(CRect pos, CWnd* pParent /*=NULL*/)
	: CDialog(CEditCombo::IDD, pParent)
  , m_pos(pos)
  , m_nChoice(0)
{
}

CEditCombo::~CEditCombo()
{
}

void CEditCombo::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO, m_lbChoices);
  DDX_CBIndex(pDX, IDC_COMBO, m_nChoice);
}


BEGIN_MESSAGE_MAP(CEditCombo, CDialog)
END_MESSAGE_MAP()

BOOL CEditCombo::OnInitDialog() 
{
	CDialog::OnInitDialog();
  CRect rDlg;
  GetWindowRect(&rDlg);
  CRect newRect(m_pos.TopLeft(), rDlg.Size());
  MoveWindow(newRect);
  for (int i=0; i<strChoices.GetSize(); ++i)
    m_lbChoices.InsertString(i, strChoices[i]);
  m_lbChoices.SetCurSel(m_nChoice);
	return TRUE;
}

void CEditCombo::addChoice(int i, CString txt)
{
  strChoices.SetAtGrow(i, txt);
}


/////////////////////////////////////////////////////////////////////////////
// CEditClock dialog

IMPLEMENT_DYNAMIC(CEditClock, CDialog)
CEditClock::CEditClock(CRect pos, CWnd* pParent /*=NULL*/)
	: CDialog(CEditClock::IDD, pParent)
  , m_pos(pos)
  , m_time(0)
{
}

CEditClock::~CEditClock()
{
}

void CEditClock::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER, m_time);
}


BEGIN_MESSAGE_MAP(CEditClock, CDialog)
END_MESSAGE_MAP()

BOOL CEditClock::OnInitDialog() 
{
	CDialog::OnInitDialog();
  CRect rDlg;
  GetWindowRect(&rDlg);
  CRect newRect(m_pos.TopLeft(), rDlg.Size());
  MoveWindow(newRect);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// sspGUIfileDialogs namespace

bool sspGUIfileDialogs::SelectFolder(HWND hOwner, const wchar_t* strTitle, CString& strFolder)
{
	bool bRet = false;

	BROWSEINFO brws = {0};
	wchar_t szName[260];       // buffer for file name
	szName[0] = 0;

	LPMALLOC pMalloc = NULL;
	SHGetMalloc(&pMalloc);

	brws.hwndOwner = hOwner;
	brws.pszDisplayName = szName;
	brws.lpszTitle = strTitle;
	brws.pidlRoot = NULL;
	brws.lpfn = NULL;
	brws.ulFlags = BIF_NONEWFOLDERBUTTON | BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN;
	LPITEMIDLIST pList = ::SHBrowseForFolder(&brws);
	if (::SHGetPathFromIDList(pList, szName)) {
		strFolder = szName;
		bRet = true;
	}
	if(pList) pMalloc->Free(pList);
	pMalloc->Release();
	return bRet;
}

bool sspGUIfileDialogs::SelectFile(HWND hOwner, const wchar_t* strTitle, const wchar_t* strFilter, CString& strFile)
{
	OPENFILENAME ofn;		// common dialog box structure
	wchar_t strBuf[260];
	strBuf[0] = 0;

	// Initialize OPENFILENAME
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hOwner;
	ofn.lpstrFilter = strFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = strTitle;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrFile = strBuf;
	ofn.nMaxFile = sizeof(strBuf);

	// Display the Open dialog box. 

	if (::GetOpenFileName(&ofn) == TRUE) {
		strFile = ofn.lpstrFile;
		return true;
	}
	return false;
}

bool sspGUIfileDialogs::SelectMultipleFiles(HWND hOwner, const wchar_t* strTitle, const wchar_t* strFilter, CStringArray& strNames)
{
	OPENFILENAME ofn;		// common dialog box structure
	wchar_t strBuf[MAXBUFFER];
	strBuf[0] = 0;

	// Initialize OPENFILENAME
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hOwner;
	ofn.lpstrFilter = strFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = strTitle;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY 
		| OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	ofn.lpstrFile = strBuf;
	ofn.nMaxFile = sizeof(strBuf);

	// Display the Open dialog box. 

	if (::GetOpenFileName(&ofn) == TRUE) {
		strNames.RemoveAll();
		wchar_t strPath[260];
		wchar_t strFile[260];
		wcsncpy_s(strPath, 260, ofn.lpstrFile, ofn.nFileOffset);
		strPath[ofn.nFileOffset-1] = '\\';
		strPath[ofn.nFileOffset] = 0;
		wchar_t* strPos = ofn.lpstrFile + ofn.nFileOffset;
		while (*strPos != '\0') {
			wcscpy_s<260>(strFile, strPath);
			wcscat_s(strFile, 260, strPos);
			strNames.Add(strFile);
			strPos += wcslen(strPos) + 1;
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// sspSaveDialog dialog

sspSaveDialog::sspSaveDialog(LPCTSTR lpszFileName) 
: CFileDialog(FALSE, NULL, lpszFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL)
{
	//{{AFX_DATA_INIT(sspFolderDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void sspSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(sspFolderDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(sspSaveDialog, CFileDialog)
END_MESSAGE_MAP()

BOOL sspSaveDialog::OnInitDialog() 
{
	BOOL bRet = CFileDialog::OnInitDialog();
	SetControlText(IDOK, _T("Select"));
	return bRet;
}
