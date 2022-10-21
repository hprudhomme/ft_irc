#include "ft_irc.hpp"

NoticeCommand::NoticeCommand(Server *server) : Command(server) {}

NoticeCommand::~NoticeCommand() {}

void NoticeCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2 || arguments[0].empty() || arguments[1].empty()) {
		return;
	}
}
