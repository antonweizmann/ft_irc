
#include "Server.hpp"

bool Server::_signal = false;

Server::Server(int port) : _port(port), _name("irc.andiarbeit.com") {}

Server::Server(int port, std::string pass) : _port(port), _pass(pass), _name("irc.andiarbeit.com") {}

Server::~Server() {}

void Server::sendResponse(std::string message, int fd)
{
	Client &reciever = *(getClient(fd));
	reciever.receiveMsg(message);
}

Client*	Server::getClient(int fd)
{
	if (_clients.find(fd) == _clients.end())
		return NULL;
	return &_clients[fd];
}

Client*	Server::getClient(std::string nick)
{
	for (auto iter = _clients.begin(); iter != _clients.end(); iter++)
		if ((*iter).second.getNickname() == nick)
			return & (*iter).second;
	return NULL;
}

void splitComma(std::string &cmd, std::vector<std::string> &split)
{
	std::istringstream iss(cmd);
	std::string line;
	while(std::getline(iss, line, ','))
		if (!line.empty())
			split.push_back(line);
}

void Server::systemMessage(const std::string &message)
{
	std::for_each(_clients.begin(), _clients.end(), [message](auto pair) {
		pair.second.receiveMsg(message);
	});
}
