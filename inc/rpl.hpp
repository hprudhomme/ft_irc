#pragma once

#include <iostream>

// NUMERIC REPLIES
#define RPL_WELCOME(source)						"001 " + source + " :Welcome " + source + " to the ft_irc network"
#define RPL_NAMREPLY(source, channel, users)	"353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)			"366 " + source + " " + channel + " :End of /NAMES list."

#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason
