#pragma once
class Buffer
{
public:
	LPSTR buffer_in;
	DWORD buffer_in_total;
	DWORD buffer_in_size;
public:
	Buffer()
	{
		buffer_in = NULL;
		buffer_in_total = 0;
		buffer_in_size = 0;
	}
	~Buffer()
	{
		if (buffer_in)
			delete[] buffer_in;
		buffer_in_total = 0;
		buffer_in_size = 0;
	}
	bool Allocate(size_t size)
	{
		if (size > buffer_in_size)
		{
			if (buffer_in)
			{
				delete[] buffer_in;
				buffer_in_total = 0;
				buffer_in_size = 0;
			}
			buffer_in = new(std::nothrow)char[size];
			if (!buffer_in)
			{
				buffer_in_total = 0;
				buffer_in_size = 0;
				return false;
			}
			buffer_in_size = size;
		}
		buffer_in_total = 0;
		return true;
	}
};