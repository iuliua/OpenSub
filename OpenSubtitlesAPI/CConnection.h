#pragma once

class CConnection
{
private:
	HINTERNET m_connection;
	HINTERNET m_internet;
	HINTERNET m_request;
public:
	CConnection();
	~CConnection();
	bool Connect();
	void Disconnect();
	bool SendRequest(XMLMemoryWriter&,Buffer&,IEventListener*);
};