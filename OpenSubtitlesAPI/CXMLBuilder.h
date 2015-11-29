#pragma once

class CXMLBuilder 
{
private:
	std::wstring m_user;
	std::wstring m_pass;
	std::wstring m_user_agent;
	std::string m_token;
public:
	CXMLBuilder(const std::wstring &user, const std::wstring &pass, const std::wstring &user_agent);
	bool BuildLogin(XMLMemoryWriter&);
	bool BuildLogout(XMLMemoryWriter&);
	bool BuildSearch(XMLMemoryWriter&, InputFileInfo &,const std::wstring);
	bool IsResponseOK(Buffer&);
	bool GetToken(Buffer&);
	bool GetSubtitlesFound(IOpenSubtitlesAPI::SubtitleInfoList &,Buffer&,IEventListener*);
};