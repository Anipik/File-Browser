
// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "championships.h"
#include <windows.h>
#include <stdio.h>
#include "Exercise.h"
#include "LeftView.h"
#include <afxwin.h>
#include <tchar.h>
#include <strsafe.h>
#include <malloc.h>
#include <iostream>
#include "RightView.h"
#include "Userenv.h"
#pragma comment(lib, "userenv.lib")
using namespace std;
//using namespace System.Windows.Forms;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INFO_BUFFER_SIZE 32767
#define BUFSIZE MAX_PATH
// CLeftView
int GLOBAL = 0;
IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_WM_CREATE()
	//ON_COMMAND(AFX_ID_PREVIEW_CLOSE, &CLeftView::itemexpanded)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CLeftView::OnTvnItemexpanding)
	//ON_NOTIFY_REFLECT(TVN_SINGLEEXPAND, &CLeftView::OnTvnSingleExpand)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CLeftView::OnTvnSelchanged)

END_MESSAGE_MAP()


// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	//getting tree lines and expand and collapse signs
	GetTreeCtrl().ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);

	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
}


// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CExerciseDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExerciseDoc)));
	return (CExerciseDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView message handlers


int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//Getting the document from exercise.doc 
	GetDocument()->pLeftView = this;
	return 0;
}


HTREEITEM hRoot;
HTREEITEM hSeason1, hSeason;
HTREEITEM hSubseason;
TCHAR  infoBuf[INFO_BUFFER_SIZE];

//default display of rightview
void CLeftView::CreateLeagues()
{
	// If there is anything in the tree, remove it
	GetTreeCtrl().DeleteAllItems();

	CTreeCtrl &ctlLeagues = this->GetTreeCtrl();
	//m_TreeImages.Create(IDB_TREE_BMP, 16, 1, RGB(255, 255, 255));

	ctlLeagues.SetImageList(&m_TreeImages, TVSIL_NORMAL);

	HTREEITEM hRoot, hRoot1, ui;

	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	GetUserName(infoBuf, &bufCharCount);
	//hRoot = ctlLeagues.InsertItem( ,0,1)
	TVITEM tvitem;

	hRoot = ctlLeagues.InsertItem(_T("My Computer"), 0, 1);

	//make number of child as one without populating that
	tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
	tvitem.hItem = hRoot;
	tvitem.cChildren = 1;
	TreeView_SetItem(ctlLeagues, &tvitem);
}


//add a plus if a directory contains a sub directory
void CLeftView::SubDir(LPCTSTR pstr)
{
	CFileFind finder;
	CTreeCtrl &ctlLeagues = this->GetTreeCtrl();
	CString strWildcard(pstr); 
	strWildcard += _T("\\*.*");
	BOOL bWorking = finder.FindFile(strWildcard);//searches all the files in that folder
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsHidden() || finder.IsDots()) //if a file is hidden or is dots
			continue;

		if (finder.IsDirectory())
			GLOBAL = 55;//use as tag to differentiate between directory and folder 

	}
	finder.Close();
	return;
}

//for populating a directory
void CLeftView::REC(LPCTSTR pstr, int num1, int num2, HTREEITEM prev)
{
	CFileFind finder;
	CTreeCtrl &ctlLeagues = this->GetTreeCtrl();

	CString strWildcard(pstr);
	strWildcard += _T("\\*.*");//adding wildcardas for searching of files
	HTREEITEM hsubRoot;
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsHidden() || finder.IsDots())
			continue;

		if (finder.IsDirectory())

		{
			CString str = finder.GetFilePath();
			CString name = finder.GetFileName();//getting file name to be shown in display
			hsubRoot = ctlLeagues.InsertItem(name, num1, num2, prev);
			GLOBAL = 0;
			SubDir(str);
			if (GLOBAL == 55)
			{
				TVITEM tvitem;
				tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
				tvitem.hItem = hsubRoot;
				tvitem.cChildren = 1;//making child as expandable item
				TreeView_SetItem(ctlLeagues, &tvitem);
			}

			else
			{
				TVITEM tvitem;
				tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
				tvitem.hItem = hsubRoot;
				tvitem.cChildren = 0;
				TreeView_SetItem(ctlLeagues, &tvitem);

			}
			
			num1++;
			num2++;
			
		}
	}

	finder.Close();
	return;
}

//same function between the name
void CLeftView::Recurse(LPCTSTR pstr, int num1, int num2, HTREEITEM prev)
{
	CFileFind finder;
	CTreeCtrl &ctlLeagues = this->GetTreeCtrl();

	// build a string with wildcards
	CString strWildcard(pstr);
	strWildcard += _T("\\*.*");
	HTREEITEM hsubRoot;
	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);



	while (bWorking)
	{


		bWorking = finder.FindNextFile();

		if (finder.IsHidden() || finder.IsDots())
		{
			

			continue;
		}
		// if it's a directory, recursively search it

		if (finder.IsDirectory())

		{
			CString str = finder.GetFilePath();
			
			CString name = finder.GetFileName();
			\


			hsubRoot = ctlLeagues.InsertItem(name, num1, num2, prev);
			GLOBAL = 0;
			SubDir(str);
			if (GLOBAL == 55)
			{
				TVITEM tvitem;
				tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
				tvitem.hItem = hsubRoot;
				tvitem.cChildren = 1;
				TreeView_SetItem(ctlLeagues, &tvitem);
			}

			else
			{
				TVITEM tvitem;
				tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
				tvitem.hItem = hsubRoot;
				tvitem.cChildren = 0;
				TreeView_SetItem(ctlLeagues, &tvitem);

			}
			


			num1++;
			num2++;
			 
		}
	}

	finder.Close();
	return;
}

