#include "Server.hpp"

void getVars(std::string &target, std::string &modestring, std::string &params, std::string &cmd)
{
	std::istringstream iss(cmd);

	iss >> target;
	iss >> modestring;
	size_t found = cmd.find_first_not_of(target + modestring + " \t\v");
	if (found != std::string::npos)
		params = cmd.substr(found);
}

void addMode(std::string& modeReport, char toAdd, bool sign)
{
	size_t lastPlus = modeReport.find_last_of('+');
	size_t lastMinus = modeReport.find_last_of('-');

	if (modeReport.empty() ||
		(sign && (lastPlus == std::string::npos || (lastMinus != std::string::npos && lastMinus > lastPlus))) ||
		(!sign && (lastMinus == std::string::npos || (lastPlus != std::string::npos && lastPlus > lastMinus))))
	{
		modeReport += (sign ? '+' : '-');
	}

	modeReport += toAdd;
}

void inviteOnly(bool sign, Channel &channel, std::string &modeReport)
{
	if (sign)
		channel.setMode(INVITE_ONLY, "", true);
	else
		channel.setMode(INVITE_ONLY, "", false);
	addMode(modeReport, 'i', sign);
}

void topicRestrict(bool sign, Channel &channel, std::string &modeReport)
{
	if (sign)
		channel.setMode(TOPIC_SETTABLE_BY_OPERATOR, "", true);
	else
		channel.setMode(TOPIC_SETTABLE_BY_OPERATOR, "", false);
	addMode(modeReport, 't', sign);
}

bool Server::channelKey(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator &argsIt, std::string &argsReport, int fd)
{
	if (sign)
	{
		std::string key = *argsIt;
		if (key.empty())
			{sendResponse(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "(k)"), fd); return false;}
		if (key.find_first_of(" \t\v") != std::string::npos)
			{sendResponse(ERR_INVALIDMODEPARAM(getClient(fd)->getNickname(), channel.getName(), "k", key, "Should be without Whitespaces"), fd); return false;}
		channel.setMode(KEY, key, true);
		if (!argsReport.empty())
			argsReport += " ";
		argsReport += *argsIt;
		argsIt++;	
		addMode(modeReport, 'k', sign);
	}
	else
	{
		if (channel.isModeSet(USER_LIMIT))
			channel.setMode(USER_LIMIT, "", false);
		addMode(modeReport, 'k', sign);
	}
		return true;
}

bool Server::operatorPriv(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator &argsIt, std::string &argsReport, int fd)
{
	std::string test = *argsIt;
	if (test.empty())
		{sendResponse(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "(o)"), fd); return false;}
	if (!getClient(test))
		{sendResponse(ERR_NOSUCHNICK(getClient(fd)->getNickname() ,*argsIt), fd); return false;}
	Client &user = *(getClient(test));
	if (!channel.getUser(*argsIt))
		{sendResponse(ERR_USERNOTINCHANNEL(getClient(fd)->getNickname(), *argsIt, channel.getName()), fd); return false;}
	if (sign)
		channel.addOperator(user);
	else
		channel.removeOperator(user);
	if (!argsReport.empty())
		argsReport += " ";
	argsReport += *argsIt;
	argsIt++;
	addMode(modeReport, 'o', sign);
		return true;
}

bool Server::userLimit(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator &argsIt, std::string &argsReport, int fd)
{
	if (sign)
	{
		std::string limit = *argsIt;
		if (limit.empty())
			{sendResponse(ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "(l)"), fd); return false;}
		if (limit.find_first_not_of("0123456789") != std::string::npos || std::atoi(limit.c_str()) <= 0)
			{sendResponse(ERR_INVALIDMODEPARAM(getClient(fd)->getNickname(), channel.getName(), "i", limit, "Should be a Int Number"), fd); return false;}
		channel.setMode(USER_LIMIT, limit, true);
		if (!argsReport.empty())
			argsReport += " ";
		argsReport += *argsIt;
		argsIt++;
	}
	else
		channel.setMode(USER_LIMIT, "0", false);

	addMode(modeReport, 'l', sign);
	return true;
}

void Server::MODE(std::vector<std::string> cmd, int fd)
{
	std::string target;
	std::string modestring;
	std::string params;
	bool	sign;
	std::vector<std::string> modeArgs;
	Client &sender = *getClient(fd);
	std::string modeReport;
	std::string argsReport;
	if (cmd.empty())
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNickname(), "MODE"), fd); return;}
	auto cmdIt = cmd.begin();
	target = *cmdIt++;
	if (target.empty() || target[0] != '#' || _channels.find(target.substr(1)) == _channels.end())
		{sendResponse(ERR_NOSUCHCHANNEL(sender.getNickname(), target.substr(1)), fd); return;}
	if (cmdIt != cmd.end())
		modestring = *cmdIt++;
	Channel &channel = _channels[target.substr(1)];
	if (!channel.getUser(sender.getNickname())) // check if sender is in channel
		{sendResponse(ERR_NOTONCHANNEL(sender.getNickname(), target.substr(1)), fd); return ;}
	if (modestring.empty())
		{sendResponse(RPL_CHANNELMODEIS(sender.getNickname(), target.substr(1), channel.getModestring(), channel.getModesvalues()), fd); return ;}
	if (!channel.getOperator(sender.getNickname())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender.getNickname(), target.substr(1)), fd); return ;}
	for (auto pos = modestring.begin(); pos != modestring.end(); pos++)
	{
		bool test = true;
		if (*pos == '+' || *pos == '-')
			sign = (*pos == '+'); //if *pos == + then sign will be 1
		else if (*pos == 'i')
			inviteOnly(sign, channel, modeReport);
		else if (*pos == 't')
			topicRestrict(sign, channel, modeReport);
		else if (*pos == 'k')
			test = channelKey(sign, channel, modeReport, cmdIt, argsReport, fd);
		else if (*pos =='o')
			test = operatorPriv(sign, channel, modeReport, cmdIt, argsReport, fd);
		else if (*pos == 'l')
			test = userLimit(sign, channel, modeReport, cmdIt, argsReport, fd);
		else
			sendResponse(ERR_UMODEUNKOWNFLAG(sender.getNickname()), fd);
		if (!test)
			return ;
	}

	channel.systemMessage(RPL_CHANGEDMODE(sender.getNickname(), sender.getUsername(), channel.getName(), modeReport, argsReport));
}
