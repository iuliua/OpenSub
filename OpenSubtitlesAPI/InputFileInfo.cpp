#include "stdafx.h"
#include "InputFileInfo.h"

#define MAX(x,y) (((x) > (y)) ? (x) : (y))

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
InputFileInfo::InputFileInfo(LPCWSTR file_path)
  {
   file_directory[0]=0;
   file_name_no_extension[0]=0;
   file_hash[0]=0;
   file_size[0]=0;
   wcscpy_s(this->file_full_name,sizeof(this->file_full_name)/sizeof(WCHAR),file_path);
   file_name_total=wcslen(file_path);
   DWORD attrib=GetFileAttributes(file_path);
   if(attrib==INVALID_FILE_ATTRIBUTES || attrib==FILE_ATTRIBUTE_DIRECTORY)
      return;
   FileDirectory();
   FileNameNoExtension();
   FileSize();
   swprintf_s(file_full_name,MAX_PATH,L"%s\\%s",file_directory,this->file_name);
   Hash();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void InputFileInfo::FileDirectory()
  {
   file_directory[0]=0;
   int i;
   for(i=file_name_total-1;i>=0;i--)
      if(file_full_name[i]==L'\\')
         break;
   if(i>=0)
     {
      memcpy(file_directory,file_full_name,i*sizeof(WCHAR));
      file_directory[i]=0;
     }
   else
   {
      WCHAR curr_dir[MAX_PATH]={0};
      if(GetCurrentDirectory(MAX_PATH,curr_dir))
         wcscpy_s(file_directory,MAX_PATH,curr_dir);
   }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void InputFileInfo::FileNameNoExtension()
  {
   file_name_no_extension[0]=0;
   int i;
   for(i=file_name_total-1;i>=0;i--)
      if(file_full_name[i]=='\\')
         break;
   if(i>=0)
      wcscpy_s(file_name_no_extension,sizeof(file_name_no_extension)/sizeof(WCHAR),file_full_name+i+1);
   else
      wcscpy_s(file_name_no_extension,sizeof(file_name_no_extension)/sizeof(WCHAR),file_full_name);
   wcscpy_s(file_name,sizeof(file_name)/sizeof(WCHAR),file_name_no_extension);
   for(i=wcslen(file_name_no_extension)-1;i>=0;i--)
       if(file_name_no_extension[i]=='.')
       {
           file_name_no_extension[i]=0;
           break;
       }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void InputFileInfo::Hash()
  {
   FILE * handle;
   UINT64 hash, fsize;

   _wfopen_s(&handle,file_full_name,L"rb");
   if(handle==NULL)
      return;
   fseek(handle, 0, SEEK_END);
   fsize=ftell(handle);
   fseek(handle, 0, SEEK_SET);

   hash=fsize;

   for(UINT64 tmp=0, i=0; i < 65536/sizeof(tmp) && fread((char*)&tmp, sizeof(tmp), 1, handle); hash+=tmp, i++);
   fseek(handle, (long)MAX(0, fsize - 65536), SEEK_SET);
   for(UINT64 tmp=0, i=0; i < 65536/sizeof(tmp) && fread((char*)&tmp, sizeof(tmp), 1, handle); hash+=tmp, i++);

   swprintf_s(file_hash,sizeof(file_hash)/sizeof(WCHAR),L"%016I64x",hash);
   fclose(handle);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void InputFileInfo::FileSize()
  {
   WIN32_FIND_DATA find_data;
   HANDLE handle=FindFirstFile(file_full_name,&find_data);
   INT64 file_size1=((INT64)find_data.nFileSizeHigh << 32) | find_data.nFileSizeLow;
   swprintf_s(file_size,L"%I64d",file_size1);
   FindClose(handle);
  }
//+------------------------------------------------------------------+
