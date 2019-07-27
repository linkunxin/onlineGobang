#pragma once
#include <afxext.h>
#include "Resource.h"
class CMyFormDlg :
	public CFormView
{
	DECLARE_DYNCREATE(CMyFormDlg)
protected:
	CMyFormDlg();
	virtual ~CMyFormDlg();
	//
public:
	//virtual void OnInitialUpdate();
	CListBox my_msg_list;

	DECLARE_MESSAGE_MAP()
	//afx_msg void OnIddDialog1();

public:
	enum { IDD = IDD_FORMVIEW};
#ifdef _DEBUG virtual void AssertValid() const;
#ifndef _WIN32_WCE virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

