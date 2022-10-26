#include "ft_irc.hpp"

NickCommand::NickCommand(Server *server, bool auth) : Command(server, auth) {}

NickCommand::~NickCommand() {}

void NickCommand::execute(Client *client, std::vector<std::string> arguments) {

	if (arguments.empty() || arguments[0].empty())
	{
		client->reply(ERR_NONICKNAMEGIVEN(client->getNickName()));
		return;
	}

	std::string nickname = arguments[0];

	if (_server->getClient(nickname))
	{
		client->reply(ERR_NICKNAMEINUSE(client->getNickName()));
		return;
	}
	client->setNickname(nickname);
	// client->welcome(); do welcome
}
