#include "stdafx.h"

IOpenSubtitlesAPI* COpenSubtitlesAPIFactory::Create(const std::wstring &user,const std::wstring &pass,const std::wstring &user_agent,IEventListener *event_listener)
{
	if (!m_impl)
	{
		m_impl = new(std::nothrow)COpenSubtitlesAPI_Impl(user, pass, user_agent, event_listener);
		if (m_impl && event_listener)
			event_listener->OnApiReady(m_impl);
	}
	return m_impl;
}
COpenSubtitlesAPIFactory::~COpenSubtitlesAPIFactory()
{
    Release();
}
void COpenSubtitlesAPIFactory::Release()
{
    if (m_impl)
    {
        delete m_impl;
        m_impl = NULL;
    }
}
