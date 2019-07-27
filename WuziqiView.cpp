
// WuziqiView.cpp: CWuziqiView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Wuziqi.h"
#endif
#include<iostream>
#include "WuziqiDoc.h"
#include "WuziqiView.h"
#include "Chess.h"
#include "MainFrm.h"
#include "ChessManager.h"
#include "chess_cli.h"
//#include<Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





int g_Brick[15][15];
int g_Brick_verify[15][15];
 CWuziqiView *obj = NULL;
// CWuziqiView

IMPLEMENT_DYNCREATE(CWuziqiView, CView)

BEGIN_MESSAGE_MAP(CWuziqiView, CView)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	//ON_COMMAND(ID_REGRET, &CWuziqiView::OnRegret)
	ON_UPDATE_COMMAND_UI(ID_32771, &CWuziqiView::ID_REGRET)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_OK, OnPushButtonClicked)
	ON_BN_CLICKED(IDC_OK1, OnPushButtonClicked1)
	ON_BN_CLICKED(IDC_OK2, OnPushButtonClicked2)
	ON_BN_CLICKED(IDC_OK3, OnPushButtonClicked3)
	ON_BN_CLICKED(IDC_OK4, OnPushButtonClicked4)
	ON_BN_CLICKED(IDC_OK5, OnPushButtonClicked5)



	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_BUTTON1, &CWuziqiView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CWuziqiView::person_and_computer)
	ON_BN_CLICKED(IDC_BUTTON2, &CWuziqiView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CWuziqiView::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK1, &CWuziqiView::OnBnClickedCheck1)
END_MESSAGE_MAP()

// CWuziqiView 构造/析构

CWuziqiView::CWuziqiView() noexcept
{
	// TODO: 在此处添加构造代码
}

CWuziqiView::~CWuziqiView()
{
}

BOOL CWuziqiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	
	return CView::PreCreateWindow(cs);
}

// CWuziqiView 绘图

int CWuziqiView::getCoordinate(int x)
{
	if (x < 60 || x>960) {
		return -1;
	}
	else {
		int minPx = x / 60;
		x - minPx * 60 < (minPx + 1) * 60 - x ? minPx = minPx : minPx = minPx + 1;
		return minPx;
	}
}

void CWuziqiView::DrawChess(CDC *dc, CRect rect, COLORREF color)
{
	CBrush brush(color);
	dc->SelectObject(brush);
	dc->Ellipse(rect);
}

void CWuziqiView::OnDraw(CDC* pDoc)
{
	CWuziqiDoc* doc = GetDocument();
	ASSERT_VALID(doc);
	if (!doc)
		return;
	//CMyDVDoc* doc = GetDocument();
	// TODO: 在此处为本机数据添加绘制代码
	/*CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP1);

	CBrush brush;
	brush.CreatePatternBrush(&bmp);
	CBrush*   pOldBrush = pDoc->SelectObject(&brush);

	CRect rect;
	GetClientRect(&rect);
	//pDoc.Rectangle(0, 0, rect.Width(), rect.Height()); // 这些参数可以调整图片添加位置和大小
	pDoc->Rectangle(0, 0, 1000, 1000);   // 这些参数可以调整图片添加位置和大小
	pDoc->SelectObject(pOldBrush);

	//pDoc->FillSolidRect(rect, backgroundColor);
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* oldPen = pDoc->SelectObject(&pen);
	for (int i = 50; i < 800; i += 50) {
		pDoc->MoveTo(50, i);
		pDoc->LineTo(750, i);
		pDoc->MoveTo(i, 50);
		pDoc->LineTo(i, 750);
	}*/

	// TODO: 在此处为本机数据添加绘制代码
}

// CWuziqiView 诊断

#ifdef _DEBUG
void CWuziqiView::AssertValid() const
{
	CView::AssertValid();
}

void CWuziqiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWuziqiDoc* CWuziqiView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWuziqiDoc)));
	return (CWuziqiDoc*)m_pDocument;
}
#endif //_DEBUG

// CWuziqiView 消息处理程序

void CWuziqiView::OnLButtonUp(UINT nFlags, CPoint point)
{
	int px = getCoordinate(point.x); //需要减一才能作为坐标
	int py = getCoordinate(point.y);
	CWuziqiDoc* doc = GetDocument();
	ChessManager* chessManager = doc->getChessManager();
	//双人模式
	if (px != -1 && py != -1&& chessManager->gameBegin) {
		if (doc->addChess(px - 1, py - 1)) {//可以添加棋子
			init();
			CRect rect(px * 60 - 25, py * 60 - 25, px * 60 + 25, py * 60 + 25);
			CClientDC dc(this);
			//mciSendString(_T("play C:\\Users\\18451\\Desktop\\落地声.wav"), NULL, 0, NULL);
			//sndPlaySound((LPCTSTR)IDR_WAVE1, SND_RESOURCE | SND_ASYNC);
			/*CWuziqiView::init();
			CWuziqiView::init();
			CWuziqiView::init();*/
			play();

			//PlaySound((LPCTSTR)IDR_WAVE1, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);

			
			int colorInt = 0;
			if (doc->getNextColor() == 0) {//绘制白色棋子
				DrawChess(&dc, rect, RGB(255, 255, 255));
				CRect rect2(px * 60 - 5, py * 60 - 5, px * 60 + 5, py * 60 + 5);
				CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
				dc.SelectObject(pen);
				dc.MoveTo(px * 60 - 7, py * 60);
				dc.LineTo(px * 60 + 5, py * 60);
				dc.MoveTo(px * 60, py * 60 - 5);
				dc.LineTo(px * 60, py * 60 + 5);
				//dc.Ellipse(rect2);		
				if (x != -1) {
					CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
					dc.SelectObject(pen);
					dc.MoveTo(x * 60 - 7, y * 60);
					dc.LineTo(x * 60 + 5, y * 60);
					dc.MoveTo(x * 60, y * 60 - 5);
					dc.LineTo(x * 60, y * 60 + 5);
				}
				x = px;
				y = py;
				colorInt = 0;
			}
			else {//绘制黑色棋子
				DrawChess(&dc, rect, RGB(0, 0, 0));
				CRect rect2(px * 60 - 5, py * 60 - 5, px * 60 + 5, py * 60 + 5);

				CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
				dc.SelectObject(pen);
				dc.MoveTo(px * 60 - 7, py * 60);
				dc.LineTo(px * 60 + 5, py * 60);
				dc.MoveTo(px * 60, py * 60 - 5);
				dc.LineTo(px * 60, py * 60 + 5);
				//dc.Ellipse(rect2);
				if (x != -1) {
					CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
					dc.SelectObject(pen);
					dc.MoveTo(x * 60 - 7, y * 60);
					dc.LineTo(x * 60 + 5, y * 60);
					dc.MoveTo(x * 60, y * 60 - 5);
					dc.LineTo(x * 60, y * 60 + 5);
				}
				x = px;
				y = py;
				colorInt = 1;
			}
			if (doc->checkGameOver(px - 1, py - 1, colorInt)) {
				CString winner;
				winner = colorInt == 1 ? "黑色" : "白色";
				CWuziqiView::MessageBox(_T("游戏结束," + winner + "胜利"), _T("确定"), MB_OK);
				//在这里应该有新开一局游戏的机制
			//更新棋子颜色状态
				CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
				p->updateStatusBar(1, doc->getCurrentColor());
																
			}
		}
	}
	//人机模式
	if (px != -1 && py != -1 && chessManager->gameBeginAI) {
		if (doc->addChess(px - 1, py - 1)) {//可以添加棋子
			init();

			CRect rect(px * 60 - 25, py * 60 - 25, px * 60 + 25, py * 60 + 25);
			CClientDC dc(this);
			doc->getNextColor();
			DrawChess(&dc, rect, RGB(0, 0, 0));
			g_Brick[px-1][py-1] = 1;

			CRect rect2(px * 60 - 5, py * 60 - 5, px * 60 + 5, py * 60 + 5);

			CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
			dc.SelectObject(pen);
			dc.MoveTo(px * 60 - 7, py * 60);
			dc.LineTo(px * 60 + 5, py * 60);
			dc.MoveTo(px * 60, py * 60 - 5);
			dc.LineTo(px * 60, py * 60 + 5);
			//dc.Ellipse(rect2);
			if (x != -1) {
				CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
				dc.SelectObject(pen);
				dc.MoveTo(x * 60 - 7, y * 60);
				dc.LineTo(x * 60 + 5, y * 60);
				dc.MoveTo(x * 60, y * 60 - 5);
				dc.LineTo(x * 60, y * 60 + 5);
			}
			x = px;
			y = py;
			//PlaySound((LPCTSTR)IDR_WAVE1, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			//mciSendString(_T("play C:\\Users\\18451\\Desktop\\落地声.wav"), NULL, 0, NULL);
			//sndPlaySound((LPCTSTR)IDR_WAVE1, SND_RESOURCE | SND_ASYNC);
			/*CWuziqiView::init();
			CWuziqiView::init();
			CWuziqiView::init();*/
			play();
			if (doc->checkGameOver(px - 1, py - 1, 1))
			{
				CWuziqiView::MessageBox(TEXT("游戏结束,黑棋胜利"), _T("确定"), MB_OK);
				//send_rst();
			}
			CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
			p->updateStatusBar(1, doc->getCurrentColor());




			RobotPlayer(0, 0);
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j] == 3&& g_Brick_verify[i][j]==0)
					{
						//MessageBox(TEXT("ddd"));
						g_Brick_verify[i][j] = 1;
						px = i+1;
						py = j+1;
					}
				}			
			
			if (doc->addChess(px - 1, py - 1)) {
				CRect rect1(px * 60 - 25, py * 60 - 25, px * 60 + 25, py * 60 + 25);
				CClientDC dc1(this);
				//PlaySound((LPCTSTR)IDR_WAVE1, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				//mciSendString(_T("play C:\\Users\\18451\\Desktop\\落地声.wav"), NULL, 0, NULL);
				//sndPlaySound((LPCTSTR)IDR_WAVE1, SND_RESOURCE | SND_ASYNC);
				/*CWuziqiView::init();
				CWuziqiView::init();
				CWuziqiView::init();*/ 
				play();

				doc->getNextColor();
				DrawChess(&dc1, rect1, RGB(255, 255, 255));

				CRect rect2(px * 60 - 5, py * 60 - 5, px * 60 + 5, py * 60 + 5);
				CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
				dc1.SelectObject(pen);
				dc1.MoveTo(px * 60 - 7, py * 60);
				dc1.LineTo(px * 60 + 5, py * 60);
				dc1.MoveTo(px * 60, py * 60 - 5);
				dc1.LineTo(px * 60, py * 60 + 5);
				//dc.Ellipse(rect2);		
				if (x != -1) {
					CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
					dc1.SelectObject(pen);
					dc1.MoveTo(x * 60 - 7, y * 60);
					dc1.LineTo(x * 60 + 5, y * 60);
					dc1.MoveTo(x * 60, y * 60 - 5);
					dc1.LineTo(x * 60, y * 60 + 5);
				}
				x = px;
				y = py;
				if (doc->checkGameOver(px - 1, py - 1, 0))
				{
					CWuziqiView::MessageBox(TEXT("游戏结束,白棋胜利"), _T("确定"), MB_OK);
					//send_rst();
				}
			}

		}
		
	}
	//在线PK
	if (px != -1 && py != -1 && chessManager->gameBeginPK) {
		if (doc->addChess(px - 1, py - 1)) {//可以添加棋子
			init();
			CRect rect(px * 60 - 25, py * 60 - 25, px * 60 + 25, py * 60 + 25);
			CClientDC dc(this);

			if (chessManager->nextColorInt == 1) {
				DrawChess(&dc, rect, RGB(0, 0, 0));
				CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
				dc.SelectObject(pen);
				dc.MoveTo(px * 60 - 7, py * 60);
				dc.LineTo(px * 60 + 5, py * 60);
				dc.MoveTo(px * 60, py * 60 - 5);
				dc.LineTo(px * 60, py * 60 + 5);
				//dc.Ellipse(rect2);
				if (x != -1) {
					CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
					dc.SelectObject(pen);
					dc.MoveTo(x * 60 - 7, y * 60);
					dc.LineTo(x * 60 + 5, y * 60);
					dc.MoveTo(x * 60, y * 60 - 5);
					dc.LineTo(x * 60, y * 60 + 5);
				}
				x = px;
				y = py;
			}
			else {
				DrawChess(&dc, rect, RGB(255, 255, 255));
				CRect rect2(px * 60 - 5, py * 60 - 5, px * 60 + 5, py * 60 + 5);
				CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
				dc.SelectObject(pen);
				dc.MoveTo(px * 60 - 7, py * 60);
				dc.LineTo(px * 60 + 5, py * 60);
				dc.MoveTo(px * 60, py * 60 - 5);
				dc.LineTo(px * 60, py * 60 + 5);
				//dc.Ellipse(rect2);		
				if (x != -1) {
					CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
					dc.SelectObject(pen);
					dc.MoveTo(x * 60 - 7, y * 60);
					dc.LineTo(x * 60 + 5, y * 60);
					dc.MoveTo(x * 60, y * 60 - 5);
					dc.LineTo(x * 60, y * 60 + 5);
				}
				x = px;
				y = py;
			}

			//doc->getNextColor();

			play();

			send_pos(px, py);//发送位置给对手

			//判断游戏是否结束
			if (doc->checkGameOver(px - 1, py - 1, 1))
			{
				CWuziqiView::MessageBox(TEXT("游戏结束,黑棋胜利"), _T("确定"), MB_OK);
				chessManager->gameBeginPK = false;

				send_rst();
			}
			if (doc->checkGameOver(px - 1, py - 1, 0))
			{
				CWuziqiView::MessageBox(TEXT("游戏结束,白棋胜利"), _T("确定"), MB_OK);
				chessManager->gameBeginPK = false;

				send_rst();
			}



			//更新棋子颜色状态
			CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
			p->updateStatusBar(1, doc->getCurrentColor());
			
			chessManager->gameBeginPK = false;
			//int a = px + 1;
			//int b = py + 1;
			//draw_pos(a,b);

			//aaa((LPVOID)&draw_pos);
			//Draw_pos(px, py);

		}

	}
	//Invalidate(false);
	//CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	//p->updateStatusBar(1, doc->getCurrentColor());
	

}

