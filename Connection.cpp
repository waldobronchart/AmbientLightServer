
#include "Connection.h"
#include "Logging.h"

#include "MsgGenericRequest.h"
#include "MsgSetBounds.h"
#include "MsgSetColorSettings.h"

Connection::Connection(boost::asio::io_service& ioService) : m_socket(ioService)
{
}

Connection::~Connection(void)
{
}

tcp::socket& Connection::Socket()
{
	return m_socket;
}

void Connection::StartReading()
{
	// Read header
	boost::asio::async_read(m_socket, 
		boost::asio::buffer(m_incomingMsg.Header(), NetIncomingMessage::HEADER_LENGTH),
		boost::bind(&Connection::HandleReadHeader, this, boost::asio::placeholders::error));
}

void Connection::HandleReadHeader(const boost::system::error_code& error)
{
	if (!error)
	{
		m_incomingMsg.DecodeHeader();
		LOG_INFO("Connection: incoming message (Type=" << m_incomingMsg.Type() << " BodyLength=" << m_incomingMsg.BodyLength() << "bytes)");

		// Read body
		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_incomingMsg.Body(), m_incomingMsg.BodyLength()),
			boost::bind(&Connection::HandleReadBody, this, boost::asio::placeholders::error));
	}
	else
	{
		// Close connection, connection lost?
		delete this;
	}
}

void Connection::HandleReadBody(const boost::system::error_code& error)
{
	if (error)
	{
		LOG_ERROR("Connection: failed to read message body - " << error);
		return;
	}

	// Handle messages
	MsgHandlerBase* msgHandler = 0;
	switch (m_incomingMsg.Type())
	{
	case MSG_GENERIC_REQUEST:
		msgHandler = new MsgGenericRequest();
		break;

	case MSG_SET_BOUNDS:
		msgHandler = new MsgSetBounds();
		break;

	case MSG_SET_COLOR_SETTINGS:
		msgHandler = new MsgSetColorSettings();
		break;

	default:
		LOG_WARN("Connection: Unhandled message received (Type=" << m_incomingMsg.Type() << ")");
	}

	// Parse the message
	if (msgHandler != 0)
	{
		msgHandler->Read(this, m_incomingMsg);
		delete msgHandler; msgHandler = 0;
	}

	// Continue reading
	StartReading();
}

void Connection::Send(NetOutgoingMessage* msg)
{
	boost::asio::async_write(m_socket,
		boost::asio::buffer(msg->Data(), msg->Length()),
		boost::bind(&Connection::HandleSend, this, msg, boost::asio::placeholders::error));
}

void Connection::HandleSend(NetOutgoingMessage* msg, const boost::system::error_code& error)
{
	if (error)
	{
		LOG_ERROR("Connection: failed to send message (Type=" << msg->Type() << ", Length=" << msg->Length() << "bytes) - " << error);
	}
	else
	{
		LOG_INFO("Connection: message sent! (Type=" << msg->Type() << "  FullLength=" << msg->Length() << "bytes)");
	}
	
	delete msg;
}