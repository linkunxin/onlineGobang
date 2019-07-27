
// WuziqiView.h: CWuziqiView 类的接口
//

#pragma once
#include <mmsystem.h>
#define BKLocation_xy 60
#define WidthAndHigh 15
#define SizeOfQiZi  25
#define IDC_OK 100
#define IDC_OK1 101
#define IDC_OK2 102
#define IDC_OK3 103
#define IDC_OK4 104
#define IDC_OK5 105
#define IDC_MYEDIT 106
#define Listbox 107
#define IDC_MYEDIT1 108

class CWuziqiView : public CView
{
protected: // 仅从序列化创建
	CWuziqiView() noexcept;
	DECLARE_DYNCREATE(CWuziqiView)
// 特性
public:
	int currentColor = 0;
	CWuziqiDoc* GetDocument() const;
	CButton m_wndPushButton;
	CButton pButton4;
	int x = -1, y = -1;//记录下棋XY坐标，用于画最新棋子落地位置，主要用于悔棋定位
	int PKgame_times = -1;
	CEdit edit;
	CListBox list_box;
	CStatic static_text;
	CEdit edit_show;
// 操作
public:
	int getCoordinate(int x);
	void DrawChess(CDC *dc, CRect rect, COLORREF color);
	bool AIPlay(int Location_x, int Location_y);
	int Robot4Qizi();
	int Robot3QiZi();
	void RobotPlayer(int Location_x, int Location_y);
	bool CWuziqiView::ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName);
	void CWuziqiView::init();
	void CWuziqiView::play();
	TCHAR mcicmd[300];
	void Draw_pos(int x, int y , int regret, char* ch);//CWuziqiView::[100]
	void aa();
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// 实现
public:
	virtual ~CWuziqiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// 生成的消息映射函数
protected:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void ID_REGRET(CCmdUI *pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPushButtonClicked();
	afx_msg void OnPushButtonClicked1();
	afx_msg void OnPushButtonClicked2();
	afx_msg void OnPushButtonClicked3();
	afx_msg void OnPushButtonClicked4();
	afx_msg void OnPushButtonClicked5();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedButton1();
	afx_msg void person_and_computer();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedCheck1();
};
void draw_pos(int px, int py ,int regret, char ch[100]);
#ifndef _DEBUG  // WuziqiView.cpp 中的调试版本
inline CWuziqiDoc* CWuziqiView::GetDocument() const
   { return reinterpret_cast<CWuziqiDoc*>(m_pDocument); }
#endif

