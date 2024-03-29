
// MainFrm.h: CMainFrame 类的接口
//

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)//动态创建对象（窗口什么的吧）

// 特性
public:
	CListBox list_box;
	CSplitterWnd Splitter;
// 操作
public:
	CStatusBar        m_wndStatusBar;
// 重写
public:
	//virtual BOOL MoveWindow(int, int,int,int);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);//重写窗口
	void updateStatusBar(int id, int info);//创建状态栏，就是窗口下面的小框
	//void updateCurrentPaneColor(int color);
	void updateNextChessColor(int color);//更新状态栏的下一个下棋子的颜色
	virtual int  OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CButton m_wndPushButton;
	CToolBar toolBar;
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//int OnCreateClient();

};


