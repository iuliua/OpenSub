#pragma once
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
