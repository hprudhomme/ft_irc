#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <map>

#include "Client.hpp"
#include "Channel.hpp"

class Server
{
private:
	bool					_running;
	int						_sock;
	std::string const 		_host;
	std::string const 		_port;
	std::string const 		_password;

	std::map<int, Client *> _clients;
	std::vector<Channel *> 	_channels;
public:
	Server(std::string const &port, std::string const &password);
	~Server();

	std::string const &getPassword();
	std::string const &getPort();

	int newSocket();
};
