
// TetrisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Tetris.h"
#include "TetrisDlg.h"
#include "afxdialogex.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTetrisDlg 对话框

CTetrisDlg::CTetrisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTetrisDlg::IDD, pParent)
	, m_firstCreate(true)
	, m_score(0)
	, m_gamelevel(0)
	, m_transformTimes(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_PreObj = RussiaBlock();
}

void CTetrisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTetrisDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_Start, &CTetrisDlg::OnStart)
	ON_COMMAND(IDM_Pause, &CTetrisDlg::OnPause)
	ON_COMMAND(IDM_Continue, &CTetrisDlg::OnContinue)
	ON_COMMAND(IDM_Exit, &CTetrisDlg::OnExit)
	ON_COMMAND(IDM_About, &CTetrisDlg::OnAbout)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTetrisDlg 消息处理程序

BOOL CTetrisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	//绘制游戏主体框架，以24个像素为一个大格，主游戏区域为240*432，即10*18，预留4个像素以优化主游戏框架
	CWnd *pWnd = GetDlgItem(IDC_GameArea);
	pWnd->SetWindowPos(NULL, 0, 0, 244, 436, SWP_NOZORDER | SWP_NOMOVE);
	//绘制游戏“下一个方块”框架以及得分框架
	pWnd = GetDlgItem(IDC_NextBK);
	pWnd->SetWindowPos(NULL, 0, 0, 160, 144, SWP_NOZORDER | SWP_NOMOVE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTetrisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTetrisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTetrisDlg::OnStart()
{
	// TODO:  在此添加命令处理程序代码
	AfxMessageBox(L"游戏三秒后开始！");
	SetTimer(1, 3000, NULL);
	RedrawWindow();
	//重设主界面
	ResetGameArea();
	m_score = 0;
	m_firstCreate = true;
	m_gamelevel = 0;
}


void CTetrisDlg::OnPause()
{
	// TODO:  在此添加命令处理程序代码
	KillTimer(1);
	AfxMessageBox(L"游戏暂停！");
}


void CTetrisDlg::OnContinue()
{
	// TODO:  在此添加命令处理程序代码
	SetTimer(1, 20, NULL);
}


void CTetrisDlg::OnExit()
{
	// TODO:  在此添加命令处理程序代码
	
	AfxMessageBox(L"退出游戏!");
	exit(0);
}


void CTetrisDlg::OnAbout()
{
	// TODO:  在此添加命令处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}


BOOL CTetrisDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_LEFT:
			if (false == IsBorder(m_currentObj, OBJ_LEFT))//如果方块左边无物体遮挡
				MoveObject(m_currentObj, OBJ_LEFT);
			return true;
			break;
		case VK_RIGHT:
			if (false == IsBorder(m_currentObj, OBJ_RIGHT))//如果方块右边无物体遮挡
				MoveObject(m_currentObj, OBJ_RIGHT);
			return true;
			break;
		case VK_DOWN:
			if (false == IsBorder(m_currentObj, OBJ_DOWN))//如果方块下边界未碰到物体
				MoveObject(m_currentObj, OBJ_DOWN);
			return true;
			break;
		case VK_SPACE://直接到底操作
			while (false == IsBorder(m_currentObj, OBJ_DOWN))
			{
				MoveObject(m_currentObj, OBJ_DOWN);
			}
			return true;
			break;
		case VK_UP://变形操作
			m_transformTimes++;
			Transform(m_currentObj,m_transformTimes);
			return true;
			break;
		}
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTetrisDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	SetGameLevel();
	DrawMainFrame();
	CDialogEx::OnTimer(nIDEvent);
}


void CTetrisDlg::GameOver()
{
	KillTimer(1);
	CString str;
	str.Format(L"游戏结束！，你的分数为%d ",m_score);
	AfxMessageBox(str);
}


void CTetrisDlg::SetGameLevel()
{
	if (m_score / 100 < 5){
		SetTimer(1, 500, NULL);//难度1
		m_gamelevel = 1;
	}
	else if (m_score / 100 <10)
	{
		SetTimer(1, 450, NULL);
		m_gamelevel = 2;
	}
	else if (m_score / 100 <15){
		SetTimer(1, 400, NULL);
		m_gamelevel = 3;
	}
	else if (m_score / 100 <20)
	{
		SetTimer(1, 350, NULL);
		m_gamelevel = 4;
	}
	else
	{
		SetTimer(1, 300, NULL);
		m_gamelevel = 5;
	}
}


void CTetrisDlg::DrawMainFrame()
{
	//绘制游戏主界面
	CWnd *pWnd = GetDlgItem(IDC_GameArea);
	CDC *pDC = pWnd->GetDC();
	CDC *pDCNextObj = GetDlgItem(IDC_NextBK)->GetDC();

	if (m_firstCreate == true){//如果第一次产生方块
		m_firstCreate = false;
		RandomCreateRussiaBlock(m_nextObj);
		
		DrawNextObj(m_nextObj, pDCNextObj);
		RandomCreateRussiaBlock(m_currentObj);
	}
	
	//DrawGameArea(pDC);//画主界面
	DrawFallingObj(m_currentObj, m_PreObj, pDC);//画下落物体
	if (IsBorder(m_currentObj, OBJ_DOWN) == true){//当前物体到底或落在其他物体上
		UpdateTopLine(m_currentObj);//更新游戏顶行行号
		if (GetGameAreaTop() <= GAMETOPWALL){//如果当前方块落稳后方块最顶端超过了游戏区域，则游戏结束
			GameOver();
		}
		else{//否则设置主界面相应位，并产生新物体
			SetGameAreaBit(m_currentObj);
			int LineNum;
			IsLineFull(m_currentObj, LineNum);
			m_score += LineNum * 100;
			RedrawWindow();//清空主界面
			DrawGameArea(pDC);//消行后立即重画主界面

			m_currentObj = m_nextObj;
			RandomCreateRussiaBlock(m_nextObj);
			DrawNextObj(m_nextObj, pDCNextObj);
		}
	}
	
	
	//显示分数与游戏等级
	CWnd *pWndScore = GetDlgItem(IDC_SCORE);
	CString str;
	str.Format(L"%d 分", m_score);
	pWndScore->SetWindowTextW(str);
	CWnd *pWndLevel = GetDlgItem(IDC_LEVEL);
	str.Format(L"%d 级", m_gamelevel);
	pWndLevel->SetWindowTextW(str);

	//随着时间推移，方块缓缓下落
	m_PreObj = m_currentObj;
	MoveObject(m_currentObj, OBJ_DOWN);//下落一行
}
