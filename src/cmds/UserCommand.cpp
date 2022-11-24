#include "ft_irc.hpp"

UserCommand::UserCommand(Server *server, bool auth) : Command(server, auth) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client *client, std::vector<std::string> arguments) {

	if (client->isRegistered())
	{
		client->reply(ERR_ALREADYREGISTERED(client->getNickName()));
		return;
	}

	if (arguments.size() < 4) {
		client->reply(ERR_NEEDMOREPARAMS(client->getNickName(), "USER"));
		return;
	}
	client->setUsername(arguments[0]);
	client->setRealName(arguments[3].substr(1));
	client->welcome();
}
