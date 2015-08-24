
// TetrisDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Tetris.h"
#include "TetrisDlg.h"
#include "afxdialogex.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTetrisDlg �Ի���

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


// CTetrisDlg ��Ϣ�������

BOOL CTetrisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	//������Ϸ�����ܣ���24������Ϊһ���������Ϸ����Ϊ240*432����10*18��Ԥ��4���������Ż�����Ϸ���
	CWnd *pWnd = GetDlgItem(IDC_GameArea);
	pWnd->SetWindowPos(NULL, 0, 0, 244, 436, SWP_NOZORDER | SWP_NOMOVE);
	//������Ϸ����һ�����顱����Լ��÷ֿ��
	pWnd = GetDlgItem(IDC_NextBK);
	pWnd->SetWindowPos(NULL, 0, 0, 160, 144, SWP_NOZORDER | SWP_NOMOVE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTetrisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTetrisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTetrisDlg::OnStart()
{
	// TODO:  �ڴ���������������
	AfxMessageBox(L"��Ϸ�����ʼ��");
	SetTimer(1, 3000, NULL);
	RedrawWindow();
	//����������
	ResetGameArea();
	m_score = 0;
	m_firstCreate = true;
	m_gamelevel = 0;
}


void CTetrisDlg::OnPause()
{
	// TODO:  �ڴ���������������
	KillTimer(1);
	AfxMessageBox(L"��Ϸ��ͣ��");
}


void CTetrisDlg::OnContinue()
{
	// TODO:  �ڴ���������������
	SetTimer(1, 20, NULL);
}


void CTetrisDlg::OnExit()
{
	// TODO:  �ڴ���������������
	
	AfxMessageBox(L"�˳���Ϸ!");
	exit(0);
}


void CTetrisDlg::OnAbout()
{
	// TODO:  �ڴ���������������
	CAboutDlg dlg;
	dlg.DoModal();
}


BOOL CTetrisDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_LEFT:
			if (false == IsBorder(m_currentObj, OBJ_LEFT))//�����������������ڵ�
				MoveObject(m_currentObj, OBJ_LEFT);
			return true;
			break;
		case VK_RIGHT:
			if (false == IsBorder(m_currentObj, OBJ_RIGHT))//��������ұ��������ڵ�
				MoveObject(m_currentObj, OBJ_RIGHT);
			return true;
			break;
		case VK_DOWN:
			if (false == IsBorder(m_currentObj, OBJ_DOWN))//��������±߽�δ��������
				MoveObject(m_currentObj, OBJ_DOWN);
			return true;
			break;
		case VK_SPACE://ֱ�ӵ��ײ���
			while (false == IsBorder(m_currentObj, OBJ_DOWN))
			{
				MoveObject(m_currentObj, OBJ_DOWN);
			}
			return true;
			break;
		case VK_UP://���β���
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SetGameLevel();
	DrawMainFrame();
	CDialogEx::OnTimer(nIDEvent);
}


void CTetrisDlg::GameOver()
{
	KillTimer(1);
	CString str;
	str.Format(L"��Ϸ����������ķ���Ϊ%d ",m_score);
	AfxMessageBox(str);
}


void CTetrisDlg::SetGameLevel()
{
	if (m_score / 100 < 5){
		SetTimer(1, 500, NULL);//�Ѷ�1
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
	//������Ϸ������
	CWnd *pWnd = GetDlgItem(IDC_GameArea);
	CDC *pDC = pWnd->GetDC();
	CDC *pDCNextObj = GetDlgItem(IDC_NextBK)->GetDC();

	if (m_firstCreate == true){//�����һ�β�������
		m_firstCreate = false;
		RandomCreateRussiaBlock(m_nextObj);
		
		DrawNextObj(m_nextObj, pDCNextObj);
		RandomCreateRussiaBlock(m_currentObj);
	}
	
	//DrawGameArea(pDC);//��������
	DrawFallingObj(m_currentObj, m_PreObj, pDC);//����������
	if (IsBorder(m_currentObj, OBJ_DOWN) == true){//��ǰ���嵽�׻���������������
		UpdateTopLine(m_currentObj);//������Ϸ�����к�
		if (GetGameAreaTop() <= GAMETOPWALL){//�����ǰ�������Ⱥ󷽿���˳�������Ϸ��������Ϸ����
			GameOver();
		}
		else{//����������������Ӧλ��������������
			SetGameAreaBit(m_currentObj);
			int LineNum;
			IsLineFull(m_currentObj, LineNum);
			m_score += LineNum * 100;
			RedrawWindow();//���������
			DrawGameArea(pDC);//���к������ػ�������

			m_currentObj = m_nextObj;
			RandomCreateRussiaBlock(m_nextObj);
			DrawNextObj(m_nextObj, pDCNextObj);
		}
	}
	
	
	//��ʾ��������Ϸ�ȼ�
	CWnd *pWndScore = GetDlgItem(IDC_SCORE);
	CString str;
	str.Format(L"%d ��", m_score);
	pWndScore->SetWindowTextW(str);
	CWnd *pWndLevel = GetDlgItem(IDC_LEVEL);
	str.Format(L"%d ��", m_gamelevel);
	pWndLevel->SetWindowTextW(str);

	//����ʱ�����ƣ����黺������
	m_PreObj = m_currentObj;
	MoveObject(m_currentObj, OBJ_DOWN);//����һ��
}
