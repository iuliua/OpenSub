//+------------------------------------------------------------------+
//|                                                                  |
//|                   Copyright 2001-2011, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"

#define _OSMETHOD_CALL "methodCall"
#define _OSMETHOD_NAME "methodName"
#define _OSPARAM       "param"
#define _OSPARAMS      "params"
#define _OSVALUE       "value"
#define _OSSTRING      "string"
#define _OSMETHOD_RESP "methodResponse"
#define _OSSTRUCT      "struct"
#define _OSMEMBER      "member"
#define _OSNAME        "name"
#define _OSTOKEN       "token"
#define _OSARRAY       "array"
#define _OSDATA        "data"

#define STRCMP         strcmp   // wcscmp

class OSApi
  {
private:
   struct xml_string_writer: pugi::xml_writer
     {
      LPSTR             xml_data;
      DWORD             xml_buffer_size;
      DWORD             xml_data_size;
      virtual void write(const void* data, size_t size)
        {
         if(xml_buffer_size < size)
         {
             if(xml_data)
             {
                 delete[] xml_data;
                 xml_data=NULL;
                 xml_buffer_size=0;
                 xml_data_size=0;
             }
             xml_data=new(std::nothrow) char[size];
             if(!xml_data)
             {
                 xml_buffer_size=0;
                 xml_data_size=0;
                 return;
             }
             xml_buffer_size=size;
         }
        memcpy(xml_data,data,size);
        xml_data_size=size;
        }
      xml_string_writer()
      {
          xml_data=NULL;
          xml_data_size=0;
          xml_buffer_size=0;
      }
      ~xml_string_writer()
      {
          if(xml_data)
              delete[] xml_data;
      }
     };
   class Convertor
   {
   private:
       LPSTR  m_buffer_char;
       size_t m_buffer_char_size;
       LPWSTR m_buffer_wchar;
       size_t m_buffer_wchar_size;
   public:
       Convertor();
       ~Convertor();
       LPCSTR  ToUTF8(LPCWSTR str);
       LPCWSTR ToWCHAR(LPCSTR str);
   };
   class Buffer
   {
   public:
       LPSTR buffer_in;
       DWORD buffer_in_total;
       DWORD buffer_in_size;
   public:
       Buffer()
       {
           buffer_in=NULL;
           buffer_in_total=0;
           buffer_in_size=0;
       }
       ~Buffer()
       {
           if(buffer_in)
               delete[] buffer_in;
           buffer_in_total=0;
           buffer_in_size=0;
       }
       bool Allocate(size_t size)
       {
         if(size>buffer_in_size)
         {
             if(buffer_in)
             {
                 delete[] buffer_in;
                 buffer_in_total=0;
                 buffer_in_size=0;
             }
             buffer_in=new(std::nothrow)char[size];
             if(!buffer_in)
             {
                 buffer_in_total=0;
                 buffer_in_size=0;
                 return false;
             }
             buffer_in_size=size;
         }
         buffer_in_total=0;
         return true;
       }
   };
  public:
   struct subtitle_info
     {
      WCHAR              sub_file_name[128];
      WCHAR              sub_download_count[64];
      WCHAR              sub_format[32];
      WCHAR              mov_name[128];
      WCHAR              mov_release_name[128];
      WCHAR              season[32];
      WCHAR              episode[32];
      WCHAR              zip_link[128];
      WCHAR              matched_by[32];
      WCHAR              lang[32];
     };
   typedef std::vector<subtitle_info> SubtitleInfoList;
public:
                     OSApi(LPCWSTR user,LPCWSTR passwd, LPCWSTR lang, LPCWSTR user_agent);
                    ~OSApi(); 
   bool              IsValid() { return m_valid; };
   bool              Login();
   bool              SearchSubtitle(LPCWSTR file_name_no_extension,LPCWSTR file_size,LPCWSTR file_hash,SubtitleInfoList &result_list);
   bool              Logout();

private:
   WCHAR             m_user[64];
   WCHAR             m_passwd[64];
   WCHAR             m_lang[64];
   WCHAR             m_user_agent[64];

   HINTERNET         m_internet;
   HINTERNET         m_connection;
   HINTERNET         m_request;
   CHAR              m_token[64];

   
   CHAR              buffer_tmp[1024];
   xml_string_writer xml_writer;
   bool              m_valid;
   Convertor         conv;
   Buffer            m_buffer;

private:
   void              BuildLoginXml();
   void              BuildLogoutXml();
   void              BuildSearchXml(LPCWSTR file_name_no_extension,LPCWSTR file_size,LPCWSTR file_hash);
   bool              SendRequest();
   bool              ReadToken();
   bool              CheckResponseStatus();
   bool              ParseResultXml(SubtitleInfoList &result_list);
  };
//+------------------------------------------------------------------+