void CWuziqiView::Draw_pos(int px,int py ,int status, char* ch) {//[100]

	CWuziqiDoc* doc = GetDocument();
	ChessManager* chessManager = doc->getChessManager();
	
	if (status == 0)
	{
		CRect rect1(px * 60 - 25, py * 60 - 25, px * 60 + 25, py * 60 + 25);
		CClientDC dc1(this);
		
		if (chessManager->nextColorInt == 1) {

			doc->getNextColor();

			doc->addChess(px - 1, py - 1);
			DrawChess(&dc1, rect1, RGB(255, 255, 255));

			CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
			dc1.SelectObject(pen);
			dc1.MoveTo(px * 60 - 7, py * 60);
			dc1.LineTo(px * 60 + 5, py * 60);
			dc1.MoveTo(px * 60, py * 60 - 5);
			dc1.LineTo(px * 60, py * 60 + 5);
			//dc.Ellipse(rect2);
			if (x != -1) {
				CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
				dc1.SelectObject(pen);
				dc1.MoveTo(x * 60 - 7, y * 60);
				dc1.LineTo(x * 60 + 5, y * 60);
				dc1.MoveTo(x * 60, y * 60 - 5);
				dc1.LineTo(x * 60, y * 60 + 5);
			}
			x = px;
			y = py;
			doc->getNextColor();
		}

		else {
			CString str;
			str.Format(L"%d", chessManager->nextColorInt);
			//MessageBox(str);

			doc->getNextColor();

			doc->addChess(px - 1, py - 1);
			DrawChess(&dc1, rect1, RGB(0, 0, 0));
			CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
			dc1.SelectObject(pen);
			dc1.MoveTo(px * 60 - 7, py * 60);
			dc1.LineTo(px * 60 + 5, py * 60);
			dc1.MoveTo(px * 60, py * 60 - 5);
			dc1.LineTo(px * 60, py * 60 + 5);
			//dc.Ellipse(rect2);		
			if (x != -1) {
				CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
				dc1.SelectObject(pen);
				dc1.MoveTo(x * 60 - 7, y * 60);
				dc1.LineTo(x * 60 + 5, y * 60);
				dc1.MoveTo(x * 60, y * 60 - 5);
				dc1.LineTo(x * 60, y * 60 + 5);
			}
			x = px;
			y = py;
			doc->getNextColor();

		}

		play();
		chessManager->gameBeginPK = true;
		if (doc->checkGameOver(px - 1, py - 1, 1))
		{
			CWuziqiView::MessageBox(TEXT("游戏结束,黑棋胜利"), _T("确定"), MB_OK);
			chessManager->gameBeginPK = false;
			send_rst();
		}
		if (doc->checkGameOver(px - 1, py - 1, 0))
		{
			CWuziqiView::MessageBox(TEXT("游戏结束,白棋胜利"), _T("确定"), MB_OK);
			chessManager->gameBeginPK = false;

			send_rst();
		}

		

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		//p->updateStatusBar(1, doc->getCurrentColor());

	}
	
	else
	{
		if (status == regret)
		{
			int 结果 = MessageBox(TEXT("对方请求悔棋。。。"), TEXT("悔棋"), MB_YESNO | MB_ICONINFORMATION);//, MB_OKCANCEL
			if (结果 == IDNO)
			{
				//MessageBox(TEXT("sdfasdfasfas..."));
				send_no_regret();
			}
				
			else if (结果 == IDYES)
			{
				//MessageBox(TEXT("对"));
				Chess* lastChess = chessManager->getLastChess();
				int color = lastChess->color;
				if (chessManager->nextColorInt != color)
				{

					agree_regret();
					GetDocument()->undoOperation();
					chessManager->gameBeginPK = false;

					Invalidate(0);
				}
				else
				{
					agree_regret();
					GetDocument()->undoOperation();
					GetDocument()->undoOperation();
					chessManager->gameBeginPK = false;

					Invalidate(0);
				}
			}
		}
		
		else if (status == no_regret)
		{
			MessageBox(TEXT("对方拒绝了你的请求。。。"));
		}
		
		else if (status == agree_regret_num)
		{
			MessageBox(TEXT("对方已同意，即将帮你撤销棋子。。。"));
			Chess* lastChess = chessManager->getLastChess();
			int color = lastChess->color;
			if (chessManager->nextColorInt == color)
			{

				GetDocument()->undoOperation();
				chessManager->gameBeginPK = true;
				Invalidate(0);
			}
			else
			{
				GetDocument()->undoOperation();
				GetDocument()->undoOperation();
				chessManager->gameBeginPK = true;

				Invalidate(0);
			}
		}

		else if (status == new_game)
		{
			MessageBox(TEXT("对手新开了一局 , 想玩就尽快开吧，这次换一个人先下喔"));
		}
		else if (status == 8)
		{
			CString str;
			str = CString(ch);
			list_box.AddString(str);
			//str.Format(_T("%s"), ch);
			//MessageBox(str);
			//from char* to CString
			//char *ch = "char pointer.";
			//CString cStr1 = ch;
			//printCString(cStr1);
			//printCString(cStr2);

			/*char chArray[] = "Char  test";
			TCHAR * p = _T("Char  test");//(或LPTSTR p = _T("Char  test");)
			CString theString;
			theString.Format(_T("%s"), chArray);
			theString = p;*/

			/*int len = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)str, -1, NULL, 0);

			unsigned short * wszGBK = new unsigned short[len + 1];

			memset(wszGBK, 0, len * 2 + 2);

			MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)str, -1, (LPWSTR)wszGBK, len);


			len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);

			char *szGBK = new char[len + 1];

			memset(szGBK, 0, len + 1);

			WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);



			strUtf8 = szGBK;

			delete[] szGBK;

			delete[] wszGBK;*/

		

			//GetDlgItem(IDC_LIST2)->SetDlgItemTextW(TEXT("sdfs"));//SetWindowTextW(_T("asd"));
			
			//strcpy(char, CString, sizeof(char));

			
		}
	}
}



