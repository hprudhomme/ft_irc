#include "ft_irc.hpp"
#include "rpl.hpp"

Client::Client(Server *server, int fd, std::string const &hostname, int port)
					: _fd(fd), _hostname(hostname), _port(port), _server(server) {}
Client::~Client() {}

void	Client::write(const std::string &message) const
{
	this->_server->send(message, this->getFD());
}

std::string Client::getPrefix() const
{
	if (this->getNickName().empty())
		return ("*");
	return _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
}

bool	Client::isRegistered() const
{
	if (!this->getNickName().empty() && !this->getUserName().empty() && !this->getRealName().empty())
		return (true);
	return (false);
}

void	Client::reply(const std::string &reply) {
	// TODO: récupérer le nom du serveur
	this->write(":" + this->_server->getServerName() + " " + reply);
}

void	Client::join(Channel *chan)
{
	chan->addClient(this);
	_user_chans.push_back(chan);

	if (chan->getNbrClients() == 1)
	{
		chan->setAdmin(this);
		chan->addOper(this);
	}
	std::string users;
	std::vector<std::string> nicknames = chan->getNickNames();
	for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); it++)
		users.append(*it + " ");

	chan->broadcast(RPL_JOIN(getPrefix(), chan->getName()));

	reply(RPL_NOTOPIC(this->getNickName(), chan->getName()));
	reply(RPL_NAMREPLY(this->getNickName(), chan->getName(), users));
	reply(RPL_ENDOFNAMES(this->getNickName(), chan->getName()));


	// std::vector<Channel *> client_chans = this->getUserChans();
	// std::vector<Channel *>:: iterator it = client_chans.begin();

	// Channel *chann;
	// while (it != client_chans.end())
	// {
	// 	chann = it.operator*();
	// 	if (chann->getName() == chan->getName())
	// 		break ;
	// 	++it;
	// }
	//chan->broadcast(RPL_JOIN(getPrefix(), chan->getName()), this);
}

void 	Client::leave(Channel *chan, int kicked)
{
	if (!_user_chans.empty())
		_user_chans.erase(this->_user_chans.begin() + this->_channelIndex(chan));
	//chan->broadcast(RPL_PART(getPrefix(), chan->getName()));
	if (!kicked)
		chan->removeClient(this);

	// message leave chan
}

void	Client::welcome()
{
	if (!this->isRegistered())
		return ;
	// TODO: voir à quoi correspondent les usermodes et les chanmodes
	reply(RPL_WELCOME(this->getNickName(), this->getPrefix()));
	reply(RPL_YOURHOST(this->getNickName(), this->_server->getServerName(), "0.1"));
	reply(RPL_CREATED(this->getNickName(), this->_server->getStartTime()));
	reply(RPL_MYINFO(this->getNickName(), this->_server->getServerName(), "0.1", "aiorsw", "IObeiklmnopstv"));

	// TODO: faire des fonction RPL
	reply("375 " + this->getNickName() + " :- " + this->_server->getServerName() + " Message of the day -");
	reply("372 " + this->getNickName() + " :- Welcome to our IRC server!");

	reply("372 " + this->getNickName() + " :- .-.-----------.-.");
	reply("372 " + this->getNickName() + " :- | |--FT_IRC---|#|");
	reply("372 " + this->getNickName() + " :- | |-----------| |");
	reply("372 " + this->getNickName() + " :- | |-ocartier--| |");
	reply("372 " + this->getNickName() + " :- | |-hprudhomme| |");
	reply("372 " + this->getNickName() + " :- | \"--------42-' |");
	reply("372 " + this->getNickName() + " :- |  .-----.-..   |");
	reply("372 " + this->getNickName() + " :- |  |     | || |||");
	reply("372 " + this->getNickName() + " :- |  |     | || \\/|");
	reply("372 " + this->getNickName() + " :- \"--^-----^-^^---'");

	reply("376 " + this->getNickName() + " :End of MOTD command");
}

unsigned long	Client::_channelIndex(Channel *channel)
{
	unsigned long i = 0;
	std::vector<Channel *>::iterator it = this->_user_chans.begin();

	while (it != this->_user_chans.end())
	{
		if (*it == channel)
			return i;
		it++;
		i++;
	}
	return 0;
}
