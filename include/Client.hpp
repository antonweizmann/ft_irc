
#pragma once

#include "ft_irc.hpp"
#include "Channel.hpp"
#include <sys/socket.h>

class Channel;

class Client
{
	private:
		int			_fd;
		std::string	_ip_addr;
		std::string _nick;
		std::string _username;
		int			_auth_stage; // 2 no pass; 1 no nick; 0 authenticated
	public:
		Client();
		~Client();

		int		getFd() const;
		void	setFd(int fd);
		void	setIpAddr(const std::string ip_addr);
		const std::string& getNick(void) const;
		void	setNick(const std::string &new_nick);
		const std::string& getUsername(void) const;
		void	setUsername(const std::string &new_username);
		int		getAuth() const;
		void	setAuth(int auth_stage);
		void	joinChannel(Channel *channel);
		void	leaveChannel(Channel *channel);
		void	receiveMsg(const std::string& msg);
};
