struct XMLMemoryWriter : pugi::xml_writer
{
	LPSTR             xml_data;
	DWORD             xml_buffer_size;
	DWORD             xml_data_size;
	virtual void write(const void* data, size_t size)
	{
		if (xml_buffer_size < size)
		{
			if (xml_data)
			{
				delete[] xml_data;
				xml_data = NULL;
				xml_buffer_size = 0;
				xml_data_size = 0;
			}
			xml_data = new(std::nothrow) char[size];
			if (!xml_data)
			{
				xml_buffer_size = 0;
				xml_data_size = 0;
				return;
			}
			xml_buffer_size = size;
		}
		memcpy(xml_data, data, size);
		xml_data_size = size;
	}
	XMLMemoryWriter()
	{
		xml_data = NULL;
		xml_data_size = 0;
		xml_buffer_size = 0;
	}
	~XMLMemoryWriter()
	{
		if (xml_data)
			delete[] xml_data;
	}
};