//event handler for expanding
void CLeftView::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	NMTREEVIEW* pnmtv = (NMTREEVIEW*)pNMHDR;

	CTreeCtrl &ctlLeagues = GetTreeCtrl();

	HTREEITEM hroot, hsubRoot45;
	// getting clicked item
	HTREEITEM item = pnmtv->itemNew.hItem;

	hsubRoot45 = item;
	CString PAR;
	while (ctlLeagues.GetParentItem(ctlLeagues.GetParentItem(hsubRoot45)) != NULL)
		hsubRoot45 = ctlLeagues.GetParentItem(hsubRoot45);

	PAR = ctlLeagues.GetItemText(hsubRoot45);

	CString fullpath = _T("");
	HTREEITEM current = item;
	//calculating the path
	while (ctlLeagues.GetParentItem(current) != NULL)
	{
		CString thistext = ctlLeagues.GetItemText(current);
		fullpath = thistext + _T("\\") + fullpath;
		current = ctlLeagues.GetParentItem(current);
	}

	//getting path for special folders like my documents etc
	TCHAR szHomeDirBuf[MAX_PATH] = { 0 };
	HANDLE hToken = 0;
	VERIFY(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken));
	DWORD BufSize = MAX_PATH;
	GetUserProfileDirectory(hToken, szHomeDirBuf, &BufSize);
	CloseHandle(hToken);

	if (PAR == "Desktop")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;

	else if (PAR == "Documents")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;
	else if (PAR == "Downloads")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;

	else if (PAR == "Music")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;

	else if (PAR == "Pictures")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;

	else if (PAR == "Videos")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;

	
	//checking number of kids
	int kidcount = 0;
	HTREEITEM hKid = ctlLeagues.GetChildItem(item);
	while (hKid != NULL)
	{
		kidcount++;
		hKid = ctlLeagues.GetNextSiblingItem(hKid);//finding next file

	}

	//if it is expanded
	if (pnmtv->action == TVE_EXPAND)

	{

		if (ctlLeagues.GetItemText(item) == "My Computer")
		{
			if (kidcount < 2)
			{
				//populating the directories
				HTREEITEM item45;
				TVITEM tvitem;
				item45 = ctlLeagues.InsertItem(_T("Desktop"), 2, 3, item);

				tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
				tvitem.hItem = item45;
				tvitem.cChildren = 1;
				TreeView_SetItem(ctlLeagues, &tvitem);

				item45 = ctlLeagues.InsertItem(_T("Documents"), 2, 3, item);
				tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
				tvitem.hItem = item45;
				tvitem.cChildren = 1;
				TreeView_SetItem(ctlLeagues, &tvitem);

				item45 = ctlLeagues.InsertItem(_T("Music"), 2, 3, item);
				tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
				tvitem.hItem = item45;
				tvitem.cChildren = 1;
				TreeView_SetItem(ctlLeagues, &tvitem);

				item45 = ctlLeagues.InsertItem(_T("Pictures"), 2, 3, item);
				tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
				tvitem.hItem = item45;
				tvitem.cChildren = 1;
				TreeView_SetItem(ctlLeagues, &tvitem);

				item45 = ctlLeagues.InsertItem(_T("Videos"), 2, 3, item);
				tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
				tvitem.hItem = item45;
				tvitem.cChildren = 1;
				TreeView_SetItem(ctlLeagues, &tvitem);

				//Getting the logical drives

				int i = 2, j = 0;
				UINT drivetype;
				DWORD bitarr = GetLogicalDrives();
				while (j<40)
				{
					if (bitarr & 2 * i)
					{
						char ch1 = (char)(j + 67);//converting bit to drives
						CString s1 = _T(":");
						s1 = ch1 + s1;//getting if it is a local derive or external drive
						CString ch2 = s1;
						drivetype = GetDriveType(ch2);
						switch (drivetype)
						{
						case 0: ch2 = _T("Cannot be determined");
							break;
						case 1: ch2 = _T("Invalid Root");
							break;
						case 2:ch2 = s1;
							break;

						case 3: ch2 = s1;
							break;
						case 4: ch2 = _T("Network (") + s1 + _T(")");
							break;
						case 5: ch2 = _T("CDRom Drive (") + s1 + _T(")");
							break;
						case 6: ch2 = _T("RAM Drive (") + s1 + _T(")");
							break;
						default: ch2 = _T("UNKNOWN DEVICE");
							break;
						}

						item45 = ctlLeagues.InsertItem(ch2, 2, 3, item);

						//TVITEM tvitem;
						tvitem.mask = TVIF_HANDLE | TVIF_CHILDREN;
						tvitem.hItem = item45;
						tvitem.cChildren = 1;
						TreeView_SetItem(ctlLeagues, &tvitem);
						//Recurse(s, 3, 4, ui);			
					}
					i = 2 * i;
					j++;

				}
			}
		}
		else if (ctlLeagues.GetItemText(item) == "Desktop")
		{
			wchar_t* localAppData = 0;
			SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &localAppData);//getting special folder path

			CString val;
			val = localAppData;
			CoTaskMemFree(static_cast<void*>(localAppData));

			Recurse(val, 4, 5, item);
		}

		else if (ctlLeagues.GetItemText(item) == "Downloads")
		{
			wchar_t* localAppData = 0;
			SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &localAppData);

			CString val;
			val = localAppData;
			CoTaskMemFree(static_cast<void*>(localAppData));

			Recurse(val, 4, 5, item);
		}

		else if (ctlLeagues.GetItemText(item) == "Music")
		{
			wchar_t* localAppData = 0;
			SHGetKnownFolderPath(FOLDERID_Music, 0, NULL, &localAppData);

			CString val;
			val = localAppData;
			CoTaskMemFree(static_cast<void*>(localAppData));

			Recurse(val, 4, 5, item);
		}

		else if (ctlLeagues.GetItemText(item) == "Pictures")
		{
			wchar_t* localAppData = 0;
			SHGetKnownFolderPath(FOLDERID_Pictures, 0, NULL, &localAppData);

			CString val;
			val = localAppData;
			CoTaskMemFree(static_cast<void*>(localAppData));

			Recurse(val, 4, 5, item);
		}

		else if (ctlLeagues.GetItemText(item) == "Documents")
		{
			wchar_t* localAppData = 0;
			SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &localAppData);

			CString val;
			val = localAppData;
			CoTaskMemFree(static_cast<void*>(localAppData));

			Recurse(val, 4, 5, item);
		}

		else if (ctlLeagues.GetItemText(item) == "Videos")
		{
			wchar_t* localAppData = 0;
			SHGetKnownFolderPath(FOLDERID_Videos, 0, NULL, &localAppData);

			CString val;
			val = localAppData;
			CoTaskMemFree(static_cast<void*>(localAppData));

			Recurse(val, 4, 5, item);
		}

		else if (ctlLeagues.GetItemText(item) == "Favorites")
		{
			wchar_t* localAppData = 0;
			SHGetKnownFolderPath(FOLDERID_Links, 0, NULL, &localAppData);

			CString val;
			val = localAppData;
			CoTaskMemFree(static_cast<void*>(localAppData));

			Recurse(val, 4, 5, item);
		}
		else
		if (kidcount < 1)
		{
			//hsubRoot45=ctlLeagues.InsertItem(fullpath, 4, 5, item);
			Recurse(fullpath, 4, 5, item);
		}

		//CString str = _T("C:\\");
		//Recurse(str, 2, 3, item);
	}

	else if (pnmtv->action == TVE_COLLAPSE)
	{
		//  ctlLeagues.DeleteItem( hsubRoot );
		// continue;
		//deleting item so that it doesnt add again
		ctlLeagues.Expand(pNMTreeView->itemNew.hItem, TVE_COLLAPSE | TVE_COLLAPSERESET);
	}


	*pResult = 0;
}




