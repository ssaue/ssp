// SSpGUIPlayObjectEditor.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSpace.h"
#include "SSpGUIPlayObjectEditor.h"
#include "SSpGUIUtilityEditor.h"
#include "sspPools.h"
#include "SSpPlayObjects.h"
#include "PlayObjectDialog.h"
#include "ConditionalDialog.h"
#include "ValueDialog.h"
#include "sspGUIStringDialog.h"
#include "sspDeviceManager.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectWavDialog dialog


CObjectWavDialog::CObjectWavDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectWavDialog::IDD, pParent)
	, m_bFolder(FALSE)
{
	//{{AFX_DATA_INIT(CObjectWavDialog)
	//}}AFX_DATA_INIT
}


void CObjectWavDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectWavDialog)
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_FOLDER, m_bFolder);
}


BEGIN_MESSAGE_MAP(CObjectWavDialog, CDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_FILTER, OnFileFilter)
	ON_BN_CLICKED(IDC_FOLDER, OnCheckFolder)
END_MESSAGE_MAP()

BOOL CObjectWavDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpWavFile* pObj = dynamic_cast<SSpWavFile*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strFileName.Add(m_editObj.getFileName());
	}
	m_bFolder = m_editObj.getFolder();
	CDialog::OnInitDialog();
	GetDlgItem(IDC_FILENAME)->SetWindowText(m_editObj.getFileName());
	if (m_editObj.getFilterIndex() >= 0) 
    GetDlgItem(IDC_FILTER)->SetWindowText(sspPool::Instance().strings.GetName(m_editObj.getFilterIndex()));
  GetDlgItem(IDC_FILTER)->EnableWindow(m_bFolder);
	SetWindowText(m_strTitle);
	return TRUE;
}

void CObjectWavDialog::OnCheckFolder()
{
	m_bFolder = !m_bFolder;
  GetDlgItem(IDC_FILTER)->EnableWindow(m_bFolder);
	if (m_bFolder && !m_strFileName.IsEmpty()) {
		m_strFileName.SetSize(1);
		int nChar = m_strFileName.GetAt(0).ReverseFind('\\');
		m_strFileName.SetAt(0, m_strFileName.GetAt(0).Left(nChar));
		GetDlgItem(IDC_FILENAME)->SetWindowText(m_strFileName.GetAt(0));
	}
	else {
		m_strFileName.RemoveAll();
		GetDlgItem(IDC_FILENAME)->SetWindowText(_T(""));
	}
}

void CObjectWavDialog::OnFileFilter()
{
	sspGUIStringDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    m_editObj.setFilterIndex(dlg.m_nSelection[0]);
		GetDlgItem(IDC_FILTER)->SetWindowText(sspPool::Instance().strings.GetName(m_editObj.getFilterIndex()));
	}
}

void CObjectWavDialog::OnBrowse() 
{
	if (m_bFolder) {
		CString strFolder;
		if (sspGUIfileDialogs::SelectFolder(
				GetSafeHwnd(), 
				_T("Select an audio folder"), 
				strFolder)) {
			m_strFileName.SetSize(1);
			m_strFileName.SetAt(0, strFolder);
		}
		else
			return;
	}
	else {
		if (m_bMultipleSelect) {
			if (!sspGUIfileDialogs::SelectMultipleFiles(
					GetSafeHwnd(), 
					_T("Select one or more audio files"),
					_T("All Files (*.*)\0*.*\0WAV files (*.wav)\0*.wav\0AIFF files (*.aiff)\0*.aiff\0AU files (*.au)\0*.au\0RAW files (*.raw)\0*.raw\0"),
					m_strFileName)) {
				return;
			}
		}
		else {
			CString strFile;
			if (sspGUIfileDialogs::SelectFile(
					GetSafeHwnd(), 
					_T("Select an audio file"),
					_T("All Files (*.*)\0*.*\0WAV files (*.wav)\0*.wav\0AIFF files (*.aiff)\0*.aiff\0AU files (*.au)\0*.au\0RAW files (*.raw)\0*.raw\0"),
					strFile)) {
				m_strFileName.SetSize(1);
				m_strFileName.SetAt(0, strFile);				
			}
			else {
				return;
			}
		}
	}
	if (m_strFileName.GetSize() == 1) {
		GetDlgItem(IDC_FILENAME)->SetWindowText(m_strFileName.GetAt(0));
	}
	else {
		CString text;
		text.Format(_T("%d files selected"), m_strFileName.GetSize());
		GetDlgItem(IDC_FILENAME)->SetWindowText(text);
	}
}

void CObjectWavDialog::OnOK()
{
	if (m_strFileName.IsEmpty()) {
		AfxMessageBox(_T("Please select at least one file or folder."));
		return;
	}
	CDialog::OnOK();

	m_editObj.setFolder(m_bFolder == TRUE);
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strFileName.GetAt(0);
		m_editObj.setFileName(m_pObjs[0]->strName);
		*((SSpWavFile*)m_pObjs[0]->pObj) = m_editObj;
		int nChar = m_pObjs[0]->strName.GetLength() - m_pObjs[0]->strName.ReverseFind('\\');
		m_pObjs[0]->strName = m_pObjs[0]->strName.Right(nChar-1);
	}
	else {
		for (int i=0; i<m_strFileName.GetSize(); ++i) {
			sspMultipleObject* pMO = new sspMultipleObject;
			pMO->strName = m_strFileName.GetAt(i);
			SSpWavFile* pNew = new SSpWavFile(m_editObj);
			pNew->setFileName(pMO->strName);
			pMO->pObj = pNew;
			int nChar = pMO->strName.GetLength() - pMO->strName.ReverseFind('\\');
			pMO->strName = pMO->strName.Right(nChar-1);
			m_pObjs.push_back(pMO);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectMidiDialog dialog


CObjectMidiDialog::CObjectMidiDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectMidiDialog::IDD, pParent)
	, m_nDevice(0)
	, m_bFolder(FALSE)
{
	//{{AFX_DATA_INIT(CObjectMidiDialog)
	//}}AFX_DATA_INIT
}


void CObjectMidiDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectMidiDialog)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DEVICES, m_lbDevice);
	DDX_LBIndex(pDX, IDC_DEVICES, m_nDevice);
	DDX_Check(pDX, IDC_CHECK1, m_bFolder);
}


BEGIN_MESSAGE_MAP(CObjectMidiDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectMidiDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TEMPOFACTOR, OnTempofactor)
	ON_BN_CLICKED(IDC_CHECK1, OnCheckFolder)
END_MESSAGE_MAP()


BOOL CObjectMidiDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpMidiFile* pObj = dynamic_cast<SSpMidiFile*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strFileName.Add(m_editObj.getFileName());
	}
	m_nDevice = m_editObj.getMidiDevice();
	m_bFolder = m_editObj.getFolder();
	CDialog::OnInitDialog();
	m_lbDevice.ResetContent();
	sspDeviceGroup* pGroup = sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT];
	for (unsigned int i=0; i<pGroup->getSubsetSize(); i++)
		m_lbDevice.InsertString(-1, pGroup->getSubsetName(i).c_str());
	m_lbDevice.SetCurSel(m_nDevice);
	GetDlgItem(IDC_FILENAME)->SetWindowText(m_editObj.getFileName());
	SetWindowText(m_strTitle);
	if (m_editObj.getTempoFactor() >= 0)
		GetDlgItem(IDC_TEMPOFACTOR)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getTempoFactor()));

	return TRUE;
}

void CObjectMidiDialog::OnCheckFolder()
{
	m_bFolder = !m_bFolder;
	if (m_bFolder && !m_strFileName.IsEmpty()) {
		m_strFileName.SetSize(1);
		int nChar = m_strFileName.GetAt(0).ReverseFind('\\');
		m_strFileName.SetAt(0, m_strFileName.GetAt(0).Left(nChar));
		GetDlgItem(IDC_FILENAME)->SetWindowText(m_strFileName.GetAt(0));
	}
	else {
		m_strFileName.RemoveAll();
		GetDlgItem(IDC_FILENAME)->SetWindowText(_T(""));
	}
}

void CObjectMidiDialog::OnBrowse() 
{
	if (m_bFolder) {
		CString strFolder;
		if (sspGUIfileDialogs::SelectFolder(
				GetSafeHwnd(), 
				_T("Select a Midi folder"),
				strFolder)) {
			m_strFileName.SetSize(1);
			m_strFileName.SetAt(0, strFolder);
		}
		else
			return;
	}
	else {
		if (m_bMultipleSelect) {
			if (!sspGUIfileDialogs::SelectMultipleFiles(
					GetSafeHwnd(), 
					_T("Select one or more MIDI files"),
					_T("Midi Files (*.mid)\0*.mid\0"),
					m_strFileName)) {
				return;
			}
		}
		else {
			CString strFile;
			if (sspGUIfileDialogs::SelectFile(
					GetSafeHwnd(), 
					_T("Specify a MIDI file"),
					_T("Midi Files (*.mid)\0*.mid\0"),
					strFile)) {
				m_strFileName.SetSize(1);
				m_strFileName.SetAt(0, strFile);				
			}
			else {
				return;
			}
		}
	}
	if (m_strFileName.GetSize() == 1) {
		GetDlgItem(IDC_FILENAME)->SetWindowText(m_strFileName.GetAt(0));
	}
	else {
		CString text;
		text.Format(_T("%d files selected"), m_strFileName.GetSize());
		GetDlgItem(IDC_FILENAME)->SetWindowText(text);
	}
}

