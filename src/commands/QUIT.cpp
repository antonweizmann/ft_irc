#include "Server.hpp"

void	Server::QUIT(std::vector<std::string> cmd, int fd)
{
	std::string msg;
	Client &client = _clients[fd];

	msg = "ERROR :";
	if (cmd.size())
		msg.append(cmd[0]);
	// sendResponse(msg, fd);

	msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + _name + " QUIT :Quit: ";
	if (cmd.size())
		msg.append(cmd[0]);
	clearClient(fd);
	systemMessage(msg);
}
