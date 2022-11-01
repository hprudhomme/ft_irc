#include "ft_irc.hpp"

KickCommand::KickCommand(Server *server) : Command(server) {}

KickCommand::~KickCommand() {}

// format: KICK <channel> <user> *( "," <user> ) [<comment>]
void KickCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickName(), "KICK"));
		return;
	}
	std::string chan_name = arguments[0];
	std::string target = arguments[1];
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
	if (chan->getAdmin() != client)
	{
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickName(), target));
		return;
	}

	// is_oper
	std::vector<Client *> opers_chan = chan->getChanOpers();
	std::vector<Client *>:: iterator it_oper = opers_chan.begin();

	while (it_oper != opers_chan.end())
	{
		Client *oper = it_oper.operator*();
		if (oper == client)
			break ;
		++it_oper;
	}
	if (it_oper == opers_chan.end())
	{
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickName(), target));
		return;
	}

	// check if target is in chan
	std::vector<Client *> chan_users = chan->getChanClients();
	std::vector<Client *>:: iterator it_user = chan_users.begin();

	Client *user;
	while (it_user != chan_users.end())
	{
		user = it_user.operator*();
		if (user->getNickName() == target)
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
