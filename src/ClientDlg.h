// ClientDlg.h : 头文件
//

#pragma once

#include "flashplayer.h"
#include "TextConfig.h"
#include "VideoPlayer.h"
#include "PlayerFactory.h"

// CClientDlg 对话框
class CClientDlg : public CDialog
{				 
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CClientDlg();

	// 对话框数据
	enum { IDD = IDD_FWCLIENT_DIALOG };

private:

	//与界面FLASH传递变量
	CString GetFSVariable(LPCTSTR name);
	void	SetFSVariable(LPCTSTR name,LPCTSTR value);

	//界面大小控制
	void	AutoFitFlashOriginSize();
	void	AdjustVideoWindow();

	//全屏操作
	void	SetFullScreen(BOOL bFullScreen);

	//加载皮肤
	void	LoadCurSkin(CString strCurSkin=_T(""));
	void	LoadCurMiniSkin();

	//弹出菜单
	void	PopupContextMenu();
	void	PopupFileMenu();
	void	PopupHelpMenu();
	void	PopupSwitchSkinMenu();

	//开始停止播放及调整播放位置
	void OnPlayingBegin();
	void OnPlayingEnd();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
private:
	HICON m_hIcon; //图标

	TextConfig* _config; //配置数据

	CFlashPlayer m_FlashPlayer; //界面FLASH,覆盖整个界面
	CVideoPlayer* m_pVideoPlayer; //叠加在播放区上

	BOOL	m_bLoadFlashSkin; //界面Flash加载成功标记
	BOOL	m_bFullScreen; //全屏标记
	CRect	m_rcRestore; //全屏前大小

	DWORD	m_dwScreenSave; //屏保标记
	BOOL m_bCanChangeProgressByExternProgram; //界面Flash与程序控制播放位置

	//文件路径
	CString m_AppPath;
	CString m_SkinPath;

	//皮肤切换菜单
	HMENU	m_hSwitchSkinMenu;
	UINT	m_nSwitchSkinID;

protected:
	// 生成的消息映射函数
	virtual	BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg	void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnFSCommandFlashPlayer(LPCTSTR command, LPCTSTR args); //响应界面FLASH消息
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
