#include <boost/assert.hpp>
#include "NetIncomingMessage.h"
#include "Logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

NetIncomingMessage::NetIncomingMessage() :
	m_messageType(0), m_body(0), m_bodyLength(0), m_readPosition(0)
{
	m_header = new char[HEADER_LENGTH+1];
	m_header[HEADER_LENGTH] = '\0';
}

NetIncomingMessage::~NetIncomingMessage()
{
	delete m_header; m_header = 0;
	delete[] m_header;
	delete m_body; m_body = 0;
	delete[] m_body;
}

void NetIncomingMessage::DecodeHeader()
{
	// Header format:
	//  0005,0000564538

	// Read message type (4chars)
	char messageTypeData[5] = "";
	strncat(messageTypeData, m_header, 4);
	m_messageType = atoi(messageTypeData);

	// Read message length (10chars)
	char bodyLengthData[11] = "";
	strncat(bodyLengthData, m_header + 5, 10);
	size_t bodyLength = atoi(bodyLengthData);

	// Set m_body size
	if (bodyLength != m_bodyLength)
	{
		delete m_body; m_body = 0;
		delete[] m_body;

		m_bodyLength = bodyLength;
		m_body = new char[m_bodyLength];
	}
}