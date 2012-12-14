#pragma once

#include "NetIncomingMessage.h"
#include <string>
using namespace std;

class NetOutgoingMessage
{
public:
	NetOutgoingMessage(short messageType);
	~NetOutgoingMessage();

	size_t Length() const { return m_length; }
	short Type() const { return m_messageType; }
	char* Data() { return m_data; }
	void SetData(const char* data);

private:
	short m_messageType;
	char* m_data;
	size_t m_length;
	size_t m_writePosition;
};