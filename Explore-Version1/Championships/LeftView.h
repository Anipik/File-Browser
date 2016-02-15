
// LeftView.h : interface of the CLeftView class
//


#pragma once

class CExerciseDoc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

	// Attributes
public:
	CExerciseDoc* GetDocument();

	// Operations
public:
	CImageList m_TreeImages;

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

	// Implementation
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void CreateLeagues();


	//void GetValidDrives(void);
	//HTREEITEM getHTreeParent(int);
	//void setHTreeParent(HTREEITEM , int );
	//void AddToTree(char*, int , int );
	void Recurse(LPCTSTR, int, int, HTREEITEM);
	void REC(LPCTSTR, int, int, HTREEITEM);
	void SubDir(LPCTSTR);
	int subdir(LPCTSTR);
	//	afx_msg void itemexpanded();
	afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnTvnSingleExpand(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);

};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CExerciseDoc* CLeftView::GetDocument()
{
	return reinterpret_cast<CExerciseDoc*>(m_pDocument);
}
#endif

