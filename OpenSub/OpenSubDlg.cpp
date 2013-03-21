
#include "stdafx.h"
#include "OpenSub.h"
#include "OpenSubDlg.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
   CAboutDlg();

   // Dialog Data
   enum { IDD = IDD_ABOUTBOX };

protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   // Implementation
protected:
   DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cabout_dialogDlg dialog
// COpenSubDlg dialog

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
COpenSubDlg::COpenSubDlg(CWnd *pParent)
   : CDialog(COpenSubDlg::IDD, pParent)
   , m_search_finished(1)
  {
   m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::DoDataExchange(CDataExchange* pDX)
  {
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_LIST1, m_list1);
   DDX_Control(pDX, IDC_COMBO1, m_cmb_lang);
   DDX_Control(pDX, IDC_COMBO2, m_cmb_match);
   DDX_Control(pDX, IDC_BUTTON1, m_btn_download);
   DDX_Control(pDX, IDC_BUTTON2, m_btn_explore);
  }
BEGIN_MESSAGE_MAP(COpenSubDlg, CDialog)
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()
   //}}AFX_MSG_MAP
	ON_WM_SYSCOMMAND()
    ON_MESSAGE(WM_SEARCH_FINISHED,OnSearchFinished)
    ON_MESSAGE(WM_COPYDATA,OnCopyData)
    ON_BN_CLICKED(IDC_BUTTON1, &COpenSubDlg::OnBnClickedButton1)
    ON_CBN_SELCHANGE(IDC_COMBO1, &COpenSubDlg::OnCbnSelchangeCombo1)
    ON_CBN_SELCHANGE(IDC_COMBO2, &COpenSubDlg::OnCbnSelchangeCombo1)
    ON_BN_CLICKED(IDC_BUTTON2, &COpenSubDlg::OnBnClickedButton2)
