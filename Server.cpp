#include "Server.hpp"

Server::Server(std::string const &port, std::string const &password)
				: _running(1), _host("127.0.0.1"), _port(port), _password(password)
{
	_sock = newSocket();
}
Server::~Server() {}

std::string const &Server::getPassword() { return this->_password; }
std::string const &Server::getPort() { return this->_port; }

void	Server::start() {}

int 	Server::newSocket()
{
	struct sockaddr_in address;

    // Creating socket file descriptor
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
		throw std::runtime_error("Error while opening socket.");

    // Forcefully attaching socket to the port 8080
	int val = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &val,sizeof(val)))
		throw std::runtime_error("Error while setting socket options.");

	if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(std::stoi(_port));

    // Forcefully attaching socket to the port 8080
    if (bind(sock_fd, (struct sockaddr*) &address, sizeof(address)) < 0)
		throw std::runtime_error("Error while binding socket.");
    if (listen(sock_fd, 1000) < 0)
		throw std::runtime_error("Error while listening on socket.");
    return sock_fd;
}
