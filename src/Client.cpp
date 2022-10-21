#include "ft_irc.hpp"
#include "rpl.hpp"

Client::Client(int fd, std::string const &hostname, int port)
					: _fd(fd), _hostname(hostname), _port(port) {}
Client::~Client() {}

void Client::write(const std::string &message) const {

	//std::cout << "-> " << message << std::endl;

	std::string buffer = message + "\r\n";
	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error while sending message to client.");
}

void Client::reply(const std::string &reply) {
	write(":" + reply);
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
}
