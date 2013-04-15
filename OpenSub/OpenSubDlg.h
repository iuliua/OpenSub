#pragma once
#include "afxwin.h"
#include "inputfileinfo.h"
#include "OSApi.h"
#include "afxcmn.h"

// COpenSubDlg dialog
class COpenSubDlg : public CDialog
  {

public:
                     COpenSubDlg(CWnd *pParent=NULL);

   enum { IDD=IDD_OPENSUB_DIALOG };
   enum EnMsgType
   {
       MESSAGE_NORMAL,
       MESSAGE_ERROR
   };

protected:
   virtual void      DoDataExchange(CDataExchange* pDX);

protected:
   HICON             m_hIcon;

   virtual BOOL      OnInitDialog();
   afx_msg void      OnPaint();
   afx_msg HCURSOR   OnQueryDragIcon();
   DECLARE_MESSAGE_MAP()
private:
   LRESULT           OnSearchFinished(WPARAM wParam, LPARAM lParam);
   LRESULT           OnCopyData(WPARAM wParam, LPARAM lParam);
   static UINT       ThreadSearchSub(LPVOID pvParam);
   static UINT       ThreadDownload(LPVOID pvParam);
   static UINT       ThreadTestSub(LPVOID pvParam);
   static void       PrintMessage(HWND handle,LPCWSTR msg,EnMsgType msg_type=MESSAGE_NORMAL);
   void              InitializeList();
   void              SetTitle();
   static CString    Read7ZipPath();
private:
   OSApi::SubtitleInfoList m_result_list;
   volatile long     m_search_finished;
   bool              DownloadFinished() { return InterlockedExchangeAdd(&m_search_finished,0)>0; }
   void              DownloadFinished(int flag) { InterlockedExchange(&m_search_finished,flag); }
   void              UpdateList();
   BOOL              DownloadAndUnzip(OSApi::subtitle_info &item);
   BOOL              GetSubInfo(OSApi::subtitle_info& sub_info);
public:
   afx_msg void      OnLbnSelchangeList2();
   afx_msg void      OnSysCommand(UINT nID,LPARAM lParam);
private:
   CListCtrl         m_results_list_control;
   InputFileInfo     file_info;
   bool              m_error_detected;
public:
   afx_msg void      OnBnClickedDownload();
   afx_msg void      OnLinkClicked();
   afx_msg void      OnLinkClicked2();
   CComboBox         m_cmb_lang;
   afx_msg void      OnCbnSelchangeCombo1();
   CComboBox         m_cmb_match;
   CButton           m_btn_download;
   CButton           m_btn_explore;
   CButton           m_btn_play;
   CLink             m_link;
   CLink             m_link_websearch;
   afx_msg void      OnBnClickedExplore();
   afx_msg void      OnBnClickedPlay();
  };
//+------------------------------------------------------------------+
