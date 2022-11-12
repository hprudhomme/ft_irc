#include "ft_irc.hpp"
#include "rpl.hpp"

Channel::Channel(std::string const &name, std::string const &password, Client *admin, Server *server)
					: _name(name) , _admin(admin), _l(1000), _k(password), _server(server) {}
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
	std::cout << "buzz broadcast 1\n";
	std::vector<Client *>::iterator it;;
	for (it = _clients.begin(); it != _clients.end(); it++)
		(*it)->write(message);
}

void Channel::broadcast(const std::string &message, Client *exclude)
{
	std::cout << "buzz broadcast 2\n";
	std::vector<Client *>::iterator it;;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*it == exclude)
			continue;
		(*it)->write(message);
	}
}

void						Channel::broadcast_channel(std::string const &message) const
{
	_server->broadcast(message);
}

Client *Channel::getClient(const std::string &nickname)
{
	std::vector<Client *>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		if ((*it)->getNickName() == nickname)
			return *it;
		it++;
	}
	return NULL;
}

int Channel::is_oper(Client *client)
{
	std::cout << "is_oper ?\n";
	std::vector<Client *> opers_chan = this->getChanOpers();
	std::vector<Client *>:: iterator it_oper = opers_chan.begin();

	while (it_oper != opers_chan.end())
	{
		Client *oper = it_oper.operator*();
		if (oper == client)
			return 1;
		++it_oper;
	}
	if (it_oper == opers_chan.end())
		return 0;
	return 0;
}

void Channel::removeClient(Client *client)
{
	std::cout << "Channel::removeClient\n";
	_oper_clients.erase(std::remove(_oper_clients.begin(), _oper_clients.end(), client), _oper_clients.end());
	_clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
	client->leave(this, 1);

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
	std::cout << "remove_oper`\n";
	_oper_clients.erase(std::remove(_oper_clients.begin(), _oper_clients.end(), client), _oper_clients.end());
}

void Channel::kick(Client *client, Client *target, std::string const &reason)
{
	std::cout << "Channel::kick\n";
	broadcast(RPL_KICK(client->getPrefix(), _name, target->getNickName(), reason));
	removeClient(target);
}


void Channel::invit(Client *client, Client *target)
{
	std::cout << "Channel::invit\n";
	broadcast(RPL_INVITING(client->getPrefix(), _name, target->getNickName()));
	target->join(this);
}