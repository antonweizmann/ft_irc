# include "Server.hpp"

void Server::TOPIC(std::vector<std::string> cmd, int fd)
{
	std::string topic;
	Client &sender = *getClient(fd);
	if (cmd.empty())
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNickname(), "TOPIC"), fd); return;}
	std::string targetChannel = cmd[0];
	if (targetChannel.empty() || targetChannel[0] != '#' || _channels.find(targetChannel.substr(1)) == _channels.end())
		{sendResponse(ERR_NOSUCHCHANNEL(sender.getNickname(), targetChannel.substr(1)), fd); return;}
	Channel &channel = _channels[targetChannel.substr(1)];
	if (!channel.getUser(sender.getNickname())) // check if sender is in channel
		{sendResponse(ERR_NOTONCHANNEL(sender.getNickname(), targetChannel.substr(1)), fd); return ;}
	if (cmd.size() > 1)
	{
		topic = cmd[1];
	}
	else
	{
		std::pair<std::string, std::string> curTopic = channel.getTopic();
		if (curTopic.first.empty())
			{sendResponse(RPL_NOTOPIC(sender.getNickname(), targetChannel), fd); return ;}
		else
			{sendResponse(RPL_TOPIC(sender.getNickname(), targetChannel, curTopic.first) + "\r\n" + RPL_TOPICWHOTIME(sender.getNickname(), targetChannel, curTopic.second), fd); return;}
	}
	if (channel.isModeSet(TOPIC_SETTABLE_BY_OPERATOR) && !channel.getOperator(sender.getNickname())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender.getNickname(), targetChannel.substr(1)), fd); return ;}
	std::pair<std::string, std::string> newTopic;
	newTopic.first = topic;
	std::time_t t = std::time(nullptr);
	newTopic.second = sender.getNickname() + " " + std::to_string(t);
	channel.setTopic(newTopic);
	channel.systemMessage(":" + sender.getNickname() + "!" + sender.getUsername() + "@localhost TOPIC " + targetChannel + " :" + topic);
}
