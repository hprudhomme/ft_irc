#ifndef CHANNEL_CLASS_H
# define CHANNEL_CLASS_H

#include <vector>

class Client;
class Server;

class Channel
{
private:
	std::string _name;
	Client		*_admin;

	int 		_l; //max user in chan
	bool		_i; //invite-only chan
	std::string _k; //chan's key(paasword)

	std::vector<Client *> _clients;
	std::vector<Client *> _oper_clients;

	Server *_server;
public:
	Channel(std::string const &name, Client *admin, Server *server);
	~Channel();

	// GETTERS

	Client						*getAdmin() const { return _admin; };
	std::string const 			&getName() const { return _name; };
	std::string const 			&getPassWord() const { return _k; };
	int							getMaxUsers() const { return _l; };

	int							getNbrClients() const { return _clients.size(); };
	std::vector<std::string>	getNickNames();

	// SETTERS

	void						setPassword(std::string k) { _k = k; };
	void						setMaxClients(int l) { _l = l; };

	// OTHER

	void 						removeClient(Client *client);
	void 						removeOper(Client *client);
	void						addClient(Client *client) { _clients.push_back(client); };
	void						addOper(Client *client) { _oper_clients.push_back(client); };
	void						kick(Client *client, Client *target, std::string const &reason);

	void						broadcast_channel(std::string const &message) const;
};

#endif
