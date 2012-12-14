#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "Connection.h"

using boost::asio::ip::tcp;

class Server
{
public:
	Server(boost::asio::io_service& ioService, const tcp::endpoint& endpoint);
	~Server(void);

private:
	void StartAccept();
	void HandleAccept(Connection* connection, const boost::system::error_code& error);

	boost::asio::io_service& m_ioService;
	tcp::acceptor m_acceptor;
};

