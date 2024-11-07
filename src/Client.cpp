
#include "Client.hpp"

Client::Client() : _signalBuffer("")
{
}

Client::~Client()
{
}

int		Client::getFd() const
{
	return (_fd);
}

void	Client::setFd(int fd)
{
	_fd = fd;
}

void	Client::setIpAddr(const std::string ip_addr)
{
	_ip_addr = ip_addr;
}

const std::string&	Client::getNickname(void) const
{
	return _nick;
}

void	Client::setNickname(const std::string &new_nick)
{
	_nick = new_nick;
}

const std::string& Client::getUsername(void) const
{
	return _username;
}

void	Client::setUsername(const std::string &new_username)
{
	_username = new_username;
}

const std::string& Client::getRealname(void) const
{
	return _realname;
}

void	Client::setRealname(const std::string &new_realname)
{
	_realname = new_realname;
}

int		Client::getAuth() const
{
	return _auth_stage;
}

void	Client::setAuth(int auth_stage)
{
	_auth_stage = auth_stage;
}

void	Client::joinChannel(Channel *channel)
{
	channel->addUser(*this);
}

void	Client::leaveChannel(Channel *channel)
{
	channel->removeUser(*this);
}

void	Client::receiveMsg(const std::string &msg)
{
	_signalBuffer += msg + "\r\n";
}

std::string	Client::getBuffer() const
{
	return _signalBuffer;
}

void	Client::setBuffer(std::string msg)
{
	_signalBuffer = msg;
}
