#include "stdafx.h"
#include "AboutDialog.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
BOOL CAboutDlg::OnInitDialog()
{
   CDialog::OnInitDialog();
   return TRUE;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CAboutDlg::OnLinkClicked()
{
  SHELLEXECUTEINFO sh_exec_info = { 0 };
  sh_exec_info.cbSize = sizeof(SHELLEXECUTEINFO);
  sh_exec_info.fMask = 0;
  sh_exec_info.hwnd = NULL;
  sh_exec_info.lpVerb = NULL;
  sh_exec_info.lpFile = L"http://www.opensubtitles.org";
  sh_exec_info.lpParameters = L"";
  sh_exec_info.lpDirectory = NULL;
  sh_exec_info.nShow = SW_SHOWNORMAL;
  sh_exec_info.hInstApp = NULL;
  ShellExecuteEx(&sh_exec_info);
  return;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LINK,m_link);
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  ON_BN_CLICKED(IDC_LINK,&OnLinkClicked)
END_MESSAGE_MAP()
//+------------------------------------------------------------------+