/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 13:40:09 by ocartier          #+#    #+#             */
/*   Updated: 2022/11/15 08:54:55 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CLASS_H
# define SERVER_CLASS_H

# include <vector>
# include <iostream>

# include <stdio.h>
# include <string>
# include <fcntl.h>
# include <stdlib.h>
# include <errno.h>
# include <unistd.h>   //close
# include <arpa/inet.h>    //close
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <sys/poll.h>
# include <netinet/in.h>
# include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

# define DEFAULT_PORT 6667
# define DEFAULT_WELCOME_MESSAGE "ft_irc, by ocartier and hprudhom\n"
# define DEFAULT_SERVER_NAME "irc.42.fr"

class Client;

class Server {
private:
	const int				_port;
	std::string 			_password;
	std::vector<Client *>	_clients;
	std::vector<Channel *>	_channels;
	std::string				_welcome_message;
	std::string				_server_name; // TODO: le récupérer de l'utilisateur

	int						_server_socket;
	struct pollfd			*_clients_fds;

	void					_waitActivity(void);
	void					_constructFds(void);
	void 					_setNonBlocking(int fd);
	void					_acceptConnection(void);
	void 					_receiveData(Client *client);
	void					_handleMessage(std::string const message, Client *client);

public:
	Server(void);
	Server(int port);
	Server(int port, std::string welcome_message);
	// Server(const Server &src);
	~Server(void);
	// Server &operator =(const Server &src);

	// Server
	void			listen(void);
	ssize_t			send(std::string const message, int const client_fd) const;
	void			broadcast(std::string const message) const;
	void			broadcast(std::string const message, int const exclude_fd) const;
	void			broadcastChannel(std::string const message, Channel const *channel) const;
	void			broadcastChannel(std::string message, int exclude_fd, Channel const *channel) const;
	std::string&	getPassword() { return _password; };
	std::string&	getServerName() { return _server_name; };
	// Client
	std::vector<std::string>	getNickNames();
	std::vector<Client *> 		getServClients() const { return _clients; };
	int							addClient(int const fd, std::string const ip, int const port);
	int							delClient(int fd);
	Client*						getClient(int fd);
	Client*						getClient(const std::string &nickname);
	// Channel
	Channel*					getChannel(std::string const &name);
	std::vector<Channel *>		getServChannels() const { return _channels; };
	Channel* 					createChannel(std::string const &name, std::string const &password, Client *client);
};

#endif
