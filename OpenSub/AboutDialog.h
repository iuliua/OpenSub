#pragma once

class CAboutDlg : public CDialog
{
public:
   CAboutDlg();
   virtual BOOL      OnInitDialog();

   // Dialog Data
   enum { IDD = IDD_ABOUTBOX };

protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   afx_msg void OnLinkClicked();

   // Implementation
protected:
   DECLARE_MESSAGE_MAP()
public:
   CLink             m_link;
   CFont             m_font;

};