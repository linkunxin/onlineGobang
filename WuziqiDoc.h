
// WuziqiDoc.h: CWuziqiDoc 类的接口
//


#pragma once

#include "ChessManager.h"
class CWuziqiDoc : public CDocument
{
// 特性
public:
	ChessManager* chessManager;
	CWuziqiDoc() noexcept;
	DECLARE_DYNCREATE(CWuziqiDoc)
// 操作
public:
	ChessManager* getChessManager();//取出棋局管理
	bool addChess(int h, int v);
	int getNextColor();//获得下一个颜色
	void undoOperation();//悔棋
	int getCurrentColor();//获得当前颜色
	bool checkGameOver(int h, int v, int color);//检查胜利
// 重写
public:
	virtual BOOL OnNewDocument();//新建文档
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);//打开文档
	virtual void Serialize(CArchive& ar);//序列化保存重写
	virtual void SetTitle(LPCTSTR lpszTitle);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS
// 实现
public:
	virtual ~CWuziqiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
