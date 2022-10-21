#include "ft_irc.hpp"

CommandHandler::CommandHandler(Server *server) : _server(server)
{
	_commands["PASS"] = new PassCommand(_server, false);
	_commands["NICK"] = new NickCommand(_server, false);
	_commands["USER"] = new UserCommand(_server, false);
	_commands["QUIT"] = new QuitCommand(_server, false);

	_commands["PING"] = new PingCommand(_server);
	_commands["PONG"] = new PongCommand(_server);
	_commands["JOIN"] = new JoinCommand(_server);
	_commands["MODE"] = new ModeCommand(_server);
	_commands["PART"] = new PartCommand(_server);
	_commands["KICK"] = new KickCommand(_server);

	_commands["PRIVMSG"] = new PrivMsgCommand(_server);
	_commands["NOTICE"] = new NoticeCommand(_server);
}

CommandHandler::~CommandHandler()
{
	for (std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
		delete it->second;
}
