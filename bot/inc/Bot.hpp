/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 13:54:26 by ocartier          #+#    #+#             */
/*   Updated: 2022/11/29 12:46:08 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_CLASS_HPP
# define BOT_CLASS_HPP

# include <string>
# include <iostream>
# include <vector>

class Bot {
private:
	int 		_server_socket;
	std::string _server_address;
	int 		_server_port;
	std::string _password;

	std::string _nickname;
	std::string _username;
	std::string _realname;
	std::string _channel;

	bool		_isPrivateMessage(std::vector<std::string> splitted);
	std::string _getMessage(std::string str);
	std::string _extractSender(std::string str);
	std::string _extractChannel(std::string message);

public:
	Bot(std::string const &server_address, int server_port, std::string const &password, std::string const &nickname, std::string const &username, std::string const &realname, std::string const &channel);
	~Bot(void);

	bool connect(void);
	bool disconnect(void);
	bool listen(std::string (*getAnswer)(std::string message, std::string sender));
	bool send(std::string const &message);
	bool auth(bool sendPass, bool sendNick, bool sendUser, bool sendJoin);
};

#endif
