
#include "Server.hpp"

bool Server::_signal = false;

Server::Server(int port) : _port(port) {}

Server::Server(int port, std::string pass) : _port(port), _pass(pass) {}

Server::~Server() {}

template <typename... Args>
void Server::sendError(std::string numeric, int fd, std::string client, Args... args) // Pls write : infront of the msg
{
	std::stringstream ss;

	ss << ":" << _name << " " << numeric;

	((ss << args << " "), ...);
	ss <<"\r\n";
	std::string out = ss.str();
	if (send(fd, out.c_str(), out.size(), 0) == -1)
		std::cerr << "send() failed" << std::endl;
}

void Server::sendResponse(std::string message, int fd)
{
	std::string out = message + "\r\n";
	if (send(fd, out.c_str(), out.size(), 0) == -1)
		std::cerr << "send() failed" << std::endl;
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
