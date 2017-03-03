// sspGUIStringEditors.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "sspGUIStringEditors.h"
#include "sspGUIStringDialog.h"
#include "ValueDialog.h"
#include "sspPools.h"


// sspSimpleStringEditor dialog

IMPLEMENT_DYNAMIC(sspSimpleStringEditor, CDialog)

sspSimpleStringEditor::sspSimpleStringEditor(CWnd* pParent /*=NULL*/)
	: sspGUIStringEditor(sspSimpleStringEditor::IDD, pParent)
{
  m_strName = _T("");
}

sspSimpleStringEditor::~sspSimpleStringEditor()
{
}

void sspSimpleStringEditor::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_STRING, m_strName);
}


BEGIN_MESSAGE_MAP(sspSimpleStringEditor, CDialog)
END_MESSAGE_MAP()

BOOL sspSimpleStringEditor::OnInitDialog() 
{
	if (m_pString) {
		sspSimpleString* pString = dynamic_cast<sspSimpleString*>(m_pString);
		if (!pString) {
			AfxMessageBox(L"Unable to assign string.");
			EndDialog(IDCANCEL);
		}
		m_editString = *pString;
	}
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}

void sspSimpleStringEditor::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	CDialog::OnOK();
  m_editString.setString(m_strName.GetBuffer());
	if (m_pString)
		*((sspSimpleString*) m_pString) = m_editString;
	else
		m_pString = new sspSimpleString(m_editString);
}

// sspConcatStringEditor dialog

IMPLEMENT_DYNAMIC(sspConcatStringEditor, CDialog)

sspConcatStringEditor::sspConcatStringEditor(CWnd* pParent /*=NULL*/)
	: sspGUIStringEditor(sspConcatStringEditor::IDD, pParent)
{
  m_strName = _T("");
	m_nSelected = -1;
}

sspConcatStringEditor::~sspConcatStringEditor()
{
}

void sspConcatStringEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STRINGS, m_lbStrings);
	DDX_Text(pDX, IDC_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(sspConcatStringEditor, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_LBN_SELCHANGE(IDC_STRINGS, OnSelectString)
END_MESSAGE_MAP()


BOOL sspConcatStringEditor::OnInitDialog()
{
	if (m_pString) {
		sspConcatString* pString = dynamic_cast<sspConcatString*>(m_pString);
		if (!pString) {
			AfxMessageBox(L"Unable to assign string.");
			EndDialog(IDCANCEL);
		}
		m_editString = *pString;
	}
  m_nStrings = m_editString.getStringList();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
  updateButtons();
	m_lbStrings.ResetContent();
	for (unsigned int i=0; i<m_nStrings.size(); i++)
		m_lbStrings.AddString(sspPool::Instance().strings.GetName(m_nStrings[i]));
	return TRUE;
}

void sspConcatStringEditor::OnAdd() 
{
	sspGUIStringDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nStrings.insert(m_nStrings.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbStrings.AddString(sspPool::Instance().strings.GetName(dlg.m_nSelection[i]));
		}
		m_nSelected = m_nStrings.size()-1;
		m_lbStrings.SetCurSel(m_nSelected);
		updateButtons();
	}
}

void sspConcatStringEditor::OnRemove() 
{
	if (m_nSelected >= 0 && m_nSelected < (int) m_nStrings.size()) {
		m_nStrings.erase(m_nStrings.begin() + m_nSelected);
		m_lbStrings.DeleteString(m_nSelected);
		if (m_nSelected == (int) m_nStrings.size()) m_nSelected--;
		m_lbStrings.SetCurSel(m_nSelected);
		updateButtons();
	}
}

void sspConcatStringEditor::OnUp()
{
	CString text;
	if (m_nSelected > 0) {
		int nObj = m_nStrings[m_nSelected];
		m_nStrings[m_nSelected] = m_nStrings[m_nSelected-1];
		m_nStrings[m_nSelected-1] = nObj;
		m_lbStrings.GetText(m_nSelected, text);
		m_lbStrings.DeleteString(m_nSelected);
		m_lbStrings.InsertString(m_nSelected-1, text);
		m_nSelected--;
		m_lbStrings.SetCurSel(m_nSelected);
		updateButtons();
	}
}

void sspConcatStringEditor::OnDown()
{
	CString text;
	if (m_nSelected + 1 < (int) m_nStrings.size()) {
		int nObj = m_nStrings[m_nSelected];
		m_nStrings[m_nSelected] = m_nStrings[m_nSelected+1];
		m_nStrings[m_nSelected+1] = nObj;
		m_lbStrings.GetText(m_nSelected, text);
		m_lbStrings.DeleteString(m_nSelected);
		m_lbStrings.InsertString(m_nSelected+1, text);
		m_nSelected++;
		m_lbStrings.SetCurSel(m_nSelected);
		updateButtons();
	}
}

void sspConcatStringEditor::updateButtons()
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_nSelected >= 0);
	GetDlgItem(IDC_DOWN)->EnableWindow(m_nSelected >= 0 && m_nSelected + 1 < (int) m_nStrings.size());
	GetDlgItem(IDC_UP)->EnableWindow(m_nSelected > 0);
}

