#include "stdafx.h"

struct InputFileInfo
  {
public:
                     InputFileInfo(LPCWSTR file_name);
   WCHAR             file_full_name[MAX_PATH];
   WCHAR             file_name[MAX_PATH];
   WCHAR             file_directory[MAX_PATH];
   WCHAR             file_name_no_extension[MAX_PATH];
   WCHAR             file_hash[64];
   WCHAR             file_size[64];
private:
   void              FileDirectory();
   void              FileNameNoExtension();
   void              FileSize();
   void              Hash();
   int               file_name_total;
  };
//+------------------------------------------------------------------+
