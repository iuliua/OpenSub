
#include "stdafx.h"
#include "OpenSub.h"
#include "OpenSubDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COpenSubApp

BEGIN_MESSAGE_MAP(COpenSubApp, CWinApp)
   ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// COpenSubApp construction

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
COpenSubApp::COpenSubApp()
  {

  }

// The one and only COpenSubApp object

COpenSubApp theApp;

// COpenSubApp initialization

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
BOOL COpenSubApp::InitInstance()
  {
   CWinApp::InitInstance();

   // Standard initialization
   // If you are not using these features and wish to reduce the size
   // of your final executable, you should remove from the following
   // the specific initialization routines you do not need
   // Change the registry key under which our settings are stored
   // TODO: You should modify this string to be something appropriate
   // such as the name of your company or organization
   SetRegistryKey(L"Local AppWizard-Generated Applications");

   COpenSubDlg dlg;
   m_pMainWnd=&dlg;
   INT_PTR nResponse=dlg.DoModal();
   if (nResponse==IDOK)
     {

     }
   else if (nResponse==IDCANCEL)
        {

        }

   return(FALSE);
  }
//+------------------------------------------------------------------+
