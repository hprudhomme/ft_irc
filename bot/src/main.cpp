/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:12:52 by ocartier          #+#    #+#             */
/*   Updated: 2022/11/29 14:56:15 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "ircbot.hpp"

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <vector>
#include <stdlib.h>

std::vector<std::string> jokes;

std::string getAnswer(std::string message, std::string sender)
{
	std::cout << "Message received: " << message << std::endl;
	if (message == "help")
		return ("Commands available: help, ping, pong, hi, dice, joke");
	else if (message == "ping")
		return ("pong?");
	else if (message == "pong")
		return ("ping?");
	else if (message == "hi")
		return ("Hi " + sender + "! Glad to see you!");
	else if (message == "dice")
		return ("You rolled a " + intToString(randInt(1, 6)));
	else if (message == "joke")
		return (jokes[randInt(0, jokes.size() - 1)]);
	return ("");
}

int main(int argc, char **argv)
{
	if (argc !=	4)
	{
		std::cout << "Usage: " << argv[0] << " <host> <port> <password>" << std::endl;
		return (1);
	}
	else if (!containsOnlyDigits(argv[2]))
	{
		std::cout << "Port must be a number" << std::endl;
		return (1);
	}

	srand(time(0));
	// Init jokes
	jokes.push_back("Why does C get all the girls, and Java gets none? Because C doesn't treat them as objects.");
	jokes.push_back("Two programmers are talking about their social life, and one says: The only date I get is the Java Update.");
	jokes.push_back("Why do Java developers wear glasses? Because they don't C#!");
	jokes.push_back("I had a problem. I used Java. Now, I have a ProblemFactory.");
	jokes.push_back("Java and C are telling jokes to each other. C writes something on the blackboard, and asks Java: Do you get the reference? Java didn't.");
	jokes.push_back("C programmers never die. They are just <cast> into VOID.");

	std::string host = argv[1];
	if (host == "localhost")
		host = "127.0.0.1";
	Bot bot(host, atoi(argv[2]), argv[3], "ircbot", "ircbot", "IRC Bot", "#bot");

	if (!bot.connect())
	{
		std::cout << "Connection failed" << std::endl;
		return (1);
	}
	if (!bot.listen(getAnswer))
		return (1);
}
