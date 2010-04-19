// DlgOpenURL.cpp : implementation file
//

#include "stdafx.h"
#include "fwclient.h"
#include "OpenUrlDlg.h"


// COpenUrlDlg dialog

IMPLEMENT_DYNAMIC(COpenUrlDlg, CDialog)

COpenUrlDlg::COpenUrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenUrlDlg::IDD, pParent)
	, m_strComboURL(_T(""))
{

}

COpenUrlDlg::~COpenUrlDlg()
{
}

void COpenUrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_URL, m_strComboURL);
}


BEGIN_MESSAGE_MAP(COpenUrlDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &COpenUrlDlg::OnBnClickedBrowse)
END_MESSAGE_MAP()


// COpenUrlDlg message handlers

void COpenUrlDlg::OnBnClickedBrowse()
{
	// TODO: Add your control notification handler code here
	CFileDialog	FileDlg(TRUE);
	FileDlg.m_ofn.lpstrFilter = _T("Windows Media movies(*.avi;*.wmv;*.wmp;*.wm;*.asf)\0*.avi;*.wmv;*.wmp;*.wm;*.asf\0Windows Media audios(*.wma;*.wav;*.aif;*.aifc;*.aiff;*.mid;*.midi;*.rmi)\0*.wma;*.wav;*.aif;*.aifc;*.aiff;*.mid;*.midi;*.rmi\0All files(*.*)\0*.*\0\0");
	FileDlg.m_ofn.Flags  |=OFN_ENABLESIZING;
	CString	strFile;
	if(FileDlg.DoModal() == IDOK)
	{
		strFile = FileDlg.GetPathName();
		SetDlgItemText(IDC_COMBO_URL,strFile);
	}	
}
