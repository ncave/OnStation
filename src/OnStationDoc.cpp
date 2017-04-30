// OnStationDoc.cpp : implementation of the COnStationDoc class
//

#include "stdafx.h"
#include "OnStation.h"
#include "OnStationDoc.h"
#include "realfolder.h"
#include "surfacedata.h"
#include "surveyleg.h"
#include "colortool.h"
#include "nodetree.h"
#include "saveformattedtext.h"
#include "pagelayoutpreferences.h"
#include "overalldataview.h"
#include "mainfrm.h"
#include "filehelper.h"
#include "onstationview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COnStationApp theApp;
COnStationDoc * pDocument_G=NULL;
extern COnStationView * pView_G;
extern CSettings * pSettings_G;
/////////////////////////////////////////////////////////////////////////////
// COnStationDoc

IMPLEMENT_DYNCREATE(COnStationDoc, CDocument)

BEGIN_MESSAGE_MAP(COnStationDoc, CDocument)
	//{{AFX_MSG_MAP(COnStationDoc)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_FILE_EXPORTFORMATTEDTEXT, OnFileSaveFormattedText)
	ON_COMMAND(ID_INSERT_FILE,OnInsertFile)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SENDMAIL,OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SENDMAIL,OnUpdateFileSendMail)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnStationDoc construction/destruction

COnStationDoc::COnStationDoc()
{
#ifdef _DEBUG
	ASSERT(AfxCheckMemory());
#endif
	pDocument_G=this;
	m_SurveyDataFolder=new CRealFolder(this);
	CString CS;
	CS.LoadString(IDS_SURVEYROOT);
	m_SurveyDataFolder->SetName(CS);
	m_SurfaceData=new CSurfaceData(this);
	m_NodeTree=new CNodeTree(this);
#ifdef _DEBUG
	ASSERT(AfxCheckMemory());
#endif
}

COnStationDoc::~COnStationDoc()
{
#ifdef _DEBUG
	ASSERT(AfxCheckMemory());
#endif
	delete m_SurveyDataFolder;
	m_SurveyDataFolder=NULL;
	delete m_SurfaceData;
	m_SurfaceData=NULL;
	delete m_NodeTree;
	m_NodeTree=NULL;
}


void COnStationDoc::DeleteContents()
{
	pSettings_G->ResetContents();
	CMainFrame * pFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pFrame)
	{
		pFrame->m_dataView.RemoveAll();
	}
	m_SurveyDataFolder->DeleteContents();
	m_NodeTree->DeleteContents();
	m_SurfaceData->DeleteContents();
	if (pView_G!=NULL)
	{
		pView_G->DeleteContents();
	}
	CString CS;
	CS.LoadString(IDS_SURVEYROOT);
	m_SurveyDataFolder->SetName(CS);
}

BOOL COnStationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}

#ifdef _UNICODE

