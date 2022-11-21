#include "ft_irc.hpp"

ListCommand::ListCommand(Server *server) : Command(server) {};

ListCommand::~ListCommand() {};

void ListCommand::execute(Client *client, std::vector<std::string> arguments)
{
    if (arguments.empty())
    {
        std::string message = "";
        std::vector<Channel *> chans = _server->getServChannels();
        std::vector<Channel *>::iterator it = chans.begin();

        while (it != chans.end())
        {
            Channel *chan = it.operator*();
            message.append(chan->getName() + " ");
        }

        client->reply(RPL_LIST(client->getNickName(), message));
        client->reply(RPL_LISTEND(client->getNickName()));
    }
    client->reply(RPL_LISTEND(client->getNickName()));
}