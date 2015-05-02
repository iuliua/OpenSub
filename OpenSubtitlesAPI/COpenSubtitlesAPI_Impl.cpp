#include "stdafx.h"
#include "COpenSubtitlesAPI_Impl.h"

COpenSubtitlesAPI_Impl::COpenSubtitlesAPI_Impl(const std::wstring &user, const std::wstring &pass, const std::wstring &user_agent,IEventListener* event_listener)
	:m_xml_builder(user, pass, user_agent), m_event_listener(event_listener)
{
	Run();
}

COpenSubtitlesAPI_Impl::~COpenSubtitlesAPI_Impl()
{

}

BOOL COpenSubtitlesAPI_Impl::Search(const std::wstring &file_path)
{
	wchar_t *buf = new(std::nothrow)wchar_t[file_path.size() + 1];
	wcscpy_s(buf,file_path.size()+1, file_path.c_str());
	return post_message(MSG_SEARCH,0,(LPARAM)buf);
}
std::wstring COpenSubtitlesAPI_Impl::GetFileHash(std::wstring file_path)
{
	InputFileInfo info(file_path.c_str());
	return info.file_hash;
};
std::wstring COpenSubtitlesAPI_Impl::GetFileSize(std::wstring file_path)
{
	InputFileInfo info(file_path.c_str());
	return info.file_size;
};
std::wstring COpenSubtitlesAPI_Impl::GetFileNameNoExt(std::wstring file_path)
{
	InputFileInfo info(file_path.c_str());
	return info.file_name_no_extension;
};
bool COpenSubtitlesAPI_Impl::Login()
{
	m_xml_builder.BuildLogin(m_xml_memory_writer);
	m_connection.SendRequest(m_xml_memory_writer,m_response,m_event_listener);
	if (m_xml_builder.IsResponseOK(m_response))
		return m_xml_builder.GetToken(m_response);
	else
		return false;
}

bool COpenSubtitlesAPI_Impl::Logout()
{
	m_xml_builder.BuildLogout(m_xml_memory_writer);
	m_connection.SendRequest(m_xml_memory_writer,m_response,m_event_listener);
	return m_xml_builder.IsResponseOK(m_response);
}

bool COpenSubtitlesAPI_Impl::Search(LPCWSTR file_path)
{
	InputFileInfo file_info(file_path);
	m_xml_builder.BuildSearch(m_xml_memory_writer, file_info, L"eng");
	m_connection.SendRequest(m_xml_memory_writer, m_response, m_event_listener);
	return m_xml_builder.GetSubtitlesFound(m_sub_list, m_response,m_event_listener);
}

void COpenSubtitlesAPI_Impl::ProcessMessage(MSG& msg)
{
	switch (msg.message)
	{
	case MSG_SEARCH:
	{
		if (m_connection.Connect())
		{
			if (Login())
			{
				wchar_t* file_path = (wchar_t*)msg.lParam;
				Search(file_path);
				m_event_listener->OnSearchFinish();
				delete[] file_path;
			}
			else
				m_event_listener->OnError(L"failed to login");
		}
		else
			m_event_listener->OnError(L"failed to connect");
		Logout();
		m_connection.Disconnect();
	}
		break;
	}
}
