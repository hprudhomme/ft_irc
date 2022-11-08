#include "ft_irc.hpp"

ModeCommand::ModeCommand(Server *server) : Command(server) {}

ModeCommand::~ModeCommand() {}

void ModeCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2 || arguments[0].empty() || arguments[1].empty()) {
		return;
	}

	std::string target = arguments.at(0);

	Channel *channel = _server->getChannel(target); //MODE on clients not implemented
	if (!channel) {
		client->reply(ERR_NOSUCHCHANNEL(client->getNickName(), target));
		return;
	}

	// check if admin
	if (channel->getAdmin() != client) {
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickName(), target));
		return;
	}

	// check if chanop

	std::vector<Client *> opers_chan = channel->getChanOpers();
	std::vector<Client *>:: iterator it_oper = opers_chan.begin();

	while (it_oper != opers_chan.end())
	{
		Client *oper = it_oper.operator*();
		if (oper == client)
			break ;
		++it_oper;
	}
	if (it_oper == opers_chan.end())
	{
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickName(), target));
		return;
	}

	int i = 0;
	int p = 2;
	char c;

	while ((c = arguments[1][i])) {

		char prevC = i > 0 ? arguments[1][i - 1] : '\0';
		bool active = prevC == '+';

		switch (c) {

			case 'i': {
				std::cout << "invit\n";
				channel->setInviteOnly(active);
				channel->broadcast(RPL_MODE(client->getPrefix(), channel->getName(), (active ? "+i" : "-i"), ""));
				break;
			}

			case 'l': {
				std::cout << "maxClient\n";
				channel->setMaxClients(active ? std::stol(arguments[p]) : 0);
				channel->broadcast(RPL_MODE(client->getPrefix(), channel->getName(), (active ? "+l" : "-l"), (active ? arguments[p] : "")));
				p += active ? 1 : 0;
				break;
			}

			case 'k': {
				std::cout << "PassWord\n";
				channel->setPassword(active ? arguments[p] : "");
				channel->broadcast(RPL_MODE(client->getPrefix(), channel->getName(), (active ? "+k" : "-k"), (active ? arguments[p] : "")));
				p += active ? 1 : 0;
				break;
			}

			default:
				break;
		}
		i++;
	}
}
