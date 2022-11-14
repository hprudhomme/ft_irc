#include "ft_irc.hpp"
#include "rpl.hpp"

Client::Client(Server *server, int fd, std::string const &hostname, int port)
					: _is_registered(0), _fd(fd), _hostname(hostname), _port(port), _server(server) {}
Client::~Client() {}

void	Client::write(const std::string &message) const
{
	this->_server->send(message + "\r\n", this->getFD());
}

std::string Client::getPrefix() const
{
	return _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
}

void	Client::reply(const std::string &reply) {
	this->write(":" + getPrefix() + " " + reply);
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
	std::string n = "test";
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
	chan->broadcast(RPL_JOIN(getPrefix(), chan->getName()));
}

void 	Client::leave(Channel *chan, int kicked)
{
	_user_chans.erase(std::remove(_user_chans.begin(), _user_chans.end(), chan), _user_chans.end());
	chan->broadcast(RPL_PART(getPrefix(), chan->getName()));
	if (!kicked)
		chan->removeClient(this);

	// message leave chan
}

void	Client::welcome()
{
	// TODO: send welcome message
	// reply(RPL_WELCOME(_nickname, _username, _hostname));
	// reply(RPL_YOURHOST(_nickname, _hostname));
	// reply(RPL_CREATED(_nickname, "2019-10-10"));
	// reply(RPL_MYINFO(_nickname, "ft_irc", "0.1", "i", "o"));
}
