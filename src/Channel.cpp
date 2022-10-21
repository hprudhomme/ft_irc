#include <ft_irc.hpp>

Channel::Channel(std::string const &name, Client *admin, Server *server)
					: _name(name), _admin(admin), _server(server) {}
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
	_server->broadcast(message);
}

void Channel::removeClient(Client *client)
{
	_oper_clients.erase(std::remove(_oper_clients.begin(), _oper_clients.end(), client), _oper_clients.end());
	_clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
	client->leave(this);

	if (_clients.empty())
	{
		// free chan and remove it from server
		return;
	}

	if (_admin == client)
		_admin = _clients.begin().operator*();

	// message to say that there is a new admin
}

void Channel::removeOper(Client *client)
{
	_oper_clients.erase(std::remove(_oper_clients.begin(), _oper_clients.end(), client), _oper_clients.end());
}


