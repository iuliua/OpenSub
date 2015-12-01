#pragma once
class COpenSubtitlesAPI_Impl;
class IOpenSubtitlesAPI
{
public:
	virtual ~IOpenSubtitlesAPI() {}
	virtual BOOL Search(const std::wstring &file_path) = 0;
	virtual std::wstring GetFileHash(std::wstring file_path) = 0;
	virtual std::wstring GetFileSize(std::wstring file_path) = 0;
	virtual std::wstring GetFileNameNoExt(std::wstring file_path) = 0;
	virtual std::wstring GetFileDirectory(std::wstring file_path) = 0;
public:
	struct subtitle_info
	{
		std::wstring download_count;
		std::wstring format;
		std::wstring release_name;
		std::wstring zip_link;
		std::wstring sub_file_name;
		std::wstring matched_by;
		bool operator < (const subtitle_info& str) const
		{
			return (_wtoi(download_count.c_str()) < _wtoi(str.download_count.c_str()));
		}
	};
	typedef std::vector<subtitle_info> SubtitleInfoList;
};

class IEventListener
{
public:
	virtual void OnError(std::wstring error_details) = 0;
	virtual void OnSubtitle(IOpenSubtitlesAPI::subtitle_info&) = 0;
	virtual void OnSearchComplete() = 0;
	virtual void OnApiReady(IOpenSubtitlesAPI* api) = 0;
};

class COpenSubtitlesAPIFactory
{
private:
	COpenSubtitlesAPI_Impl *m_impl;
public:
	COpenSubtitlesAPIFactory() :m_impl(NULL){};
    ~COpenSubtitlesAPIFactory();
	IOpenSubtitlesAPI* Create(const std::wstring &user, const std::wstring &pass, const std::wstring &user_agent, IEventListener*);
	void Release();
};