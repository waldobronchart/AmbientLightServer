#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "NetOutgoingMessage.h"
#include "Logging.h"

NetOutgoingMessage::NetOutgoingMessage(short messageType)
	: m_messageType(messageType), m_data(0), m_length(0)
{

}

NetOutgoingMessage::~NetOutgoingMessage()
{
	delete m_data; m_data = 0;
	delete[] m_data;
}

void NetOutgoingMessage::SetData(const char* data)
{
	size_t dataLength = strlen(data);

	// Set m_data size
	if (dataLength != m_length)
	{
		delete m_data; m_data = 0;
		delete[] m_data;
		
		m_length = dataLength + NetIncomingMessage::HEADER_LENGTH;
		m_data = new char[m_length];
	}

	// Write header
	char header[NetIncomingMessage::HEADER_LENGTH + 1] = "";
	sprintf(header, "%4d,%10d", m_messageType, dataLength);
	memcpy(m_data, header, NetIncomingMessage::HEADER_LENGTH);
	
	// Copy data into buffer
	memcpy(m_data + NetIncomingMessage::HEADER_LENGTH, data, dataLength);
}