#include "Channel.hpp"

Channel::Channel(std::string const &name, Client *admin, Server server)
					: _name(name), _admin(admin) _server(server) {}
Channel::~Channel() {}

std::vector<std::string>	Channel::getNickNames()
{
	std::vector<std::string> nicknames;
	std::vector<Client *>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		Client *client = it.operator*();
		nicknames.push_back((_admin == client ? "@" : "") + (*it)->getNickName());
	}
	return nicknames;
}

void						Channel::broadcast_channel(std::string const &message) const
{
	_server.broadcast(message);
}


