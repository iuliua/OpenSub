#pragma once

class COpenSubtitlesAPI_Impl :public IOpenSubtitlesAPI,public IMessageProcessor
{
	
private:
	CConnection     m_connection;
	XMLMemoryWriter m_xml_memory_writer;
	IEventListener *m_event_listener;
	CXMLBuilder     m_xml_builder;
	Buffer          m_response;
	CHAR            m_token[64];
	SubtitleInfoList m_sub_list;
	std::wstring     m_error_text;
    CMessageLoop    m_loop;

public:
	COpenSubtitlesAPI_Impl(const std::wstring &user, const std::wstring &pass, const std::wstring &user_agent,IEventListener*);
	~COpenSubtitlesAPI_Impl();

public:
	virtual BOOL Search(const std::wstring &file_path) override; 
	virtual std::wstring GetFileHash(std::wstring file_path) override;
	virtual std::wstring GetFileSize(std::wstring file_path) override;
	virtual std::wstring GetFileNameNoExt(std::wstring file_path) override;
	virtual std::wstring GetFileDirectory(std::wstring file_path) override;

private:
	bool Login();
	bool Logout();
	bool Search(LPCWSTR);
    virtual BOOL ProcessMessage(UINT, LPVOID, BOOL) override;
};