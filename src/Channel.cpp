#include "ft_irc.hpp"
#include "rpl.hpp"

Channel::Channel(std::string const &name, std::string const &password, Client *admin, Server *server)
					: _name(name) , _admin(admin), _k(password), _server(server) {}
Channel::~Channel() {}

std::vector<std::string>	Channel::getNickNames()
{
	std::vector<std::string> nicknames;
	std::vector<Client *>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		Client *client = it.operator*();
		nicknames.push_back((_admin == client ? "@" : "") + (*it)->getNickName());
		it++;
	}
	return nicknames;
}

void Channel::broadcast(const std::string &message)
{
	std::vector<Client *>::iterator it;;
	for (it = _clients.begin(); it != _clients.end(); it++)
		(*it)->write(message);
	// this->_server->broadcastChannel(message, this);
}

void Channel::broadcast(const std::string &message, Client *exclude)
{
	std::vector<Client *>::iterator it;;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*it == exclude)
			continue;
		(*it)->write(message);
	}
	// this->_server->broadcastChannel(message, exclude->getFD(), this);
}

void Channel::removeClient(Client *client)
{
	std::cout << "Channel::removeClient\n";
	_oper_clients.erase(std::remove(_oper_clients.begin(), _oper_clients.end(), client), _oper_clients.end());
	_clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
	client->leave(this, 1);

	std::vector<Client *>:: iterator it = _clients.begin();

	Client *user;
	while (it != _clients.end())
	{
		user = it.operator*();
		std::cout << user->getNickName() << "   xxxxxxx\n";
		++it;
	}

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

void Channel::kick(Client *client, Client *target, std::string const &reason)
{
	std::cout << "Channel::kick\n";
	broadcast(RPL_KICK(client->getPrefix(), _name, target->getNickName(), reason));
	removeClient(target);

	std::cout << "kicked\n";
}


