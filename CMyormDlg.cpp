#include "stdafx.h"
#include "CMyormDlg.h"
#include "WuziqiDoc.h"
#include "WuziqiView.h"
#include "Chess.h"
#include "MainFrm.h"
#include "ChessManager.h"
#include "chess_cli.h"




IMPLEMENT_DYNCREATE(CMyFormDlg, CFormView)


CMyFormDlg::CMyFormDlg() : CFormView(CMyFormDlg::IDD)//,s(_T(""))
{


}

CMyFormDlg::~CMyFormDlg()
{
}
BEGIN_MESSAGE_MAP(CMyFormDlg, CFormView)
	//ON_COMMAND(IDD_FORMVIEW, &CMyFormDlg::OnIddDialog1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMyFormDlg::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON1, &CWuziqiView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CWuziqiView::person_and_computer)
	ON_BN_CLICKED(IDC_BUTTON2, &CWuziqiView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CWuziqiView::OnBnClickedButton4)

	ON_BN_CLICKED(IDC_BUTTON5, &CMyFormDlg::OnBnClickedButton5)
	ON_WM_CREATE()
END_MESSAGE_MAP()



/*void CMyFormDlg::OnIddDialog1()
{
	// TODO: 在此添加命令处理程序代码
}*/


void CMyFormDlg::OnLbnSelchangeList2()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(TEXT("BGM"));
}


void CMyFormDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//OnPushButtonClicked();
	MessageBox(TEXT("sdfsd"));
	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	//p->GetActiveView.SendMessageW(IDC_OK);
//	p->SendMessageW(ON_BN_CLICKED);//SendMessageW(IDC_OK);
	
	//AfxGetMainWnd()->SendMessage(ON_BN_CLICKED,IDC_OK);
	CWuziqiDoc* doc = ((CWuziqiView *)((CFrameWnd *)AfxGetMainWnd())->GetActiveView())->GetDocument();//GetDocument();//CWuziqiDoc*)p->GetActiveDocument();//
	ASSERT_VALID(doc);
	if (!doc)
		return;
	doc->OnNewDocument();
	//x = -1;
	//y = -1;
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
			//PKgame_times = 1;

			chessManager->gameBeginPK = true;
			CBrush brush(RGB(0, 0, 0));
			CRect rect(1000, 550, 1100, 650);

			dc.SelectObject(brush);
			dc.Ellipse(rect);
		}
		else {
			chessManager->nextColorInt = 0;
			MessageBox(TEXT("你是白色，对方先下。"));
			//PKgame_times = 0;

			CBrush brush(RGB(255, 255, 255));
			CRect rect(1000, 550, 1100, 650);

			dc.SelectObject(brush);
			dc.Ellipse(rect);

		}
	}
	//obj = this;


	//void(*pf)(int, int) = &CWuziqiView::Draw_pos;
	CloseHandle(CreateThread(NULL, 0, recvMsgThread, (LPVOID)&draw_pos, 0, 0));
}


void CMyFormDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR ch[100];
	char chh[1024] = { 0 };
	
	CString strrr;
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(strrr);
	//strcpy_s(nstring, orig);
	//memcpy(chh, strrr, strrr.GetLength());

	//MessageBox(strrr);
	//MessageBox(chh);
	//TCHAR* unicode_string = _T("unicode string");
	//int size = wcslen(strrr);
	//char ansi_string[30];
	//wcstombs(chh, ch, size);

	/*MessageBox(TEXT("sldffsdfsdfjs"));

	SetDlgItemText(IDC_LIST2, strrr);

	//GetDlgItem(IDC_LIST2)->SetWindowText(strrr);
	MessageBox(TEXT("sldfjs"));*/
	//CString strPath = L"adfafs主声音文件fsfsa";
	int nLength = strrr.GetLength();
	int nBytes = WideCharToMultiByte(CP_ACP, 0, strrr, nLength, NULL, 0, NULL, NULL);
	char* VoicePath = new char[nBytes + 1];
	memset(VoicePath, 0, nLength + 1);
	WideCharToMultiByte(CP_OEMCP, 0, strrr, nLength, VoicePath, nBytes, NULL, NULL);
	VoicePath[nBytes] = 0;
	
	my_msg_list.AddString(strrr);
	

	
	//const char* msg = TCHAR_TO_ANSI(TEXT("dddd"));
	//int iLength;
	//获取字节长度   
	//iLength = WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)strrr, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	//WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)strrr, -1, chh, iLength, NULL, NULL);


	send_msg(VoicePath);
	
	
	
}


int CMyFormDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rect(17, 280, 330, 850);
	//edit_show.Create(ES_LEFT | ES_AUTOHSCROLL | WS_BORDER | WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect, this, IDC_MYEDIT1);
	//edit_show.set
	//edit_show.SetReadOnly(TRUE);


	my_msg_list.Create(WS_CHILD | WS_VISIBLE, rect, this, Listbox);//TEXT("聊天天啦!"), // | LBS_STANDARD

	return 0;
}
