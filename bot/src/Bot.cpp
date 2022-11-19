/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 13:58:32 by ocartier          #+#    #+#             */
/*   Updated: 2022/11/19 15:14:19 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ircbot.hpp"

# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>

Bot::Bot(
	std::string const &server_address,
	int server_port,
	std::string const &nickname,
	std::string const &username,
	std::string const &realname,
	std::string const &channel
):
	_server_address(server_address),
	_server_port(server_port),
	_nickname(nickname),
	_username(username),
	_realname(realname),
	_channel(channel)
{

}

Bot::~Bot(void)
{

}

bool Bot::connect(void)
{
	this->_server_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(this->_server_port);
	hint.sin_addr.s_addr = inet_addr(this->_server_address.c_str());

	if (::connect(this->_server_socket, (sockaddr*)&hint, (socklen_t)(sizeof(hint))) != 0)
		return (false);

	if (!this->send("NICK " + this->_nickname +"\r\nUSER " + this->_username + " 0 * :" + this->_realname + "\r\nJOIN " + this->_channel + "\r\n"))
		return (false);

	return (true);
}

bool Bot::listen(std::string (*getAnswer)(std::string message, std::string sender))
{
	char	buffer[BUFFER_SIZE + 1];

	std::cout << "Listening..." << std::endl;
	do {
		int ret = recv(this->_server_socket, buffer, sizeof(buffer), 0);
		buffer[ret] = '\0';
		std::string buff = buffer;
		if (DEBUG)
			std::cout << "recv: " << buff;

		std::string message = this->_getMessage(buff);
		if (message != "")
		{
			std::string answer = getAnswer(message, this->_extractSender(buff));

			if (!answer.empty())
				this->send("PRIVMSG " + this->_extractChannel(buff) + " :" + answer + "\r\n");
		}

	} while (42);
}

bool Bot::send(std::string const &message)
{
	ssize_t	sent_size = ::send(this->_server_socket, message.c_str(), message.size(), 0);
	if (DEBUG)
		std::cout << "send: " << message;

	if(sent_size != (ssize_t) message.length())
	{
		std::cout << "Error: The message has not been sent entirely." << std::endl;
		std::cout << "sent_size: " << sent_size << std::endl;
		return (false);
	}
	return (true);
}

bool Bot::_isPrivateMessage(std::vector<std::string> splitted)
{
	if (splitted.size() < 4)
		return (false);
	else if (splitted[0][0] != ':')
		return (false);
	else if (splitted[1] != "PRIVMSG")
		return (false);
	return (true);
}

std::string Bot::_getMessage(std::string str)
{
	std::vector<std::string> splitted = split(str, " ");
	if (!this->_isPrivateMessage(splitted))
		return ("");

	std::string message = splitted[3];
	for (unsigned long i = 4; i < splitted.size(); i++)
		message += " " + splitted[i];
	// remove first :
	message = message.substr(1, message.size() - 1);
	// remove last \r\n
	if (message[message.size() - 1] == '\n' && message[message.size() - 2] == '\r')
		message = message.substr(0, message.size() - 2);
	// remove last \n
	if (message[message.size() - 1] == '\n')
		message = message.substr(0, message.size() - 1);
	// remove last space
	if (message[message.size() - 1] == ' ')
		message = message.substr(0, message.size() - 1);
	return (message);
}

std::string Bot::_extractSender(std::string str)
{
	std::string sender = str;
	// remove before first : and after fisrt !
	sender = sender.substr(1, sender.find("!") - 1);
	return (sender);
}

std::string Bot::_extractChannel(std::string message)
{
	std::vector<std::string> splitted = split(message, " ");
	if (!this->_isPrivateMessage(splitted))
		return ("");

	if (splitted[2] != this->_nickname)
		return (splitted[2]);
	return (this->_extractSender(message));
}
