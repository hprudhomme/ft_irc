#include "ft_irc.hpp"

NoticeCommand::NoticeCommand(Server *server) : Command(server) {};

NoticeCommand::~NoticeCommand() {};

void NoticeCommand::execute(Client *client, std::vector<std::string> arguments) {

	if (arguments.size() < 2 || arguments[0].empty() || arguments[1].empty()) {
		//client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "NOTICE"));
		return;
	}

	std::string target = arguments.at(0);
	std::string message;

	for (std::vector<std::string>::iterator it = arguments.begin() + 1; it != arguments.end(); it++) {
		message.append(*it + " ");
	}

	message = message.at(0) == ':' ? message.substr(1) : message;

	if (target.at(0) == '#')
    {
        std::vector<Channel *> client_chans = client->getUserChans();
        std::vector<Channel *>:: iterator it = client_chans.begin();

        Channel *chan;
		std::string chan_name;
        while (it != client_chans.end())
        {
            chan = it.operator*();
            if (chan->getName() == target)
                break ;
            ++it;
        }
        if (it == client_chans.end())
        {
            // client->reply(ERR_NOTONCHANNEL(client->getNickName(), chan->getName()));
            return;
        }

		chan->broadcast(RPL_NOTICE(client->getPrefix(), target, message), client);
		return;
	}

	Client *dest = _server->getClient(target);
	if (!dest) 
    {
		//client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
		return;
	}
	dest->write(RPL_NOTICE(client->getPrefix(), target, message));
}