END_MESSAGE_MAP()
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::InitializeList()
  {
   m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_AUTOSIZECOLUMNS);

   LVCOLUMN lvColumn;
   int nCol;

   lvColumn.mask=LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
   lvColumn.fmt=LVCFMT_LEFT;
   lvColumn.cx=600;
   lvColumn.pszText=L"Name";
   nCol=m_list1.InsertColumn(0, &lvColumn);

   lvColumn.mask=LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
   lvColumn.fmt=LVCFMT_LEFT;
   lvColumn.cx=100;
   lvColumn.pszText=L"Lang";
   m_list1.InsertColumn(1, &lvColumn);

   lvColumn.mask=LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
   lvColumn.fmt=LVCFMT_LEFT;
   lvColumn.cx=80;
   lvColumn.pszText=L"Match";
   m_list1.InsertColumn(2, &lvColumn);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::SetTitle()
  {
   CString       str;
   InputFileInfo file_info(theApp.m_lpCmdLine);
   if(file_info.file_size[0]>0)
     {
      str.Format(L"OpenSub - %s",file_info.file_name);
      SetWindowText(str);
     }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
BOOL COpenSubDlg::OnInitDialog()
  {
   CDialog::OnInitDialog();
   // Add "About..." menu item to system menu.

   // IDM_ABOUTBOX must be in the system command range.
   ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
   ASSERT(IDM_ABOUTBOX < 0xF000);

   CMenu* pSysMenu = GetSystemMenu(FALSE);
   if (pSysMenu != NULL)
   {
      BOOL bNameValid;
      CString strAboutMenu;
      bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
      ASSERT(bNameValid);
      if (!strAboutMenu.IsEmpty())
      {
         pSysMenu->AppendMenu(MF_SEPARATOR);
         pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
      }
   }
   // Set the icon for this dialog.  The framework does this automatically
   //  when the application's main window is not a dialog
   SetIcon(m_hIcon, TRUE);         // Set big icon
   SetIcon(m_hIcon, FALSE);      // Set small icon
//--- set dialog title
   SetTitle();
//--- create list columns
   InitializeList();
   m_cmb_lang.AddString(L"eng");
   m_cmb_lang.AddString(L"rum");
   m_cmb_lang.SetCurSel(0);
   m_cmb_match.AddString(L"moviehash");
   m_cmb_match.AddString(L"fulltext");
   m_cmb_match.SetCurSel(1);
   m_btn_download.EnableWindow(FALSE);
   m_btn_explore.EnableWindow(FALSE);
   m_cmb_lang.EnableWindow(FALSE);
   m_cmb_match.EnableWindow(FALSE);
   m_list1.EnableWindow(FALSE);
//--- start searching
   AfxBeginThread(ThreadSearchSub,this);
   return(TRUE);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::PrintMessage(HWND handle,LPCWSTR msg)
  {
   COPYDATASTRUCT cds;
   cds.dwData=PRINT_MESSAGE;
   cds.cbData=(wcslen(msg)+1)*sizeof(WCHAR);
   cds.lpData=(PVOID)msg;
   ::SendMessage(handle,WM_COPYDATA,0,(LPARAM)(LPVOID)&cds);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
LRESULT COpenSubDlg::OnCopyData(WPARAM wParam, LPARAM lParam)
{
    PCOPYDATASTRUCT pcds=(PCOPYDATASTRUCT)lParam;
    switch(pcds->dwData)
    {
    case PRINT_MESSAGE:
        GetDlgItem(IDC_STATIC)->SetWindowText((LPCWSTR)pcds->lpData);
        break;
    }
    return(0);
}
CString COpenSubDlg::Read7ZipPath()
{
    CString cSvar = L"";
    HKEY hKey;
    if (::RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\7-Zip",0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        WCHAR szData[256];
        DWORD dwKeyDataType;
        DWORD dwDataBufSize = 256;
        if (::RegQueryValueEx(hKey, L"Path", NULL, &dwKeyDataType,
            (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
            switch ( dwKeyDataType )
            {
            case REG_SZ:
                cSvar = CString(szData);
                break;
            }
        }
        ::RegCloseKey( hKey );
    }
    return cSvar;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT COpenSubDlg::ThreadDownload(LPVOID pvParam)
  {
   COpenSubDlg *dlg=(COpenSubDlg*)pvParam;
   if(!dlg->DownloadFinished())
      return(0);
   int selected_item=dlg->m_list1.GetNextItem(-1,LVNI_SELECTED);
   if(selected_item==-1)
      return(0);
   InputFileInfo file_info(theApp.m_lpCmdLine);
   LVITEM item={0};
   item.mask=LVIF_PARAM;
   item.iItem=selected_item;
   if(!dlg->m_list1.GetItem(&item))
      return(0);
   CString zip_exe_path=Read7ZipPath(); 
   if(zip_exe_path.IsEmpty())
   {
     PrintMessage(dlg->GetSafeHwnd(),L"7-Zip not installed.");
     dlg->DownloadFinished(1);
     return(0);
   }
   dlg->DownloadFinished(0);
   OSApi::subtitle_info sub_info=dlg->m_result_list[item.lParam];
   if(URLDownloadToFile(NULL,sub_info.zip_link,L"c:\\sub.zip",0,NULL)!=S_OK)
     {
      PrintMessage(dlg->GetSafeHwnd(),L"Download failed.");
      dlg->DownloadFinished(1);
      return(0);
     }
//--- unzip
   WCHAR command[1024];
   swprintf_s(command,sizeof(command)/sizeof(WCHAR),L"%s\\7z.exe e c:\\sub.zip -oc:\\ \"sub\" -y",zip_exe_path);
   STARTUPINFO info={0};
   info.cb=sizeof(info);
   info.dwFlags=STARTF_USESHOWWINDOW;
   info.wShowWindow=SW_HIDE;
   PROCESS_INFORMATION processInfo;
   bool unzip_result=false;
   if (CreateProcess(NULL,command, NULL, NULL, TRUE, 0, NULL, L"C:\\", &info, &processInfo))
     {
      DWORD exit_code=0;
      PrintMessage(dlg->GetSafeHwnd(),L"Unzipping...");
      ::WaitForSingleObject(processInfo.hProcess, 5000);
      if(GetExitCodeProcess(processInfo.hProcess,&exit_code) && exit_code ==0)
         unzip_result=true;
      CloseHandle(processInfo.hProcess);
      CloseHandle(processInfo.hThread);
     }
   if(!unzip_result)
     {
      PrintMessage(dlg->GetSafeHwnd(),L"Cannot unzip file.");
      dlg->DownloadFinished(1);
      ::DeleteFile(L"c:\\sub.zip");
      return(0);
     }
//--- rename subtitle and move to movie folder 
   WCHAR existing_location[512];
   WCHAR new_location[512];
   swprintf_s(existing_location,sizeof(existing_location)/sizeof(WCHAR),L"c:\\sub");
   swprintf_s(new_location,sizeof(existing_location)/sizeof(WCHAR),L"%s\\%s.%s",file_info.file_directory,file_info.file_name_no_extension,sub_info.sub_format);
   if(GetFileAttributes(new_location)!=INVALID_FILE_ATTRIBUTES && !::DeleteFile(new_location))
     {
      PrintMessage(dlg->GetSafeHwnd(),L"Cannot delete target file.");
      ::DeleteFile(L"c:\\sub.zip");
      ::DeleteFile(existing_location);
      dlg->DownloadFinished(1);
      return(0);
     }
   PrintMessage(dlg->GetSafeHwnd(),L"Moving file...");
   if(!::MoveFile(existing_location,new_location))
      PrintMessage(dlg->GetSafeHwnd(),L"Cannot move file.");
   else
      PrintMessage(dlg->GetSafeHwnd(),L"Done.");
   ::DeleteFile(L"c:\\sub.zip");
   ::DeleteFile(existing_location);
   dlg->DownloadFinished(1);
   return(0);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT COpenSubDlg::ThreadSearchSub(LPVOID pvParam)
  {
   COpenSubDlg            *dlg=(COpenSubDlg*)pvParam;
   OSApi                   m_api(L"",L"",L"eng,rum",L"OS Test User Agent");
   OSApi::SubtitleInfoList result_list;
   InputFileInfo           file_info(theApp.m_lpCmdLine);
   //--- validate file
   if(file_info.file_size[0]==0)
     {
      PrintMessage(dlg->GetSafeHwnd(),L"Invalid input file.");
      return(0);
     }
   //--- validate api object
   if(!m_api.IsValid())
   {
       PrintMessage(dlg->GetSafeHwnd(),L"Failed to start api.");
       return(0);
   }
   PrintMessage(dlg->GetSafeHwnd(),L"Logging in...");
   if(!m_api.Login())
     {
      PrintMessage(dlg->GetSafeHwnd(),L"Log in failed.");
      return(0);
     }
   PrintMessage(dlg->GetSafeHwnd(),L"Searching...");
   if(!m_api.SearchSubtitle(file_info.file_name_no_extension,file_info.file_size,file_info.file_hash,result_list))
     {
      PrintMessage(dlg->GetSafeHwnd(),L"Search failed.");
      return(0);
     }
   dlg->m_result_list=result_list;
   ::PostMessage(dlg->GetSafeHwnd(),WM_SEARCH_FINISHED,0,0);
   PrintMessage(dlg->GetSafeHwnd(),L"Logging out...");
   if(!m_api.Logout())
     {
      PrintMessage(dlg->GetSafeHwnd(),L"Log out failed.");
      return(0);
     }
   CString    strText;
   strText.Format(L"Found %d subtitles.",result_list.size());
   PrintMessage(dlg->GetSafeHwnd(),strText);
   return(0);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
LRESULT COpenSubDlg::OnSearchFinished(WPARAM wParam, LPARAM lParam)
  {
   UpdateList();
   m_btn_download.EnableWindow(TRUE);
   m_btn_explore.EnableWindow(TRUE);
   m_cmb_lang.EnableWindow(TRUE);
   m_cmb_match.EnableWindow(TRUE);
   m_list1.EnableWindow(TRUE);
   return(0);
  }
void COpenSubDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
   if ((nID & 0xFFF0) == IDM_ABOUTBOX)
   {
      CAboutDlg dlgAbout;
      dlgAbout.DoModal();
   }
   else
   {
      CDialog::OnSysCommand(nID, lParam);
   }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::OnPaint()
  {
   if (IsIconic())
     {
      CPaintDC dc(this); // device context for painting

      SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

      // Center icon in client rectangle
      int cxIcon=GetSystemMetrics(SM_CXICON);
      int cyIcon=GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x=(rect.Width() - cxIcon + 1)/2;
      int y=(rect.Height() - cyIcon + 1)/2;

      // Draw the icon
      dc.DrawIcon(x, y, m_hIcon);
     }
   else
     {
      CDialog::OnPaint();
     }
  }

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
HCURSOR COpenSubDlg::OnQueryDragIcon()
  {
   return static_cast<HCURSOR>(m_hIcon);
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::OnBnClickedButton1()
  {
   AfxBeginThread(ThreadDownload,this);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::OnCbnSelchangeCombo1()
  {
   UpdateList();
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::UpdateList()
  {
   m_list1.DeleteAllItems();
   for(size_t i=0;i<m_result_list.size();i++)
     {
      OSApi::subtitle_info data=m_result_list[i];
      int index=m_list1.GetItemCount();

      int idx_lang=m_cmb_lang.GetCurSel();
      int idx_match=m_cmb_match.GetCurSel();
      if(idx_lang>=0&&idx_match>=0)
        {
         CString str_lang,str_match;
         m_cmb_lang.GetLBText(idx_lang,str_lang);
         m_cmb_match.GetLBText(idx_match,str_match);
         if(wcscmp(data.lang,str_lang)!=0||wcscmp(data.matched_by,str_match)!=0)
            continue;
        }

      LVITEM lvItem;
      int nItem;

      lvItem.mask=LVIF_TEXT|LVIF_PARAM;
      lvItem.iItem=index;
      lvItem.iSubItem=0;
      lvItem.pszText=data.sub_file_name;
      lvItem.lParam=i;
      nItem=m_list1.InsertItem(&lvItem);

      m_list1.SetItemText(nItem, 1, data.lang);
      m_list1.SetItemText(nItem, 2, data.matched_by);
     }
  }
void COpenSubDlg::OnBnClickedButton2()
  {
   InputFileInfo file_info(theApp.m_lpCmdLine);
   WCHAR command[512];
   swprintf_s(command,sizeof(command)/sizeof(WCHAR),L"explorer /select,%s",file_info.file_full_name);
   STARTUPINFO info={0};
   info.cb=sizeof(info);
   info.dwFlags=STARTF_USESHOWWINDOW;
   info.wShowWindow=SW_SHOW;
   PROCESS_INFORMATION processInfo;
   if (CreateProcess(NULL,command, NULL, NULL, TRUE, 0, NULL, L"C:\\", &info, &processInfo))
     {
      ::WaitForSingleObject(processInfo.hProcess, INFINITE);
      CloseHandle(processInfo.hProcess);
      CloseHandle(processInfo.hThread);
     }
  }
//+------------------------------------------------------------------+
