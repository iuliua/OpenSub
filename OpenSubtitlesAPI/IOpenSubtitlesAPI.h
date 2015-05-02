#pragma once

class COpenSubtitlesAPI_Impl;
class IOpenSubtitlesAPI;
class IEventListener
{
public:
	virtual void OnError(std::wstring error_details) = 0;
	virtual void OnSubtitle(std::wstring name,std::wstring download_count,std::wstring zip_link) = 0;
	virtual void OnSearchFinish() = 0;
	virtual void OnApiReady(IOpenSubtitlesAPI* api) = 0;
};
class IOpenSubtitlesAPI
{
public:
	virtual ~IOpenSubtitlesAPI() {}
	virtual BOOL Search(const std::wstring &file_path) = 0;
	virtual std::wstring GetFileHash(std::wstring file_path) = 0;
	virtual std::wstring GetFileSize(std::wstring file_path) = 0;
	virtual std::wstring GetFileNameNoExt(std::wstring file_path) = 0;
};

class COpenSubtitlesAPIFactory
{

private:
	COpenSubtitlesAPI_Impl *m_impl;

public:
	bool Create(const std::wstring &user, const std::wstring &pass, const std::wstring &user_agent,IEventListener*, IOpenSubtitlesAPI** osub_api=NULL);
	void Release();
};