bool CWuziqiView::ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName) {
	//中间调用函数// 创建文件
	HANDLE hFile = ::CreateFile(strDstFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	// 查找资源文件中、加载资源到内存、得到资源大小
	HRSRC    hRes = ::FindResource(NULL, strResName, strResType);
	HGLOBAL    hMem = ::LoadResource(NULL, hRes);
	DWORD    dwSize = ::SizeofResource(NULL, hRes);
	// 写入文件
	DWORD dwWrite = 0;      // 返回写入字节
	::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
	::CloseHandle(hFile);

	return true;
}
//--------------------------------------------------------------

void CWuziqiView::init() {//初始化函数
	// 产生临时文件的文件名
	TCHAR tmpwav[_MAX_PATH];
	::GetTempPath(_MAX_PATH, tmpwav);
	_tcscat_s(tmpwav, _T("testapp_background.wav"));        // VC2008、VC2010 请用 _tcscat_s 函数
	// 将wav 资源提取为临时文件
	//ExtractResource(tmpwav,_T("WAVE"), "background.mp3");
	// 注：如果wave资源的 ID 为宏 IDR_BACKGROUND，这样做：
	CWuziqiView::ExtractResource(tmpwav, _T("WAVE"), MAKEINTRESOURCE(IDR_WAVE1));
	// 打开音乐
	//TCHAR mcicmd[300];
	_stprintf_s(mcicmd, _T("open \"%s\" alias mymusic"), tmpwav);        // VC2008、VC2010 请用 _stprintf_s 函数
	/*/mciSendString(mcicmd, NULL, 0, NULL);
	// 播放音乐
	//mciSendString(_T("play mymusic"), NULL, 0, NULL);
	// 停止播放并关闭音乐
	//mciSendString(TEXT("stop mymusic"), NULL, 0, NULL);
	//mciSendString(TEXT("close mymusic"), NULL, 0, NULL);
	// 删除临时文件
	//DeleteFile(tmpmp3);*/
}
//--------------------------------------------------------------
//如果是初始化里的情况只能播放一次，想要多次播放，先要把mcicmd[]设为全局变量
//再把:下面几条语句注释掉
//mciSendString(mcicmd, NULL,0, NULL);
//mciSendString(_T("play mymusic"), NULL, 0, NULL);
//mciSendString(("stop mymusic"), NULL, 0, NULL);
//mciSendString(("close mymusic"), NULL, 0, NULL);
//DeleteFile(tmpmp3);
//再在该调用的位置使用下面的play函数！！！
void CWuziqiView::play() {//播放函数
	mciSendString(_T("close mymusic"), NULL, 0, NULL);//先关再开，注意顺序！！！
	mciSendString(mcicmd, NULL, 0, NULL);
	mciSendString(_T("play mymusic"), NULL, 0, NULL);
}//------------------------------------------------------------

//判断玩家连子数并作出对策
void CWuziqiView::RobotPlayer(int Location_x, int Location_y)
{

	//判断玩家连子数
	if (Robot4Qizi() == 0)//AI没有4个子
	{
		//玩家斜着4个子1
		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 2][j + 2] == 1 && g_Brick[i + 3][j + 3] == 1)
				{
					//MessageBox(TEXT("看看大放送"));

					if (g_Brick[i + 4][j + 4] == 0 && g_Brick[i - 1][j - 1] == 0)
					{
						/*if (Robot3QiZi() == 0)
						{
							AIPlay(Location_x, Location_y);
							return;
						}
						else
							return;*/
						g_Brick[i + 4][j + 4] = 3;
						return;
					}
					if (g_Brick[i + 4][j + 4] == 0 && g_Brick[i - 1][j - 1] != 0)
					{
						if (i + 4 < WidthAndHigh && j + 4 < WidthAndHigh)
						{
							g_Brick[i + 4][j + 4] = 3;
							return;
						}

					}
					if (g_Brick[i + 4][j + 4] != 0 && g_Brick[i - 1][j - 1] == 0)
					{
						if (i - 1 >= 0 && j - 1 >= 0)
						{
							g_Brick[i - 1][j - 1] = 3;
							return;
						}
					}
					if (g_Brick[i + 4][j + 4] != 0 && g_Brick[i - 1][j - 1] != 0)
					{
						if (Robot3QiZi() == 0)
						{
							AIPlay(Location_x, Location_y);
							return;
						}
						else
							return;
					}
				}
			}

		//玩家斜着4个子2
		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i][j] == 1 && g_Brick[i - 1][j + 1] == 1 && g_Brick[i - 2][j + 2] == 1 && g_Brick[i - 3][j + 3] == 1)
				{
					//MessageBox(TEXT("看看大放送"));

					//MessageBox(_T("玩家斜着4个子"));
					if (g_Brick[i - 4][j + 4] == 0 && g_Brick[i + 1][j - 1] == 0)
					{

						/*if (Robot3QiZi() == 0)
						{
							AIPlay(Location_x, Location_y);
							return;
						}
						else
							return;*/
						g_Brick[i - 4][j + 4] = 3;
						return;
					}
					if (g_Brick[i + 1][j - 1] != 0 && g_Brick[i - 4][j + 4] == 0)
					{
						if (i - 4 >= 0 && j + 4 < WidthAndHigh)
						{
							g_Brick[i - 4][j + 4] = 3;
							return;
						}
					}
					if (g_Brick[i + 1][j - 1] == 0 && g_Brick[i - 4][j + 4] != 0)
					{
						if (i + 1 < WidthAndHigh && j - 1 >= 0)
						{
							g_Brick[i + 1][j - 1] = 3;
							return;
						}
					}
					if (g_Brick[i - 4][j + 4] != 0 && g_Brick[i + 1][j - 1] != 0)
					{
						if (Robot3QiZi() == 0)
						{
							AIPlay(Location_x, Location_y);
							return;
						}
						else
							return;
					}
				}
			}
		//玩家竖着4个子
		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i][j] == 1 && g_Brick[i][j + 1] == 1 && g_Brick[i][j + 2] == 1 && g_Brick[i][j + 3] == 1)
				{
					//MessageBox(TEXT("看看大放送"));

					if (g_Brick[i][j + 4] == 0 && g_Brick[i][j - 1] == 0)
					{
						/*if (Robot3QiZi() == 0)
						{
							AIPlay(Location_x, Location_y);
							return;
						}
						else
							return;*/
						g_Brick[i][j + 4] = 3;
						return;
					}
					else if (g_Brick[i][j - 1] != 0 && g_Brick[i][j + 4] == 0)
					{
						if (j + 4 < WidthAndHigh)
						{
							g_Brick[i][j + 4] = 3;
							return;
						}
					}
					else if (g_Brick[i][j - 1] == 0 && g_Brick[i][j + 4] != 0)
					{
						if (j - 1 >= 0)
						{
							g_Brick[i][j - 1] = 3;
							return;
						}
					}
					else if (g_Brick[i][j - 1] != 0 && g_Brick[i][j + 4] != 0)
						if (Robot3QiZi() == 0)
						{
							AIPlay(Location_x, Location_y);
							return;
						}
						else
							return;
				}
			}

		//玩家横着4个子
		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i][j] == 1 && g_Brick[i + 1][j] == 1 && g_Brick[i + 2][j] == 1 && g_Brick[i + 3][j] == 1)
				{
					//MessageBox(TEXT("看看大放送"));
					if (g_Brick[i + 4][j] == 0 && g_Brick[i - 1][j] == 0)
					{
						/*if (Robot3QiZi() == 0)
						{
							AIPlay(Location_x, Location_y);
							return;
						}
						else
							return;*/
						g_Brick[i + 4][j] = 3;
						return;
					}
					if (g_Brick[i - 1][j] != 0 && g_Brick[i + 4][j] == 0)
					{
						if (i + 4 < WidthAndHigh)
						{
							g_Brick[i + 4][j] = 3;
							return;
						}
					}
					if (g_Brick[i - 1][j] == 0 && g_Brick[i + 4][j] != 0)
					{
						if (i - 1 >= 0)
						{
							g_Brick[i - 1][j] = 3;
							return;
						}
					}
					if (g_Brick[i - 1][j] != 0 && g_Brick[i + 4][j] != 0)
						if (Robot3QiZi() == 0)
						{
							AIPlay(Location_x, Location_y);
							return;
						}
						else
							return;
				}
			}

		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i - 1][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 0 && g_Brick[i + 2][j + 2] == 1 && g_Brick[i + 3][j + 3] == 1 )
				{
					//MessageBox(TEXT("sizi"));

					if (i + 1 < WidthAndHigh && j + 1 < WidthAndHigh)
					{
						g_Brick[i + 1][j + 1] = 3;
						return;
					}
				}
			}
		//1011型
		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i - 1][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 2][j + 2] == 0 && g_Brick[i + 3][j + 3] == 1)
				{
					//MessageBox(TEXT("sizi"));
					if (i + 2 < WidthAndHigh && j + 2 < WidthAndHigh)
					{
						g_Brick[i + 2][j + 2] = 3;
						return;
					}
				}
			}

		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i - 1][j - 1] == 1 && g_Brick[i][j] == 0 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 2][j + 2] == 1 && g_Brick[i + 3][j + 3] == 1)
				{
					//MessageBox(TEXT("sizi"));
					if (i + 2 < WidthAndHigh && j + 2 < WidthAndHigh)
					{
						g_Brick[i][j] = 3;
						return;
					}
				}
			}

		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i + 1][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i - 1][j + 1] == 0 && g_Brick[i - 2][j + 2] == 1 && g_Brick[i - 3][j + 3] == 1 )
				{
					//MessageBox(TEXT("sizi"));
					if (i - 1 >= 0 && j + 1 < WidthAndHigh)
					{
						g_Brick[i - 1][j + 1] = 3;
						return;
					}
				}
			}

		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i + 1][j - 1] == 1 && g_Brick[i][j] == 0 && g_Brick[i - 1][j + 1] == 1 && g_Brick[i - 2][j + 2] == 1 && g_Brick[i - 3][j + 3] == 1)
				{
					if (i - 1 >= 0 && j + 1 < WidthAndHigh)
					{
						g_Brick[i][j] = 3;
						return;
					}
				}
			}

		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i + 1][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i - 1][j + 1] == 1 && g_Brick[i - 2][j + 2] == 0 && g_Brick[i - 3][j + 3] == 1)
				{
					if (i - 2 >= 0 && j + 2 < WidthAndHigh)
					{
						g_Brick[i - 2][j + 2] = 3;
						return;
					}
				}
			}
		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i][j + 1] == 0 && g_Brick[i][j + 2] == 1 && g_Brick[i][j + 3] == 1)
				{
					if (j + 1 < WidthAndHigh)
					{
						g_Brick[i][j + 1] = 3;
						return;
					}
				}
			}
		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i][j + 1] == 1 && g_Brick[i][j + 2] == 0 && g_Brick[i][j + 3] == 1)
				{
					if (j + 2 < WidthAndHigh)
					{
						g_Brick[i][j + 2] = 3;
						return;
					}
				}
			}

		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i][j - 1] == 1 && g_Brick[i][j] == 0 && g_Brick[i][j + 1] == 1 && g_Brick[i][j + 2] == 1 && g_Brick[i][j + 3] == 1)
				{
					if (j + 2 < WidthAndHigh)
					{
						g_Brick[i][j] = 3;
						return;
					}
				}
			}


		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j] == 0 && g_Brick[i + 2][j] == 1 && g_Brick[i + 3][j] == 1)
				{
					if (i + 1 < WidthAndHigh)
					{
						g_Brick[i + 1][j] = 3;
						return;
					}
				}
			}

		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 0 && g_Brick[i + 1][j] == 1 && g_Brick[i + 2][j] == 1 && g_Brick[i + 3][j] == 1)
				{
					if (i + 1 < WidthAndHigh)
					{
						g_Brick[i][j] = 3;
						return;
					}
				}
			}

		for (int i = 0; i < WidthAndHigh; i++)
			for (int j = 0; j < WidthAndHigh; j++)
			{
				if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j] == 1 && g_Brick[i + 2][j] == 0 && g_Brick[i + 3][j] == 1)
				{
					if (i + 2 < WidthAndHigh)
					{
						g_Brick[i + 2][j] = 3;
						return;
					}
				}
			}


		if (0 == Robot3QiZi())//AI没有3个子
		{
			//判断玩家是否有3个子
						//玩家三子情况
						//1101型
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j - 1] != 3 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 0 && g_Brick[i + 2][j + 2] == 1 && g_Brick[i + 3][j + 3] == 1 && g_Brick[i + 4][j + 4] != 3)
					{
						if (i + 1 < WidthAndHigh && j + 1 < WidthAndHigh)
						{
							g_Brick[i + 1][j + 1] = 3;
							return;
						}
					}
				}
			//1011型
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j - 1] != 3 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 2][j + 2] == 0 && g_Brick[i + 3][j + 3] == 1 && g_Brick[i + 4][j + 4] != 3)
					{
						if (i + 2 < WidthAndHigh && j + 2 < WidthAndHigh)
						{
							g_Brick[i + 2][j + 2] = 3;
							return;
						}
					}
				}
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i + 1][j - 1] != 3 && g_Brick[i][j] == 1 && g_Brick[i - 1][j + 1] == 0 && g_Brick[i - 2][j + 2] == 1 && g_Brick[i - 3][j + 3] == 1 && g_Brick[i - 4][j + 4] != 3)
					{
						if (i - 1 >= 0 && j + 1 < WidthAndHigh)
						{
							g_Brick[i - 1][j + 1] = 3;
							return;
						}
					}
				}
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i + 1][j - 1] != 3 && g_Brick[i][j] == 1 && g_Brick[i - 1][j + 1] == 1 && g_Brick[i - 2][j + 2] == 0 && g_Brick[i - 3][j + 3] == 1 && g_Brick[i - 4][j + 4] != 3)
					{
						if (i - 2 >= 0 && j + 2 < WidthAndHigh)
						{
							g_Brick[i - 2][j + 2] = 3;
							return;
						}
					}
				}
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j - 1] != 3 && g_Brick[i][j] == 1 && g_Brick[i][j + 1] == 0 && g_Brick[i][j + 2] == 1 && g_Brick[i][j + 3] == 1 && g_Brick[i][j + 4] != 3)
					{
						if (j + 1 < WidthAndHigh)
						{
							g_Brick[i][j + 1] = 3;
							return;
						}
					}
				}
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j - 1] != 3 && g_Brick[i][j] == 1 && g_Brick[i][j + 1] == 1 && g_Brick[i][j + 2] == 0 && g_Brick[i][j + 3] == 1 && g_Brick[i][j + 4] != 3)
					{
						if (j + 2 < WidthAndHigh)
						{
							g_Brick[i][j + 2] = 3;
							return;
						}
					}
				}
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j] != 3 && g_Brick[i][j] == 1 && g_Brick[i + 1][j] == 0 && g_Brick[i + 2][j] == 1 && g_Brick[i + 3][j] == 1 && g_Brick[i + 4][j] != 3)
					{
						if (i + 1 < WidthAndHigh)
						{
							g_Brick[i + 1][j] = 3;
							return;
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j] != 3 && g_Brick[i][j] == 1 && g_Brick[i + 1][j] == 1 && g_Brick[i + 2][j] == 0 && g_Brick[i + 3][j] == 1 && g_Brick[i + 4][j] != 3)
					{
						if (i + 2 < WidthAndHigh)
						{
							g_Brick[i + 2][j] = 3;
							return;
						}
					}
				}
			//玩家竖着3个子
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{

					if (g_Brick[i][j] == 1 && g_Brick[i][j + 1] == 1 && g_Brick[i][j + 2] == 1)
					{
						//MessageBox(_T("玩家竖着3个子"));
						if (g_Brick[i][j + 3] == 0 && g_Brick[i][j - 1] == 0)
						{
							if (j + 3 < WidthAndHigh)
							{
								g_Brick[i][j + 3] = 3;
								return;
							}
						}
					}
				}
			//玩家横着3个子
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j] == 1 && g_Brick[i + 1][j] == 1 && g_Brick[i + 2][j] == 1)
					{
						//MessageBox(_T("玩家横着3个子"));
						if (g_Brick[i - 1][j] == 0 && g_Brick[i + 3][j] == 0)
						{
							if (i + 3 < WidthAndHigh)
							{
								g_Brick[i + 3][j] = 3;
								return;
							}
						}
					}
				}

			//玩家斜着3个子
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 2][j + 2] == 1)
					{
						//MessageBox(_T("玩家斜着3个子"));
						if (g_Brick[i - 1][j - 1] == 0 && g_Brick[i + 3][j + 3] == 0)
						{
							if (i + 3 < WidthAndHigh && j + 3 < WidthAndHigh)
							{
								g_Brick[i + 3][j + 3] = 3;
								return;
							}
						}
					}
				}
			//玩家斜着3个子
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j] == 1 && g_Brick[i + 1][j - 1] == 1 && g_Brick[i + 2][j - 2] == 1)
					{
						//MessageBox(_T("玩家斜着3个子"));
						if (g_Brick[i - 1][j + 1] == 0 && g_Brick[i + 3][j - 3] == 0)
						{
							if (i + 3 < WidthAndHigh && j - 3 >= 0)
							{
								g_Brick[i + 3][j - 3] = 3;
								return;
							}
						}
					}
				}

			//玩家斜着3个子
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j] == 1 && g_Brick[i - 1][j + 1] == 1 && g_Brick[i - 2][j + 2] == 1)
					{
						//MessageBox(_T("玩家斜着3个子"));
						if (g_Brick[i + 1][j - 1] == 0 && g_Brick[i - 3][j + 3] == 0)
						{
							if (i - 3 >= 0 && j + 3 < WidthAndHigh)
							{
								g_Brick[i - 3][j + 3] = 3;
								return;
							}
						}
					}
				}

			//玩家斜着2个子
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i + 4][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 3][j + 1] == 1)
					{
						if (g_Brick[i + 2][j + 2] == 0)
						{
							//MessageBox(_T("1"));
							if (i + 2 < WidthAndHigh && j + 2 < WidthAndHigh)
							{
								g_Brick[i + 2][j + 2] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i + 2][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i - 1][j + 1] == 1 && g_Brick[i + 3][j + 1] == 1)
					{
						if (g_Brick[i + 1][j - 1] == 0)
						{
							if (i + 1 < WidthAndHigh && j - 1 >= 0)
							{
								//MessageBox(_T("2"));
								g_Brick[i + 1][j - 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i + 2][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 2][j + 1] == 1)
					{
						if (g_Brick[i + 2][j + 2] == 0)
						{
							if (i + 2 < WidthAndHigh && j + 2 < WidthAndHigh)
							{
								//MessageBox(_T("3"));
								g_Brick[i + 2][j + 2] = 3;
								return;
							}
						}
					}
				}
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i - 1][j + 1] == 1)
					{
						if (g_Brick[i - 1][j - 1] == 0)
						{
							if (i - 1 >= 0 && j - 1 >= 0)
							{
								//MessageBox(_T("4"));
								g_Brick[i - 1][j - 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j + 4] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 1][j + 3] == 1)
					{
						if (g_Brick[i + 2][j + 2] == 0)
						{
							if (i + 2 < WidthAndHigh && j + 2 < WidthAndHigh)
							{
								//MessageBox(_T("5"));
								g_Brick[i + 2][j + 2] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j - 2] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 1][j - 3] == 1)
					{
						if (g_Brick[i + 2][j + 2] == 0)
						{
							if (i - 1 >= 0 && j - 1 >= 0)
							{
								//MessageBox(_T("6"));
								g_Brick[i - 1][j - 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i][j + 1] == 1 && g_Brick[i - 2][j + 1] == 1)
					{
						if (g_Brick[i][j - 1] == 0)
						{
							if (j - 1 >= 0)
							{
								//MessageBox(_T("7"));
								g_Brick[i][j - 1] = 3;
								return;
							}
						}
					}
				}
			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j] == 1 && g_Brick[i + 2][j - 1] == 1)
					{
						if (g_Brick[i][j + 1] == 0)
						{
							if (j + 1 < WidthAndHigh)
							{
								//MessageBox(_T("8"));
								g_Brick[i][j + 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i][j + 1] == 1 && g_Brick[i - 1][j + 2] == 1)
					{
						if (g_Brick[i + 1][j] == 0)
						{
							if (i + 1 < WidthAndHigh)
							{
								//MessageBox(_T("9"));
								g_Brick[i + 1][j] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i + 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i][j + 1] == 1 && g_Brick[i + 1][j + 2] == 1)
					{
						if (g_Brick[i - 1][j] == 0)
						{
							if (i - 1 >= 0)
							{
								//MessageBox(_T("10"));
								g_Brick[i - 1][j] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i + 1][j + 2] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i][j + 2] == 1)
					{
						if (g_Brick[i + 2][j + 2] == 0)
						{
							if (i + 2 < WidthAndHigh && j + 2 < WidthAndHigh)
							{
								//MessageBox(_T("11"));
								g_Brick[i + 2][j + 2] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i + 1][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i][j + 1] == 1 && g_Brick[i + 1][j + 1] == 1)
					{
						if (g_Brick[i - 1][j + 1] == 0)
						{
							if (i - 1 >= 0 && j + 1 < WidthAndHigh)
							{
								//MessageBox(_T("12"));
								g_Brick[i - 1][j + 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i - 1][j + 2] == 1 && g_Brick[i - 2][j + 3] == 1)
					{
						if (g_Brick[i][j + 1] == 0)
						{
							if (j + 1 < WidthAndHigh)
							{
								//MessageBox(_T("13"));
								g_Brick[i][j + 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 2] == 1 && g_Brick[i + 2][j + 3] == 1)
					{
						if (g_Brick[i][j + 1] == 0)
						{
							if (j + 1 < WidthAndHigh)
							{
								//MessageBox(_T("14"));
								g_Brick[i][j + 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j + 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 2][j - 1] == 1 && g_Brick[i + 3][j - 1] == 1)
					{
						if (g_Brick[i + 1][j - 1] == 0)
						{
							if (i + 1 < WidthAndHigh && j - 1 >= 0)
							{
								//MessageBox(_T("15"));
								g_Brick[i + 1][j - 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 2][j + 1] == 1 && g_Brick[i + 3][j + 2] == 1)
					{
						if (g_Brick[i + 1][j] == 0)
						{
							if (i + 1 < WidthAndHigh)
							{
								//MessageBox(_T("16"));
								g_Brick[i + 1][j] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i + 1][j + 3] == 1 && g_Brick[i][j] == 1 && g_Brick[i - 1][j - 1] == 1 && g_Brick[i + 1][j + 2] == 1)
					{
						if (g_Brick[i + 1][j + 1] == 0)
						{
							if (i + 1 < WidthAndHigh && j + 1 < WidthAndHigh)
							{
								//MessageBox(_T("17"));
								g_Brick[i + 1][j + 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j + 3] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j - 1] == 1 && g_Brick[i - 1][j + 2] == 1)
					{
						if (g_Brick[i - 1][j + 1] == 0)
						{
							if (i - 1 >= 0 && j + 1 < WidthAndHigh)
							{
								//MessageBox(_T("18"));
								g_Brick[i - 1][j + 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i + 2][j + 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 3][j + 1] == 1)
					{
						if (g_Brick[i + 2][j + 2] == 0)
						{
							if (i + 2 < WidthAndHigh && j + 2 < WidthAndHigh)
							{
								//MessageBox(_T("19"));
								g_Brick[i + 2][j + 2] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 2][j - 1] == 1 && g_Brick[i + 3][j - 2] == 1)
					{
						if (g_Brick[i + 1][j] == 0)
						{
							if (i + 1 < WidthAndHigh)
							{
								//MessageBox(_T("20"));
								g_Brick[i + 1][j] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j - 1] == 1 && g_Brick[i + 1][j - 2] == 1)
					{
						if (g_Brick[i + 1][j] == 0)
						{
							if (i + 1 < WidthAndHigh)
							{
								//MessageBox(_T("21"));
								g_Brick[i + 1][j] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 2][j + 1] == 1)
					{
						if (g_Brick[i][j + 1] == 0)
						{
							if (j + 1 < WidthAndHigh)
							{
								//MessageBox(_T("22"));
								g_Brick[i][j + 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j + 1] == 1 && g_Brick[i + 1][j + 2] == 1)
					{
						if (g_Brick[i + 1][j] == 0)
						{
							if (i + 1 < WidthAndHigh)
							{
								//MessageBox(_T("23"));
								g_Brick[i + 1][j] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 1][j - 1] == 1 && g_Brick[i + 2][j - 1] == 1)
					{
						if (g_Brick[i][j - 1] == 0)
						{
							if (j - 1 >= 0)
							{
								//MessageBox(_T("24"));
								g_Brick[i][j - 1] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i - 1][j] == 1 && g_Brick[i][j] == 1 && g_Brick[i + 2][j] == 1 && g_Brick[i + 3][j] == 1)
					{
						if (g_Brick[i + 1][j] == 0)
						{
							if (i + 1 < WidthAndHigh)
							{
								//MessageBox(_T("25"));
								g_Brick[i + 1][j] = 3;
								return;
							}
						}
					}
				}

			for (int i = 0; i < WidthAndHigh; i++)
				for (int j = 0; j < WidthAndHigh; j++)
				{
					if (g_Brick[i][j - 1] == 1 && g_Brick[i][j] == 1 && g_Brick[i][j + 2] == 1 && g_Brick[i][j + 3] == 1)
					{
						if (g_Brick[i][j + 1] == 0)
						{
							if (j + 1 < WidthAndHigh)
							{
								//MessageBox(_T("26"));
								g_Brick[i][j + 1] = 3;
								return;
							}
						}
					}
				}
			//以上情况都不符合，玩家没有3个子,AI自己下
			AIPlay(Location_x, Location_y);
			return;
		}
		else
			return;
	}
	else
		return;
}

//AI4子情况
int CWuziqiView::Robot4Qizi()
{
	//AI斜着4个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{

			if (g_Brick[i][j] == 3 && g_Brick[i + 1][j + 1] == 3 && g_Brick[i + 2][j + 2] == 3 && g_Brick[i + 3][j + 3] == 3)
			{
				if (g_Brick[i + 4][j + 4] == 0)
				{
					if (i + 4 < WidthAndHigh && j + 4 < WidthAndHigh)
					{
						g_Brick[i + 4][j + 4] = 3;
						return 1;
					}
				}
				else if (g_Brick[i - 1][j - 1] == 0)
				{
					if (i - 1 >= 0 && j - 1 >= 0)
					{
						g_Brick[i - 1][j - 1] = 3;
						return 1;
					}
				}
				else if (g_Brick[i + 4][j + 4] != 0 && g_Brick[i - 1][j - 1] != 0)
				{
					break;
				}
			}
		}

	//AI斜着4个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i - 1][j + 1] == 3 && g_Brick[i - 2][j + 2] == 3 && g_Brick[i - 3][j + 3] == 3)
			{
				if (g_Brick[i - 4][j + 4] == 0)
				{
					if (i - 4 >= 0 && j + 4 < WidthAndHigh)
					{
						g_Brick[i - 4][j + 4] = 3;
						return 1;
					}
				}
				else if (g_Brick[i + 1][j - 1] == 0)
				{
					if (i + 1 < WidthAndHigh && j - 1 >= 0)
					{
						g_Brick[i + 1][j - 1] = 3;
						return 1;
					}
				}
				else if (g_Brick[i - 4][j + 4] != 0 && g_Brick[i + 1][j - 1] != 0)
				{
					break;
				}
			}
		}

	//横着有4个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i][j + 1] == 3 && g_Brick[i][j + 2] == 3 && g_Brick[i][j + 3] == 3)
			{
				if (g_Brick[i][j + 4] == 0)
				{
					if (j + 4 < WidthAndHigh)
					{
						g_Brick[i][j + 4] = 3;
						return 1;
					}
				}
				else if (g_Brick[i][j - 1] == 0)
				{
					if (j - 1 >= 0)
					{
						g_Brick[i][j - 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i][j + 4] != 0 && g_Brick[i][j - 1] != 0)
				{
					break;
				}
			}
		}

	//竖着有4个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i + 1][j] == 3 && g_Brick[i + 2][j] == 3 && g_Brick[i + 3][j] == 3)
			{
				if (g_Brick[i + 4][j] == 0)
				{
					if (i + 4 < WidthAndHigh)
					{
						g_Brick[i + 4][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i - 1][j] == 0)
				{
					if (i - 1 >= 0)
					{
						g_Brick[i - 1][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 4][j] != 0 && g_Brick[i - 1][j] != 0)
				{
					break;
				}
			}
		}
	return 0;
}


//AI三子情况
int CWuziqiView::Robot3QiZi()
{
	//斜着WidthAndHigh01
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i + 1][j + 1] == 0 && g_Brick[i + 2][j + 2] == 3 && g_Brick[i + 3][j + 3] == 3)
			{
				if (g_Brick[i - 1][j - 1] != 1)
				{
					if (i + 1 < WidthAndHigh && j + 1 < WidthAndHigh)
					{
						g_Brick[i + 1][j + 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 4][j + 4] != 1)
				{
					if (i + 1 < WidthAndHigh && j + 1 < WidthAndHigh)
					{
						g_Brick[i + 1][j + 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 4][j + 4] == 1 && g_Brick[i - 1][j - 1] == 1)
					break;
			}
		}

	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i + 1][j + 1] == 3 && g_Brick[i + 2][j + 2] == 0 && g_Brick[i + 3][j + 3] == 3)
			{
				if (g_Brick[i - 1][j - 1] != 1)
				{
					if (i + 2 < WidthAndHigh && j + 2 < WidthAndHigh)
					{
						g_Brick[i + 2][j + 2] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 4][j + 4] != 1)
				{
					if (i + 2 < WidthAndHigh && j + 2 < WidthAndHigh)
					{
						g_Brick[i + 2][j + 2] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 4][j + 4] == 1 && g_Brick[i - 1][j - 1] == 1)
					break;
			}
		}
	//斜着WidthAndHigh01
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i - 1][j + 1] == 0 && g_Brick[i - 2][j + 2] == 3 && g_Brick[i - 3][j + 3] == 3)
			{
				if (g_Brick[i + 1][j - 1] != 1)
				{
					if (i - 1 >= 0 && j + 1 < WidthAndHigh)
					{
						g_Brick[i - 1][j + 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i - 4][j + 4] != 1)
				{
					if (i - 1 >= 0 && j + 1 < WidthAndHigh)
					{
						g_Brick[i - 1][j + 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i - 4][j + 4] == 1 && g_Brick[i + 1][j - 1] == 1)
					break;
			}
		}

	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i - 1][j + 1] == 3 && g_Brick[i - 2][j + 2] == 0 && g_Brick[i - 3][j + 3] == 3)
			{
				if (g_Brick[i + 1][j - 1] != 1)
				{
					if (i - 2 >= 0 && j + 2 < WidthAndHigh)
					{
						g_Brick[i - 2][j + 2] = 3;
						return 1;
					}
				}
				if (g_Brick[i - 4][j + 4] != 1)
				{
					if (i - 2 >= 0 && j + 2 < WidthAndHigh)
					{
						g_Brick[i - 2][j + 2] = 3;
						return 1;
					}
				}
				if (g_Brick[i - 4][j + 4] == 1 && g_Brick[i + 1][j - 1] == 1)
					break;
			}
		}
	//横着1101
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i][j + 1] == 0 && g_Brick[i][j + 2] == 3 && g_Brick[i][j + 3] == 3)
			{
				if (g_Brick[i][j - 1] != 1)
				{
					if (j + 1 < WidthAndHigh)
					{
						g_Brick[i][j + 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i][j + 4] != 1)
				{
					if (j + 1 < WidthAndHigh)
					{
						g_Brick[i][j + 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i][j + 4] == 1 && g_Brick[i][j - 1] == 1)
					break;
			}
		}

	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i][j + 1] == 3 && g_Brick[i][j + 2] == 0 && g_Brick[i][j + 3] == 3)
			{
				if (g_Brick[i][j - 1] != 1)
				{
					if (j + 2 < WidthAndHigh)
					{
						g_Brick[i][j + 2] = 3;
						return 1;
					}
				}
				if (g_Brick[i][j + 4] != 1)
				{
					if (j + 2 < WidthAndHigh)
					{
						g_Brick[i][j + 2] = 3;
						return 1;
					}
				}
				if (g_Brick[i][j + 4] == 1 && g_Brick[i][j - 1] == 1)
					break;
			}
		}
	//竖着WidthAndHigh01
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i + 1][j] == 0 && g_Brick[i + 2][j] == 3 && g_Brick[i + 3][j] == 3)
			{
				if (g_Brick[i - 1][j] != 1)
				{
					if (j + 1 < WidthAndHigh)
					{
						g_Brick[i + 1][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 4][j] != 1)
				{
					if (i + 1 < WidthAndHigh)
					{
						g_Brick[i + 1][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 4][j] == 1 && g_Brick[i - 1][j] == 1)
					break;
			}
		}
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i + 1][j] == 3 && g_Brick[i + 2][j] == 0 && g_Brick[i + 3][j] == 3)
			{
				if (g_Brick[i - 1][j] != 1)
				{
					if (i + 2 < WidthAndHigh)
					{
						g_Brick[i + 2][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 4][j] != 1)
				{
					if (i + 2 < WidthAndHigh)
					{
						g_Brick[i + 2][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 4][j] == 1 && g_Brick[i - 1][j] == 1)
					break;
			}
		}
	//处理三个子的情况
	//AI斜着三个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i + 1][j + 1] == 3 && g_Brick[i + 2][j + 2] == 3)
			{
				if (g_Brick[i + 3][j + 3] == 0)
				{
					if (j + 3 < WidthAndHigh && i + 3 < WidthAndHigh)
					{
						g_Brick[i + 3][j + 3] = 3;
						return 1;
					}
				}
				else if (g_Brick[i - 1][j - 1] == 0)
				{
					if (i - 1 >= 0 && j - 1 >= 0)
					{
						g_Brick[i - 1][j - 1] = 3;
						return 1;
					}
				}
				else if (g_Brick[i + 3][j + 3] == 1 && g_Brick[i - 1][j - 1] == 1)
				{
					break;
				}
			}
		}

	//AI斜着三个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i - 1][j + 1] == 3 && g_Brick[i - 2][j + 2] == 3)
			{
				if (g_Brick[i - 3][j + 3] == 0)
				{
					if (j + 3 < WidthAndHigh && i - 3 >= 0)
					{
						g_Brick[i - 3][j + 3] = 3;
						return 1;
					}
				}
				else if (g_Brick[i + 1][j - 1] == 0)
				{
					if (j + 1 < WidthAndHigh && i - 1 >= 0)
					{
						g_Brick[i + 1][j - 1] = 3;
						return 1;
					}
				}
				else if (g_Brick[i - 3][j + 3] == 1 && g_Brick[i + 1][j - 1] == 1)
				{
					break;
				}
			}
		}


	//横着有3个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i][j + 1] == 3 && g_Brick[i][j + 2] == 3)
			{
				if (g_Brick[i][j + 3] == 0)
				{
					if (j + 3 < WidthAndHigh)
					{
						g_Brick[i][j + 3] = 3;
						return 1;
					}
				}
				else if (g_Brick[i][j - 1] == 0)
				{
					if (j - 1 >= 0)
					{
						g_Brick[i][j - 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i][j + 3] == 1 && g_Brick[i][j - 1] == 1)
				{
					break;
				}
			}
		}
	//竖着有3个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i + 1][j] == 3 && g_Brick[i + 2][j] == 3)
			{
				if (g_Brick[i + 3][j] == 0)
				{
					if (i + 3 < WidthAndHigh)
					{
						g_Brick[i + 3][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i - 1][j] == 0)
				{
					if (i - 1 >= 0)
					{
						g_Brick[i - 1][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 3][j] == 1 && g_Brick[i - 1][j] == 1)
				{
					break;
				}
			}
		}
	return 0;
}


//AI2子情况
bool CWuziqiView::AIPlay(int Location_x, int Location_y)
{
	//AI斜着2个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{

			if (g_Brick[i][j] == 3 && g_Brick[i + 1][j + 1] == 3)
			{
				if (g_Brick[i + 2][j + 2] == 0)
				{
					if (j + 2 < WidthAndHigh && i + 2 < WidthAndHigh)
					{
						g_Brick[i + 2][j + 2] = 3;
						return 1;
					}
				}
				else if (g_Brick[i - 1][j - 1] == 0)
				{
					if (i - 1 >= 0 && j - 1 >= 0)
					{
						g_Brick[i - 1][j - 1] = 3;
						return 1;
					}
				}
				else if (g_Brick[i + 3][j + 3] != 0 && g_Brick[i - 1][j - 1] != 0)
				{
					break;
				}
			}
		}

	//AI斜着2个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i - 1][j + 1] == 3)
			{
				if (g_Brick[i - 2][j + 2] == 0)
				{
					if (j + 2 < WidthAndHigh && i - 2 >= 0)
					{
						g_Brick[i - 2][j + 2] = 3;
						return 1;
					}
				}
				else if (g_Brick[i + 1][j - 1] == 0)
				{
					if (i + 1 < WidthAndHigh && j - 1 >= 0)
					{
						g_Brick[i + 1][j - 1] = 3;
						return 1;
					}
				}
				else if (g_Brick[i - 2][j + 2] != 0 && g_Brick[i + 1][j - 1] != 0)
				{
					break;
				}
			}
		}

	//横着有2个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i][j + 1] == 3)
			{
				if (g_Brick[i][j + 2] == 0)
				{
					if (j + 2 < WidthAndHigh)
					{
						g_Brick[i][j + 2] = 3;
						return 1;
					}
				}
				else if (g_Brick[i][j - 1] == 0)
				{
					if (j - 1 >= 0)
					{
						g_Brick[i][j - 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i][j + 2] != 0 && g_Brick[i][j - 1] != 0)
				{
					break;
				}
			}
		}

	//竖着有2个子
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3 && g_Brick[i + 1][j] == 3)
			{
				if (g_Brick[i + 2][j] == 0)
				{
					if (i + 2 < WidthAndHigh)
					{
						g_Brick[i + 2][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i - 1][j] == 0)
				{
					if (i - 1 >= 0)
					{
						g_Brick[i - 1][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 2][j] != 0 && g_Brick[i - 1][j] != 0)
				{
					break;
				}
			}
		}

	//AI1子情况
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			if (g_Brick[i][j] == 3)
			{
				if (g_Brick[i + 1][j] == 0)
				{
					if (i + 1 < WidthAndHigh)
					{
						g_Brick[i + 1][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i - 1][j] == 0)
				{
					if (i - 1 >= 0)
					{
						g_Brick[i - 1][j] = 3;
						return 1;
					}
				}
				if (g_Brick[i - 1][j - 1] == 0)
				{
					if (i - 1 >= 0 && j - 1 >= 0)
					{
						g_Brick[i - 1][j - 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i + 1][j + 1] == 0)
				{
					if (i + 1 < WidthAndHigh && j + 1 < WidthAndHigh)
					{
						g_Brick[i + 1][j + 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i][j + 1] == 0)
				{
					if (j + 1 < WidthAndHigh)
					{
						g_Brick[i][j + 1] = 3;
						return 1;
					}
				}
				if (g_Brick[i][j - 1] == 0)
				{
					if (j - 1 >= 0)
					{
						g_Brick[i][j - 1] = 3;
						return 1;
					}
				}
			}
		}


	//以上情况都不符合AI随机下

	while (1)
	{
		int robotRandom_x = rand() % (WidthAndHigh - 5);
		int robotRandom_y = rand() % (WidthAndHigh - 5);
		robotRandom_x += 3;
		robotRandom_y += 3;
		//MessageBox(TEXT("ddd"));
		if (g_Brick[robotRandom_x][robotRandom_y] == 0)
		{
			g_Brick[robotRandom_x][robotRandom_y] = 3;
			break;
		}
	}
}


void CWuziqiView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CView::OnPaint()
	CWuziqiDoc* doc = GetDocument();
	ASSERT_VALID(doc);
	if (!doc)
		return;
	// TODO: 在此处为本机数据添加绘制代码

	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP1);

	CBrush brush;
	brush.CreatePatternBrush(&bmp);
	CBrush*   pOldBrush = dc.SelectObject(&brush);
	CRect rect;
	GetClientRect(&rect);
	dc.Rectangle(0, 0, rect.Width(), rect.Height()); // 这些参数可以调整图片添加位置和大小
	//dc.Rectangle(0, 0, 1000, 1000);   // 这些参数可以调整图片添加位置和大小
	dc.SelectObject(pOldBrush);

	//画棋盘
	CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
	CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
	dc.SelectObject(pen);
	dc.MoveTo(BKLocation_xy, BKLocation_xy);
	for (int i = BKLocation_xy; i <= 900; i += 60) {
		dc.MoveTo(BKLocation_xy, i);
		dc.LineTo(900, i);
		dc.MoveTo(i, BKLocation_xy);
		dc.LineTo(i, 900);
	}
	dc.SelectObject(&pen1);
	ChessManager* chessManager = doc->getChessManager();
	Chess** allChess = chessManager->getAllChess();
	if (chessManager->gameBegin|| chessManager->gameBeginAI||chessManager->gameBeginPK==true|| chessManager->gameBeginPK_paint == true) {
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				Chess currentChess = allChess[i][j];
				if (currentChess.color == 1) {//黑色
					CRect chessRect(currentChess.h * 60 + 35, currentChess.v * 60 + 35, currentChess.h * 60 + 85, currentChess.v * 60 + 85);
					DrawChess(&dc, chessRect, RGB(0, 0, 0));
				}
				else if (currentChess.color == 0) {//白色
					CRect chessRect(currentChess.h * 60 + 35, currentChess.v * 60 + 35, currentChess.h * 60 + 85, currentChess.v * 60 + 85);
					DrawChess(&dc, chessRect, RGB(255, 255, 255));
				}
			}
		}
	}
	if (x != -1) {
		if (chessManager->currentOperation != nullptr) {
			Chess* lastChess = chessManager->getLastChess();
			x = lastChess->h + 1;
			y = lastChess->v + 1;
			int color = lastChess->color;
			if (color == 0)
			{
				//MessageBox(TEXT("sdfs"));
				CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
				dc.SelectObject(pen);
				dc.MoveTo(x * 60 - 7, y * 60);
				dc.LineTo(x * 60 + 5, y * 60);
				dc.MoveTo(x * 60, y * 60 - 5);
				dc.LineTo(x * 60, y * 60 + 5);
			}
			else //if (color == 0)
			{
				//MessageBox(TEXT("sdfsbaise"));
				CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
				dc.SelectObject(pen);
				dc.MoveTo(x * 60 - 7, y * 60);
				dc.LineTo(x * 60 + 5, y * 60);
				dc.MoveTo(x * 60, y * 60 - 5);
				dc.LineTo(x * 60, y * 60 + 5);

			}
		}
	}
	
	if (PKgame_times != -1) {
		//MessageBox(TEXT("jinrula"));
		if (PKgame_times % 2 == 1)
		{
			//chessManager->gameBeginPK = true;
			CBrush brush(RGB(0, 0, 0));
			CRect rect(1015, 80, 1115, 180);
			CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
			dc.SelectObject(pen);
			//MessageBox(TEXT("jingrus黑色sdf................"));

			dc.SelectObject(brush);
			dc.Ellipse(rect);
			//Invalidate(0);
		}
		else
		{
			//chessManager->gameBeginPK = false;

			CBrush brush(RGB(255, 255, 255));
			CRect rect(1015, 80, 1115, 180);
			CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
			dc.SelectObject(pen);
			//MessageBox(TEXT("jingru白色df................"));

			dc.SelectObject(brush);
			dc.Ellipse(rect);
			//Invalidate(0);

		}
	}

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->updateStatusBar(1, doc->getCurrentColor());

	

}


