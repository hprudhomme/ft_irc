/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 13:40:01 by ocartier          #+#    #+#             */
/*   Updated: 2022/10/25 22:27:58 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Server::Server(void): _port(DEFAULT_PORT), _welcome_message(DEFAULT_WELCOME_MESSAGE), _clients_fds(NULL)
{

}

Server::Server(int port): _port(port), _welcome_message(DEFAULT_WELCOME_MESSAGE), _clients_fds(NULL)
{
	// TODO: check that port is ok, throw exception
}

Server::Server(int port, std::string welcome_message): _port(port), _welcome_message(welcome_message), _clients_fds(NULL)
{
	// TODO: check that port is ok, throw exception
	// If message doesn't end with a \n, add one
}

Server::~Server(void)
{

}

void	Server::listen(void)
{
	struct sockaddr_in6 address;
	int opt = 1;

	//create a master socket
	this->_server_socket = socket(AF_INET6, SOCK_STREAM , IPPROTO_TCP);
	if(this->_server_socket == 0)
	{
		std::cout << "Error: Socket creation failed." << std::endl;
		return;
	}

	//set master socket to allow multiple connections
	if(setsockopt(this->_server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		std::cout << "Error: Can't set socket options." << std::endl;
		return;
	}

	this->_setNonBlocking(this->_server_socket);

	//type of socket created
	const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
	address.sin6_family = AF_INET6;
	address.sin6_addr = in6addr_any;
	address.sin6_port = htons(this->_port);


	if (bind(this->_server_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		std::cout << "Error: Can't bind socket." << std::endl;
		close(this->_server_socket);
		return;
	}

	std::cout << "Starting ft_irc on port " << this->_port << std::endl;

	// listen
	if (::listen(this->_server_socket, 32) < 0)
	{
		std::cout << "Error: Can't listen on socket." << std::endl;
		return;
	}

	std::cout << "Waiting for connections ..." << std::endl;
	this->_constructFds();
	while (42)
		this->_waitActivity();
}

void	Server::_waitActivity(void)
{
	// wait for an activity in a socket
	int rc = poll(this->_clients_fds, this->_clients.size() + 1, -1);
	if (rc < 0)
		std::cout << "Error: Can't look for socket(s) activity." << std::endl;

	// loop in every client socket for a connection
	for(unsigned long i=0; i < this->_clients.size() + 1; i++)
	{
		// revents will be != 0 if there is an activity on the socket
		if(this->_clients_fds[i].revents == 0)
			continue;

		// if something append in the master socket, it's an incomming connection
		if (this->_clients_fds[i].fd == this->_server_socket)
			this->_acceptConnection();
		else if (i > 0) // should be always true, because the first socket is the server socket
		{
			Client *client = this->_clients[i - 1];
			this->_receiveData(client);
		}
	}
}

void	Server::_acceptConnection(void)
{
	int	socket;

	do {
		struct sockaddr_in6 address;
		int addrlen;

		socket = accept(this->_server_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		if (socket < 0)
		{
			if (errno != EWOULDBLOCK)
				std::cout << "Error: Failed to accept connection." << std::endl;
			break;
		}
		this->send(this->_welcome_message, socket);
		this->addClient(socket, ft_inet_ntop6(&address.sin6_addr), ntohs(address.sin6_port));
	} while (socket != -1);
}

void	Server::_receiveData(Client *client)
{
	char	buffer[BUFFER_SIZE + 1];

	do {
		int ret = recv(client->getFD(), buffer, sizeof(buffer), 0);
		if (ret < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cout << "Error: recv() failed for fd " << client->getFD();
				this->delClient(client->getFD());
			}
			// no more data to receive
			break;
		}
		else if (!ret)
		{
			this->delClient(client->getFD());
			break;
		}
		else
		{
			buffer[ret] = '\0';
			std::string buff = buffer;

			if (buff.at(buff.size() - 1) == '\n') {
				this->_handleMessage(client->getPartialRecv() + buff, *client);
				client->setPartialRecv("");
			}
			else
			{
				client->setPartialRecv(client->getPartialRecv() + buff);
				if (DEBUG)
					std::cout << "partial recv(" << client->getFD() << "): " << buff << std::endl;
			}
		}
	} while(TRUE);
}

void	Server::_setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		flags = 0;

	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		std::cout << "Error: Can't set socket to non-blocking." << std::endl;
		if (fd != this->_server_socket)
			this->delClient(fd);
	}
}

ssize_t	Server::send(std::string message, int client_fd) const
{
	ssize_t	sent_size = ::send(client_fd, message.c_str(), message.length(), 0);
	if(sent_size != (ssize_t) message.length())
		std::cout << "Error: The message has not been sent entirely." << std::endl;
	return (sent_size);
}

void	Server::broadcast(std::string message) const
{
	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		this->send(message, this->_clients[i]->getFD());
	}
}

void	Server::broadcastExclude(std::string message, int exclude_fd) const
{
	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getFD() != exclude_fd)
			this->send(message, this->_clients[i]->getFD());
	}
}

int	Server::addClient(int socket, std::string ip, int port)
{
	this->_clients.push_back(new Client(socket, ip, port));
	this->_setNonBlocking(socket);
	this->_constructFds();
	std::cout << "* New connection {fd: " << socket
		<< ", ip: " << ip
		<< ", port: " << port
		<< "}" << std::endl;
	return this->_clients.size();
}

int	Server::delClient(int socket)
{
	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getFD() == socket)
		{
			close(socket);

			std::cout << "* Closed connection {fd: " << this->_clients[i]->getFD()
				<< ", ip: " << this->_clients[i]->getHostName()
				<< ", port: " << this->_clients[i]->getPort()
				<< "}" << std::endl;

			this->_clients.erase(this->_clients.begin() + i);
			break;
		}
	}
	this->_constructFds();
	return this->_clients.size();
}

Client	*Server::getClient(int fd)
{
	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getFD() == fd)
			return this->_clients[i];
	}
	return NULL;
}

Client *Server::getClient(const std::string &nickname)
{
	std::vector<Client *>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		if ((*it)->getNickName() == nickname)
			return *it;
	}
	return NULL;
}

void	Server::_constructFds(void)
{
	if (this->_clients_fds)
		delete this->_clients_fds;
	this->_clients_fds = new struct pollfd[this->_clients.size() + 1];

	this->_clients_fds[0].fd = this->_server_socket;
	this->_clients_fds[0].events = POLLIN;

	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		this->_clients_fds[i + 1].fd = this->_clients[i]->getFD();
		this->_clients_fds[i + 1].events = POLLIN;
	}
}

Channel *Server::getChannel(const std::string &name)
{
	std::vector<Channel *>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
		if (it.operator*()->getName() == name)
			return it.operator*();

	return NULL;
}

Channel *Server::createChannel(const std::string &name, std::string const &password, Client *client)
{
	Channel *channel = new Channel(name, password, client, this);
	_channels.push_back(channel);

	return channel;
}

void	Server::_handleMessage(std::string const message, Client client)
{
	if (DEBUG)
		std::cout << "recv(" << client.getFD() << "): " << message;

	this->send("You sent: " + message, client.getFD());
	this->broadcastExclude("Someone sent: " + message, client.getFD());
}
