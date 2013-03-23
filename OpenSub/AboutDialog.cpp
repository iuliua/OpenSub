#include "stdafx.h"
#include "AboutDialog.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
BOOL CAboutDlg::OnInitDialog()
{
   CDialog::OnInitDialog();
   return TRUE;
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_LINK,m_link);
}
void CAboutDlg::OnLinkClicked()
{
  ShellExecute(NULL,L"open",L"http://www.opensubtitles.org",NULL,NULL,SW_SHOW);
  return;
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  ON_BN_CLICKED(IDC_LINK,&OnLinkClicked)
END_MESSAGE_MAP()