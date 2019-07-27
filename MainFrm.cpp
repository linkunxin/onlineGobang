
// MainFrm.cpp: CMainFrame 类的实现
//

#include "stdafx.h"
#include "Wuziqi.h"
#include "WuziqiDoc.h"
#include "MainFrm.h"
#include "CMyormDlg.h"
#include "WuziqiDoc.h"
#include "WuziqiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_INDICATOR_TIME 0
//#define Listbox 1000

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)//动态创建对象（窗口什么的吧）

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	ID_INDICATOR_TIME,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_STRETCH, 0);
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, 250);
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NORMAL, 200);
	m_wndStatusBar.SetPaneInfo(6, ID_SEPARATOR, SBPS_NORMAL, 280);

	// 启动定时器，定时器ID为1，定时时间为1000ms，即1s   
	SetTimer(1, 1000, NULL);

	/*CRect rect(905, 450, 1250, 800);
	int a = list_box.CreateEx(WS_EX_CLIENTEDGE, _T("聊天天啦!"), NULL, WS_VISIBLE | WS_CHILD | LBS_STANDARD, rect, this, Listbox);//WS_EX_TRANSPARENT

	if (a == 0)
	{
		MessageBox(TEXT("sdfsdfsdfas............."));
	}
	//list_box.ResetContent();
	//list_box.AddString(TEXT("sdfsdfsdljl了健身房里看电视"));
	ShowWindow(TRUE);
	UpdateWindow();
	*/
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.cx = 1630;
	cs.cy = 1000;
	//MoveWindow(500, 20,1900,1020,TRUE);
	cs.x = 200;
	cs.y = 10;
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::updateNextChessColor(int color)
{
	CString tips;
	if (color == 1) {
		tips = "\tNext color is: Black";
	}
	else {
		tips = "\tNext color is: Write";
	}
	m_wndStatusBar.SetPaneText(1, tips);
}

void CMainFrame::updateStatusBar(int id, int info)
{
	switch (id) {
	case 1:
		updateNextChessColor(info);
		break;
	case 2:
		//OnExitSizeMove();
		//updateCurrentPaneColor(info);
		break;
	}
}



void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	str.Format(_T("\tx = %d, y = %d"), point.x, point.y);

	m_wndStatusBar.SetPaneText(6, str);

	CFrameWnd::OnMouseMove(nFlags, point);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString strTime;
	// 获取系统当前时间，并保存到curTime   
	CTime curTime = CTime::GetCurrentTime();

	// 格式化curTime，将字符串保存到strTime   
	strTime = curTime.Format(_T("\t%H:%M:%S"));
	// 在状态栏的时间窗格中显示系统时间字符串   
	m_wndStatusBar.SetPaneText(6, strTime);



	CFrameWnd::OnTimer(nIDEvent);
}

int CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	Splitter.CreateStatic(this, 1, 2); // 分窗口
	Splitter.CreateView(0, 0, RUNTIME_CLASS(CWuziqiView), CSize(0, 0), pContext); //左侧为视图
	Splitter.CreateView(0, 1, RUNTIME_CLASS(CMyFormDlg), CSize(0, 0), pContext); //右侧为对话框
	//以下设置窗口大小
	CRect rc;
	int width;
	GetClientRect(&rc);//获取整个客户区大小
	width = rc.Width() - 360;//设置对话框区域
	Splitter.SetColumnInfo(0, width, 0);
	Splitter.RecalcLayout();

	return true;


}
