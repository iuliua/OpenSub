//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "OpenSubDlg.h"
#include "AboutDialog.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
COpenSubDlg::COpenSubDlg(CWnd *pParent)
   : CDialog(COpenSubDlg::IDD, pParent),
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
   ON_MESSAGE(WM_COPYDATA,OnCopyData)
   ON_BN_CLICKED(IDC_BUTTON1, &COpenSubDlg::OnBnClickedDownload)
   ON_BN_CLICKED(IDC_BUTTON2, &COpenSubDlg::OnBnClickedExplore)
   ON_BN_CLICKED(IDC_BUTTON3, &COpenSubDlg::OnBnClickedPlay)
   ON_BN_CLICKED(IDC_LINK_MAIN,&OnLinkClicked)
   ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &COpenSubDlg::OnNMDblclkList1)
   ON_COMMAND(IDC_RADIO_HASH, &COpenSubDlg::OnRadioHash)
   ON_COMMAND(IDC_RADIO_TEXT, &COpenSubDlg::OnRadioHash)
END_MESSAGE_MAP()

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
   lvColumn.cx=660;
   lvColumn.pszText=L"Name";
   nCol=m_results_list_control.InsertColumn(1, &lvColumn);
  }

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
   std::wstring path(theApp.m_lpCmdLine);
   SetWindowText(path.substr(path.rfind(L'\\') + 1).c_str());
//--- create list columns
   InitializeList();
   m_button_lang.SetWindowText(L"eng");
   m_hash_match.SetCheck(1);
   EnableButtons(FALSE);
   return(TRUE);
  }

void COpenSubDlg::PrintMessage(HWND handle,LPCWSTR msg)
  {
   COPYDATASTRUCT cds;
   cds.cbData=(wcslen(msg)+1)*sizeof(WCHAR);
   cds.lpData=(PVOID)msg;
   ::SendMessage(handle,WM_COPYDATA,0,(LPARAM)(LPVOID)&cds);
  }

LRESULT COpenSubDlg::OnCopyData(WPARAM wParam, LPARAM lParam)
{
    PCOPYDATASTRUCT pcds=(PCOPYDATASTRUCT)lParam;
    GetDlgItem(IDC_STATIC)->SetWindowText((LPCWSTR)pcds->lpData);
    return(0);
}

BOOL COpenSubDlg::DownloadAndUnzip(LPCWSTR zip_link)
{
  PrintMessage(GetSafeHwnd(), L"downloading...");
  return TRUE;
}
/*
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
}*/
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
void COpenSubDlg::OnBnClickedDownload()
  {
  }

void COpenSubDlg::OnBnClickedExplore()
  {
   WCHAR command[512];
   swprintf_s(command, sizeof(command) / sizeof(WCHAR), L"explorer /select,%s", theApp.m_lpCmdLine);
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
void COpenSubDlg::OnBnClickedPlay()
{
   //AfxBeginThread(ThreadTestSub,this);
}
void COpenSubDlg::GetMatchingMethod(CString &str)
{
	if (m_hash_match.GetCheck())
		str.Format(L"hash");
	if (m_text_match.GetCheck())
		str.Format(L"text");
}
void COpenSubDlg::OnLinkClicked()
{
	CString str_lang, str_match;
	CString str;
	m_button_lang.GetWindowText(str_lang);
	GetMatchingMethod(str_match);
	if (wcscmp(str_match, L"text") == 0)
		str.Format(L"http://www.opensubtitles.org/search/sublanguageid-%s/moviename-%s", (LPCWSTR)str_lang, m_api->GetFileNameNoExt(theApp.m_lpCmdLine).c_str());
	if (wcscmp(str_match, L"hash") == 0)
		str.Format(L"http://www.opensubtitles.org/search/sublanguageid-%s/moviebytesize-%s/moviehash-%s", (LPCWSTR)str_lang, m_api->GetFileSize(theApp.m_lpCmdLine).c_str(),m_api->GetFileHash(theApp.m_lpCmdLine).c_str());
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
	*pResult = 0;
	m_should_exit = true;
	OnBnClickedDownload();
}

void COpenSubDlg::OnRadioHash()
{
}

void COpenSubDlg::OnError(std::wstring error_details)
{
	MessageBox(error_details.c_str(), L"error", MB_OK);
}

void COpenSubDlg::OnSubtitle(std::wstring name, std::wstring download_count, std::wstring zip_link)
{
	LVITEM lvItem = { 0 };

	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iItem = 0;
	lvItem.iSubItem = 0;
	lvItem.pszText = const_cast<LPWSTR>(download_count.c_str());
	lvItem.lParam = (LPARAM)(new(std::nothrow) std::wstring(zip_link));

	m_results_list_control.InsertItem(&lvItem);
	m_results_list_control.SetItemText(0, 1, name.c_str());
	m_results_list_control.SortItemsEx(&SortFunc, (LPARAM)&m_results_list_control);
}

void COpenSubDlg::OnSearchFinish()
{
	EnableButtons(TRUE);
}

void COpenSubDlg::OnApiReady(IOpenSubtitlesAPI* api)
{
	m_api = api;
}
