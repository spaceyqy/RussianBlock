
// TetrisDlg.h : ͷ�ļ�
//

#pragma once
#include "RussiaBlock.h"


// CTetrisDlg �Ի���
class CTetrisDlg : public CDialogEx
{
// ����
public:
	CTetrisDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TETRIS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStart();
	afx_msg void OnPause();
	afx_msg void OnContinue();
	afx_msg void OnExit();
	afx_msg void OnAbout();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	RussiaBlock m_currentObj;
	RussiaBlock m_nextObj;
private:
	bool m_firstCreate;
public:
	RussiaBlock m_PreObj;
private:
	int m_score;
public:
	void GameOver();
private:
	int m_gamelevel;
	void SetGameLevel();
	void DrawMainFrame();
	int m_transformTimes;
};
