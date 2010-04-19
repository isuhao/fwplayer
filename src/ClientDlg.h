// ClientDlg.h : ͷ�ļ�
//

#pragma once

#include "flashplayer.h"
#include "TextConfig.h"
#include "VideoPlayer.h"
#include "PlayerFactory.h"

// CClientDlg �Ի���
class CClientDlg : public CDialog
{				 
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CClientDlg();

	// �Ի�������
	enum { IDD = IDD_FWCLIENT_DIALOG };

private:

	//�����FLASH���ݱ���
	CString GetFSVariable(LPCTSTR name);
	void	SetFSVariable(LPCTSTR name,LPCTSTR value);

	//�����С����
	void	AutoFitFlashOriginSize();
	void	AdjustVideoWindow();

	//ȫ������
	void	SetFullScreen(BOOL bFullScreen);

	//����Ƥ��
	void	LoadCurSkin(CString strCurSkin=_T(""));
	void	LoadCurMiniSkin();

	//�����˵�
	void	PopupContextMenu();
	void	PopupFileMenu();
	void	PopupHelpMenu();
	void	PopupSwitchSkinMenu();

	//��ʼֹͣ���ż���������λ��
	void OnPlayingBegin();
	void OnPlayingEnd();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
private:
	HICON m_hIcon; //ͼ��

	TextConfig* _config; //��������

	CFlashPlayer m_FlashPlayer; //����FLASH,������������
	CVideoPlayer* m_pVideoPlayer; //�����ڲ�������

	BOOL	m_bLoadFlashSkin; //����Flash���سɹ����
	BOOL	m_bFullScreen; //ȫ�����
	CRect	m_rcRestore; //ȫ��ǰ��С

	DWORD	m_dwScreenSave; //�������
	BOOL m_bCanChangeProgressByExternProgram; //����Flash�������Ʋ���λ��

	//�ļ�·��
	CString m_AppPath;
	CString m_SkinPath;

	//Ƥ���л��˵�
	HMENU	m_hSwitchSkinMenu;
	UINT	m_nSwitchSkinID;

protected:
	// ���ɵ���Ϣӳ�亯��
	virtual	BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg	void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnFSCommandFlashPlayer(LPCTSTR command, LPCTSTR args); //��Ӧ����FLASH��Ϣ
	DECLARE_EVENTSINK_MAP()	

private:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenurl();
	afx_msg void OnFileExit();
	afx_msg void OnHelpAbout();
	afx_msg void OnHelpWebsite();
	afx_msg void OnSwitchSkinRange( UINT nID );
	afx_msg void OnUpdateSwitchSkinRange(CCmdUI* pCmdUI);

	afx_msg LRESULT	OnWmpOpenState(WPARAM x,LPARAM y);
	afx_msg LRESULT OnWmpPlayState(WPARAM x, LPARAM y);
	afx_msg LRESULT OnWmpPosChange(WPARAM x, LPARAM y);
};
