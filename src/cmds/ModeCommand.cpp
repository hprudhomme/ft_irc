#include "ft_irc.hpp"

ModeCommand::ModeCommand(Server *server) : Command(server) {}

ModeCommand::~ModeCommand() {}

void ModeCommand::execute(Client *client, std::vector<std::string> arguments)
{
	int chanop = 0;
	if (arguments.size() < 2 || arguments[0].empty() || arguments[1].empty()) {
		return;
	}

	std::string target = arguments.at(0);

	Channel *channel = _server->getChannel(target); //MODE on clients not implemented
	if (!channel)
	{
		client->reply(ERR_NOSUCHCHANNEL(client->getNickName(), target));
		return;
	}

	// check if admin or chanop
	if (channel->getAdmin() == client)
		chanop += 1;
	if (channel->is_oper(client))
		chanop += 1;
	if (chanop == 0)
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

			case 'o': {
				std::cout << "Oper\n";
				Client *c_tar = channel->getClient(arguments[p]);
				if (!c_tar)
				{
					channel->broadcast(ERR_USERNOTINCHANNEL(client->getNickName(), arguments[p], channel->getName()));
					return ;
				}
				if (active)
					channel->addOper(c_tar);
				else
					channel->removeOper(c_tar);
				channel->broadcast(RPL_MODE(client->getPrefix(), channel->getName(), (active ? "+o" : "-o"), (c_tar->getNickName())));
				p += active ? 1 : 0;
				break;
			}

			default:
				break;
		}
		i++;
	}
}