BOOL COnStationDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	ENSURE(lpszPathName);

	FILE *pStream;
	errno_t err;
	
	// open for reading in UTF-8
	err = _tfopen_s(&pStream, lpszPathName, _T("rt,ccs=UTF-8"));
	if (err != 0)
	{
		TCHAR strError[80];
		_tcserror_s(strError, err);
		AfxMessageBox(strError, MB_ICONEXCLAMATION);
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CStdioFile f(pStream);
	try
	{
		CWaitCursor wait;
		CFileHelper h(&f);
		Read(&h, FALSE);
		f.Close();
	}
	catch (CException* ex)
	{
		try
		{
			f.Abort();
			DeleteContents();   // remove failed contents
			ReportSaveLoadException(lpszPathName, ex,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		catch(...) { }
		return FALSE;
	}

	SetModifiedFlag(FALSE);     // start off with unmodified
	return TRUE;
}

BOOL COnStationDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	ENSURE(lpszPathName);

	FILE *pStream;
	errno_t err;

	err = _tfopen_s(&pStream, lpszPathName, _T("wt,ccs=UTF-8"));
	if (err != 0)
	{
		TCHAR strError[80];
		_tcserror_s(strError, err);
		AfxMessageBox(strError, MB_ICONEXCLAMATION);
		return FALSE;
	}

	CStdioFile f(pStream);
	try
	{
		CWaitCursor wait;
		f.SeekToBegin(); // no BOM in UTF-8 so we can create legacy ANSI files (if no multibyte encoding needed).
		CFileHelper h(&f);
		Write(&h);
		f.Close();
	}
	catch (CException* ex)
	{
		try
		{
			f.Abort();
			ReportSaveLoadException(lpszPathName, ex,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		}
		catch(...) { }
		return FALSE;
	}

	SetModifiedFlag(FALSE);     // back to unmodified
	return TRUE;        // success
}

#else

void COnStationDoc::Serialize(CArchive& ar)
{
	try
	{
		CFileHelper h(&ar);
		if (ar.IsStoring())
		{
			Write(&h);
		}
		else
		{
			Read(&h,FALSE);
		}
	}
	catch (...)
	{
		AfxMessageBox(IDS_UNABLETOOPENFILE);
		DeleteContents();
		CString CS;
		CS.LoadString(IDS_UNTITLED);
		SetTitle(CS);
	}
}
#endif

void COnStationDoc::Write(CFileHelper * h)
{
	h->WriteToken(_T("FileVersion"), _T("2.0"));	
	h->WriteToken(_T("Program"), _T("On Station 3.1"));
	m_SurveyDataFolder->Write(h);
	m_SurfaceData->WriteFixedPoints(h);
	m_SurfaceData->Write(h);
	pSettings_G->Write(h);
}

void COnStationDoc::Read(CFileHelper * h,BOOL bInsertFile)
{
	float fDataVersion=h->ReadTokenFloat(_T("FileVersion"));
	h->m_fVersion=fDataVersion;
	LPCTSTR szProgram=h->ReadTokenString(_T("Program"));
	if (lstrcmpi(szProgram, _T("On Station"))==0)
	{
		AfxMessageBox(IDS_OLDFORMATUSETRANSLATOR);
		return;
	}
	if (fDataVersion>CURRENT_VERSION)
	{
		AfxMessageBox(IDS_FILEOPENLATERVERSION);
	}
	if (bInsertFile)
	{
		CRealFolder * pNew=new CRealFolder(this);
		pNew->Read(h);
		pNew->SetParentFolder(m_SurveyDataFolder,NULL);
	}
	else
	{
		m_SurveyDataFolder->Read(h);
		m_SurfaceData->ReadFixedPoints(h);
		m_SurfaceData->ReadSurface(h);
		pSettings_G->Read(h);
		if (pView_G!=NULL)
		{
			pView_G->PostMessage(WM_COMMAND,ID_DRAWMODE_COLORSCHEME1,0l);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// COnStationDoc diagnostics

#ifdef _DEBUG
void COnStationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COnStationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



void COnStationDoc::OnFileImport()
{
	//if (WinExec(_T("importapp.exe"),SW_SHOW)==ERROR_FILE_NOT_FOUND)
	{
		AfxMessageBox(IDS_NO_IMPORT_EXE);
	}
}


void COnStationDoc::OnFileSaveFormattedText() 
{
	CSaveFormattedText PSD(this);
	PSD.DoModal();
}

void COnStationDoc::OnInsertFile()
{
	CFileDialog fd(TRUE, _T("*.cdi"),NULL,OFN_FILEMUSTEXIST, _T("Cave Data Interchange|*.cdi||"));
	if (fd.DoModal()==IDOK)
	{
		try
		{
			CString Cs=fd.GetPathName();
#ifdef _UNICODE
			FILE *fStream;
			errno_t err = _tfopen_s(&fStream, Cs, _T("rt,ccs=UTF-8"));
			if (err != 0)
			{
				TCHAR strError[80];
				_tcserror_s(strError, err);
				AfxMessageBox(strError, MB_ICONEXCLAMATION);
				return;
			}
			CStdioFile f(fStream);  // open the file from this stream
			CFileHelper h(&f);
#else
			CFile file(Cs,CFile::modeRead);
			CArchive archive(&file,CArchive::load);
			CFileHelper h(&archive);
#endif			
			Read(&h, TRUE);
			CMainFrame * pFrame=dynamic_cast<CMainFrame *>(AfxGetMainWnd());
			if (pFrame)
			{
				pFrame->m_dataView.OnChangedDocumentData();
			}
		}
		catch (...)
		{
			AfxMessageBox(IDS_UNABLETOOPENFILE);
			SetTitle(_T("Untitled"));
		}
	}
}
