#include "ft_irc.hpp"

QuitCommand::QuitCommand(Server *server, bool authRequired) : Command(server, authRequired) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client *client, std::vector<std::string> arguments) {

	std::string reason = arguments.empty() ? "Leaving..." : arguments.at(0);
	reason = reason.at(0) == ':' ? reason.substr(1) : reason;

	client->write(RPL_QUIT(client->getPrefix(), reason));
	// server disconnect client
	
	this->_server->delClient(client->getFD());
}