void CWuziqiView::ID_REGRET(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CWuziqiDoc* doc = GetDocument();
	ChessManager* chessManager = doc->getChessManager();
	if (chessManager->gameBeginAI == true) {
		GetDocument()->undoOperation();
		GetDocument()->undoOperation();
		Invalidate(0);
	}
	else if (chessManager->gameBegin == true) {
		GetDocument()->undoOperation();
		Invalidate(0);
	}
	else if(chessManager->gameBeginPK_paint == true)
	{
		send_regret();	
		MessageBox(TEXT("正在请求对方同意。。。"));
	}
	
}


void CWuziqiView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus = &pFrame->m_wndStatusBar;
	if (pStatus)
	{
		str.Format(_T("\tx = %d, y = %d"), point.x, point.y);
		pStatus->SetPaneText(2,str);
	}
	//str.Format(_T("\tx = %d, y = %d"), point.x, point.y);

	//m_wndStatusBar.SetPaneText(6, str);

	CView::OnMouseMove(nFlags, point);
}

//重新开始
void CWuziqiView::OnPushButtonClicked()
{
	
	//ChessManager::getLastChess();//gameBegin = true;//gameBegin = true;
	//MessageBox(TEXT("游戏即将开始！"));
	CWuziqiDoc* doc = GetDocument();
	ChessManager* chessManager = doc->getChessManager();
	CClientDC dc(this);
	ASSERT_VALID(doc);
	if (!doc)
		return;
	PKgame_times += 1;
	//doc->OnNewDocument();
	for(int i=0;i<225;i++)
		doc->undoOperation();

	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			g_Brick[i][j] = 0;
			g_Brick_verify[i][j] = 0;
		}

	if (chessManager->gameBeginPK_paint ==true)
	{
		send_new_gmae();
		if (PKgame_times % 2 == 1)
		{
			//Sleep(2000);
			CRect rect(1000, 550, 1100, 650);
			chessManager->gameBeginPK = true;
			if (chessManager->nextColorInt == 1)
			{
				CBrush brush(RGB(0, 0, 0));
				dc.SelectObject(brush);
				dc.Ellipse(rect);
				
			}
			else
			{
				CBrush brush(RGB(255, 255, 255));
				dc.SelectObject(brush);
				dc.Ellipse(rect);
			}
			
			//MessageBox(TEXT("jingru黑色df................"));

			//chessManager->nextColorInt == 1;

			//MessageBox(TEXT("jingru黑色df................"));
			//Invalidate(0);
		}
		else
		{
			chessManager->gameBeginPK = false;

			

			CRect rect(1000, 550, 1100, 650);
			if (chessManager->nextColorInt == 1)
			{
				CBrush brush(RGB(0, 0, 0));
				dc.SelectObject(brush);
				dc.Ellipse(rect);
			}
			else
			{
				CBrush brush(RGB(255, 255, 255));
				dc.SelectObject(brush);
				dc.Ellipse(rect);
			}
			//MessageBox(TEXT("jingru白色df................"));
			//Sleep(2000);
			//Invalidate(0);
			//chessManager->nextColorInt == 0;

		}
	}
	Invalidate(0);

}

