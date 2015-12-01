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
   : CDialog(COpenSubDlg::IDD, pParent)
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
   ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &COpenSubDlg::OnDoubleClickSubtitle)
END_MESSAGE_MAP()

void COpenSubDlg::InitializeList()
{
    ListView_SetExtendedListViewStyle(m_results_list_control.m_hWnd, LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

    CRect rect;
    m_results_list_control.GetClientRect(&rect);
    int nColInterval = rect.Width() / 8;
    m_results_list_control.InsertColumn(0, L"Count", LVCFMT_CENTER, nColInterval);
    m_results_list_control.InsertColumn(1, L"Name", LVCFMT_LEFT, nColInterval * 7);
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
   EnableButtons(FALSE);
   PrintMessage(GetSafeHwnd(), L"Searching");
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
HCURSOR COpenSubDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COpenSubDlg::OnBnClickedExplore()
  {
   WCHAR command[512];
   swprintf_s(command, sizeof(command) / sizeof(WCHAR), L"explorer /select,\"%s\"", theApp.m_lpCmdLine);
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

void COpenSubDlg::OnLinkClicked()
{
	CString str;
    //str.Format(L"http://www.opensubtitles.org/search/sublanguageid-%s/moviename-%s", L"eng", m_api->GetFileNameNoExt(theApp.m_lpCmdLine).c_str());
    str.Format(L"http://www.opensubtitles.org/search/sublanguageid-%s/moviebytesize-%s/moviehash-%s", L"eng", m_api->GetFileSize(theApp.m_lpCmdLine).c_str(),m_api->GetFileHash(theApp.m_lpCmdLine).c_str());
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
	m_results_list_control.EnableWindow(flag);
	m_link.EnableWindow(flag);
}

BOOL COpenSubDlg::GetSelectedSubtitle(IOpenSubtitlesAPI::subtitle_info& info)
{
    POSITION s=m_results_list_control.GetFirstSelectedItemPosition();
    int selected_item = m_results_list_control.GetNextSelectedItem(s);
	if (selected_item < 0)
		return FALSE;
    info = m_subtitles[m_results_list_control.GetItemData(selected_item)];
	return TRUE;
}
void COpenSubDlg::OnBnClickedDownload()
{
    IOpenSubtitlesAPI::subtitle_info selected_sub;
    if (GetSelectedSubtitle(selected_sub))
    {
        EnableButtons(FALSE);
        PrintMessage(GetSafeHwnd(), L"Downloading");
        Tools::SetCurrDirToFileLocation(theApp.m_lpCmdLine);
        Tools::MemoryStruct memory;
        Tools::MemoryStruct extracted_sub;
        Tools::DownloadLink(selected_sub.zip_link, memory);
        Tools::UnzipFileType(memory.memory, memory.size, selected_sub.format, extracted_sub);
        std::ofstream out((m_api->GetFileNameNoExt(theApp.m_lpCmdLine) + L'.' + selected_sub.format).c_str(), std::ios::binary);
        out.write(extracted_sub.memory, extracted_sub.size);
        out.close();
        delete[] extracted_sub.memory;

        PrintMessage(GetSafeHwnd(), L"Done");
        EnableButtons(TRUE);
        EndDialog(IDOK);
    }
}

void COpenSubDlg::OnDoubleClickSubtitle(NMHDR *pNMHDR, LRESULT *pResult)
{
    OnBnClickedDownload();
}
void COpenSubDlg::OnBnClickedPlay()
{
	IOpenSubtitlesAPI::subtitle_info selected_sub;
	
	if (GetSelectedSubtitle(selected_sub))
	{
		WCHAR                existing_subtitle[512];
		WCHAR                temp_subtitle[512];
		bool                 subtitle_exists = false;
		swprintf_s(existing_subtitle, sizeof(existing_subtitle) / sizeof(WCHAR), L"%s\\%s.%s",
			m_api->GetFileDirectory(theApp.m_lpCmdLine).c_str(),
			m_api->GetFileNameNoExt(theApp.m_lpCmdLine).c_str(),
			selected_sub.format.c_str());
		swprintf_s(temp_subtitle, sizeof(temp_subtitle) / sizeof(WCHAR), L"%s\\temp_moved_open_sub.tmp",
			m_api->GetFileDirectory(theApp.m_lpCmdLine).c_str());
		::DeleteFile(temp_subtitle);

		subtitle_exists = (GetFileAttributes(existing_subtitle) != INVALID_FILE_ATTRIBUTES);
		if ((subtitle_exists && ::MoveFile(existing_subtitle, temp_subtitle)) || !subtitle_exists)
		{
			EnableButtons(FALSE);
			PrintMessage(GetSafeHwnd(), L"Downloading");
			Tools::SetCurrDirToFileLocation(theApp.m_lpCmdLine);
            Tools::MemoryStruct memory;
            Tools::MemoryStruct extracted_sub;
            Tools::DownloadLink(selected_sub.zip_link, memory);
            Tools::UnzipFileType(memory.memory, memory.size, selected_sub.format, extracted_sub);
            std::ofstream out((m_api->GetFileNameNoExt(theApp.m_lpCmdLine) + L'.' + selected_sub.format).c_str(), std::ios::binary);
            out.write(extracted_sub.memory, extracted_sub.size);
            out.close();
            delete[] extracted_sub.memory;
			
			HANDLE hProc;
			if (Launch(theApp.m_lpCmdLine, &hProc))
			{
				PrintMessage(GetSafeHwnd(), L"Playing...");
				::WaitForSingleObject(hProc, INFINITE);
				CloseHandle(hProc);
				if (subtitle_exists)
				{
					if (!::DeleteFile(existing_subtitle) || !::MoveFile(temp_subtitle, existing_subtitle))
						PrintMessage(GetSafeHwnd(), L"Cannot rename temporary file.");
				}
				else
				{
					if (!::DeleteFile(existing_subtitle))
						PrintMessage(GetSafeHwnd(), L"Cannot delete test subtitle.");
				}
			}
			else
				PrintMessage(GetSafeHwnd(), L"failed to open video");
			PrintMessage(GetSafeHwnd(), L"Done");
			EnableButtons(TRUE);
		}
		return;
	}
}

void COpenSubDlg::OnError(std::wstring error_details)
{
	MessageBox(error_details.c_str(), L"error", MB_OK);
}

void COpenSubDlg::OnSubtitle(IOpenSubtitlesAPI::subtitle_info& info)
{
	m_subtitles.push_back(info);
}

void COpenSubDlg::OnSearchComplete()
{
    if (m_results_list_control.GetSafeHwnd())//if close window before search finish
    {
        std::sort(m_subtitles.begin(), m_subtitles.end());

        for (unsigned int i = 0; i < m_subtitles.size(); i++)
        {
            LVITEM lvItem;
            lvItem.mask = LVIF_TEXT | LVIF_PARAM;
            lvItem.iItem = 0;
            lvItem.iSubItem = 0;
            lvItem.pszText = const_cast<LPWSTR>(m_subtitles[i].download_count.c_str());
            lvItem.lParam = i;
            m_results_list_control.InsertItem(&lvItem);

            if (m_subtitles[i].sub_file_name.size())
                m_results_list_control.SetItemText(0, 1, m_subtitles[i].sub_file_name.c_str());
            else
                if (m_subtitles[i].release_name.size())
                    m_results_list_control.SetItemText(0, 1, m_subtitles[i].release_name.c_str());
        }
        if (m_subtitles.size())
            m_results_list_control.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
        EnableButtons(TRUE);
        PrintMessage(GetSafeHwnd(), L"Search finished");
    }
}

void COpenSubDlg::OnApiReady(IOpenSubtitlesAPI* api)
{
	m_api = api;
}
