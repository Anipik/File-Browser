
// RightView.h : interface of the CRightView class
//

#pragma once


class CRightView : public CListView
{
protected: // create from serialization only
	CRightView();
	DECLARE_DYNCREATE(CRightView)

	// Attributes
public:
	CExerciseDoc* GetDocument() const;

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

	// Implementation
public:
	virtual ~CRightView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void ResetLeagues();
	void DisplayLeagues();
	void DisplayLeague(CString League);
	void DisplaySeason(CString League, CString Season);
	//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditOpen();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
};

#ifndef _DEBUG  // debug version in RightView.cpp
inline CExerciseDoc* CRightView::GetDocument() const
{
	return reinterpret_cast<CExerciseDoc*>(m_pDocument);
}
#endif
