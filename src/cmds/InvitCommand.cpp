#include "ft_irc.hpp"

InvitCommand::InvitCommand(Server *server) : Command(server) {}

InvitCommand::~InvitCommand() {}

void InvitCommand::execute(Client *client, std::vector<std::string> arguments)
{
	int chanop = 0;
    std::cout << "InvitExec\n";
	if (arguments.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickName(), "INVIT"));
		return;
	}
	std::string chan_name = arguments[0];
	std::string target = arguments[1]; 

	// Check if inviteur is in chan

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

	if (chan->getAdmin() == client)
		chanop += 1;
	if (chan->is_oper(client))
		chanop += 1;
	if (chanop == 0)
	{
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickName(), chan->getName()));
		return;
	}

	Client *user = _server->getClient(target);
    if (!user)
    {
        client->reply(ERR_NOSUCHNICK(client->getNickName(), target));
        return ;
    }

	// check if target is in chan
	std::vector<Client *> chan_users = chan->getChanClients();
	std::vector<Client *>:: iterator it_user = chan_users.begin();

	while (it_user != chan_users.end())
	{
		user = it_user.operator*();
		if (user->getNickName() == target)
			break ;
		++it_user;
	}
	if (it_user != chan_users.end())
	{
		client->reply(ERR_USERONCHANNEL(client->getNickName(), user->getNickName(), chan_name));
		return ;
	}

    chan->invit(client, user);
}