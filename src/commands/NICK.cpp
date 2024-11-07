# include "Server.hpp"

bool Server::checkDuplicate(std::string nick)
{
	for (auto &user : _clients)
	{
		if (user.second.getNickname() == nick)
			return true;
	}
	return false;
}

bool checkValidNick(std::string nick)
{
	std::regex pattern("^[a-zA-Z0-9{}\\$$\\$$\\\\|]+$");
	return (std::regex_match(nick, pattern));
}

void	Server::NICK(std::vector<std::string> cmd, int fd)
{
	Client &sender = _clients[fd];
	if (cmd.empty() || cmd[0].empty())
		{sendResponse(ERR_NONICKNAMEGIVEN(sender.getNickname()), fd); return ;}
	if (checkDuplicate(cmd[0]))
		{sendResponse(ERR_NICKNAMEINUSE(sender.getNickname(), cmd[0]), fd); return ;}
	if (!checkValidNick(cmd[0]))
		{sendResponse(ERR_ERRONEUSNICKNAME(sender.getNickname(), cmd[0]), fd); return ;}
	std::stringstream ss;
	ss << ":" << sender.getNickname() << "!" << sender.getUsername() << "@" << "localhost" << " NICK " << cmd[0];
	sender.receiveMsg(ss.str());
	sender.setNickname(cmd[0]);
}

