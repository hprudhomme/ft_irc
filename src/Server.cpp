/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 13:40:01 by ocartier          #+#    #+#             */
/*   Updated: 2022/10/21 15:42:07 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define TRUE 1
#define FALSE 0

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
	struct sockaddr_in address;
	int opt = 1;

	//create a master socket
	this->_server_socket = socket(AF_INET , SOCK_STREAM , 0);
	if(this->_server_socket == 0)
	{
		std::cout << "Error: Socket creation failed." << std::endl;
		exit(EXIT_FAILURE);
	}

	//fcntl(this->_server_socket, F_SETFL, O_NONBLOCK);

	//set master socket to allow multiple connections
	if(setsockopt(this->_server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		std::cout << "Error: Can't set socket options." << std::endl;
		exit(EXIT_FAILURE);
	}

	// Set socket to be nonblocking. (clients sockets will inherit)
	int rc = ioctl(this->_server_socket, FIONBIO, (char *)&opt);
	if (rc < 0)
	{
		perror("ioctl() failed");
		close(this->_server_socket);
		exit(-1);
	}

	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->_port);
	// address.sin6_family = AF_INET6;
	// address.sin6_addr.s_addr = INADDR_ANY;
	// address.sin6_port = htons(this->_port);

	if (bind(this->_server_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		std::cout << "Error: Can't bind socket." << std::endl;
		close(this->_server_socket);
		exit(EXIT_FAILURE);
	}

	std::cout << "Starting ft_irc on port " << ntohs(address.sin_port) << std::endl;

	// listen
	if (::listen(this->_server_socket, 32) < 0)
	{
		std::cout << "Error: Can't listen on socket." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Waiting for connections ..." << std::endl;
	this->_constructFds();
	while (42)
		this->_waitActivity();
}

void	Server::_waitActivity(void)
{
	char	buffer[1025];
	int		socket;

	int		close_conn;

	// wait for an activity in a socket
	int rc = poll(this->_clients_fds, this->_clients.size() + 1, -1);
	if (rc < 0)
	{
		perror("  poll() failed");
	}

	// loop in every client socket for a connection
	for(unsigned long i=0; i < this->_clients.size() + 1; i++)
	{
		if(this->_clients_fds[i].revents == 0)
			continue;

		// if something append in the master socket, it's an incomming connection
		if (this->_clients_fds[i].fd == this->_server_socket)
		{
			do {
				struct sockaddr_in address;
				int addrlen;

				socket = accept(this->_server_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
				if (socket < 0)
				{
					if (errno != EWOULDBLOCK)
						perror("  accept() failed");
					break;
				}
				this->send(this->_welcome_message, socket);
				this->addClient(socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
			} while (socket != -1);
		}
		else
		{
			close_conn = FALSE;
			do {
				int ret = recv(this->_clients_fds[i].fd, buffer, sizeof(buffer), 0);
				if (ret < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						perror("  recv() failed");
						close_conn = TRUE;
					}
					break;
				}

				if (ret == 0)
				{
					std::cout << "  Connection closed" << std::endl;
					close_conn = TRUE;
					break;
				}

				buffer[ret] = '\0';
				std::string buff = buffer;

				std::cout << "recv(" << this->_clients_fds[i].fd << "): " << buff;

				this->send("You sent: " + buff, this->_clients_fds[i].fd);
				this->broadcastExclude("Someone sent: " + buff, this->_clients_fds[i].fd);
			} while(TRUE);

			if (close_conn)
				this->delClient(this->_clients_fds[i].fd);
		}
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
		this->send(message, this->_clients[i]);
	}
}

void	Server::broadcastExclude(std::string message, int exclude_fd) const
{
	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i] != exclude_fd)
			this->send(message, this->_clients[i]);
	}
}

int	Server::addClient(int socket, std::string ip, int port)
{
	this->_clients.push_back(socket);
	this->_constructFds();
	std::cout << "* New connection {fd: " << socket
		<< ", ip: " << ip
		<< ", port: " << port
		<< "}" << std::endl;
	return this->_clients.size();
}

int	Server::delClient(int socket)
{
	// getpeername forbidden, will be removed when data from accept() will be stored in client class
	struct sockaddr_in address;
	int addrlen;
	getpeername(socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	std::string ip = inet_ntoa(address.sin_addr);
	int port = ntohs(address.sin_port);
	/////////////////////////////////////////////////////////////////////////////////////////////////

	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i] == socket)
		{
			close(socket);
			this->_clients.erase(this->_clients.begin() + i);

			break;
		}
	}
	this->_constructFds();

	std::cout << "* Closed connection {fd: " << socket
		<< ", ip: " << ip
		<< ", port: " << port
		<< "}" << std::endl;
	return this->_clients.size();
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
		this->_clients_fds[i + 1].fd = this->_clients[i];
		this->_clients_fds[i + 1].events = POLLIN;
	}
}