#ifndef CLIENT_CLASS_H
# define CLIENT_CLASS_H

#include <iostream>
#include <vector>

class Channel;

class Client
{
	private:
		int			_fd;
		std::string _hostname;
		int 		_port;

		std::string _nickname;
		std::string _username;
		std::string _realname;

		std::vector<Channel *> _user_chans;

		std::string	_command;
	public:
		Client(int fd, std::string const &hostname, int port);
		~Client();

		// GETTERS

		int						getFD() const { return _fd; };
		std::string const 		&getHostName() const { return _hostname; };
		int 					getPort() const { return _port; };

		std::string const 		&getNickName () { return _nickname; };
		std::string const 		&getUserName () { return _username; };
		std::string const 		&getRealName () { return _realname; };

		std::vector<Channel *> 	getUserChans() { return _user_chans; };

		// SETTERS

		void 					setNickname(const std::string &nickname) { _nickname = nickname; };
		void 					setUsername(const std::string &username) { _username = username; };
		void 					setRealName(const std::string &realname) { _realname = realname; };

		// OTHER

		void 					write(const std::string &message) const;
		void 					reply(const std::string &reply);
		std::string 			getPrefix() const;
		void 					welcome();
		void					join(Channel *chan) { _user_chans.push_back(chan); };
		void					leave(Channel *chan) { _user_chans.erase(std::remove(_user_chans.begin(), _user_chans.end(), chan), _user_chans.end()); };
};

#endif
