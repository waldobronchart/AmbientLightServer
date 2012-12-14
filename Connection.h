#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "NetIncomingMessage.h"
#include "NetOutgoingMessage.h"

using boost::asio::ip::tcp;
using namespace std;

class Connection
{
public:
	Connection(boost::asio::io_service& ioService);
	~Connection(void);

	tcp::socket& Socket();

	void StartReading();
	void Send(NetOutgoingMessage* msg);

private:
	void HandleReadHeader(const boost::system::error_code& error);
	void HandleReadBody(const boost::system::error_code& error);
	void HandleSend(NetOutgoingMessage* msg, const boost::system::error_code& error);

	tcp::socket m_socket;
	NetIncomingMessage m_incomingMsg;
};