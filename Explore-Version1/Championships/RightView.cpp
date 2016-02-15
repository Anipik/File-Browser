
// RightView.cpp : implementation of the CRightView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Championships.h"
#endif

#include<vector>
#include "Exercise.h"
#include "RightView.h"
#include "Userenv.h"
#pragma comment(lib, "userenv.lib")
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include "windows.h"
#include <shellapi.h>

#pragma comment(lib,"shell32.lib")
using namespace std;
static HWND hBut;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRightView

IMPLEMENT_DYNCREATE(CRightView, CListView)

BEGIN_MESSAGE_MAP(CRightView, CListView)
	ON_WM_STYLECHANGED()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	//ON_WM_LBUTTONDBLCLK()
	//ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CRightView::OnNMDblclk)
	ON_COMMAND(ID_EDIT_OPEN, &CRightView::OnEditOpen)
	ON_COMMAND(ID_EDIT_CUT, &CRightView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CRightView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CRightView::OnEditPaste)
END_MESSAGE_MAP()

// CRightView construction/destruction

CRightView::CRightView()
{
	// TODO: add construction code here

}

CRightView::~CRightView()
{
}

BOOL CRightView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CRightView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl &ctlCarParts = this->GetListCtrl();
	ctlCarParts.ModifyStyle(NULL, LVS_REPORT);
	ctlCarParts.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

void CRightView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRightView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRightView diagnostics

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CListView::AssertValid();
}

void CRightView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CExerciseDoc* CRightView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExerciseDoc)));
	return (CExerciseDoc*)m_pDocument;
}
#endif //_DEBUG


// CRightView message handlers
void CRightView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window	
	CListView::OnStyleChanged(nStyleType, lpStyleStruct);
}


int CRightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	GetDocument()->pRightView = this;
	return 0;
}


void CRightView::ResetLeagues()
{
	//deleting all the files in the listview
	CListCtrl &ctlLView = GetListCtrl();

	ctlLView.DeleteAllItems();
	while (ctlLView.DeleteColumn(0))
		;

	UpdateWindow();
}


void CRightView::DisplayLeagues()
{
	CListCtrl &ctlLeagues = this->GetListCtrl();

	ResetLeagues();
	int nItem;

	//adding the colums to the listview
	ctlLeagues.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 180);
	ctlLeagues.InsertColumn(1, _T("Path"), LVCFMT_LEFT, 210);
	ctlLeagues.InsertColumn(2, _T("Type"), LVCFMT_LEFT, 180);

	CFileFind finder;


	// build a string with wildcards
	CString strWildcard("");
	strWildcard += _T("\\*.*");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);


	//same as leftview
	while (bWorking)
	{


		bWorking = finder.FindNextFile();
		CString str = finder.GetFilePath();
		CString name2 = finder.GetFileName();

		nItem = ctlLeagues.InsertItem(0, name2);
		ctlLeagues.SetItemText(nItem, 1, str);
		
	}

	finder.Close();
	return;
}

//displaying files by getting path

void CRightView::DisplayLeague(CString League)
{
	//same as tree view just displaying as columns
	//if(Hidden)
	CListCtrl &ctlRightView = this->GetListCtrl();

	ResetLeagues();
	int nItem;
	ResetLeagues();

	//adding columns
	ctlRightView.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 180);
	ctlRightView.InsertColumn(1, _T("Path"), LVCFMT_LEFT, 210);
	ctlRightView.InsertColumn(2, _T("Type"), LVCFMT_LEFT, 180);

	CFileFind finder;

	if (League != "My Computer")
	{//build a string with wildcards
		CString strWildcard(League);
		strWildcard += _T("*.*");

		// start working for files
		BOOL bWorking = finder.FindFile(strWildcard);

		while (bWorking)
		{


			bWorking = finder.FindNextFile();
			if (finder.IsHidden() || finder.IsDots())
				//skips hidden files
				continue;

			CString name2 = finder.GetFileName();
			CString name1 = finder.GetFilePath();
			nItem = ctlRightView.InsertItem(0, name2);
			ctlRightView.SetItemText(nItem, 1, name1);
			//ctlLeagues.SetItemText(nItem, 2, _T("Don't Know"));
			//}

		}

		finder.Close();
		return;
	}
	//adding files for special folders

	else if (League == "My Computer")
	{
		CString name1;
		int a;
		TCHAR szHomeDirBuf[MAX_PATH] = { 0 };
		HANDLE hToken = 0;
		VERIFY(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken));
		DWORD BufSize = MAX_PATH;
		GetUserProfileDirectory(hToken, szHomeDirBuf, &BufSize);
		CloseHandle(hToken);
		a = ctlRightView.InsertItem(0, _T("Desktop"));
		name1 = CString(szHomeDirBuf) + _T("\\Desktop");
		ctlRightView.SetItemText(a, 1, name1);

		a = ctlRightView.InsertItem(1, _T("Documents"));
		name1 = CString(szHomeDirBuf) + _T("\\Documents");
		ctlRightView.SetItemText(a, 1, name1);

		a = ctlRightView.InsertItem(2, _T("Downloads"));
		name1 = CString(szHomeDirBuf) + _T("\\Downloads");
		ctlRightView.SetItemText(a, 1, name1);

		a = ctlRightView.InsertItem(3, _T("Music"));
		name1 = CString(szHomeDirBuf) + _T("\\Music");
		ctlRightView.SetItemText(a, 1, name1);

		a = ctlRightView.InsertItem(4, _T("Pictures"));
		name1 = CString(szHomeDirBuf) + _T("\\Pictures");
		ctlRightView.SetItemText(a, 1, name1);

		a = ctlRightView.InsertItem(5, _T("Videos"));
		name1 = CString(szHomeDirBuf) + _T("\\Videos");
		ctlRightView.SetItemText(a, 1, name1);
		//ctlRightView.SetItemText(nItem, 1, name1);
		a = 6;
		int i = 2, j = 0;
		UINT drivetype;
		DWORD bitarr = GetLogicalDrives();
		while (j<40)
		{
			if (bitarr & 2 * i)
			{
				char ch1 = (char)(j + 67);
				CString s1 = _T(":");
				s1 = ch1 + s1;
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

				a = ctlRightView.InsertItem(a, ch2);
			
				a++;



			}
			i = 2 * i;
			j++;

		}

	}

}


