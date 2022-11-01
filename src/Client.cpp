#include "ft_irc.hpp"
#include "rpl.hpp"

Client::Client(int fd, std::string const &hostname, int port)
					:  _is_registered(0), _fd(fd), _hostname(hostname), _port(port){}
Client::~Client() {}

void	Client::write(const std::string &message) const
{
	std::string buffer = message + "\r\n";
	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error while sending message to client.");
}

std::string Client::getPrefix() const
{
	return _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
}

void	Client::reply(const std::string &reply) {
	write(":" + getPrefix() + " " + reply);
}

void	Client::join(Channel *chan)
{
	chan->addClient(this);

	std::string users;
	std::vector<std::string> nicknames = chan->getNickNames();
	for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); it++)
		users.append(*it + " ");
	reply(RPL_NAMREPLY(_nickname, chan->getName(), users));
	reply(RPL_ENDOFNAMES(_nickname, chan->getName()));

	chan->broadcast_channel(RPL_JOIN(getPrefix(), chan->getName()));
}

void 	Client::leave(Channel *chan)
{
	_user_chans.erase(std::remove(_user_chans.begin(), _user_chans.end(), chan), _user_chans.end());
	chan->broadcast_channel(RPL_PART(getPrefix(), chan->getName()));
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
	std::cout << "Welcome " << _nickname << std::endl;
}
