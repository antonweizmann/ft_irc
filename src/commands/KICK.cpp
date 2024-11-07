#include "Server.hpp"

std::string splitCmdKick(std::vector<std::string>& cmd, std::string &channel, std::vector<std::string>& usersToKick)
{
	size_t pos;
	size_t comma_pos;
	std::string reason;
	std::string args = cmd[1];
	while ((comma_pos = args.find(',')) != std::string::npos)
	{
		usersToKick.push_back(args.substr(0, comma_pos));
		args = args.substr(comma_pos + 1);
	}
	pos = args.find(' ');
	usersToKick.push_back(args.substr(0, pos));
	reason = cmd[2];
	if (reason.empty())
		reason = "Kick because lol";
	return reason;
}

void Server::KICK(std::vector<std::string> cmd, int fd)
{
	std::vector<std::string> usersToKick;
	Client &sender = *getClient(fd);
	if (cmd.size() < 2)
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNickname(), (cmd.empty() ? "KICK" : "KICK " + cmd[0])), fd); return ;}
	std::string channel = cmd[0].substr(1);
	std::string reason = splitCmdKick(cmd, channel, usersToKick);
	if (_channels.find(channel) != _channels.end())
	{
		Channel &curChannel = _channels[channel];
		if (!curChannel.getUser(sender.getNickname())) // check if sender is in channel
			{sendResponse(ERR_NOTONCHANNEL(sender.getNickname(), channel), fd); return ;}
		if (!curChannel.getOperator(sender.getNickname())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender.getNickname(), channel), fd); return ;}
		for (auto it = usersToKick.begin(); it != usersToKick.end(); it++) // Iterate through all users to kick
		{
				if (!curChannel.getUser(*it)) // check if user to kick is in channel
					{sendResponse(ERR_USERNOTINCHANNEL(sender.getNickname(), *it, channel), fd); continue;}
				std::stringstream ss;
				ss << ":" << sender.getNickname() << "!" << curChannel.getUser(*it)->getUsername() << "@" << "localhost" << " KICK #" << curChannel.getName() << " " << curChannel.getUser(*it)->getNickname() << " :" << reason;
				curChannel.broadcastMessage(ss.str(), sender);
				curChannel.removeUser(*curChannel.getUser(*it));
				if (curChannel.getOperator(*it))
					curChannel.removeOperator(*curChannel.getOperator(*it));
				if (curChannel.getUserCount() == 0)
					{_channels.erase(curChannel.getName()); return ;}
		}
	}
	else
		sendResponse(ERR_NOSUCHCHANNEL(sender.getNickname(), channel), fd);
}