void CObjectMidiDialog::OnTempofactor()
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setTempoFactor(dlg.m_nSelection[0]);
		GetDlgItem(IDC_TEMPOFACTOR)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getTempoFactor()));
	}
}

void CObjectMidiDialog::OnOK()
{
	UpdateData(TRUE);
	m_editObj.setMidiDevice(m_nDevice);
	if (m_strFileName.IsEmpty()) {
		AfxMessageBox(L"Please select at least one file.");
		return;
	}
	if (m_editObj.getTempoFactor() < 0 || m_editObj.getTempoFactor() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid tempo factor selection.");
		return;
	}
	if (m_editObj.getMidiDevice() < 0 || m_editObj.getMidiDevice() > (int)sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT]->getSubsetSize()) {
		AfxMessageBox(L"Invalid MIDI device selection.");
		return;
	}
	CDialog::OnOK();
	m_editObj.setFolder(m_bFolder == TRUE);
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strFileName.GetAt(0);
		m_editObj.setFileName(m_pObjs[0]->strName);
		*((SSpMidiFile*)m_pObjs[0]->pObj) = m_editObj;
		int nChar = m_pObjs[0]->strName.GetLength() - m_pObjs[0]->strName.ReverseFind('\\');
		m_pObjs[0]->strName = m_pObjs[0]->strName.Right(nChar-1);
	}
	else {
		for (int i=0; i<m_strFileName.GetSize(); ++i) {
			sspMultipleObject* pMO = new sspMultipleObject;
			pMO->strName = m_strFileName.GetAt(i);
			SSpMidiFile* pNew = new SSpMidiFile(m_editObj);
			pNew->setFileName(pMO->strName);
			pMO->pObj = pNew;
			int nChar = pMO->strName.GetLength() - pMO->strName.ReverseFind('\\');
			pMO->strName = pMO->strName.Right(nChar-1);
			m_pObjs.push_back(pMO);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectDSControlDialog dialog


CObjectDSControlDialog::CObjectDSControlDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectDSControlDialog::IDD, pParent)
	, m_bFolder(FALSE)
{
	//{{AFX_DATA_INIT(CObjectDSControlDialog)
	//}}AFX_DATA_INIT
}


void CObjectDSControlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectDSControlDialog)
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK1, m_bFolder);
}


BEGIN_MESSAGE_MAP(CObjectDSControlDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectDSControlDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, OnCheckFolder)
END_MESSAGE_MAP()

BOOL CObjectDSControlDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpDSFile* pObj = dynamic_cast<SSpDSFile*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strFileName.Add(m_editObj.getFileName());
	}
	m_bFolder = m_editObj.getFolder();
	CDialog::OnInitDialog();
	GetDlgItem(IDC_FILENAME)->SetWindowText(m_editObj.getFileName());
	SetWindowText(m_strTitle);
	return TRUE;
}

void CObjectDSControlDialog::OnCheckFolder()
{
	m_bFolder = !m_bFolder;
	if (m_bFolder && !m_strFileName.IsEmpty()) {
		m_strFileName.SetSize(1);
		int nChar = m_strFileName.GetAt(0).ReverseFind('\\');
		m_strFileName.SetAt(0, m_strFileName.GetAt(0).Left(nChar));
		GetDlgItem(IDC_FILENAME)->SetWindowText(m_strFileName.GetAt(0));
	}
	else {
		m_strFileName.RemoveAll();
		GetDlgItem(IDC_FILENAME)->SetWindowText(_T(""));
	}
}

void CObjectDSControlDialog::OnBrowse() 
{
	if (m_bFolder) {
		CString strFolder;
		if (sspGUIfileDialogs::SelectFolder(
				GetSafeHwnd(), 
				_T("Select a DirectSound control folder"),
				strFolder)) {
			m_strFileName.SetSize(1);
			m_strFileName.SetAt(0, strFolder);
		}
		else
			return;
	}
	else {
		if (m_bMultipleSelect) {
			if (!sspGUIfileDialogs::SelectMultipleFiles(
					GetSafeHwnd(), 
					_T("Select one or more DirectSound control files"),
					_T("DirectSound Control Files (*.dsc)\0*.dsc\0"),
					m_strFileName)) {
				return;
			}
		}
		else {
			CString strFile;
			if (sspGUIfileDialogs::SelectFile(
					GetSafeHwnd(), 
					_T("Select a DirectSound control file"),
					_T("DirectSound Control Files (*.dsc)\0*.dsc\0"),
					strFile)) {
				m_strFileName.SetSize(1);
				m_strFileName.SetAt(0, strFile);				
			}
			else {
				return;
			}
		}
	}
	if (m_strFileName.GetSize() == 1) {
		GetDlgItem(IDC_FILENAME)->SetWindowText(m_strFileName.GetAt(0));
	}
	else {
		CString text;
		text.Format(_T("%d files selected"), m_strFileName.GetSize());
		GetDlgItem(IDC_FILENAME)->SetWindowText(text);
	}
}

void CObjectDSControlDialog::OnOK()
{
	if (m_strFileName.IsEmpty()) {
		AfxMessageBox(_T("Please select at least one file."));
		return;
	}
	CDialog::OnOK();
	m_editObj.setFolder(m_bFolder == TRUE);

	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strFileName.GetAt(0);
		m_editObj.setFileName(m_pObjs[0]->strName);
		*((SSpDSFile*)m_pObjs[0]->pObj) = m_editObj;
		int nChar = m_pObjs[0]->strName.GetLength() - m_pObjs[0]->strName.ReverseFind('\\');
		m_pObjs[0]->strName = m_pObjs[0]->strName.Right(nChar-1);
	}
	else {
		for (int i=0; i<m_strFileName.GetSize(); ++i) {
			sspMultipleObject* pMO = new sspMultipleObject;
			pMO->strName = m_strFileName.GetAt(i);
			SSpDSFile* pNew = new SSpDSFile(m_editObj);
			pNew->setFileName(pMO->strName);
			pMO->pObj = pNew;
			int nChar = pMO->strName.GetLength() - pMO->strName.ReverseFind('\\');
			pMO->strName = pMO->strName.Right(nChar-1);
			m_pObjs.push_back(pMO);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectMidiListDialog dialog


CObjectMidiListDialog::CObjectMidiListDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectMidiListDialog::IDD, pParent)
	, m_bFolder(FALSE)
{
	//{{AFX_DATA_INIT(CObjectMidiListDialog)
	m_nDevice = 0;
	//}}AFX_DATA_INIT
}


void CObjectMidiListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectMidiListDialog)
	DDX_LBIndex(pDX, IDC_DEVICES, m_nDevice);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DEVICES, m_lbDevice);
	DDX_Check(pDX, IDC_CHECK1, m_bFolder);
}


BEGIN_MESSAGE_MAP(CObjectMidiListDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectMidiListDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, OnCheckFolder)
END_MESSAGE_MAP()

BOOL CObjectMidiListDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpMidiEventList* pObj = dynamic_cast<SSpMidiEventList*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strFileName.Add(m_editObj.getFileName());
	}
	m_nDevice = m_editObj.getMidiDevice();
	m_bFolder = m_editObj.getFolder();
	CDialog::OnInitDialog();
	m_lbDevice.ResetContent();
	sspDeviceGroup* pGroup = sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT];
	for (unsigned int i=0; i<pGroup->getSubsetSize(); i++)
		m_lbDevice.InsertString(-1, pGroup->getSubsetName(i).c_str());
	m_lbDevice.SetCurSel(m_nDevice);
	GetDlgItem(IDC_FILENAME)->SetWindowText(m_editObj.getFileName());
	SetWindowText(m_strTitle);
	return TRUE;
}

void CObjectMidiListDialog::OnCheckFolder()
{
	m_bFolder = !m_bFolder;
	if (m_bFolder && !m_strFileName.IsEmpty()) {
		m_strFileName.SetSize(1);
		int nChar = m_strFileName.GetAt(0).ReverseFind('\\');
		m_strFileName.SetAt(0, m_strFileName.GetAt(0).Left(nChar));
		GetDlgItem(IDC_FILENAME)->SetWindowText(m_strFileName.GetAt(0));
	}
	else {
		m_strFileName.RemoveAll();
		GetDlgItem(IDC_FILENAME)->SetWindowText(_T(""));
	}
}

void CObjectMidiListDialog::OnBrowse() 
{
	if (m_bFolder) {
		CString strFolder;
		if (sspGUIfileDialogs::SelectFolder(
				GetSafeHwnd(), 
				_T("Select a MIDI event folder"),
				strFolder)) {
			m_strFileName.SetSize(1);
			m_strFileName.SetAt(0, strFolder);
		}
		else
			return;
	}
	else {
		if (m_bMultipleSelect) {
			if (!sspGUIfileDialogs::SelectMultipleFiles(
					GetSafeHwnd(), 
					_T("Select one or more MIDI event files"),
					_T("Midi Event Files (*.evt)\0*.evt\0"),
					m_strFileName)) {
				return;
			}
		}
		else {
			CString strFile;
			if (sspGUIfileDialogs::SelectFile(
					GetSafeHwnd(), 
					_T("Select a MIDI event file"),
					_T("Midi Event Files (*.evt)\0*.evt\0"),
					strFile)) {
				m_strFileName.SetSize(1);
				m_strFileName.SetAt(0, strFile);				
			}
			else {
				return;
			}
		}
	}
	if (m_strFileName.GetSize() == 1) {
		GetDlgItem(IDC_FILENAME)->SetWindowText(m_strFileName.GetAt(0));
	}
	else {
		CString text;
		text.Format(_T("%d files selected"), m_strFileName.GetSize());
		GetDlgItem(IDC_FILENAME)->SetWindowText(text);
	}
}

