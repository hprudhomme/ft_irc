#pragma once

#include <iostream>

//ERROR REPLIES
#define ERR_NEEDMOREPARAMS(source, command)				"461 " + source + " " + command + " :Not enough parameters"
#define ERR_NOTONCHANNEL(source, channel)				"442 " + source + " " + channel + " :You're not on that channel"
#define ERR_USERNOTINCHANNEL(source, nickname, channel)	"441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_CHANNELISFULL(source, channel)				"471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_BADCHANNELKEY(source, channel)				"475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_INVITONLYCHAN(source, channel)				"473 " + source + " " + channel + " :Cannot join channel (+i)"
#define ERR_NOSUCHCHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel"
#define ERR_CHANOPRIVSNEEDED(source, channel)			"482 " + source + " " + channel + " :You're not channel operator"
#define ERR_NONICKNAMEGIVEN(source)						"431 " + source + " :Nickname not given"
#define ERR_NICKNAMEINUSE(source)						"433 " + source + " " + source  + " :Nickname is already in use"
#define ERR_ALREADYREGISTERED(source)					"462 " + source + " :You may not reregister"
#define ERR_PASSWDMISMATCH(source)						"464 " + source + " :Password incorrect"
#define ERR_NOTREGISTERED(source)						"451 " + source + " :You have not registered"
#define ERR_UNKNOWNCOMMAND(source, command)				"421 " + source + " " + command + " :Unknown command"
#define ERR_USERONCHANNEL(source, target, channel)		"443 " + source + " " + target + " " + channel + " :is already on channel"
#define ERR_NOSUCHNICK(source, name)					"401 " + source + " " + name + " :No such nick/channel"
#define ERR_INVITEONLYCHAN(source, channel)				"473 " + source + " " + channel + " :Cannot join channel (+i)"

// NUMERIC REPLIES
#define RPL_WELCOME(source)								"001 " + source + " :Welcome " + source + " to the ft_irc network"
#define RPL_NAMREPLY(source, channel, users)			"353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)					"366 " + source + " " + channel + " :End of /NAMES list."
#define RPL_INVITING(source, channel, target)			"341 " + source + " INVIT " + channel + " " + target

#define RPL_NOTOPIC(source, channel)					"331 " + source + " " + channel + " :No topic is set"
#define RPL_TOPIC(source, channel, topic)				"332 " + source + " " + channel + " :" + topic

// COMMAND REPLIES

#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args
#define RPL_PING(source, token)						":" + source + " PONG :" + token
#define RPL_QUIT(source, message)					":" + source + " QUIT :Quit: " + message
#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)					":" + source + " PART :" + channel
#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason
#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message
