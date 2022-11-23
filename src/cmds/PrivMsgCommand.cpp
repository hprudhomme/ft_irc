#include "ft_irc.hpp"

PrivMsgCommand::PrivMsgCommand(Server *server) : Command(server) {};

PrivMsgCommand::~PrivMsgCommand() {};

void PrivMsgCommand::execute(Client *client, std::vector<std::string> arguments) {

	if (arguments.size() < 2 || arguments[0].empty() || arguments[1].empty()) {
		client->reply(ERR_NEEDMOREPARAMS(client->getNickName(), "PRIVMSG"));
		return;
	}

	std::string target = arguments.at(0);
	std::string message;

	for (std::vector<std::string>::iterator it = arguments.begin() + 1; it != arguments.end(); it++) {
		message.append(*it + " ");
	}

	message = message.at(0) == ':' ? message.substr(1) : message;

	if (target.at(0) == '#') {

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
            _server->broadcast(ERR_NOTONCHANNEL(client->getNickName(), target));
            return;
        }

		// Channel *channel = client->getChannel();
		// if (!channel) {
		// 	client->reply(ERR_NOSUCHCHANNEL(client->getNickName(), target));
		// 	return;
		// }

		// if (channel->isNoExt()){
		// 	std::vector<std::string>			nicknames(channel->getNicknames());
		// 	std::vector<std::string>::iterator	i;

		// 	for (i = nicknames.begin(); i != nicknames.end(); i++)
		// 		if (*i == client->getNickname())
		// 			break;
		// 	if (i == nicknames.end())
		// 	{
		// 		client->reply(ERR_CANNOTSENDTOCHAN(client->getNickname(), target));
		// 		return;
		// 	}
		// }

		chan->broadcast(RPL_PRIVMSG(client->getPrefix(), target, message), client);
		return;
	}

	Client *dest = _server->getClient(target);
	if (!dest)
    {
		client->reply(ERR_NOSUCHNICK(client->getNickName(), target));
		return;
	}

	dest->write(RPL_PRIVMSG(client->getPrefix(), target, message));
}
