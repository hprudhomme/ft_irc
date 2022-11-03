#ifndef CLIENT_CLASS_H
# define CLIENT_CLASS_H

#include <iostream>
#include <vector>

class Channel;

class Client
{
	private:
		int 		_is_registered;
		int			_fd;
		std::string _hostname;
		int 		_port;

		std::string _nickname;
		std::string _username;
		std::string _realname;

		std::vector<Channel *> _user_chans;

		std::string	_partial_recv;
	public:
		Client(int fd, std::string const &hostname, int port);
		~Client();

		// GETTERS

		int 					isRegistered() const { return _is_registered; };
		int						getFD() const { return _fd; };
		std::string const 		&getHostName() const { return _hostname; };
		int 					getPort() const { return _port; };

		std::string const 		&getNickName () { return _nickname; };
		std::string const 		&getUserName () { return _username; };
		std::string const 		&getRealName () { return _realname; };
		std::string const 		&getPartialRecv () { return _partial_recv; };

		std::vector<Channel *> 	getUserChans() { return _user_chans; };

		// SETTERS

		void 					setRegistered(int i) { _is_registered = i ;};
		void 					setNickname(const std::string &nickname) { _nickname = nickname; };
		void 					setUsername(const std::string &username) { _username = username; };
		void 					setRealName(const std::string &realname) { _realname = realname; };
		void 					setPartialRecv(const std::string &partial_recv) { _partial_recv = partial_recv; };

		// OTHER

		void 					write(const std::string &message) const;
		void 					reply(const std::string &reply);
		std::string 			getPrefix() const;
		void 					welcome();
		void					join(Channel *chan);
		void					leave(Channel *chan, int kicked);
};

#endif
