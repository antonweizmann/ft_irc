# include "Server.hpp"

void	Server::USER(std::vector<std::string> cmd, int fd)
{
	Client &client = _clients[fd];

	if (!cmd.size() || cmd[0].empty())
		sendResponse(ERR_NEEDMOREPARAMS(client.getNickname(), "USER"), fd);
	else if (client.getAuth() != 1)
		sendResponse(ERR_ALREADYREGISTERED(client.getNickname()), fd);
	else
	{
		client.setUsername(cmd[0]);
		if (cmd.size() > 3)
			client.setRealname(cmd[3]);
	}
}