//conveting / to // character
std::string escape(const std::string& s)
{
	int n = s.size(), wp = 0;
	std::vector<char> result(n * 2);
	for (int i = 0; i<n; i++)
	{
		if (s[i] == '/' || s[i] == '\\')
			result[wp++] = '\\';
		result[wp++] = s[i];
	}
	return std::string(&result[0], &result[wp]);
}




//opening files on double clicking and opening directories in our file explorer
void CRightView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CListCtrl &ctlRightView = this->GetListCtrl();
	CString strCol1, strCol2;
	int pos;
	NMLISTVIEW* pnmtv = (NMLISTVIEW*)pNMHDR;
	pos = (int)ctlRightView.GetFirstSelectedItemPosition() - 1;//getting selected file position
	strCol1 = ctlRightView.GetItemText(pos, 1); //geting the cstring associated

	//converting cstring to string
	CT2CA pszConvertedAnsiString(strCol1);
	std::string strStd(pszConvertedAnsiString);


	//duplicating escape character
	escape(strStd);

	
	CFileFind finder;

	//checking if it is a directory or a file
	finder.FindFile(strCol1);
	finder.FindNextFile();
	if (finder.IsDirectory()){
		CListCtrl &ctlRightView = this->GetListCtrl();

		ResetLeagues();
		int nItem;
		ResetLeagues();

		//opening directory in our file viewer if it is a directory 
		ctlRightView.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 180);
		ctlRightView.InsertColumn(1, _T("Path"), LVCFMT_LEFT, 210);
		ctlRightView.InsertColumn(2, _T("Type"), LVCFMT_LEFT, 180);

		CFileFind finder;
		CFileFind Hfinder;
		
			
			strCol1 += _T("\\*.*");

			// start working for files
			BOOL bWorking = finder.FindFile(strCol1);

			while (bWorking)
			{

				
				bWorking = finder.FindNextFile();
				if (finder.IsHidden() || finder.IsDots())
					//skips hidden files
					continue;

				CString name2 = finder.GetFileName();
				CString name1 = finder.GetFilePath();
				nItem = ctlRightView.InsertItem(0, name2);
				ctlRightView.SetItemText(nItem, 1, name1);
				//ctlLeagues.SetItemText(nItem, 2, _T("Don't Know"));
				//}

			}

			finder.Close();
			return;
		}
	
	else
		//opening the file if not a directory
		ShellExecuteA(*GetDesktopWindow(), "Open", LPCSTR(strStd.c_str()), NULL, NULL, SW_SHOWNORMAL);
	//ShellExecute(0, 0,path.c_str() , 0, 0 , SW_SHOW )
	*pResult = 0;
	*pResult = 0;
}

//opening file on right clicking and selecting open
void CRightView::OnEditOpen()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CListCtrl &ctlRightView = this->GetListCtrl();
	CString strCol1, strCol2;
	int pos;

	pos = (int)ctlRightView.GetFirstSelectedItemPosition() - 1;

	//checking if the right click is on item or empty space
	if (ctlRightView.GetSelectedCount() != NULL){
		strCol1 = ctlRightView.GetItemText(pos, 1);// Get the 1st column data for the row nIndex
		//conversion to string
		CT2CA pszConvertedAnsiString(strCol1);
		std::string strStd(pszConvertedAnsiString);

		escape(strStd);

		//same code as double click
		CFileFind finder;

		finder.FindFile(strCol1);
		finder.FindNextFile();
		if (finder.IsDirectory()){
			CListCtrl &ctlRightView = this->GetListCtrl();

			ResetLeagues();
			int nItem;
			ResetLeagues();

			ctlRightView.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 180);
			ctlRightView.InsertColumn(1, _T("Path"), LVCFMT_LEFT, 210);
			ctlRightView.InsertColumn(2, _T("Type"), LVCFMT_LEFT, 180);

			CFileFind finder;
			CFileFind Hfinder;


			strCol1 += _T("\\*.*");

			// start working for files
			BOOL bWorking = finder.FindFile(strCol1);

			while (bWorking)
			{


				bWorking = finder.FindNextFile();
				if (finder.IsHidden() || finder.IsDots())
					//skips hidden files
					continue;

				CString name2 = finder.GetFileName();
				CString name1 = finder.GetFilePath();
				nItem = ctlRightView.InsertItem(0, name2);
				ctlRightView.SetItemText(nItem, 1, name1);
				//ctlLeagues.SetItemText(nItem, 2, _T("Don't Know"));
				//}

			}

			finder.Close();
			return;
		}

		else

			ShellExecuteA(*GetDesktopWindow(), "Open", LPCSTR(strStd.c_str()), NULL, NULL, SW_SHOWNORMAL);
		//ShellExecute(0, 0,path.c_str() , 0, 0 , SW_SHOW )
	}
}



//added handler ...event not added
void CRightView::OnEditCut()
{
	// TODO: Add your command handler code here
}


void CRightView::OnEditCopy()
{
	// TODO: Add your command handler code here
}


void CRightView::OnEditPaste()
{
	// TODO: Add your command handler code here
}
