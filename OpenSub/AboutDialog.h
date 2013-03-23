#pragma once
//+------------------------------------------------------------------+
//| custom control for internet links                                |
//+------------------------------------------------------------------+
class CAboutDlg : public CDialog
{

private:
    CLink             m_link;
    CFont             m_font;

public:
   CAboutDlg();
   virtual BOOL      OnInitDialog();

protected:
   virtual void DoDataExchange(CDataExchange* pDX); 
   afx_msg void OnLinkClicked();

protected:
   DECLARE_MESSAGE_MAP()  
};
//+------------------------------------------------------------------+