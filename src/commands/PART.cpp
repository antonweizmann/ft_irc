# include "Server.hpp"

void Server::PART(std::vector<std::string> cmd, int fd)
{
	std::vector<std::string> channelsToPart;
	Client &sender = *getClient(fd);
	if (cmd.size() < 1)
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNickname(), (cmd.empty() ? "PART" : "PART " + cmd[0])), fd); return ;}
	splitComma(cmd[0], channelsToPart);
	std::string reason;
	if (cmd.size() >= 2)
		reason = cmd[1];
	if (reason.empty())
		reason = "Gotta gooo bye bye";

	for (auto it = channelsToPart.begin(); it != channelsToPart.end(); it++)
	{
		if (_channels.find((*it).substr(1)) == _channels.end())
			{sendResponse(ERR_NOSUCHCHANNEL(sender.getNickname(), (*it).substr(1)), fd); continue;}
		Channel &curChannel = _channels[(*it).substr(1)];
		if (!curChannel.getUser(sender.getNickname())) // check if sender is in channel
			{sendResponse(ERR_NOTONCHANNEL(sender.getNickname(), (*it).substr(1)), fd); continue ;}
		std::stringstream ss;
		ss << ":" << sender.getNickname() << "!" << sender.getUsername() << "@" << "localhost" << " PART #" << curChannel.getName() << " :" << reason;
		curChannel.systemMessage(ss.str());
		curChannel.removeUser(sender);
		if (curChannel.getOperator(sender.getNickname()))
			curChannel.removeOperator(sender);
		if (curChannel.getUserCount() == 0)
			_channels.erase(curChannel.getName());
	}
}
