#pragma once
struct MemoryStruct {
	char *memory;
	size_t size;
};
class COpenSubDlg : public CDialog,public IEventListener
  {

public:
   COpenSubDlg(CWnd *pParent=NULL);
   enum { IDD=IDD_OPENSUB_DIALOG };

protected:
   virtual void      DoDataExchange(CDataExchange* pDX);

protected:
   HICON             m_hIcon;
   virtual BOOL      OnInitDialog();
   afx_msg void      OnPaint();
   afx_msg HCURSOR   OnQueryDragIcon();
   DECLARE_MESSAGE_MAP()
   
private:
   LRESULT           OnCopyData(WPARAM wParam, LPARAM lParam);
   //static UINT       ThreadDownload(LPVOID pvParam);
   //static UINT       ThreadTestSub(LPVOID pvParam);
   static void       PrintMessage(HWND handle, LPCWSTR msg);
   void              InitializeList();
   static BOOL       Launch(LPCWSTR cmd, HANDLE *hProc=NULL);
   void              EnableButtons(BOOL);
   static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
   MemoryStruct DownloadLink(std::wstring &link);
   bool              m_should_exit;
   IOpenSubtitlesAPI *m_api;
   
private:
   BOOL              DownloadAndUnzip(LPCWSTR);
   void              GetMatchingMethod(CString &);
public:
   afx_msg void      OnSysCommand(UINT nID,LPARAM lParam);
private:
   CListCtrl         m_results_list_control;
public:
   afx_msg void      OnBnClickedDownload();
   afx_msg void      OnLinkClicked();
   afx_msg void      OnLinkClicked2();
   CButton           m_btn_download;
   CButton           m_btn_explore;
   CButton           m_btn_play;
   CLink             m_link;
   afx_msg void      OnBnClickedExplore();
   afx_msg void      OnBnClickedPlay();
   CButton m_button_lang;
   afx_msg void OnDoubleClickSubtitle(NMHDR *pNMHDR, LRESULT *pResult);
   CButton m_text_match;
   CButton m_hash_match;
   afx_msg void OnRadioHash();

   virtual void OnError(std::wstring error_details) override;
   virtual void OnSubtitle(std::wstring name, std::wstring download_count, std::wstring zip_link,std::wstring) override;

   virtual void OnSearchFinish() override;

   virtual void OnApiReady(IOpenSubtitlesAPI* api) override;

};


//+------------------------------------------------------------------+
