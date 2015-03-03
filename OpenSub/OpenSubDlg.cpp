//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "OpenSubDlg.h"
#include "AboutDialog.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
COpenSubDlg::COpenSubDlg(CWnd *pParent)
   : CDialog(COpenSubDlg::IDD, pParent),
     file_info(theApp.m_lpCmdLine),
	 m_should_exit(true)
{
   m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::DoDataExchange(CDataExchange* pDX)
  {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_results_list_control);
	DDX_Control(pDX, IDC_BUTTON1, m_btn_download);
	DDX_Control(pDX, IDC_BUTTON2, m_btn_explore);
	DDX_Control(pDX, IDC_BUTTON3, m_btn_play);
	DDX_Control(pDX, IDC_LINK_MAIN, m_link);
	DDX_Control(pDX, IDC_BUTTON4, m_button_lang);
	DDX_Control(pDX, IDC_RADIO_TEXT, m_text_match);
	DDX_Control(pDX, IDC_RADIO_HASH, m_hash_match);
}
BEGIN_MESSAGE_MAP(COpenSubDlg, CDialog)
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()
   //}}AFX_MSG_MAP
	ON_WM_SYSCOMMAND()
   ON_MESSAGE(WM_SEARCH_FINISHED,OnSearchFinished)
   ON_MESSAGE(WM_COPYDATA,OnCopyData)
   ON_BN_CLICKED(IDC_BUTTON1, &COpenSubDlg::OnBnClickedDownload)
   ON_BN_CLICKED(IDC_BUTTON2, &COpenSubDlg::OnBnClickedExplore)
   ON_BN_CLICKED(IDC_BUTTON3, &COpenSubDlg::OnBnClickedPlay)
   ON_BN_CLICKED(IDC_LINK_MAIN,&OnLinkClicked)
   ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &COpenSubDlg::OnNMDblclkList1)
   ON_COMMAND(IDC_RADIO_HASH, &COpenSubDlg::OnRadioHash)
   ON_COMMAND(IDC_RADIO_TEXT, &COpenSubDlg::OnRadioHash)