void sspConcatStringEditor::OnSelectString() 
{
  m_nSelected = m_lbStrings.GetCurSel();
  updateButtons();
}

void sspConcatStringEditor::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nStrings.size() < 2) {
		AfxMessageBox(L"Please select at least two strings.");
		return;
	}
	CDialog::OnOK();
  m_editString.setStringList(m_nStrings);
	if (m_pString)
		*((sspConcatString*) m_pString) = m_editString;
	else
		m_pString = new sspConcatString(m_editString);
}

// sspAlphabetEditor dialog

IMPLEMENT_DYNAMIC(sspAlphabetEditor, CDialog)

sspAlphabetEditor::sspAlphabetEditor(CWnd* pParent /*=NULL*/)
	: sspGUIStringEditor(sspAlphabetEditor::IDD, pParent)
  , m_alphabet(_T(""))
{
  m_strName = _T("");
}

sspAlphabetEditor::~sspAlphabetEditor()
{
}

void sspAlphabetEditor::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_ALPHABET, m_alphabet);
	DDX_Text(pDX, IDC_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(sspAlphabetEditor, CDialog)
	ON_BN_CLICKED(IDC_SOURCE_VALUE, OnSourceValue)
END_MESSAGE_MAP()

BOOL sspAlphabetEditor::OnInitDialog() 
{
	if (m_pString) {
		sspAlphabetString* pString = dynamic_cast<sspAlphabetString*>(m_pString);
		if (!pString) {
			AfxMessageBox(L"Unable to assign string.");
			EndDialog(IDCANCEL);
		}
		m_editString = *pString;
	}
  convert(m_editString.getStrings(), m_alphabet);
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	if (m_editString.getValueIndex() >= 0)
		GetDlgItem(IDC_SOURCE_VALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editString.getValueIndex()));
	return TRUE;
}

void sspAlphabetEditor::OnSourceValue() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    m_editString.setValueIndex(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SOURCE_VALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editString.getValueIndex()));
	}
}

void sspAlphabetEditor::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editString.getValueIndex() < 0 || m_editString.getValueIndex() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid value selection.");
		return;
	}
	sspStrVec strVec;
  if (!convert(m_alphabet, strVec)) {
		AfxMessageBox(L"Unable to parse the alphabet, or too few elements.");
		return;
	}

  CDialog::OnOK();
  m_editString.setStrings(strVec);
	if (m_pString)
		*((sspAlphabetString*) m_pString) = m_editString;
	else
		m_pString = new sspAlphabetString(m_editString);
}

bool sspAlphabetEditor::convert(const CString& cstr, sspStrVec& strVec)
{
  CString strTemp = cstr;
  strTemp.Remove(' ');
  if (strTemp.IsEmpty()) {
    strVec.clear();
    return false;
  }
  else {
    int nCurPos = 0;
    CString resToken = strTemp.Tokenize(_T(","), nCurPos);
    while (resToken != "") {
      if (resToken.GetLength() == 3 && resToken.Find('-',0) == 1) {
        wchar_t first = resToken[0];
				wchar_t last = resToken[2];
        if (last < first || (last - first) > 26) {
          // Illegal format
          strVec.clear();
          return false;
        }
        else {
					for (wchar_t ch = first; ch <= last; ++ch)
						strVec.push_back(sspString(1, ch));
        }
      }
      else {
        strVec.push_back(resToken.GetBuffer());
      }
      resToken= strTemp.Tokenize(_T(","),nCurPos);
    }
  }
  return strVec.size() > 1;
};

void sspAlphabetEditor::convert(const sspStrVec& strVec, CString& cstr)
{
  if (strVec.empty()) {
    cstr.Empty();
    return;
  }
  else {
    cstr = strVec[0].c_str();
    for (size_t i=1; i< strVec.size(); ++i) {
      cstr += ", ";
      cstr += strVec[i].c_str();
    }
  }
}
