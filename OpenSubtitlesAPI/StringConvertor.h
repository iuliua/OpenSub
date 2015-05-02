#pragma once

class CConvertor
{
private:
	LPSTR  m_buffer_char;
	size_t m_buffer_char_size;
	LPWSTR m_buffer_wchar;
	size_t m_buffer_wchar_size;
public:
	CConvertor()
	{
		m_buffer_wchar_size = 0;
		m_buffer_char_size = 0;
		m_buffer_wchar = NULL;
		m_buffer_char = NULL;
	}
	~CConvertor()
	{
		if (m_buffer_char)
		{
			delete[] m_buffer_char;
			m_buffer_char = NULL;
		}
		if (m_buffer_wchar)
		{
			delete[] m_buffer_wchar;
			m_buffer_wchar = NULL;
		}
	}
	LPCSTR  ToUTF8(LPCWSTR str)
	{
		if (wcslen(str) > 0 && str[wcslen(str)] != L'\0')
			return NULL;
		size_t length = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
		if (m_buffer_char_size < length)
		{
			if (m_buffer_char)
				delete[] m_buffer_char;
			m_buffer_char = new char[length];
			if (!m_buffer_char)
			{
				m_buffer_char_size = 0;
				return "";
			}
			m_buffer_char_size = length;
		}
		WideCharToMultiByte(CP_UTF8, 0, str, -1, m_buffer_char, length, 0, 0);
		return m_buffer_char;
	}
	LPCWSTR ToWCHAR(LPCSTR str)
	{
		if (strlen(str) > 0 && str[strlen(str)] != L'\0')
			return NULL;
		size_t length = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
		if (m_buffer_wchar_size < length)
		{
			if (m_buffer_wchar)
				delete[] m_buffer_wchar;
			m_buffer_wchar = new wchar_t[length];
			if (!m_buffer_wchar)
			{
				m_buffer_wchar_size = 0;
				return L"";
			}
			m_buffer_wchar_size = length;
		}
		MultiByteToWideChar(CP_UTF8, 0, str, -1, m_buffer_wchar, length);
		return m_buffer_wchar;
	}
};