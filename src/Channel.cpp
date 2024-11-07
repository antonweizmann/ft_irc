
#include "Channel.hpp"

void	Channel::clearModes()
{
	for (int i = 0; i < 5; i++)
		setMode(i, "", false);
}
Channel::Channel(std::string &channelName) : _name(channelName), _modes(5)
{
	// clearModes();
}

Channel::Channel(const std::string &channelName, const std::string &key) : _name(channelName), _modes(5)
{
	// clearModes();
	setMode(KEY, key, true);
}

Channel::~Channel()
{
}

void Channel::addUser(Client &client)
{
	if (_users.find(client.getNickname()) != _users.end())
		{std::cerr << "User is already in channel" << std::endl; return ;}
	_users[client.getNickname()] = &client;
	systemMessage(client.getNickname() + " has joined the channel");
}

void Channel::removeUser(Client &client)
{
	if (_users.find(client.getNickname()) == _users.end())
		{std::cerr << "User is not in channel" << std::endl; return ;}
	_users.erase(client.getNickname());
	systemMessage(client.getNickname() + " has left the channel");
	if (_operators.find(client.getNickname()) != _operators.end())
		_operators.erase(client.getNickname());
}

void Channel::addOperator(Client &client)
{
	if (_operators.find(client.getNickname()) != _operators.end())
		{std::cerr << "Cannot make " + client.getNickname() + " an operator: User is already an operator in this channel \r\n"; return ;}
	_operators[client.getNickname()] = client;
	client.receiveMsg("You are now an operator on #" + _name);
}

void Channel::removeOperator(Client &client)
{
	_operators.erase(client.getNickname());
	client.receiveMsg("You are no longer an operator on " + _name);
}
void Channel::setTopic(const std::pair<std::string, std::string> &topic)
{
	_topic = topic;
}

const std::pair<std::string, std::string> &Channel::getTopic() const
{
	return _topic;
}


void Channel::inviteUser(Client &invitedClient, Client &inviter)
{
	_invitedUsers.emplace(invitedClient.getNickname(), invitedClient);
}

void Channel::setMode(int mode, std::string value, bool set)
{
	if (mode >= 0 && mode < static_cast<int>(_modes.size()))
	{
		_modes[mode].first = set;
		_modes[mode].second = value;
	}
}

bool Channel::isModeSet(int mode) const
{
	return (_modes[mode].first);
}

bool Channel::isUserInvited(Client &client) const
{
	if (_invitedUsers.find(client.getNickname()) != _invitedUsers.end())
		return true;
	return false;
}

void Channel::systemMessage(const std::string &message)
{
	// std::for_each(_users.begin(), _users.end(), [message](std::pair<std::string, Client > pair) {
	// 	pair.second.receiveMsg(message);
	// });
	for (auto it = _users.begin(); it != _users.end(); it++)
	{
		Client *reciever = getUser(it->first);
		reciever->receiveMsg(message);
	}
}

void Channel::broadcastMessage(const std::string &message, Client &sender)
{
	if (_users.size() == 1)
		return ;
	if (_users.size() == 2)
	{
		if (_users.begin()->second->getNickname() != sender.getNickname())
			_users.begin()->second->receiveMsg(message);
		else
			(++_users.begin())->second->receiveMsg(message);
		return ;
	}
	for (auto it = _users.begin(); it != _users.end(); it++)
	{
		Client *reciever = getUser(it->first);
		reciever->receiveMsg(message);
		if (reciever->getNickname() != sender.getNickname())
			reciever->receiveMsg(message);
	}
}

const std::string& Channel::getName() const
{
	return _name;
}

Client *Channel::getUser(std::string user)
{
	auto it = _users.find(user);
	if (it == _users.end())
		return NULL;
	return (it->second);
}

Client *Channel::getOperator(std::string operatr)
{
	auto it = _operators.find(operatr);
	if (it == _operators.end())
		return NULL;
	return (&it->second);
}

std::string Channel::getModestring() const
{
	std::string modestring = "+";

	if (isModeSet(INVITE_ONLY))
		modestring += "i";
	if (isModeSet(TOPIC_SETTABLE_BY_OPERATOR))
		modestring += "t";
	if (isModeSet(MODERATED))
		modestring += "o";
	if (isModeSet(USER_LIMIT))
		modestring += "l";
	if (isModeSet(KEY))
		modestring += "k";
	if (modestring.size() == 1)
		return "";
	return modestring;
}


std::string Channel::getModesvalues() const
{
	std::string modestring;

	if (isModeSet(USER_LIMIT))
		modestring += _modes[USER_LIMIT].second;
	if (isModeSet(KEY))
		modestring += _modes[KEY].second;
	return modestring;
}

std::string Channel::getModeValue(int mode) const
{
	return _modes[mode].second;
}


int Channel::getUserCount() const
{
	return _users.size();
}

bool Channel::isChannelfull() const
{
	if (!isModeSet(USER_LIMIT))
		return false;
	return (_users.size() == std::atoi(getModeValue(USER_LIMIT).c_str()));
}

std::string Channel::getAllUsers() const
{
	std::string allUsers;
	bool first = true;
	for (const auto& user : _users)
	{
		if (!first)
			allUsers += " ";
		else
			first = false;
		if (_operators.find(user.second->getNickname()) != _operators.end())
			allUsers += "@";
		allUsers += user.second->getNickname();
	}
	return allUsers;
}
