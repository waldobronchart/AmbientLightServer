#include "Server.h"
#include "Logging.h"
#include <boost/bind.hpp>


Server::Server(boost::asio::io_service& ioService, const tcp::endpoint& endpoint)
	: m_ioService(ioService), m_acceptor(ioService, endpoint)
{
	StartAccept();
}

Server::~Server(void)
{
}

void Server::StartAccept()
{
	LOG_DEBUG("Server: accepting new connections...");

	Connection* connection = new Connection(m_ioService);
	m_acceptor.async_accept(connection->Socket(), 
		boost::bind(&Server::HandleAccept, this, connection, boost::asio::placeholders::error));
}

void Server::HandleAccept(Connection* connection, const boost::system::error_code& error)
{
	tcp::socket& socket = connection->Socket();
	tcp::endpoint endpoint = socket.local_endpoint();
	LOG_INFO("Server: " << "connection accepted from ??" << endpoint.address() << ":" << endpoint.port());

	connection->StartReading();

	// Accept new connections
	StartAccept();
}