void CObjectMidiListDialog::OnOK()
{
	UpdateData(TRUE);
	m_editObj.setMidiDevice(m_nDevice);
	if (m_strFileName.IsEmpty()) {
		AfxMessageBox(L"Please select at least one file.");
		return;
	}
	if (m_editObj.getMidiDevice() < 0 || m_editObj.getMidiDevice() > (int)sspDeviceManager::Instance()[SSP_DEVICE_MIDIOUT]->getSubsetSize()) {
		AfxMessageBox(L"Invalid MIDI device selection.");
		return;
	}
	CDialog::OnOK();
	m_editObj.setFolder(m_bFolder == TRUE);
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strFileName.GetAt(0);
		m_editObj.setFileName(m_pObjs[0]->strName);
		*((SSpMidiEventList*)m_pObjs[0]->pObj) = m_editObj;
		int nChar = m_pObjs[0]->strName.GetLength() - m_pObjs[0]->strName.ReverseFind('\\');
		m_pObjs[0]->strName = m_pObjs[0]->strName.Right(nChar-1);
	}
	else {
		for (int i=0; i<m_strFileName.GetSize(); ++i) {
			sspMultipleObject* pMO = new sspMultipleObject;
			pMO->strName = m_strFileName.GetAt(i);
			SSpMidiEventList* pNew = new SSpMidiEventList(m_editObj);
			pNew->setFileName(pMO->strName);
			pMO->pObj = pNew;
			int nChar = pMO->strName.GetLength() - pMO->strName.ReverseFind('\\');
			pMO->strName = pMO->strName.Right(nChar-1);
			m_pObjs.push_back(pMO);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectDMXDialog dialog


CObjectDMXDialog::CObjectDMXDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectDMXDialog::IDD, pParent)
	, m_bFolder(FALSE)
{
	//{{AFX_DATA_INIT(CObjectDMXDialog)
	m_nRefreshInterval = 100;
	m_nDevice = 0;
	//}}AFX_DATA_INIT
}

void CObjectDMXDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectDMXDialog)
	DDX_Text(pDX, IDC_REFRESH, m_nRefreshInterval);
	DDX_LBIndex(pDX, IDC_DEVICES, m_nDevice);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DEVICES, m_lbDevice);
	DDX_Check(pDX, IDC_CHECK1, m_bFolder);
}


BEGIN_MESSAGE_MAP(CObjectDMXDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectDMXDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_BTN_DURATION, OnDuration)
	ON_BN_CLICKED(IDC_BTN_SELECTINDEX, OnSelectindex)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, OnCheckFolder)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectDMXDialog message handlers

BOOL CObjectDMXDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpDmxLivePlayer* pObj = dynamic_cast<SSpDmxLivePlayer*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strFileName.Add(m_editObj.getFileName());
	}
	m_nDevice = m_editObj.getDevice();
	m_nRefreshInterval = m_editObj.getRefreshRate();
	m_bFolder = m_editObj.getFolder();
	CDialog::OnInitDialog();
	m_lbDevice.ResetContent();
	sspDeviceGroup* pGroup = sspDeviceManager::Instance()[SSP_DEVICE_DMX];
	for (unsigned int i=0; i<pGroup->getSubsetSize(); i++)
		m_lbDevice.InsertString(-1, pGroup->getSubsetName(i).c_str());
	m_lbDevice.SetCurSel(m_nDevice);
	GetDlgItem(IDC_FILENAME)->SetWindowText(m_editObj.getFileName());
	SetWindowText(m_strTitle);
	if (m_editObj.getDuration() >= 0)
		GetDlgItem(IDC_BTN_DURATION)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getDuration()));
	if (m_editObj.getSelector() >= 0)
		GetDlgItem(IDC_BTN_SELECTINDEX)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getSelector()));

	return TRUE;
}

void CObjectDMXDialog::OnCheckFolder()
{
	m_bFolder = !m_bFolder;
	if (m_bFolder && !m_strFileName.IsEmpty()) {
		m_strFileName.SetSize(1);
		int nChar = m_strFileName.GetAt(0).ReverseFind('\\');
		m_strFileName.SetAt(0, m_strFileName.GetAt(0).Left(nChar));
		GetDlgItem(IDC_FILENAME)->SetWindowText(m_strFileName.GetAt(0));
	}
	else {
		m_strFileName.RemoveAll();
		GetDlgItem(IDC_FILENAME)->SetWindowText(_T(""));
	}
}

void CObjectDMXDialog::OnBrowse() 
{
	if (m_bFolder) {
		CString strFolder;
		if (sspGUIfileDialogs::SelectFolder(
				GetSafeHwnd(), 
				_T("Select a DMX control folder"),
				strFolder)) {
			m_strFileName.SetSize(1);
			m_strFileName.SetAt(0, strFolder);
		}
		else
			return;
	}
	else {
		if (m_bMultipleSelect) {
			if (!sspGUIfileDialogs::SelectMultipleFiles(
					GetSafeHwnd(), 
					_T("Select one or more DMX control files"),
					_T("DMX Files (*.dmx)\0*.dmx\0"),
					m_strFileName)) {
				return;
			}
		}
		else {
			CString strFile;
			if (sspGUIfileDialogs::SelectFile(
					GetSafeHwnd(), 
					_T("Select a DMX control file"),
					_T("DMX Files (*.dmx)\0*.dmx\0"),
					strFile)) {
				m_strFileName.SetSize(1);
				m_strFileName.SetAt(0, strFile);				
			}
			else {
				return;
			}
		}
	}
	if (m_strFileName.GetSize() == 1) {
		GetDlgItem(IDC_FILENAME)->SetWindowText(m_strFileName.GetAt(0));
	}
	else {
		CString text;
		text.Format(_T("%d files selected"), m_strFileName.GetSize());
		GetDlgItem(IDC_FILENAME)->SetWindowText(text);
	}
}

void CObjectDMXDialog::OnDuration() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setDuration(dlg.m_nSelection[0]);
		GetDlgItem(IDC_BTN_DURATION)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getDuration()));
	}
	else {
		m_editObj.setDuration(-1);
		GetDlgItem(IDC_BTN_DURATION)->SetWindowText(_T("Unlimited duration"));
	}
}

void CObjectDMXDialog::OnSelectindex() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setSelector(dlg.m_nSelection[0]);
		GetDlgItem(IDC_BTN_SELECTINDEX)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getSelector()));
	}
}

void CObjectDMXDialog::OnOK()
{
	UpdateData(TRUE);
	m_editObj.setDmxDevice(m_nDevice);
	m_editObj.setRefreshRate(m_nRefreshInterval);
	if (m_strFileName.IsEmpty()) {
		AfxMessageBox(L"Please select at least one file.");
		return;
	}
	if (m_editObj.getDuration() < 0 || m_editObj.getDuration() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid duration selection.");
		return;
	}
	if (m_editObj.getSelector() < 0 || m_editObj.getSelector() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid index selection.");
		return;
	}
	if (m_editObj.getDevice() < 0 || m_editObj.getDevice() > (int)sspDeviceManager::Instance()[SSP_DEVICE_DMX]->getSubsetSize()) {
		AfxMessageBox(L"Invalid DMX device selection.");
		return;
	}
	CDialog::OnOK();
	m_editObj.setFolder(m_bFolder == TRUE);
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strFileName.GetAt(0);
		m_editObj.setFileName(m_pObjs[0]->strName);
		*((SSpDmxLivePlayer*)m_pObjs[0]->pObj) = m_editObj;
		int nChar = m_pObjs[0]->strName.GetLength() - m_pObjs[0]->strName.ReverseFind('\\');
		m_pObjs[0]->strName = m_pObjs[0]->strName.Right(nChar-1);
	}
	else {
		for (int i=0; i<m_strFileName.GetSize(); ++i) {
			sspMultipleObject* pMO = new sspMultipleObject;
			pMO->strName = m_strFileName.GetAt(i);
			SSpDmxLivePlayer* pNew = new SSpDmxLivePlayer(m_editObj);
			pNew->setFileName(pMO->strName);
			pMO->pObj = pNew;
			int nChar = pMO->strName.GetLength() - pMO->strName.ReverseFind('\\');
			pMO->strName = pMO->strName.Right(nChar-1);
			m_pObjs.push_back(pMO);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectSilenceDialog dialog


CObjectSilenceDialog::CObjectSilenceDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectSilenceDialog::IDD, pParent)
	, m_strName(_T(""))
{
}


void CObjectSilenceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(CObjectSilenceDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectSilenceDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DURATION_BTN, OnSelectDuration)
END_MESSAGE_MAP()

BOOL CObjectSilenceDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpSilence* pObj = dynamic_cast<SSpSilence*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strName = m_pObjs[0]->strName;
	}
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	if (m_editObj.getLengthValue() >= 0)
		GetDlgItem(IDC_DURATION_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getLengthValue()));

	return TRUE;
}

