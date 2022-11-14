#include "ft_irc.hpp"

KickCommand::KickCommand(Server *server) : Command(server) {}

KickCommand::~KickCommand() {}

// format: KICK <channel> <user> *( "," <user> ) [<comment>]
void KickCommand::execute(Client *client, std::vector<std::string> arguments)
{
	std::cout << "KickExec\n";
	int chanop = 0;
	if (arguments.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickName(), "KICK"));
		return;
	}
	std::string chan_name = arguments[1];
	std::cout << chan_name << std::endl;
	std::string target = arguments[2];
	std::string reason = "No reason specified.";

	if (arguments.size() >= 3)
	{
		reason = "";
		for (std::vector<std::string>::iterator it = arguments.begin() + 2; it != arguments.end(); it++)
			reason.append(*it + " ");
	}

	// Check if kicker is in chan

	std::vector<Channel *> client_chans = client->getUserChans();
	std::vector<Channel *>:: iterator it = client_chans.begin();

	Channel *chan;
	while (it != client_chans.end())
	{
		chan = it.operator*();
		if (chan->getName() == chan_name)
			break ;
		++it;
	}
	if (it == client_chans.end())
	{
		client->reply(ERR_NOTONCHANNEL(client->getNickName(), chan_name));
		return;
	}

	// Check if kicker is admin or oper

	// is_admin
	if (chan->getAdmin() == client)
		chanop += 1;
	if (chan->is_oper(client))
		chanop += 1;
	if (chanop == 0)
	{
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickName(), chan->getName()));
		return;
	}

	// check if target is in chan
	std::vector<Client *> chan_users = chan->getChanClients();
	std::vector<Client *>:: iterator it_user = chan_users.begin();

	Client *user;
	std::string nn;
	while (it_user != chan_users.end())
	{
		user = it_user.operator*();
		nn = ":" + user->getNickName();
		if (nn == target)
			break ;
		++it_user;
	}
	if (it_user == chan_users.end())
	{
		client->reply(ERR_USERNOTINCHANNEL(client->getNickName(), user->getNickName(), chan_name));
		return;
	}
	chan->kick(client, user, reason);
}
