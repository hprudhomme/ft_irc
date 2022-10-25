#include "ft_irc.hpp"

PartCommand::PartCommand(Server *server) : Command(server) {}

PartCommand::~PartCommand() {}

// format: PART <channel>{,<channel>} [<reason>]
void PartCommand::execute(Client *client, std::vector<std::string> arguments) {

	if (arguments.empty()) {
		client->reply(ERR_NEEDMOREPARAMS(client->getNickName(), "PART"));
		return;
	}

	std::string name = arguments[0];

	Channel *channel = _server->getChannel(name);
	if (!channel) {
		client->reply(ERR_NOSUCHCHANNEL(client->getNickName(), name));
		return;
	}

	std::vector<Channel *> chans = client->getUserChans();
	std::vector<Channel *>:: iterator it = chans.begin();

	Channel *chan;
	while (it != chans.end())
	{
		chan = it.operator*();
		if (chan->getName() == name)
			break ;
		++it;
	}
	if (it == chans.end())
	{
		client->reply(ERR_NOTONCHANNEL(client->getNickName(), name));
	 	return;
	}

	client->leave(chan);
}