void CObjectSilenceDialog::OnSelectDuration()
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setLengthValue(dlg.m_nSelection[0]);
		GetDlgItem(IDC_DURATION_BTN)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getLengthValue()));
	}
}

void CObjectSilenceDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editObj.getLengthValue() < 0 || m_editObj.getLengthValue() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid duration selection.");
		return;
	}
	CDialog::OnOK();
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strName;
		*((SSpSilence*)m_pObjs[0]->pObj) = m_editObj;
	}
	else {
		sspMultipleObject* pMO = new sspMultipleObject;
		SSpSilence* pNew = new SSpSilence(m_editObj);
		pMO->pObj = pNew;
		pMO->strName = m_strName;
		m_pObjs.push_back(pMO);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectSimultaneousDialog dialog


CObjectSimultaneousDialog::CObjectSimultaneousDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectSimultaneousDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectSimultaneousDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_nSelected = -1;
}

CObjectSimultaneousDialog::~CObjectSimultaneousDialog()
{
}

void CObjectSimultaneousDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectSimultaneousDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_OBJECTLIST, m_lbObjects);
}


BEGIN_MESSAGE_MAP(CObjectSimultaneousDialog, CDialog)
	ON_LBN_SELCHANGE(IDC_OBJECTLIST, OnSelectObject)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectSimultaneousDialog message handlers

BOOL CObjectSimultaneousDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpSimultaneousGroup* pObj = dynamic_cast<SSpSimultaneousGroup*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strName = m_pObjs[0]->strName;
	}
	m_nObjects = m_editObj.getPlayObjects();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	UpdateButtons();
	m_lbObjects.ResetContent();
	for (unsigned int i=0; i<m_nObjects.size(); i++)
		m_lbObjects.AddString(sspPool::Instance().objects.GetName(m_nObjects[i]));
	return TRUE;
}

void CObjectSimultaneousDialog::OnSelectObject()
{
	m_nSelected = m_lbObjects.GetCurSel();
	UpdateButtons();
}

void CObjectSimultaneousDialog::OnAdd()
{
	CPlayObjectDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nObjects.insert(m_nObjects.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbObjects.AddString(sspPool::Instance().objects.GetName(dlg.m_nSelection[i]));
		}
		m_nSelected = m_nObjects.size()-1;
		m_lbObjects.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CObjectSimultaneousDialog::OnRemove()
{
	if (m_nSelected >= 0 && m_nSelected < (int) m_nObjects.size()) {
		m_nObjects.erase(m_nObjects.begin() + m_nSelected);
		m_lbObjects.DeleteString(m_nSelected);
		if (m_nSelected == (int) m_nObjects.size()) m_nSelected--;
		m_lbObjects.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CObjectSimultaneousDialog::UpdateButtons()
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_nSelected >= 0);
}

void CObjectSimultaneousDialog::OnOK() 
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nObjects.size() < 2) {
		AfxMessageBox(L"Please select at least two objects.");
		return;
	}
	CDialog::OnOK();
	m_editObj.setPlayObjects(m_nObjects);
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strName;
		*((SSpSimultaneousGroup*)m_pObjs[0]->pObj) = m_editObj;
	}
	else {
		sspMultipleObject* pMO = new sspMultipleObject;
		SSpSimultaneousGroup* pNew = new SSpSimultaneousGroup(m_editObj);
		pMO->pObj = pNew;
		pMO->strName = m_strName;
		m_pObjs.push_back(pMO);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectSequentialDialog dialog


CObjectSequentialDialog::CObjectSequentialDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectSequentialDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectSequentialDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_nSelected = -1;
}


void CObjectSequentialDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectSequentialDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_OBJECTLIST, m_lbObjects);
}


BEGIN_MESSAGE_MAP(CObjectSequentialDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectSequentialDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_OBJECTLIST, OnSelectObject)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
END_MESSAGE_MAP()


BOOL CObjectSequentialDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpSequentialGroup* pObj = dynamic_cast<SSpSequentialGroup*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strName = m_pObjs[0]->strName;
	}
	m_nObjects = m_editObj.getPlayObjects();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	UpdateButtons();
	m_lbObjects.ResetContent();
	for (unsigned int i=0; i<m_nObjects.size(); i++)
		m_lbObjects.AddString(sspPool::Instance().objects.GetName(m_nObjects[i]));
	return TRUE;
}

void CObjectSequentialDialog::OnAdd() 
{
	CPlayObjectDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nObjects.insert(m_nObjects.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbObjects.AddString(sspPool::Instance().objects.GetName(dlg.m_nSelection[i]));
		}
		m_nSelected = m_nObjects.size()-1;
		m_lbObjects.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CObjectSequentialDialog::OnRemove() 
{
	if (m_nSelected >= 0 && m_nSelected < (int) m_nObjects.size()) {
		m_nObjects.erase(m_nObjects.begin() + m_nSelected);
		m_lbObjects.DeleteString(m_nSelected);
		if (m_nSelected == (int) m_nObjects.size()) m_nSelected--;
		m_lbObjects.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CObjectSequentialDialog::OnSelectObject()
{
	m_nSelected = m_lbObjects.GetCurSel();
	UpdateButtons();
}

void CObjectSequentialDialog::OnUp()
{
	CString text;
	if (m_nSelected > 0) {
		int nObj = m_nObjects[m_nSelected];
		m_nObjects[m_nSelected] = m_nObjects[m_nSelected-1];
		m_nObjects[m_nSelected-1] = nObj;
		m_lbObjects.GetText(m_nSelected, text);
		m_lbObjects.DeleteString(m_nSelected);
		m_lbObjects.InsertString(m_nSelected-1, text);
		m_nSelected--;
		m_lbObjects.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CObjectSequentialDialog::OnDown()
{
	CString text;
	if (m_nSelected + 1 < (int) m_nObjects.size()) {
		int nObj = m_nObjects[m_nSelected];
		m_nObjects[m_nSelected] = m_nObjects[m_nSelected+1];
		m_nObjects[m_nSelected+1] = nObj;
		m_lbObjects.GetText(m_nSelected, text);
		m_lbObjects.DeleteString(m_nSelected);
		m_lbObjects.InsertString(m_nSelected+1, text);
		m_nSelected++;
		m_lbObjects.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CObjectSequentialDialog::UpdateButtons()
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_nSelected >= 0);
	GetDlgItem(IDC_DOWN)->EnableWindow(m_nSelected >= 0 && m_nSelected + 1 < (int) m_nObjects.size());
	GetDlgItem(IDC_UP)->EnableWindow(m_nSelected > 0);
}

void CObjectSequentialDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nObjects.size() < 2) {
		AfxMessageBox(L"Please select at least two objects.");
		return;
	}
	CDialog::OnOK();
	m_editObj.setPlayObjects(m_nObjects);
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strName;
		*((SSpSequentialGroup*)m_pObjs[0]->pObj) = m_editObj;
	}
	else {
		sspMultipleObject* pMO = new sspMultipleObject;
		SSpSequentialGroup* pNew = new SSpSequentialGroup(m_editObj);
		pMO->pObj = pNew;
		pMO->strName = m_strName;
		m_pObjs.push_back(pMO);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectLinearSelectDialog dialog


CObjectLinearSelectDialog::CObjectLinearSelectDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectLinearSelectDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectLinearSelectDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_nSelected = -1;
}


void CObjectLinearSelectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectLinearSelectDialog)
	DDX_Control(pDX, IDC_OBJECTLIST, m_lbObjects);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectLinearSelectDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectLinearSelectDialog)
	ON_BN_CLICKED(IDC_BTN_INPUTVALUE, OnBtnInputvalue)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_SELCHANGE(IDC_OBJECTLIST, OnSelectObject)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CObjectLinearSelectDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpLinearSelectGroup* pObj = dynamic_cast<SSpLinearSelectGroup*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strName = m_pObjs[0]->strName;
	}
	m_nObjects = m_editObj.getPlayObjects();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	UpdateButtons();
	m_lbObjects.ResetContent();
	for (unsigned int i=0; i<m_nObjects.size(); i++)
		m_lbObjects.AddString(sspPool::Instance().objects.GetName(m_nObjects[i]));
	if (m_editObj.getValue() >= 0)
		GetDlgItem(IDC_BTN_INPUTVALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getValue()));
	return TRUE;
}

void CObjectLinearSelectDialog::OnBtnInputvalue() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setValue(dlg.m_nSelection[0]);
		GetDlgItem(IDC_BTN_INPUTVALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getValue()));
	}
}

void CObjectLinearSelectDialog::OnAdd() 
{
	CPlayObjectDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nObjects.insert(m_nObjects.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_lbObjects.AddString(sspPool::Instance().objects.GetName(dlg.m_nSelection[i]));
		}
		m_nSelected = m_nObjects.size()-1;
		m_lbObjects.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}


