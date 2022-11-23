#include "ft_irc.hpp"

WhoCommand::WhoCommand(Server *server) : Command(server) {}

WhoCommand::~WhoCommand() {}

void WhoCommand::execute(Client *client, std::vector<std::string> arguments)
{
	// if (arguments.empty())
	// {
	// 	client->reply(ERR_NEEDMOREPARAMS(client->getNickName(), "WHO"));
	// 	return;
	// }

    if (arguments.empty())
	{
        std::string users;
        std::vector<std::string> nicknames = _server->getNickNames();
        for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); it++)
            users.append(*it + " ");

        client->reply(RPL_WHOREPLY(client->getNickName(), users));
        client->reply(RPL_ENDOFWHO(client->getNickName()));
        return ;
	}
    else if (arguments.size() == 1)
    {
        if (arguments[0].at(0) == '#')
        {
            std::string users;
            Channel *channel = _server->getChannel(arguments[0]);
            std::vector<std::string> nicknames = channel->getNickNames();
            for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); it++)
                users.append(*it + " ");

            client->reply(RPL_WHOREPLY(client->getNickName(), users));
            client->reply(RPL_ENDOFWHO(client->getNickName()));
            return ;
        }
    }

    return ;
}