//人机模式
void CWuziqiView::OnPushButtonClicked1()
{
	CWuziqiDoc* doc = GetDocument();
	ASSERT_VALID(doc);
	if (!doc)
		return;
	x = -1;
	y = -1;
	doc->OnNewDocument();
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			g_Brick[i][j] = 0;
			g_Brick_verify[i][j] = 0;
		}
	
	ChessManager* chessManager = doc->getChessManager();
	chessManager->gameBeginAI = true;
	chessManager->gameBegin = false;
	chessManager->gameBeginPK = false;
	chessManager->gameBeginPK_enemy = false;
	chessManager->gameBeginPK_paint = false;

	
	//MessageBox(TEXT("不好意思，太难了，小弟还没开通！"));
}

//双人模式
void CWuziqiView::OnPushButtonClicked2()
{
	CWuziqiDoc* doc = GetDocument();
	ASSERT_VALID(doc);
	if (!doc)
		return;
	doc->OnNewDocument();
	x = -1;
	y = -1;
	ChessManager* chessManager = doc->getChessManager();
	chessManager->gameBegin = true;
	chessManager->gameBeginPK_enemy = false;
	chessManager->gameBeginPK_paint = false;

	chessManager->gameBeginAI = false;
	chessManager->gameBeginPK = false;

}

//PK模式
void CWuziqiView::OnPushButtonClicked3()
{
	CWuziqiDoc* doc = GetDocument();
	ASSERT_VALID(doc);
	if (!doc)
		return;
	doc->OnNewDocument();
	x = -1;
	y = -1;
	ChessManager* chessManager = doc->getChessManager();
	chessManager->gameBegin = false;
	chessManager->gameBeginAI = false;
	chessManager->gameBeginPK_paint = true;
	chessManager->gameBeginPK_enemy = true;
	MessageBox(TEXT("正在帮你匹配选手。。。"));
	CString str;


	int firstColor;//随机颜色

	firstColor = match();
	
	str.Format(L"%d", firstColor);//提示match返回值

	//MessageBox(str);
	CClientDC dc(this);
	
	if (!firstColor) {
		//str.Format(_T("piasdflksd%s"), buff1);
		MessageBox(TEXT("匹配失败，请选择其他模式继续游戏。"));
	}
	else {
		if (firstColor == 1) {
			chessManager->nextColorInt = 1;
			MessageBox(TEXT("你是黑色，你先下。"));
			PKgame_times = 1;

			chessManager->gameBeginPK = true;
			CBrush brush(RGB(0,0,0));
			CRect rect(1015,80,1115,180);

			dc.SelectObject(brush);
			dc.Ellipse(rect);
		}
		else {
			chessManager->nextColorInt = 0;
			MessageBox(TEXT("你是白色，对方先下。"));
			PKgame_times = 0;

			CBrush brush(RGB(255, 255, 255));
			
			CRect rect(1015, 80, 1115, 180);

			dc.SelectObject(brush);
			dc.Ellipse(rect);

		}
	}
	obj = this;


	//void(*pf)(int, int) = &CWuziqiView::Draw_pos;
	CloseHandle(CreateThread(NULL, 0, recvMsgThread, (LPVOID)&draw_pos , 0, 0));


	//MessageBox(TEXT("敬请期待。。。\n\n(最好不要期待)"));

}