void CObjectLinearSelectDialog::OnRemove() 
{
	if (m_nSelected >= 0 && m_nSelected < (int) m_nObjects.size()) {
		m_nObjects.erase(m_nObjects.begin() + m_nSelected);
		m_lbObjects.DeleteString(m_nSelected);
		if (m_nSelected == (int) m_nObjects.size()) m_nSelected--;
		m_lbObjects.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CObjectLinearSelectDialog::OnSelectObject() 
{
	m_nSelected = m_lbObjects.GetCurSel();
	UpdateButtons();
}

void CObjectLinearSelectDialog::OnUp() 
{
	CString text;
	if (m_nSelected > 0) {
		int nObj = m_nObjects[m_nSelected];
		m_nObjects[m_nSelected] = m_nObjects[m_nSelected-1];
		m_nObjects[m_nSelected-1] = nObj;
		m_lbObjects.GetText(m_nSelected, text);
		m_lbObjects.DeleteString(m_nSelected);
		m_lbObjects.InsertString(m_nSelected-1, text);
		m_nSelected--;
		m_lbObjects.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CObjectLinearSelectDialog::OnDown() 
{
	CString text;
	if (m_nSelected + 1 < (int) m_nObjects.size()) {
		int nObj = m_nObjects[m_nSelected];
		m_nObjects[m_nSelected] = m_nObjects[m_nSelected+1];
		m_nObjects[m_nSelected+1] = nObj;
		m_lbObjects.GetText(m_nSelected, text);
		m_lbObjects.DeleteString(m_nSelected);
		m_lbObjects.InsertString(m_nSelected+1, text);
		m_nSelected++;
		m_lbObjects.SetCurSel(m_nSelected);
		UpdateButtons();
	}
}

void CObjectLinearSelectDialog::UpdateButtons()
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_nSelected >= 0);
	GetDlgItem(IDC_DOWN)->EnableWindow(m_nSelected >= 0 && m_nSelected + 1 < (int) m_nObjects.size());
	GetDlgItem(IDC_UP)->EnableWindow(m_nSelected > 0);
}

void CObjectLinearSelectDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nObjects.size() < 2) {
		AfxMessageBox(L"Please select at least two objects.");
		return;
	}
	if (m_editObj.getValue() < 0 || m_editObj.getValue() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid value selection.");
		return;
	}
	CDialog::OnOK();
	m_editObj.setPlayObjects(m_nObjects);
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strName;
		*((SSpLinearSelectGroup*)m_pObjs[0]->pObj) = m_editObj;
	}
	else {
		sspMultipleObject* pMO = new sspMultipleObject;
		SSpLinearSelectGroup* pNew = new SSpLinearSelectGroup(m_editObj);
		pMO->pObj = pNew;
		pMO->strName = m_strName;
		m_pObjs.push_back(pMO);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectRandomDialog dialog

CObjectRandomDialog::CObjectRandomDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectRandomDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectRandomDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_nSelected = -1;
}

void CObjectRandomDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Control(pDX, IDC_WEIGHT_LIST, m_lbWeights);
}


BEGIN_MESSAGE_MAP(CObjectRandomDialog, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_NOTIFY(NM_DBLCLK, IDC_WEIGHT_LIST, OnNMDblclkWeightList)
	ON_NOTIFY(NM_RCLICK, IDC_WEIGHT_LIST, OnNMDblclkWeightList)
	ON_NOTIFY(NM_CLICK, IDC_WEIGHT_LIST, OnNMClickWeightList)
END_MESSAGE_MAP()


BOOL CObjectRandomDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpRandomGroup* pObj = dynamic_cast<SSpRandomGroup*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strName = m_pObjs[0]->strName;
	}
	m_nObjects = m_editObj.getPlayObjects();
	m_nConstWeight = m_editObj.getConstWeight();
	m_nVarWeight = m_editObj.getVariableWeight();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	UpdateButtons();

	CRect ctrlRect;
	m_lbWeights.GetClientRect(ctrlRect);
	int nRectWidth = ctrlRect.Width();
	int nConstWidth = nRectWidth / 10;
	nRectWidth = (nRectWidth - nConstWidth) / 2;
	m_lbWeights.InsertColumn(0, _T("Object"), LVCFMT_LEFT, nRectWidth, 0);
	m_lbWeights.InsertColumn(1, _T("Base"), LVCFMT_LEFT, nConstWidth, 1);
	m_lbWeights.InsertColumn(2, _T("Variable"), LVCFMT_LEFT, nRectWidth, 2);
	m_nSelected = -1;

  m_nSize = std::min(m_nObjects.size(), std::min(m_nConstWeight.size(),m_nVarWeight.size()));
	m_nObjects.resize(m_nSize);
	m_nConstWeight.resize(m_nSize);
	m_nVarWeight.resize(m_nSize);
	wchar_t number[10];
	for (unsigned int i=0; i<m_nSize; i++) {
		m_lbWeights.InsertItem(LVIF_TEXT|LVIF_STATE, i, 
		   sspPool::Instance().objects.GetName(m_nObjects[i]), 0, LVIS_SELECTED, 0, 0);
    _itow_s(m_nConstWeight[i],number,10,10);
		m_lbWeights.SetItemText(i, 1, number);
		if (m_nVarWeight[i] >= 0)
			m_lbWeights.SetItemText(i, 2, sspPool::Instance().values.GetName(m_nVarWeight[i]));
	}
	m_lbWeights.SetExtendedStyle(m_lbWeights.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	return TRUE;
}

void CObjectRandomDialog::OnNMClickWeightList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_lbWeights.SetItemState(m_nSelected, 0, LVIS_SELECTED | LVIS_FOCUSED);
	m_nSelected = pNMLV->iItem;
	if (m_nSelected == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_lbWeights.SubItemHitTest(&lvhti);
		m_nSelected = lvhti.iItem;
	}
	m_lbWeights.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	UpdateButtons();
	*pResult = 0;
}

void CObjectRandomDialog::OnNMDblclkWeightList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_lbWeights.SetItemState(m_nSelected, 0, LVIS_SELECTED | LVIS_FOCUSED);
	m_nSelected = pNMLV->iItem;
	if (m_nSelected == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_lbWeights.SubItemHitTest(&lvhti);
		m_nSelected = lvhti.iItem;
	}
	m_lbWeights.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	UpdateButtons();
	*pResult = 0;

	if (m_nSelected < 0)
		return;

	// iSubItem == 0 does not trigger editing
	if (pNMLV->iSubItem == 1) {
    CRect rItem, rWnd;
    m_lbWeights.GetWindowRect(rWnd);
    m_lbWeights.GetSubItemRect(m_nSelected, pNMLV->iSubItem, LVIR_BOUNDS, rItem);
    rItem += rWnd.TopLeft();
    CEditValue dlg(rItem);
		dlg.m_nValue = m_nConstWeight[m_nSelected];
		if (dlg.DoModal() == IDOK) {
			wchar_t number[10];
			m_nConstWeight[m_nSelected] = dlg.m_nValue; 
      _itow_s(m_nConstWeight[m_nSelected],number,10,10);
			m_lbWeights.SetItemText(m_nSelected, 1, number);
		}
	}
	else if (pNMLV->iSubItem == 2) {
		CValueDialog dlg;
		if (dlg.DoModal() == IDOK) {
			if (dlg.m_nSelection.size() > 0) {
				m_nVarWeight[m_nSelected] = dlg.m_nSelection[0]; 
				m_lbWeights.SetItemText(m_nSelected, 2, sspPool::Instance().values.GetName(m_nVarWeight[m_nSelected]));
			}
			else {
				m_nVarWeight[m_nSelected] = -1; 
				m_lbWeights.SetItemText(m_nSelected, 2, _T(""));
			}
		}
	}
}

void CObjectRandomDialog::OnAdd() 
{
	CPlayObjectDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nObjects.insert(m_nObjects.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		m_nConstWeight.reserve(m_nObjects.size());
		m_nVarWeight.reserve(m_nObjects.size());
		wchar_t number[10];
		for (unsigned int i=0; i<dlg.m_nSelection.size(); i++) {
			m_nConstWeight.push_back(1);
			m_nVarWeight.push_back(-1);
			m_lbWeights.SetItemState(m_nSelected, 0, LVIS_SELECTED|LVIS_FOCUSED);
			m_lbWeights.InsertItem(LVIF_TEXT|LVIF_STATE, m_nSize, sspPool::Instance().objects.GetName(m_nObjects[m_nSize]),
				LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
      _itow_s(m_nConstWeight[m_nSize],number,10,10);
			m_lbWeights.SetItemText(m_nSize, 1, number);
			if (m_nVarWeight[m_nSize] >= 0)
				m_lbWeights.SetItemText(m_nSize, 2, sspPool::Instance().values.GetName(m_nVarWeight[m_nSize]));
			m_nSelected = m_nSize;
			m_nSize++;
		}
		UpdateButtons();
	}
}

void CObjectRandomDialog::OnRemove() 
{
	if (m_nSelected >= 0 && m_nSelected < (int)m_nSize) {
		m_nObjects.erase(m_nObjects.begin() + m_nSelected);
		m_nConstWeight.erase(m_nConstWeight.begin() + m_nSelected);
		m_nVarWeight.erase(m_nVarWeight.begin() + m_nSelected);
		m_lbWeights.DeleteItem(m_nSelected);
		m_nSize--;
		if (m_nSelected == m_nSize)	m_nSelected--;
		m_lbWeights.SetItemState(m_nSelected, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		UpdateButtons();
	}
}

void CObjectRandomDialog::UpdateButtons()
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_nSelected >= 0);
}

void CObjectRandomDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nObjects.size() < 2) {
		AfxMessageBox(L"Please select at least two objects.");
		return;
	}
	CDialog::OnOK();
	m_editObj.setPlayObjects(m_nObjects);
	m_editObj.setConstWeight(m_nConstWeight);
	m_editObj.setVariableWeight(m_nVarWeight);
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strName;
		*((SSpRandomGroup*)m_pObjs[0]->pObj) = m_editObj;
	}
	else {
		sspMultipleObject* pMO = new sspMultipleObject;
		SSpRandomGroup* pNew = new SSpRandomGroup(m_editObj);
		pMO->pObj = pNew;
		pMO->strName = m_strName;
		m_pObjs.push_back(pMO);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectChainDialog dialog

CObjectChainDialog::CObjectChainDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectChainDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectChainDialog)
	m_strName = _T("");
	m_nChainLength = 1;
	//}}AFX_DATA_INIT
	m_nSelected = -1;
}

void CObjectChainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_CHAINLENGTH, m_nChainLength);
	DDV_MinMaxInt(pDX, m_nChainLength, 1, 1000);
	DDX_Control(pDX, IDC_CHAIN_LIST, m_lcChainList);
}


BEGIN_MESSAGE_MAP(CObjectChainDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectChainDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_CHAIN_LIST, OnClickChainList)
	ON_NOTIFY(NM_DBLCLK, IDC_CHAIN_LIST, OnDblclkChainList)
END_MESSAGE_MAP()


BOOL CObjectChainDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpChainGroup* pObj = dynamic_cast<SSpChainGroup*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strName = m_pObjs[0]->strName;
	}
	m_nObjects = m_editObj.getPlayObjects();
	m_nFollowers = m_editObj.getFollowers();
	m_nChainLength = m_editObj.getChainLength();
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPINLENGTH);
	pSpin->SetRange(1, 1000);

	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	UpdateButtons();

	CRect ctrlRect;
	m_lcChainList.GetClientRect(ctrlRect);
	int nRectWidth = ctrlRect.Width() / 2;
	m_lcChainList.InsertColumn(0, _T("Object"), LVCFMT_LEFT, nRectWidth, 0);
	m_lcChainList.InsertColumn(1, _T("Follower"), LVCFMT_LEFT, nRectWidth, 1);
	m_nSelected = -1;

  m_nSize = std::min(m_nObjects.size(), m_nFollowers.size());
	m_nObjects.resize(m_nSize);
	m_nFollowers.resize(m_nSize);
	for (unsigned int i=0; i<m_nSize; i++) {
		m_lcChainList.InsertItem(LVIF_TEXT|LVIF_STATE, i, 
		   sspPool::Instance().objects.GetName(m_nObjects[i]), 0, LVIS_SELECTED, 0, 0);
		m_lcChainList.SetItemText(i, 1, sspPool::Instance().objects.GetName(m_nObjects[m_nFollowers[i]]));
	}
	m_lcChainList.SetExtendedStyle(m_lcChainList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE );
	return TRUE;
}

void CObjectChainDialog::OnAdd() 
{
	CPlayObjectDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_nObjects.insert(m_nObjects.end(), dlg.m_nSelection.begin(), dlg.m_nSelection.end());
		int nFirst = m_nSize;
		m_nFollowers.reserve(m_nObjects.size());
		for (unsigned int i=0; i<dlg.m_nSelection.size()-1; i++) {
			m_nFollowers.push_back(m_nSize+1);
			m_lcChainList.SetItemState(m_nSelected, 0, LVIS_SELECTED|LVIS_FOCUSED);
			m_lcChainList.InsertItem(LVIF_TEXT|LVIF_STATE, m_nSize, sspPool::Instance().objects.GetName(m_nObjects[m_nSize]),
				LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
			m_lcChainList.SetItemText(m_nSize, 1, sspPool::Instance().objects.GetName(m_nObjects[m_nFollowers[m_nSize]]));
			m_nSelected = m_nSize;
			m_nSize++;
		}
		m_nFollowers.push_back(nFirst);
		m_lcChainList.SetItemState(m_nSelected, 0, LVIS_SELECTED|LVIS_FOCUSED);
		m_lcChainList.InsertItem(LVIF_TEXT|LVIF_STATE, m_nSize, sspPool::Instance().objects.GetName(m_nObjects[m_nSize]),
			LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
		m_lcChainList.SetItemText(m_nSize, 1, sspPool::Instance().objects.GetName(m_nObjects[m_nFollowers[m_nSize]]));
		m_nSelected = m_nSize;
		m_nSize++;
		UpdateButtons();
	}
}

void CObjectChainDialog::OnRemove() 
{
	if (m_nSelected >= 0 && m_nSelected < (int)m_nSize) {
		m_nObjects.erase(m_nObjects.begin() + m_nSelected);
		m_nFollowers.erase(m_nFollowers.begin() + m_nSelected);
		m_lcChainList.DeleteItem(m_nSelected);
		m_nSize--;
		if (m_nSelected == m_nSize)	m_nSelected--;
		m_lcChainList.SetItemState(m_nSelected, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		UpdateButtons();
	}
}

void CObjectChainDialog::OnClickChainList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nHit = pNMLV->iItem;
	if (nHit == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_lcChainList.SubItemHitTest(&lvhti);
		nHit = lvhti.iItem;
	}
	if (nHit >= 0) {
		if (pNMLV->iSubItem == 0) {
			m_lcChainList.SetItemState(m_nSelected, 0, LVIS_SELECTED | LVIS_FOCUSED);
			m_nSelected = nHit;
		}
		m_lcChainList.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		UpdateButtons();
	}
	*pResult = 0;
}

void CObjectChainDialog::OnDblclkChainList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nHit = pNMLV->iItem;
	if (nHit == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_lcChainList.SubItemHitTest(&lvhti);
		nHit = lvhti.iItem;
	}
	if (m_nSelected >= 0 && nHit >= 0 && pNMLV->iSubItem == 1) {
		m_nFollowers[nHit] = m_nSelected;
		m_lcChainList.SetItemText(nHit, 1, sspPool::Instance().objects.GetName(m_nObjects[m_nFollowers[nHit]]));
	}
	m_lcChainList.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}

void CObjectChainDialog::UpdateButtons()
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_nSelected >= 0);
}

void CObjectChainDialog::OnOK()
{
	UpdateData(TRUE);
	m_editObj.setChainLength(m_nChainLength);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nObjects.size() < 2) {
		AfxMessageBox(L"Please select at least two objects.");
		return;
	}
	CDialog::OnOK();
	m_editObj.setPlayObjects(m_nObjects);
	m_editObj.setFollowers(m_nFollowers);
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strName;
		*((SSpChainGroup*)m_pObjs[0]->pObj) = m_editObj;
	}
	else {
		sspMultipleObject* pMO = new sspMultipleObject;
		SSpChainGroup* pNew = new SSpChainGroup(m_editObj);
		pMO->pObj = pNew;
		pMO->strName = m_strName;
		m_pObjs.push_back(pMO);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectConditionalDialog dialog


CObjectConditionalDialog::CObjectConditionalDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectConditionalDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectConditionalDialog)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CObjectConditionalDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectConditionalDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COND_OBJECT_LIST, m_lcCondObject);
}


BEGIN_MESSAGE_MAP(CObjectConditionalDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectConditionalDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEFAULT_OBJECT, OnDefault)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_COND_OBJECT_LIST, OnClickCondObjectList)
	ON_NOTIFY(NM_DBLCLK, IDC_COND_OBJECT_LIST, OnDblclkCondObjectList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectConditionalDialog message handlers

BOOL CObjectConditionalDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpConditionalGroup* pObj = dynamic_cast<SSpConditionalGroup*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strName = m_pObjs[0]->strName;
	}
	m_nObjects = m_editObj.getPlayObjects();
	m_nConds = m_editObj.getConditionals();

	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	UpdateButtons();

	CRect ctrlRect;
	m_lcCondObject.GetClientRect(ctrlRect);
	int nRectWidth = ctrlRect.Width() / 2;
	m_lcCondObject.InsertColumn(0, _T("Condition"), LVCFMT_LEFT, nRectWidth, 0);
	m_lcCondObject.InsertColumn(1, _T("Play Object"), LVCFMT_LEFT, nRectWidth, 1);
	m_nSelected = -1;

  m_nSize = std::min(m_nObjects.size(), m_nConds.size());
	m_nObjects.resize(m_nSize);
	m_nConds.resize(m_nSize);
	for (unsigned int i=0; i<m_nSize; i++) {
		m_lcCondObject.InsertItem(LVIF_TEXT|LVIF_STATE, i, 
		   sspPool::Instance().conditionals.GetName(m_nConds[i]), 0, LVIS_SELECTED, 0, 0);
		m_lcCondObject.SetItemText(i, 1, sspPool::Instance().objects.GetName(m_nObjects[i]));
	}
	m_lcCondObject.SetExtendedStyle(m_lcCondObject.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	if (m_editObj.getDefaultObject() >= 0)
		GetDlgItem(IDC_DEFAULT_OBJECT)->SetWindowText(sspPool::Instance().objects.GetName(m_editObj.getDefaultObject()));
	return TRUE;
}