END_MESSAGE_MAP()
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::InitializeList()
  {
   m_results_list_control.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_AUTOSIZECOLUMNS);

   LVCOLUMN lvColumn;
   int nCol;

   lvColumn.mask=LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
   lvColumn.fmt=LVCFMT_LEFT;
   lvColumn.cx=75;
   lvColumn.pszText=L"Count";
   nCol=m_results_list_control.InsertColumn(0, &lvColumn);

   lvColumn.mask=LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
   lvColumn.fmt=LVCFMT_LEFT;
   lvColumn.cx=750;
   lvColumn.pszText=L"Name";
   nCol=m_results_list_control.InsertColumn(1, &lvColumn);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::SetTitle()
  {
   CString       str;
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
   m_lang = L"eng";
   m_button_lang.SetWindowText(m_lang);
   m_hash_match.SetCheck(1);

   EnableButtons(FALSE);
  
   m_sub_tmp_file_name.Format(L"sub");
   if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, m_path)))
   {
	   m_zip_tmp_file_path.Format(L"%s\\sub.zip", m_path);
	   m_sub_tmp_file_path.Format(L"%s\\%s", m_path,m_sub_tmp_file_name);
   };


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
    GetDlgItem(IDC_STATIC)->SetWindowText((LPCWSTR)pcds->lpData);
    return(0);
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
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
BOOL COpenSubDlg::DownloadAndUnzip(OSApi::subtitle_info &sub_info)
{
    //--- get path of 7zip executable
    CString zip_exe_path=Read7ZipPath(); 
	if (!zip_exe_path.IsEmpty())
	{
		::DeleteFile(m_zip_tmp_file_path);
		::DeleteFile(m_sub_tmp_file_path);
		//--- download packed subtitles
		if (SUCCEEDED(URLDownloadToFile(NULL, sub_info.zip_link, m_zip_tmp_file_path, 0, NULL)))
		{
			//--- unzip
			WCHAR command[1024];
			swprintf_s(command, sizeof(command) / sizeof(WCHAR), L"%s\\7z.exe e %s -o%s \"%s\" -y", zip_exe_path, m_zip_tmp_file_path,m_path,m_sub_tmp_file_name);
			STARTUPINFO info = { 0 };
			info.cb = sizeof(info);
			info.dwFlags = STARTF_USESHOWWINDOW;
			info.wShowWindow = SW_HIDE;
			PROCESS_INFORMATION processInfo;
			if (CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
			{
				DWORD exit_code = 0;
				PrintMessage(GetSafeHwnd(), L"Unpacking...");
				::WaitForSingleObject(processInfo.hProcess, 5000);
				if (!GetExitCodeProcess(processInfo.hProcess, &exit_code) || exit_code != 0)
				{
					PrintMessage(GetSafeHwnd(), L"Cannot unzip file.");
					::DeleteFile(m_zip_tmp_file_path);
				}
				CloseHandle(processInfo.hProcess);
				CloseHandle(processInfo.hThread);
			}
		}
		else
		{
			PrintMessage(GetSafeHwnd(), L"Download failed.");
			return FALSE;
		}
	}
	else
	{
		PrintMessage(GetSafeHwnd(), L"7-Zip not installed.");
		return FALSE;
	}
    return TRUE;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
BOOL COpenSubDlg::GetSubInfo(OSApi::subtitle_info& sub_info)
{
    //--- get info on selected item
    int selected_item=m_results_list_control.GetNextItem(-1,LVNI_SELECTED);
    if(selected_item==-1)
        return(FALSE);
    LVITEM item={0};
    item.mask=LVIF_PARAM;
    item.iItem=selected_item;
    if(!m_results_list_control.GetItem(&item))
        return(FALSE);
    sub_info=m_result_list[item.lParam];
    return(TRUE);
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT COpenSubDlg::ThreadDownload(LPVOID pvParam)
  {

   WCHAR                new_location[512];
   COpenSubDlg         *dlg       = (COpenSubDlg*)pvParam;
   InputFileInfo       &file_info = dlg->file_info;
   OSApi::subtitle_info sub_info = { 0 };

   // get selected item
   if(dlg->GetSubInfo(sub_info))
   {
	   dlg->m_btn_download.EnableWindow(FALSE);
	   // download and unzip subtitle to c:\sub file
	   if(dlg->DownloadAndUnzip(sub_info))
	   {
		   // rename subtitle and move to movie folder 
		   swprintf_s(new_location, sizeof(new_location) / sizeof(WCHAR), L"%s\\%s.%s", file_info.file_directory
			   , file_info.file_name_no_extension
			   , sub_info.sub_format);

		   if (GetFileAttributes(new_location) != INVALID_FILE_ATTRIBUTES && !::DeleteFile(new_location))
			   PrintMessage(dlg->GetSafeHwnd(), L"Cannot delete existing subtitle.");
		   else
		   {
			   PrintMessage(dlg->GetSafeHwnd(), L"Moving subtitle...");
			   if (!::MoveFile(dlg->m_sub_tmp_file_path, new_location))
				   PrintMessage(dlg->GetSafeHwnd(), L"Cannot move file.");
			   else
				   PrintMessage(dlg->GetSafeHwnd(), L"Done.");
		   }
	   }
	   // cleanup
	   ::DeleteFile(dlg->m_zip_tmp_file_path);
	   ::DeleteFile(dlg->m_sub_tmp_file_path);
	   dlg->m_btn_download.EnableWindow(TRUE);
   }
   if (dlg->m_should_exit)
	   dlg->EndDialog(IDOK);
   return(FALSE);
  }
// Test subtitle sequence
// 1. Retrieve selected subtitle information
// 2. Disable Test button
// 3. Download and unzip subtitle file
// 4. 
UINT COpenSubDlg::ThreadTestSub(LPVOID pvParam)
{
	HANDLE               hProc(NULL);
    COpenSubDlg         *dlg=(COpenSubDlg*)pvParam;
    InputFileInfo        &file_info=dlg->file_info;
	OSApi::subtitle_info sub_info = { 0 };
	WCHAR                existing_subtitle[512];
	WCHAR                temp_subtitle[512];
	bool                 subtitle_exists = false;
    
	// get selected item
	if (dlg->GetSubInfo(sub_info))
	{
		dlg->m_btn_play.EnableWindow(FALSE);
		//--- download and unzip subtitle to c:\sub file
		if (dlg->DownloadAndUnzip(sub_info))
		{
			swprintf_s(existing_subtitle, sizeof(existing_subtitle) / sizeof(WCHAR), L"%s\\%s.%s",
				file_info.file_directory
				, file_info.file_name_no_extension
				, sub_info.sub_format);
			swprintf_s(temp_subtitle, sizeof(temp_subtitle) / sizeof(WCHAR), L"%s\\temp_moved_open_sub.tmp",
				file_info.file_directory);
			::DeleteFile(temp_subtitle);
			
			subtitle_exists = (GetFileAttributes(existing_subtitle) != INVALID_FILE_ATTRIBUTES);

			//does an existing subtitle already available? then temporarily move it	
			if ((subtitle_exists && ::MoveFile(existing_subtitle, temp_subtitle)) || !subtitle_exists)
			{
				if (::MoveFile(dlg->m_sub_tmp_file_path, existing_subtitle))
				{
					if (Launch(file_info.file_full_name, &hProc))
					{
						dlg->PrintMessage(dlg->GetSafeHwnd(), L"Playing...");
						::WaitForSingleObject(hProc, INFINITE);
						dlg->PrintMessage(dlg->GetSafeHwnd(), L"");
						CloseHandle(hProc);
						if (subtitle_exists)
						{
							if (!::DeleteFile(existing_subtitle) || !::MoveFile(temp_subtitle, existing_subtitle))
								PrintMessage(dlg->GetSafeHwnd(), L"Cannot rename temporary file.");
						}
						else
						{
							if (!::DeleteFile(existing_subtitle))
								PrintMessage(dlg->GetSafeHwnd(), L"Cannot delete test subtitle.");
						}
					}
					else
						PrintMessage(dlg->GetSafeHwnd(), L"failed to open video");
				}
				else
				{
					PrintMessage(dlg->GetSafeHwnd(), L"Cannot move subtitle file.");
					if (!::MoveFile(temp_subtitle, existing_subtitle))
						PrintMessage(dlg->GetSafeHwnd(), L"Cannot rename temporary file.");
				}
			}
			else
				PrintMessage(dlg->GetSafeHwnd(), L"Cannot create temporary file.");
		}
		dlg->m_btn_play.EnableWindow(TRUE);
		::DeleteFile(dlg->m_zip_tmp_file_path);
		::DeleteFile(dlg->m_sub_tmp_file_path);
	}
    return(FALSE);
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT COpenSubDlg::ThreadSearchSub(LPVOID pvParam)
  {
   COpenSubDlg            *dlg=(COpenSubDlg*)pvParam;
   OSApi::SubtitleInfoList result_list;
   InputFileInfo          &file_info=dlg->file_info;
   OSApi                   m_api(L"",L"",dlg->m_lang,L"JulianOS");

   if (file_info.file_size[0] > 0)
   {
	   if (m_api.IsValid())
	   {
		   PrintMessage(dlg->GetSafeHwnd(), L"Logging in...");
		   if (m_api.Login())
		   {
			   PrintMessage(dlg->GetSafeHwnd(), L"Searching...");
			   if (m_api.SearchSubtitle(file_info.file_name_no_extension, file_info.file_size, file_info.file_hash, result_list))
			   {
				   dlg->m_result_list = result_list;
				   ::PostMessage(dlg->GetSafeHwnd(), WM_SEARCH_FINISHED, 0, 0);
				   PrintMessage(dlg->GetSafeHwnd(), L"Logging out...");
				   if (m_api.Logout())
				   {
					   CString    strText;
					   strText.Format(L"Found %d subtitles.", result_list.size());
					   PrintMessage(dlg->GetSafeHwnd(), strText);
				   }
				   else
					   PrintMessage(dlg->GetSafeHwnd(), L"Log out failed.");
			   }
			   else
				   PrintMessage(dlg->GetSafeHwnd(), L"Search failed.");
		   }
		   else
			   PrintMessage(dlg->GetSafeHwnd(), L"Log in failed.");
	   }
	   else
		   PrintMessage(dlg->GetSafeHwnd(), L"Failed to start api.");
   }
   else
	   PrintMessage(dlg->GetSafeHwnd(), L"Input file is empty");

   return(0);
  }
LRESULT COpenSubDlg::OnSearchFinished(WPARAM wParam, LPARAM lParam)
  {
   UpdateList();
   if (m_results_list_control.GetItemCount() == 0)
   {
	   m_text_match.SetCheck(1);
	   m_hash_match.SetCheck(0);
	   UpdateList();
   }
   EnableButtons(TRUE);
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
void COpenSubDlg::OnBnClickedDownload()
  {
   AfxBeginThread(ThreadDownload,this);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pListCtrl = (CListCtrl*)lParamSort;
	CString    strItem1 = pListCtrl->GetItemText(static_cast<int>(lParam1), 0);
	CString    strItem2 = pListCtrl->GetItemText(static_cast<int>(lParam2), 0);

	int x1 = _tstoi(strItem1.GetBuffer());
	int x2 = _tstoi(strItem2.GetBuffer());
	int result = 0;
	if ((x1 - x2) < 0)
		result = -1;
	else if ((x1 - x2) == 0)
		result = 0;
	else
		result = 1;

	return result*-1;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::UpdateList()
  {
   m_results_list_control.DeleteAllItems();
   for (size_t i = 0; i < m_result_list.size(); i++)
   {
	   OSApi::subtitle_info data = m_result_list[i];
	   int index = m_results_list_control.GetItemCount();

	   CString str_lang, str_match;
	   m_button_lang.GetWindowText(str_lang);
	   GetMatchingMethod(str_match);
	   if (str_match == L"hash")
		   str_match = L"moviehash";
	   else
		   if (str_match == L"text")
			   str_match = L"fulltext";
	   if (wcscmp(data.lang, str_lang) != 0 || wcscmp(data.matched_by, str_match) != 0)
		   continue;

	   LVITEM lvItem;
	   int nItem;

	   lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	   lvItem.iItem = index;
	   lvItem.iSubItem = 0;
	   lvItem.pszText = data.sub_download_count;
	   lvItem.lParam = i;
	   nItem = m_results_list_control.InsertItem(&lvItem);

	   m_results_list_control.SetItemText(nItem, 1, data.mov_release_name);
   }
   m_results_list_control.SortItemsEx(&SortFunc, (LPARAM)&m_results_list_control);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::OnBnClickedExplore()
  {
   WCHAR command[512];
   swprintf_s(command, sizeof(command) / sizeof(WCHAR), L"explorer /select,%s", file_info.file_full_name);
   STARTUPINFO info = { 0 };
   info.cb = sizeof(info);
   info.dwFlags = STARTF_USESHOWWINDOW;
   info.wShowWindow = SW_SHOW;
   PROCESS_INFORMATION processInfo;
   if (CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
   {
	   ::WaitForSingleObject(processInfo.hProcess, INFINITE);
	   CloseHandle(processInfo.hProcess);
	   CloseHandle(processInfo.hThread);
   }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::OnBnClickedPlay()
{
   AfxBeginThread(ThreadTestSub,this);
}
void COpenSubDlg::GetMatchingMethod(CString &str)
{
	if (m_hash_match.GetCheck())
		str.Format(L"hash");
	if (m_text_match.GetCheck())
		str.Format(L"text");
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void COpenSubDlg::OnLinkClicked()
{
	CString str_lang, str_match;
	CString str;
	m_button_lang.GetWindowText(str_lang);
	GetMatchingMethod(str_match);
	if (wcscmp(str_match, L"text") == 0)
		str.Format(L"http://www.opensubtitles.org/search/sublanguageid-%s/moviename-%s", (LPCWSTR)str_lang, file_info.file_name_no_extension);
	if (wcscmp(str_match, L"hash") == 0)
		str.Format(L"http://www.opensubtitles.org/search/sublanguageid-%s/moviebytesize-%s/moviehash-%s", (LPCWSTR)str_lang, file_info.file_size, file_info.file_hash);
	Launch(str);
}
BOOL COpenSubDlg::Launch(LPCWSTR cmd,HANDLE *hProc)
{
	SHELLEXECUTEINFO sh_exec_info = { 0 };
	sh_exec_info.cbSize = sizeof(SHELLEXECUTEINFO);
	sh_exec_info.fMask = SEE_MASK_NOCLOSEPROCESS;
	sh_exec_info.hwnd = NULL;
	sh_exec_info.lpVerb = L"open";
	sh_exec_info.lpFile = cmd;
	sh_exec_info.lpParameters = L"";
	sh_exec_info.lpDirectory = NULL;
	sh_exec_info.nShow = SW_SHOWNORMAL;
	sh_exec_info.hInstApp = NULL;
	
	if (ShellExecuteEx(&sh_exec_info))
	{
		if (hProc)
			*hProc = sh_exec_info.hProcess;
		return TRUE;
	}
	else
	return FALSE;
}
void COpenSubDlg::EnableButtons(BOOL flag)
{
	m_btn_download.EnableWindow(flag);
	m_btn_explore.EnableWindow(flag);
	m_btn_play.EnableWindow(flag);
	m_button_lang.EnableWindow(flag);
	m_hash_match.EnableWindow(flag);
	m_text_match.EnableWindow(flag);
	m_results_list_control.EnableWindow(flag);
	m_link.EnableWindow(flag);
}
void COpenSubDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	m_should_exit = true;
	OnBnClickedDownload();
}
//+------------------------------------------------------------------+

void COpenSubDlg::OnRadioHash()
{
	// TODO: Add your command handler code here
	UpdateList();
}

