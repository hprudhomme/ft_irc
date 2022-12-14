#include "ft_irc.hpp"

PingCommand::PingCommand(Server *server) : Command(server) {}

PingCommand::~PingCommand() {}

void PingCommand::execute(Client *client, std::vector<std::string> arguments)
{

	if (arguments.empty()) {
		client->reply(ERR_NEEDMOREPARAMS(client->getNickName(), "PING"));
		return;
	}

	client->write(RPL_PING(client->getPrefix(), arguments.at(0)));
}
