#include "Server.hpp"

void	Server::PASS(std::vector<std::string> cmd, int fd)
{
	Client &client = _clients[fd];

	if (!cmd.size())
		sendResponse(ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"), fd);
	else if (client.getAuth() != 2)
		sendResponse(ERR_ALREADYREGISTERED(client.getNickname()), fd);
	else if (_pass != cmd[0])
		sendResponse(ERR_PASSWDMISMATCH(client.getNickname()), fd);
	else
		client.setAuth(1);
}
