/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 13:40:09 by ocartier          #+#    #+#             */
/*   Updated: 2022/10/23 12:05:44 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CLASS_H
# define SERVER_CLASS_H

# include <vector>
#include <iostream>

#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

# define DEFAULT_PORT 6697
# define DEFAULT_WELCOME_MESSAGE "ft_irc, by ocartier and hprudhom\n"

class Client;

class Server {
private:
	const int			_port;
	std::string 		_password;
	std::vector<Client>	_clients;
	std::vector<Channel *>	_channels;
	std::string			_welcome_message;

	int					_server_socket;
	struct pollfd		*_clients_fds;

	void				_waitActivity(void);
	void				_constructFds(void);
	void 				_setNonBlocking(int fd);
	void				_acceptConnection(void);
	void 				_receiveData(Client *client);
	void				_handleMessage(std::string const message, Client client);

public:
	Server(void);
	Server(int port);
	Server(int port, std::string welcome_message);
	// Server(const Server &src);
	~Server(void);
	// Server &operator =(const Server &src);

	Channel* 			getChannel(std::string const &name);
	Client 	 			*getClient(const std::string &nickname);
	std::string const 	&getPassword() { return _password; };

	Channel* createChannel(std::string const &name, std::string const &password, Client *client);

	void	listen(void);
	int		addClient(int const fd, std::string const ip, int const port);
	int		delClient(int fd);
	Client	*getClient(int fd);
	ssize_t	send(std::string const message, int const client_fd) const;
	void	broadcast(std::string const message) const;
	void	broadcastExclude(std::string const message, int const exclude_fd) const;
};

#endif
