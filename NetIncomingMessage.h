#pragma once

#include <string>
#include <jansson.h>
using namespace std;

class NetIncomingMessage
{
public:
	NetIncomingMessage();
	~NetIncomingMessage();

	enum { HEADER_LENGTH = 15 };

	void DecodeHeader();
	size_t BodyLength() const { return m_bodyLength; }
	size_t Length() const { return m_bodyLength + HEADER_LENGTH; }
	short Type() const { return m_messageType; }

	char* Header() { return m_header; }
	char* Body() { return m_body; }

private:
	short m_messageType;
	char* m_header;
	char* m_body;
	size_t m_bodyLength;
	size_t m_readPosition;
};