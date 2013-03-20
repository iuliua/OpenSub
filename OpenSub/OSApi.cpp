
#include "stdafx.h"
#include "OSApi.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
OSApi::OSApi(LPCWSTR user,LPCWSTR passwd,LPCWSTR lang,LPCWSTR user_agent):m_valid(false)
  {
   buffer_tmp[0]=0;
   wcscpy_s(m_user,user);
   wcscpy_s(m_passwd,passwd);
   wcscpy_s(m_lang,lang);
   wcscpy_s(m_user_agent,user_agent);
   m_internet=InternetOpen(L"OpenSubtitles",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
   m_connection=InternetConnect(m_internet,L"api.opensubtitles.org",INTERNET_DEFAULT_HTTP_PORT,0,0,INTERNET_SERVICE_HTTP,0,0);
   if(m_internet && m_connection)
       m_valid=true;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
OSApi::~OSApi()
{
   InternetCloseHandle(m_internet);
   InternetCloseHandle(m_connection);
   m_valid=false;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OSApi::BuildLoginXml()
  {
   xml_document xml_doc;
   xml_doc.append_child(_OSMETHOD_CALL).append_child(_OSMETHOD_NAME).append_child(node_pcdata).set_value("LogIn");
   xml_node params_node=xml_doc.child(_OSMETHOD_CALL).append_child(_OSPARAMS);
   params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(conv.ToUTF8(m_user));
   params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(conv.ToUTF8(m_passwd));
   params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(conv.ToUTF8(m_lang));
   params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(conv.ToUTF8(m_user_agent));
   xml_doc.save(xml_writer,"\t",format_default,encoding_utf8);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OSApi::BuildLogoutXml()
  {
   xml_document xml_doc;
   xml_doc.append_child(_OSMETHOD_CALL).append_child(_OSMETHOD_NAME).append_child(node_pcdata).set_value("LogOut");
   xml_node params_node=xml_doc.child(_OSMETHOD_CALL).append_child(_OSPARAMS);
   params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(m_token);
   xml_doc.save(xml_writer,"\t",format_default,encoding_utf8);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool OSApi::ReadToken()
  {
   xml_document xml_doc;
   pugi::xml_parse_result result=xml_doc.load_buffer(m_buffer.buffer_in,m_buffer.buffer_in_total);
   if(!result)
       return(false);
   xml_node struct_node=xml_doc.child(_OSMETHOD_RESP).child(_OSPARAMS).child(_OSPARAM).child(_OSVALUE).child(_OSSTRUCT);
   for(xml_node member_node=struct_node.child(_OSMEMBER);member_node;member_node=member_node.next_sibling(_OSMEMBER))
     {
      if(STRCMP(member_node.child(_OSNAME).child_value(),_OSTOKEN)==0)
        {
         strcpy_s(m_token,64,member_node.child(_OSVALUE).child(_OSSTRING).child_value());
         return(true);
        }
     }
   return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool OSApi::CheckResponseStatus()
  {
   xml_document xml_doc;
   pugi::xml_parse_result result=xml_doc.load_buffer(m_buffer.buffer_in,m_buffer.buffer_in_total);
   if(!result)
       return(false);
   xml_node struct_node=xml_doc.child(_OSMETHOD_RESP).child(_OSPARAMS).child(_OSPARAM).child(_OSVALUE).child(_OSSTRUCT);
   for(xml_node member_node=struct_node.child(_OSMEMBER);member_node;member_node=member_node.next_sibling(_OSMEMBER))
     {
         if(STRCMP(member_node.child(_OSNAME).child_value(),"status")==0)
        {
            if(STRCMP(member_node.child(_OSVALUE).child(_OSSTRING).child_value(),"200 OK")==0)
            return(true);
        }
     }
   return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool OSApi::SendRequest()
  {
   DWORD  bytes_read;
   InternetCloseHandle(m_request);
   const WCHAR* lplpszAcceptTypes[] = {L"*/*", NULL};
   m_request=HttpOpenRequest(m_connection,L"POST",L"/xml-rpc",NULL,0,lplpszAcceptTypes,0,0);
   if(m_request==NULL)
      return(false);
   if(!HttpSendRequest(m_request,0,0,xml_writer.xml_data,xml_writer.xml_data_size))
      return(false);
   DWORD content_len;
   DWORD content_len_size=sizeof(content_len);
   if(!HttpQueryInfo(m_request,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,&content_len,&content_len_size,0))
      return(false);
   if(!m_buffer.Allocate(content_len))
      return(false);
   while(InternetReadFile(m_request,buffer_tmp,sizeof(buffer_tmp),&bytes_read) && bytes_read)
     {
      memcpy(m_buffer.buffer_in+m_buffer.buffer_in_total,buffer_tmp,bytes_read);
      m_buffer.buffer_in_total+=bytes_read;
     }
   bool status=CheckResponseStatus();
   InternetCloseHandle(m_request);
   return(status);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool OSApi::Login()
  {
   bool res;
   BuildLoginXml();
   res=SendRequest();
   res=res&&ReadToken();
   return(res);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool OSApi::SearchSubtitle(LPCWSTR file_name_no_extension,LPCWSTR file_size,LPCWSTR file_hash,SubtitleInfoList &result_list)
  {
   bool res;
   BuildSearchXml(file_name_no_extension,file_size,file_hash);
   res=SendRequest();
   res=res&&ParseResultXml(result_list);
   return(res);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool OSApi::Logout()
  {
   bool res;
   BuildLogoutXml();
   res=SendRequest();
   return(res);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OSApi::BuildSearchXml(LPCWSTR file_name_no_extension,LPCWSTR file_size,LPCWSTR file_hash)
  {
   xml_document xml_doc;
   xml_node     member_node;
   xml_node     struct_node;
   xml_node     array_data;
   xml_doc.append_child(_OSMETHOD_CALL).append_child(_OSMETHOD_NAME).append_child(node_pcdata).set_value("SearchSubtitles");
   xml_node params_node=xml_doc.child(_OSMETHOD_CALL).append_child(_OSPARAMS);
//---
   params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(m_token);
//---
   array_data=params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSARRAY).append_child("data");
//---
   struct_node=array_data.append_child(_OSVALUE).append_child(_OSSTRUCT);

   member_node=struct_node.append_child(_OSMEMBER);
   member_node.append_child(_OSNAME).append_child(node_pcdata).set_value("sublanguageid");
   member_node.append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(conv.ToUTF8(m_lang));

   member_node=struct_node.append_child(_OSMEMBER);
   member_node.append_child(_OSNAME).append_child(node_pcdata).set_value("moviehash");
   member_node.append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(conv.ToUTF8(file_hash));

   member_node=struct_node.append_child(_OSMEMBER);
   member_node.append_child(_OSNAME).append_child(node_pcdata).set_value("moviebytesize");
   member_node.append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(conv.ToUTF8(file_size));

   struct_node=array_data.append_child(_OSVALUE).append_child(_OSSTRUCT);

   member_node=struct_node.append_child(_OSMEMBER);
   member_node.append_child(_OSNAME).append_child(node_pcdata).set_value("sublanguageid");
   member_node.append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(conv.ToUTF8(m_lang));

   member_node=struct_node.append_child(_OSMEMBER);
   member_node.append_child(_OSNAME).append_child(node_pcdata).set_value("query");
   member_node.append_child(_OSVALUE).append_child(_OSSTRING).append_child(node_pcdata).set_value(conv.ToUTF8(file_name_no_extension));
//--- 
   xml_doc.save(xml_writer,"\t",format_default,encoding_utf8);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool OSApi::ParseResultXml(SubtitleInfoList &result_list)
  {
   xml_document xml_doc;
   pugi::xml_parse_result result=xml_doc.load_buffer(m_buffer.buffer_in,m_buffer.buffer_in_total,parse_default);
   if(!result)
       return(false);
//--- save struct node
   xml_node struct_node=xml_doc.child(_OSMETHOD_RESP).child(_OSPARAMS).child(_OSPARAM).child(_OSVALUE).child(_OSSTRUCT);
//--- find data member
   xml_node member_node;
   for(member_node=struct_node.child(_OSMEMBER);member_node;member_node=member_node.next_sibling(_OSMEMBER))
     {
      if(STRCMP(member_node.child(_OSNAME).child_value(),_OSDATA)==0)
         break;
     }
//--- find data node
   xml_node data_node=member_node.child(_OSVALUE).child(_OSARRAY).child(_OSDATA);
//--- go through all values
   for(xml_node value_node=data_node.child(_OSVALUE);value_node;value_node=value_node.next_sibling(_OSVALUE))
     {
      subtitle_info current_info={0};
      xml_node struct_node=value_node.child(_OSSTRUCT);
      for(member_node=struct_node.child(_OSMEMBER);member_node;member_node=member_node.next_sibling(_OSMEMBER))
        {
         if(STRCMP(member_node.child(_OSNAME).child_value(),"MatchedBy")==0)
            wcscpy_s(current_info.matched_by,conv.ToWCHAR(member_node.child(_OSVALUE).child(_OSSTRING).child_value()));
         if(STRCMP(member_node.child(_OSNAME).child_value(),"SubFileName")==0)
             wcscpy_s(current_info.sub_file_name,conv.ToWCHAR(member_node.child(_OSVALUE).child(_OSSTRING).child_value()));
         if(STRCMP(member_node.child(_OSNAME).child_value(),"SubFormat")==0)
             wcscpy_s(current_info.sub_format,conv.ToWCHAR(member_node.child(_OSVALUE).child(_OSSTRING).child_value()));      
         if(STRCMP(member_node.child(_OSNAME).child_value(),"SubDownloadsCnt")==0)
             wcscpy_s(current_info.sub_download_count,conv.ToWCHAR(member_node.child(_OSVALUE).child(_OSSTRING).child_value()));
         if(STRCMP(member_node.child(_OSNAME).child_value(),"MovieReleaseName")==0)
            wcscpy_s(current_info.mov_release_name,conv.ToWCHAR(member_node.child(_OSVALUE).child(_OSSTRING).child_value()));
         if(STRCMP(member_node.child(_OSNAME).child_value(),"MovieName")==0)
             wcscpy_s(current_info.mov_name,conv.ToWCHAR(member_node.child(_OSVALUE).child(_OSSTRING).child_value()));
         if(STRCMP(member_node.child(_OSNAME).child_value(),"SeriesSeason")==0)
             wcscpy_s(current_info.season,conv.ToWCHAR(member_node.child(_OSVALUE).child(_OSSTRING).child_value()));
         if(STRCMP(member_node.child(_OSNAME).child_value(),"SeriesEpisode")==0)
             wcscpy_s(current_info.episode,conv.ToWCHAR(member_node.child(_OSVALUE).child(_OSSTRING).child_value()));
         if(STRCMP(member_node.child(_OSNAME).child_value(),"SubDownloadLink")==0)
            wcscpy_s(current_info.zip_link,conv.ToWCHAR(member_node.child(_OSVALUE).child(_OSSTRING).child_value()));
         if(STRCMP(member_node.child(_OSNAME).child_value(),"SubLanguageID")==0)
            wcscpy_s(current_info.lang,conv.ToWCHAR(member_node.child(_OSVALUE).child(_OSSTRING).child_value()));
        }
      result_list.push_back(current_info);
     }
   return(true);
  }
LPCSTR OSApi::Convertor::ToUTF8(LPCWSTR str)
{
    if(wcslen(str)>0 && str[wcslen(str)]!=L'\0')
        return NULL;
    size_t length=WideCharToMultiByte(CP_UTF8,0,str,-1,0,0,0,0);
    if(m_buffer_char_size<length)
    {
        if(m_buffer_char)
            delete[] m_buffer_char;
        m_buffer_char=new char[length];
        if(!m_buffer_char)
        {
            m_buffer_char_size=0;
            return "";
        }
        m_buffer_char_size=length;
    }
    WideCharToMultiByte(CP_UTF8,0,str,-1,m_buffer_char,length,0,0);
    return m_buffer_char;
}

LPCWSTR OSApi::Convertor::ToWCHAR(LPCSTR str)
{
    if(strlen(str)>0 && str[strlen(str)]!=L'\0')
        return NULL;
    size_t length=MultiByteToWideChar(CP_UTF8,0,str,-1,0,0);
    if(m_buffer_wchar_size<length)
    {
        if(m_buffer_wchar)
            delete[] m_buffer_wchar;
        m_buffer_wchar=new wchar_t[length];
        if(!m_buffer_wchar)
        {
            m_buffer_wchar_size=0;
            return L"";
        }
        m_buffer_wchar_size=length;
    }
    MultiByteToWideChar(CP_UTF8,0,str,-1,m_buffer_wchar,length);
    return m_buffer_wchar;
}
OSApi::Convertor::Convertor()
{
  m_buffer_wchar_size=0;
  m_buffer_char_size=0;
  m_buffer_wchar=NULL;
  m_buffer_char=NULL;
}

OSApi::Convertor::~Convertor()
{
  if(m_buffer_char)
  {
      delete[] m_buffer_char;
      m_buffer_char=NULL;
  }
  if(m_buffer_wchar)
  {
      delete[] m_buffer_wchar;
      m_buffer_wchar=NULL;
  }
}