void CObjectConditionalDialog::OnClickCondObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW* pNMLV = (NMLISTVIEW*) pNMHDR;
	m_lcCondObject.SetItemState(m_nSelected, 0, LVIS_SELECTED | LVIS_FOCUSED);
	m_nSelected = pNMLV->iItem;
	if (m_nSelected == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_lcCondObject.SubItemHitTest(&lvhti);
		m_nSelected = lvhti.iItem;
	}
	m_lcCondObject.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	UpdateButtons();
	*pResult = 0;
}

void CObjectConditionalDialog::OnDblclkCondObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW* pNMLV = (NMLISTVIEW*) pNMHDR;
	m_lcCondObject.SetItemState(m_nSelected, 0, LVIS_SELECTED | LVIS_FOCUSED);
	m_nSelected = pNMLV->iItem;
	if (m_nSelected == -1) {
		LVHITTESTINFO lvhti;
		lvhti.pt = pNMLV->ptAction;
		m_lcCondObject.SubItemHitTest(&lvhti);
		m_nSelected = lvhti.iItem;
	}
	m_lcCondObject.SetItemState(m_nSelected, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	UpdateButtons();
	*pResult = 0;

	if (m_nSelected < 0)
		return;

	if (pNMLV->iSubItem == 0) {
		CConditionalDialog dlg;
		if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
			m_nConds[m_nSelected] = dlg.m_nSelection[0]; 
			m_lcCondObject.SetItemText(m_nSelected, 0, sspPool::Instance().conditionals.GetName(m_nConds[m_nSelected]));
		}
	}
	else if (pNMLV->iSubItem == 1) {
		CPlayObjectDialog dlg;
		if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
			m_nObjects[m_nSelected] = dlg.m_nSelection[0]; 
			m_lcCondObject.SetItemText(m_nSelected, 1, sspPool::Instance().objects.GetName(m_nObjects[m_nSelected]));
		}
	}
}

void CObjectConditionalDialog::OnAdd() 
{
	m_nConds.push_back(-1);
	m_nObjects.push_back(-1);

	m_lcCondObject.SetItemState(m_nSelected, 0, LVIS_SELECTED|LVIS_FOCUSED);
	m_lcCondObject.InsertItem(LVIF_TEXT | LVIF_STATE, m_nSize, _T("<< Select conditional >>"),
		LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
	m_lcCondObject.SetItemText(m_nSize, 1, _T("<< Select object >>"));

	m_nSelected = m_nSize;
	m_nSize++;
	UpdateButtons();
}

void CObjectConditionalDialog::OnRemove() 
{
	if (m_nSelected >= 0 && m_nSelected < (int) m_nSize) {
		m_nConds.erase(m_nConds.begin() + m_nSelected);
		m_nObjects.erase(m_nObjects.begin() + m_nSelected);
		m_lcCondObject.DeleteItem(m_nSelected);
		m_nSize--;
		if (m_nSelected == m_nSize)	m_nSelected--;
		m_lcCondObject.SetItemState(m_nSelected, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		UpdateButtons();
	}
}

void CObjectConditionalDialog::OnDown() 
{
	CString text;
	if (m_nSelected + 1 < (int) m_nSize) {
		int nCond = m_nConds[m_nSelected];
		m_nConds[m_nSelected] = m_nConds[m_nSelected+1];
		m_nConds[m_nSelected+1] = nCond;

		text = 	m_lcCondObject.GetItemText(m_nSelected, 0);
		m_lcCondObject.SetItemText(m_nSelected, 0, m_lcCondObject.GetItemText(m_nSelected+1, 0));
		m_lcCondObject.SetItemText(m_nSelected+1, 0, text);

		int nVal = m_nObjects[m_nSelected];
		m_nObjects[m_nSelected] = m_nObjects[m_nSelected+1];
		m_nObjects[m_nSelected+1] = nVal;

		text = 	m_lcCondObject.GetItemText(m_nSelected, 1);
		m_lcCondObject.SetItemText(m_nSelected, 1, m_lcCondObject.GetItemText(m_nSelected+1, 1));
		m_lcCondObject.SetItemText(m_nSelected+1, 1, text);

		m_lcCondObject.SetItemState(m_nSelected, 0, LVIS_SELECTED|LVIS_FOCUSED);
		m_nSelected++;
		m_lcCondObject.SetItemState(m_nSelected, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		UpdateButtons();
	}
}

void CObjectConditionalDialog::OnUp() 
{
	CString text;
	if (m_nSelected > 0) {
		int nCond = m_nConds[m_nSelected];
		m_nConds[m_nSelected] = m_nConds[m_nSelected-1];
		m_nConds[m_nSelected-1] = nCond;

		text = 	m_lcCondObject.GetItemText(m_nSelected, 0);
		m_lcCondObject.SetItemText(m_nSelected, 0, m_lcCondObject.GetItemText(m_nSelected-1, 0));
		m_lcCondObject.SetItemText(m_nSelected-1, 0, text);

		int nVal = m_nObjects[m_nSelected];
		m_nObjects[m_nSelected] = m_nObjects[m_nSelected-1];
		m_nObjects[m_nSelected-1] = nVal;

		text = 	m_lcCondObject.GetItemText(m_nSelected, 1);
		m_lcCondObject.SetItemText(m_nSelected, 1, m_lcCondObject.GetItemText(m_nSelected-1, 1));
		m_lcCondObject.SetItemText(m_nSelected-1, 1, text);

		m_lcCondObject.SetItemState(m_nSelected, 0, LVIS_SELECTED|LVIS_FOCUSED);
		m_nSelected--;
		m_lcCondObject.SetItemState(m_nSelected, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		UpdateButtons();
	}
}

void CObjectConditionalDialog::OnDefault() 
{
	CPlayObjectDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setDefaultObject(dlg.m_nSelection[0]);
		GetDlgItem(IDC_DEFAULT_OBJECT)->SetWindowText(sspPool::Instance().objects.GetName(m_editObj.getDefaultObject()));
	}
}

void CObjectConditionalDialog::UpdateButtons()
{
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_nSelected >= 0);
	GetDlgItem(IDC_DOWN)->EnableWindow(m_nSelected >= 0 && m_nSelected + 1 < (int) m_nObjects.size());
	GetDlgItem(IDC_UP)->EnableWindow(m_nSelected > 0);
}

void CObjectConditionalDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_nObjects.size() < 2 && m_editObj.getDefaultObject() < 0) {
		AfxMessageBox(L"Please select at least two objects.");
		return;
	}
	if (m_nObjects.size() == 0) {
		AfxMessageBox(L"Please select at least two objects.");
		return;
	}
	if (m_editObj.getDefaultObject() > (int) sspPool::Instance().objects.GetSize()) {
		AfxMessageBox(L"Invalid default object selection.");
		return;
	}
	CDialog::OnOK();
	m_editObj.setPlayObjects(m_nObjects);
	m_editObj.setConditionals(m_nConds);
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strName;
		*((SSpConditionalGroup*)m_pObjs[0]->pObj) = m_editObj;
	}
	else {
		sspMultipleObject* pMO = new sspMultipleObject;
		SSpConditionalGroup* pNew = new SSpConditionalGroup(m_editObj);
		pMO->pObj = pNew;
		pMO->strName = m_strName;
		m_pObjs.push_back(pMO);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CObjectDistributionDialog dialog


CObjectDistributionDialog::CObjectDistributionDialog(CWnd* pParent /*=NULL*/)
	: CPlayObjectEditor(CObjectDistributionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectDistributionDialog)
	m_strName = _T("");
	m_bSpecifyEnd = FALSE;
	m_nLoopMode = SSpDistributionObject::LOOP_DURATION;
	//}}AFX_DATA_INIT
}


void CObjectDistributionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectDistributionDialog)
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Check(pDX, IDC_END_DISTRIBUTION, m_bSpecifyEnd);
	DDX_Radio(pDX, IDC_RADIO_TIMEDURATION, m_nLoopMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectDistributionDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectDistributionDialog)
	ON_BN_CLICKED(IDC_SELECT_OBJECT, OnSelectObject)
	ON_BN_CLICKED(IDC_SELECT_CONDITION, OnSelectCondition)
	ON_BN_CLICKED(IDC_SELECT_START, OnSelectStart)
	ON_BN_CLICKED(IDC_SELECT_END, OnSelectEnd)
	ON_BN_CLICKED(IDC_SELECT_DURATION, OnSelectDuration)
	ON_BN_CLICKED(IDC_SELECT_COUNT, OnSelectCount)
	ON_BN_CLICKED(IDC_RADIO_CONDITION_ONLY, OnChangeMode)
	ON_BN_CLICKED(IDC_RADIO_COUNTDURATION, OnChangeMode)
	ON_BN_CLICKED(IDC_RADIO_TIMEDURATION, OnChangeMode)
	ON_BN_CLICKED(IDC_END_DISTRIBUTION, OnEndDistribution)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectDistributionDialog message handlers

