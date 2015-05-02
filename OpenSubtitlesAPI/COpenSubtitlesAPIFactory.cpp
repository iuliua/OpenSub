#include "stdafx.h"

bool COpenSubtitlesAPIFactory::Create(const std::wstring &user,const std::wstring &pass,const std::wstring &user_agent,IEventListener *event_listener,IOpenSubtitlesAPI** osub_api)
{
	m_impl = new(std::nothrow)COpenSubtitlesAPI_Impl(user,pass,user_agent,event_listener);
	if (m_impl)
	{
		if (osub_api)
		  *osub_api = m_impl;
		if (event_listener)
			event_listener->OnApiReady(m_impl);
	}
	return m_impl != NULL;
}

void COpenSubtitlesAPIFactory::Release()
{
	delete m_impl;
}
