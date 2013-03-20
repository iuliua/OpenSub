#pragma once

#ifndef __AFXWIN_H__
   #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"      // main symbols

// COpenSubApp:
// See OpenSub.cpp for the implementation of this class
//

class COpenSubApp : public CWinApp
  {
public:
                     COpenSubApp();

public:
   virtual BOOL      InitInstance();

   DECLARE_MESSAGE_MAP()
  };

extern COpenSubApp theApp;
//+------------------------------------------------------------------+