BOOL CObjectDistributionDialog::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpDistributionObject* pObj = dynamic_cast<SSpDistributionObject*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strName = m_pObjs[0]->strName;
	}
	m_nLoopMode = m_editObj.getLoopMode();
  m_bSpecifyEnd = m_editObj.getStartInterval() != m_editObj.getEndInterval();
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	updateModeButtons();
	updateEndButton();

	CDialog::OnInitDialog();
	if (m_editObj.getPlayObject() >= 0)
		GetDlgItem(IDC_SELECT_OBJECT)->SetWindowText(sspPool::Instance().objects.GetName(m_editObj.getPlayObject()));
	if (m_editObj.getLoopCondition() >= 0)
		GetDlgItem(IDC_SELECT_CONDITION)->SetWindowText(sspPool::Instance().conditionals.GetName(m_editObj.getLoopCondition()));
	if (m_editObj.getStartInterval() >= 0)
		GetDlgItem(IDC_SELECT_START)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getStartInterval()));
	if (m_editObj.getEndInterval() >= 0)
		GetDlgItem(IDC_SELECT_END)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getEndInterval()));
	if (m_nLoopMode == SSpDistributionObject::LOOP_DURATION && m_editObj.getLoopDuration() >= 0)
		GetDlgItem(IDC_SELECT_DURATION)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getLoopDuration()));
	else
		GetDlgItem(IDC_SELECT_DURATION)->SetWindowText(_T("<< Select duration >>"));
	if (m_nLoopMode == SSpDistributionObject::LOOP_COUNT && m_editObj.getLoopDuration() >= 0)
		GetDlgItem(IDC_SELECT_COUNT)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getLoopDuration()));
	else
		GetDlgItem(IDC_SELECT_COUNT)->SetWindowText(_T("<< Select count >>"));
	return TRUE;
}

void CObjectDistributionDialog::OnSelectObject() 
{
	CPlayObjectDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setPlayObject(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_OBJECT)->SetWindowText(sspPool::Instance().objects.GetName(m_editObj.getPlayObject()));
	}
}

void CObjectDistributionDialog::OnSelectCondition() 
{
	CConditionalDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setLoopCondition(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_CONDITION)->SetWindowText(sspPool::Instance().conditionals.GetName(m_editObj.getLoopCondition()));
	}
}

void CObjectDistributionDialog::OnSelectStart() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setStartInterval(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_START)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getStartInterval()));
    updateEndButton();
	}
}

void CObjectDistributionDialog::OnSelectEnd() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setEndInterval(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_END)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getEndInterval()));
	}
}

void CObjectDistributionDialog::OnSelectDuration() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setLoopDuration(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_DURATION)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getLoopDuration()));
	}
}

void CObjectDistributionDialog::OnSelectCount() 
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
		m_editObj.setLoopDuration(dlg.m_nSelection[0]);
		GetDlgItem(IDC_SELECT_COUNT)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getLoopDuration()));
	}
}

void CObjectDistributionDialog::OnChangeMode() 
{
	UpdateData(TRUE);
	if (m_nLoopMode == SSpDistributionObject::LOOP_DURATION && m_editObj.getLoopDuration() >= 0)
		GetDlgItem(IDC_SELECT_DURATION)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getLoopDuration()));
	else
		GetDlgItem(IDC_SELECT_DURATION)->SetWindowText(_T("<< Select duration >>"));
	if (m_nLoopMode == SSpDistributionObject::LOOP_COUNT && m_editObj.getLoopDuration() >= 0)
		GetDlgItem(IDC_SELECT_COUNT)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getLoopDuration()));
	else
		GetDlgItem(IDC_SELECT_COUNT)->SetWindowText(_T("<< Select count >>"));
	updateModeButtons();
}

void CObjectDistributionDialog::OnEndDistribution() 
{
	UpdateData(TRUE);
	updateEndButton();
}

void CObjectDistributionDialog::updateModeButtons()
{
	GetDlgItem(IDC_SELECT_DURATION)->EnableWindow(m_nLoopMode == SSpDistributionObject::LOOP_DURATION);
	GetDlgItem(IDC_SELECT_COUNT)->EnableWindow(m_nLoopMode == SSpDistributionObject::LOOP_COUNT);
}

void CObjectDistributionDialog::updateEndButton()
{
	GetDlgItem(IDC_SELECT_END)->EnableWindow(m_bSpecifyEnd);
  if (!m_bSpecifyEnd && m_editObj.getStartInterval() >= 0) {
		m_editObj.setEndInterval(m_editObj.getStartInterval());
    GetDlgItem(IDC_SELECT_END)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getEndInterval()));
  }
}

void CObjectDistributionDialog::OnOK()
{
	UpdateData(TRUE);
	m_editObj.SetLoopMode(m_nLoopMode);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editObj.getPlayObject() < 0 || m_editObj.getPlayObject() > (int) sspPool::Instance().objects.GetSize()) {
		AfxMessageBox(L"Invalid play object selection.");
		return;
	}
	if (m_editObj.getLoopCondition() < 0 || m_editObj.getLoopCondition() > (int) sspPool::Instance().conditionals.GetSize()) {
		AfxMessageBox(L"Invalid play object selection.");
		return;
	}
	if (m_editObj.getStartInterval() < 0 || m_editObj.getStartInterval() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid play object selection.");
		return;
	}
	if (m_editObj.getEndInterval() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid play object selection.");
		return;
	}
	if (m_editObj.getLoopDuration() < 0 || m_editObj.getLoopDuration() > (int) sspPool::Instance().values.GetSize()) {
		switch (m_nLoopMode) {
		case SSpDistributionObject::LOOP_DURATION:
			AfxMessageBox(L"Invalid loop duration selection.");
			return;
			break;
		case SSpDistributionObject::LOOP_COUNT:
			AfxMessageBox(L"Invalid loop count selection.");
			return;
			break;
		default:
			break;
		}
	}
	CDialog::OnOK();
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strName;
		*((SSpDistributionObject*)m_pObjs[0]->pObj) = m_editObj;
	}
	else {
		sspMultipleObject* pMO = new sspMultipleObject;
		SSpDistributionObject* pNew = new SSpDistributionObject(m_editObj);
		pMO->pObj = pNew;
		pMO->strName = m_strName;
		m_pObjs.push_back(pMO);
	}
}
// SSpGUIPlayObjectEditor.cpp : implementation file
//


// CObjectSetState dialog

IMPLEMENT_DYNAMIC(CObjectSetState, CDialog)

CObjectSetState::CObjectSetState(CWnd* pParent /*=NULL*/)
: CPlayObjectEditor(CObjectSetState::IDD, pParent)
{
}

CObjectSetState::~CObjectSetState()
{
}

void CObjectSetState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Radio(pDX, IDC_INCREMENT, m_nMode);
}

BEGIN_MESSAGE_MAP(CObjectSetState, CDialog)
  ON_BN_CLICKED(IDC_BTN_STATEVALUE, &CObjectSetState::OnBtnStatevalue)
  ON_BN_CLICKED(IDC_BTN_INPUTVALUE, &CObjectSetState::OnBtnInputvalue)
END_MESSAGE_MAP()

BOOL CObjectSetState::OnInitDialog() 
{
	if (!m_pObjs.empty()) {
		SSpStateObject* pObj = dynamic_cast<SSpStateObject*>(m_pObjs[0]->pObj);
		if (!pObj) {
			AfxMessageBox(L"Unable to assign object.");
			EndDialog(IDCANCEL);
		}
		m_editObj = *pObj;
		m_strName = m_pObjs[0]->strName;
	}
  
  m_nMode = m_editObj.getMode();

	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);

  if (m_editObj.getGlobalState() >= 0)
		GetDlgItem(IDC_BTN_STATEVALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getGlobalState()));
  if (m_editObj.getInputValue() >= 0)
		GetDlgItem(IDC_BTN_INPUTVALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getInputValue()));
	return TRUE;
}

// CObjectSetState message handlers

void CObjectSetState::OnBtnStatevalue()
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    m_editObj.setGlobalState(dlg.m_nSelection[0]);
		GetDlgItem(IDC_BTN_STATEVALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getGlobalState()));
	}
}

void CObjectSetState::OnBtnInputvalue()
{
	CValueDialog dlg;
	if (dlg.DoModal() == IDOK && dlg.m_nSelection.size() > 0) {
    m_editObj.setInputValue(dlg.m_nSelection[0]);
		GetDlgItem(IDC_BTN_INPUTVALUE)->SetWindowText(sspPool::Instance().values.GetName(m_editObj.getInputValue()));
	}
}

void CObjectSetState::OnOK()
{
	UpdateData(TRUE);
  m_editObj.setMode(m_nMode);
	if (m_strName.IsEmpty()) {
		AfxMessageBox(L"Please specify a name.");
		return;
	}
	if (m_editObj.getGlobalState() < 0 || m_editObj.getGlobalState() > (int) sspPool::Instance().values.GetSize()) {
		AfxMessageBox(L"Invalid state value selection.");
		return;
	}
  if (m_nMode == SSpStateObject::STATE_TYPE_SETVALUE 
    && (m_editObj.getInputValue() < 0 || m_editObj.getInputValue() > (int) sspPool::Instance().values.GetSize())) {
		AfxMessageBox(L"Invalid input value selection.");
		return;
	}
	CDialog::OnOK();
	if (!m_pObjs.empty()) {
		m_pObjs[0]->strName = m_strName;
		*((SSpStateObject*)m_pObjs[0]->pObj) = m_editObj;
	}
	else {
		sspMultipleObject* pMO = new sspMultipleObject;
		SSpStateObject* pNew = new SSpStateObject(m_editObj);
		pMO->pObj = pNew;
		pMO->strName = m_strName;
		m_pObjs.push_back(pMO);
	}
}