//connecting right and left view
void CLeftView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	

	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CExerciseDoc *pDoc = GetDocument();
	CTreeCtrl &ctlLeagues = this->GetTreeCtrl();
	HTREEITEM item = ctlLeagues.GetSelectedItem();
	CString fullpath = _T("");
	HTREEITEM current = item;

	HTREEITEM hsubRoot45 = item;
	CString PAR;
	while (ctlLeagues.GetParentItem(ctlLeagues.GetParentItem(hsubRoot45)) != NULL)
		hsubRoot45 = ctlLeagues.GetParentItem(hsubRoot45);

	PAR = ctlLeagues.GetItemText(hsubRoot45);

	//getting path for normal folders
	while (ctlLeagues.GetParentItem(current) != NULL)
	{
		CString thistext = ctlLeagues.GetItemText(current);
		fullpath = thistext + _T("\\") + fullpath;
		current = ctlLeagues.GetParentItem(current);
	}
	//getiing path of specail folders
	TCHAR szHomeDirBuf[MAX_PATH] = { 0 };
	HANDLE hToken = 0;
	VERIFY(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken));
	DWORD BufSize = MAX_PATH;
	GetUserProfileDirectory(hToken, szHomeDirBuf, &BufSize);
	CloseHandle(hToken);

	if (PAR == "Desktop")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;

	else if (PAR == "Documents")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;
	else if (PAR == "Downloads")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;

	else if (PAR == "Music")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;

	else if (PAR == "Pictures")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;

	else if (PAR == "Videos")
		fullpath = CString(szHomeDirBuf) + _T("\\") + fullpath;


	if (fullpath == "")
		fullpath = _T("My Computer");


	pDoc->pRightView->DisplayLeague(fullpath);//passing path to rightview
	
	*pResult = 0;
}



