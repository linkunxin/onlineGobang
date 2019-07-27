
// WuziqiDoc.cpp: CWuziqiDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Wuziqi.h"
#endif

#include "WuziqiDoc.h"
#include "WuziqiView.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWuziqiDoc

IMPLEMENT_DYNCREATE(CWuziqiDoc, CDocument)

BEGIN_MESSAGE_MAP(CWuziqiDoc, CDocument)
END_MESSAGE_MAP()


// CWuziqiDoc 构造/析构

CWuziqiDoc::CWuziqiDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	chessManager = new ChessManager();

}

CWuziqiDoc::~CWuziqiDoc()
{
	delete chessManager;
}


ChessManager * CWuziqiDoc::getChessManager()
{
	return chessManager;
}

bool CWuziqiDoc::addChess(int h, int v)
{
	return chessManager->addChess(h, v);
}

int CWuziqiDoc::getNextColor()
{
	return chessManager->getNextColor();
}

void CWuziqiDoc::undoOperation()
{
	chessManager->undoOperation();
}


int CWuziqiDoc::getCurrentColor()
{
	return chessManager->getCurrentColor();
}


bool CWuziqiDoc::checkGameOver(int h, int v, int color)
{
	if (chessManager->checkGameOver(h, v, color)) {
		chessManager->gameBegin = false;
		chessManager->gameBeginAI = false;
		chessManager->gameBeginPK = false;
		return true;
	}
	return false;
}


BOOL CWuziqiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	chessManager->startFromZero();
	POSITION position = GetFirstViewPosition();
	CWuziqiView* p = (CWuziqiView*)GetNextView(position);
	p->Invalidate();
	p->UpdateWindow();

	return TRUE;
}


BOOL CWuziqiDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName)) {
		return FALSE;
	}
	POSITION position = GetFirstViewPosition();
	CWuziqiView* p = (CWuziqiView*)GetNextView(position);
	chessManager->gameBegin = true;
	p->MessageBox(_T("调用了doc的openDocument，即将帮你恢复游戏！"), _T("我知道啦！"), MB_OK);
	p->SendMessage(WM_PAINT, NULL, 0);
	return TRUE;
}

// CWuziqiDoc 序列化

void CWuziqiDoc::Serialize(CArchive& ar)
{
	Chess** allChess = chessManager->getAllChess();
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
		ar << chessManager->nextColorInt;
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				ar << allChess[i][j].h << allChess[i][j].v << allChess[i][j].color;
			}
		}
	}
	else
	{
		// TODO: 在此添加加载代码
		ar >> chessManager->nextColorInt;
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				ar >> allChess[i][j].h >> allChess[i][j].v >> allChess[i][j].color;
			}
		}
	}
}


void CWuziqiDoc::SetTitle(LPCTSTR lpszTitle)
{
	CDocument::SetTitle(L"四木五子棋");
}


#ifdef SHARED_HANDLERS

// 缩略图的支持
void CWuziqiDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CWuziqiDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CWuziqiDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWuziqiDoc 诊断

#ifdef _DEBUG
void CWuziqiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWuziqiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWuziqiDoc 命令