//背景声音
void CWuziqiView::OnPushButtonClicked4() {
	if (pButton4.GetCheck() == BST_CHECKED) {
		PlaySound((LPCTSTR)IDR_WAVE2, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC | SND_LOOP);
		//CWuziqiView::init();

	}
	else 
		PlaySound(NULL, AfxGetInstanceHandle(), SND_PURGE);

}

void CWuziqiView::OnPushButtonClicked5() {
	//PlaySound((LPCTSTR)IDR_WAVE2, AfxGetInstanceHandle(),  SND_PURGE);

}

int CWuziqiView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rect1;

	rect1.SetRect(1025, 200, 1125, 230);
	CButton* pButton = new CButton;
	pButton->Create(_T("重新开始"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect1, this, IDC_OK);//ID_FILE_NEW,IDC_OK

	CButton* pButton1 = new CButton;
	rect1.SetRect(1025, 300, 1125, 330);
	pButton1->Create(_T("人机对战"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect1, this, IDC_OK1);//m_wndPushButton1.

	CButton* pButton2 = new CButton;
	rect1.SetRect(1025, 250, 1125, 280);
	pButton2->Create(_T("双人模式"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect1, this, IDC_OK2);//m_wndPushButton1.

	CButton* pButton3 = new CButton;
	rect1.SetRect(1025, 350, 1125, 380);
	pButton3->Create(_T("在线PK"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect1, this, IDC_OK3);

	//CButton* pButton4 = new CButton;
	rect1.SetRect(1025, 400, 1125, 430);
	pButton4.Create(TEXT("B G M"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, rect1, this, IDC_OK4);

	CButton* pButton5 = new CButton;
	rect1.SetRect(1210, 810, 1250, 830);
	//pButton5->Create(TEXT("发送"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect1, this, IDC_OK5);

	if (create_connect()) {
		MessageBox(TEXT("服务端连接成功！"));
	}
	else {
		MessageBox(TEXT("服务端链接失败,请检查是否在局域网内！或者服务端是否开启！"));
	}

	

	/*CClientDC dc(this);//创建客户区类型

	TEXTMETRIC tm;

	dc.GetTextMetrics(&tm);//获取当前字符高度和宽度值，来矫正插入光标的大小

	CreateSolidCaret(tm.tmAveCharWidth / 8, tm.tmHeight);//创建插入符

	ShowCaret();//显示插入符*/



	SetTimer(1, 100, NULL);//设置定时器消息，1表示定时器标识符，100表示100ms产生一个消息，NULL指的是进入消息队列产生WM_TIMER消息

	CRect rect(905, 810, 1200, 830);

	//edit.Create(ES_LEFT | ES_AUTOHSCROLL | WS_BORDER | WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect, this, IDC_MYEDIT);//ES_AUTOHSCROLL - 自动水平滚动WS_BORDER   有边框窗口
																												   //WS_TABSTOP 指定了一些控件中的一个，用户可以通过TAB键来移过它。TAB键使用户移动到下一个用WS_TABSTOP风格定义的控件。
	rect.SetRect(905, 450, 1250, 900);
	//edit_show.Create(ES_LEFT | ES_AUTOHSCROLL | WS_BORDER | WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect, this, IDC_MYEDIT1);
	//edit_show.set
	//edit_show.SetReadOnly(TRUE);


	list_box.Create(WS_CHILD | WS_VISIBLE   ,rect, this, Listbox);//TEXT("聊天天啦!"), | LBS_STANDARD
	/*int b =list_box.CreateEx(WS_EX_CLIENTEDGE, _T("聊天天啦!"),NULL, WS_VISIBLE | WS_CHILD | LBS_STANDARD, rect,this, Listbox);//WS_EX_TRANSPARENT
	if (b == 0)
	{
		MessageBox(TEXT("sdfsdshi失败............."));
	}
	//list_box.ResetContent();
	//list_box.AddString(TEXT("sdfsdfsdljl了健身房里看电视"));
	//list_box.InsertString(0,str);
	*/
	
	CString str = _T("sdfsdfsdljl了健身房里看电视");
	//list_box.AddString(str);

	//edit.GetWindowText(str);
	//ASSERT(&edit_show && edit_show.GetSafeHwnd());
	//edit_show.SetSel(-1);
	//edit_show.ReplaceSel(str);
	//edit_show.SetWindowTextW(Listbox,str);
	//edit_show.SetDlgItemTextW(IDC_MYEDIT1, str);
	//UpdateData(true);
	/*CEdit CEdit_temp;
	int length = CEdit_temp.GetWindowTextLength();
	CEdit_temp.SetSel(length, length);
	CEdit_temp.ReplaceSel(_T("末尾追加的信息"));*/
	

	return 0;
}

void draw_pos(int px, int py ,int regret, char *ch)//[100]
{
	if (NULL != obj)
	{
		obj->Draw_pos(px, py, regret, ch);	//这里有问题是的OBJ你确定没问题吗调用得了的吧，
	}
}

void CWuziqiView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	


	/*CClientDC dc(this);

	TEXTMETRIC tm;

	CString m_strLine;

	CPoint m_ptOrigin = (0,0);

	dc.GetTextMetrics(&tm);//获取当前输入字符的高度和宽度

	if (0x0d == nChar)//0x0d表示回车符，点击回车后的一些操作

	{

		m_strLine.Empty();

		m_ptOrigin.y += tm.tmHeight;//将光标下移一个字符

	}

	else if (0x08 == nChar)//0x08退格符，下面使用实现的方法是，先将文本背景色取出，然后将背景色设置为字体颜色，重写字体，即覆盖了原来字体。然后将字体颜色恢复，同时将字符串中减少一个字符，再重写字体，实现删除一个字体功能

	{

		COLORREF clr = dc.SetTextColor(dc.GetBkColor());//将字体色改为背景颜色

		dc.TextOutW(m_ptOrigin.x, m_ptOrigin.y, m_strLine);//重写字体，即覆盖原来的字体

		m_strLine = m_strLine.Left(m_strLine.GetLength() - 1);//获取字符串左边减一字符

		dc.SetTextColor(clr);//重新设置字体颜色

	}

	else

	{

		m_strLine = m_strLine + (TCHAR)nChar;

	}

	CSize cs;

	cs = dc.GetTextExtent(m_strLine);//获取这一行文本的高度和宽度值

	CPoint pt;

	pt.x = m_ptOrigin.x + cs.cx;

	pt.y = m_ptOrigin.y;

	SetCaretPos(pt);//重新设置光标位置，跟随字符输入移动

	dc.TextOutW(m_ptOrigin.x, m_ptOrigin.y, m_strLine);//根据原鼠标点击位置输出字符

	*/
	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CWuziqiView::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CWuziqiDoc* doc = GetDocument();
	ChessManager* chessManager = doc->getChessManager();
	CClientDC dc(this);
	ASSERT_VALID(doc);
	if (!doc)
		return;
	PKgame_times += 1;
	doc->OnNewDocument();
	/*for (int i = 0;i < 225;i++)
		doc->undoOperation();*/

	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			g_Brick[i][j] = 0;
			g_Brick_verify[i][j] = 0;
		}

	if (chessManager->gameBeginPK_paint == true)
	{
		send_new_gmae();
		if (PKgame_times % 2 == 1)
		{
			//Sleep(2000);
			chessManager->gameBeginPK = true;
			CBrush brush(RGB(0, 0, 0));

			CRect rect(1015, 80, 1115, 180);

			//MessageBox(TEXT("jingru黑色df................"));

			dc.SelectObject(brush);
			dc.Ellipse(rect);
			//MessageBox(TEXT("jingru黑色df................"));
			//Invalidate(0);

		}
		else
		{
			chessManager->gameBeginPK = false;

			CBrush brush(RGB(255, 255, 255));

			CRect rect(1015, 80, 1115, 180);

			//MessageBox(TEXT("jingru白色df................"));
			//Sleep(2000);

			dc.SelectObject(brush);
			dc.Ellipse(rect);
			//Invalidate(0);

		}
	}
	//MessageBox(TEXT("哈哈,我在视图的重新开始"));

	//Invalidate(0);
}


void CWuziqiView::person_and_computer()
{
	// TODO: 在此添加控件通知处理程序代码
	CWuziqiDoc* doc = GetDocument();
	ASSERT_VALID(doc);
	if (!doc)
		return;
	x = -1;
	y = -1;
	doc->OnNewDocument();
	for (int i = 0; i < WidthAndHigh; i++)
		for (int j = 0; j < WidthAndHigh; j++)
		{
			g_Brick[i][j] = 0;
			g_Brick_verify[i][j] = 0;
		}

	ChessManager* chessManager = doc->getChessManager();
	chessManager->gameBeginAI = true;
	chessManager->gameBegin = false;
	chessManager->gameBeginPK = false;
	chessManager->gameBeginPK_enemy = false;
	chessManager->gameBeginPK_paint = false;


}


void CWuziqiView::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CWuziqiDoc* doc = GetDocument();
	ASSERT_VALID(doc);
	if (!doc)
		return;
	doc->OnNewDocument();
	x = -1;
	y = -1;
	ChessManager* chessManager = doc->getChessManager();
	chessManager->gameBegin = true;
	chessManager->gameBeginPK_enemy = false;
	chessManager->gameBeginPK_paint = false;

	chessManager->gameBeginAI = false;
	chessManager->gameBeginPK = false;
}


void CWuziqiView::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CWuziqiDoc* doc = GetDocument();
	ASSERT_VALID(doc);
	if (!doc)
		return;
	doc->OnNewDocument();
	x = -1;
	y = -1;
	ChessManager* chessManager = doc->getChessManager();
	chessManager->gameBegin = false;
	chessManager->gameBeginAI = false;
	chessManager->gameBeginPK_paint = true;
	chessManager->gameBeginPK_enemy = true;
	MessageBox(TEXT("正在帮你匹配选手。。。"));
	CString str;


	int firstColor;//随机颜色

	firstColor = match();

	str.Format(L"%d", firstColor);//提示match返回值

	//MessageBox(str);
	CClientDC dc(this);

	if (!firstColor) {
		//str.Format(_T("piasdflksd%s"), buff1);
		MessageBox(TEXT("匹配失败，请选择其他模式继续游戏。"));
	}
	else {
		if (firstColor == 1) {
			chessManager->nextColorInt = 1;
			MessageBox(TEXT("你是黑色，你先下。"));
			PKgame_times = 1;

			chessManager->gameBeginPK = true;
			CBrush brush(RGB(0, 0, 0));
			CRect rect(1000, 550, 1100, 650);

			dc.SelectObject(brush);
			dc.Ellipse(rect);
		}
		else {
			chessManager->nextColorInt = 0;
			MessageBox(TEXT("你是白色，对方先下。"));
			PKgame_times = 0;

			CBrush brush(RGB(255, 255, 255));
			CRect rect(1000, 550, 1100, 650);

			dc.SelectObject(brush);
			dc.Ellipse(rect);

		}
	}
	obj = this;


	//void(*pf)(int, int) = &CWuziqiView::Draw_pos;
	CloseHandle(CreateThread(NULL, 0, recvMsgThread, (LPVOID)&draw_pos, 0, 0));
}


void CWuziqiView::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (pButton4.GetCheck() == BST_CHECKED) {
		PlaySound((LPCTSTR)IDR_WAVE2, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC | SND_LOOP);
		//CWuziqiView::init();

	}
	else
		PlaySound(NULL, AfxGetInstanceHandle(), SND_PURGE);
}
