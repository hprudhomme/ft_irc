/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:12:52 by ocartier          #+#    #+#             */
/*   Updated: 2022/11/19 16:56:41 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <vector>

#include "Bot.hpp"

std::string getAnswer(std::string message, std::string sender)
{
	if (message == "help")
		return ("Commands available: help, ping, pong, hi");
	else if (message == "ping")
		return ("pong?");
	else if (message == "pong")
		return ("ping?");
	else if (message == "hi")
		return ("Hi " + sender + "! Glad to see you!");
	return ("");
}

int main()
{
	Bot bot("127.0.0.1", 6667, "ircbot", "ircbot", "IRC Bot", "#bot");

	if (!bot.connect())
	{
		std::cout << "Connection failed" << std::endl;
		return (1);
	}
	if (!bot.listen(getAnswer))
		return (1);
}
