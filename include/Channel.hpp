#pragma once
#include <unordered_map>
#include <string>
#include <cctype>
#include <algorithm>
#include <bitset>
#include <vector>
#include "Client.hpp"

enum ChannelModes {
	INVITE_ONLY,
	TOPIC_SETTABLE_BY_OPERATOR,
	MODERATED,
	USER_LIMIT,
	KEY,
	COUNT
};

class Client;

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		std::unordered_map<std::string, Client > _users;
		std::unordered_map<std::string, Client > _operators;
		std::unordered_map<std::string, Client > _invitedUsers;
		std::vector<std::pair<bool, std::string> > _modes;

	public:
		Channel() {};
		Channel(const std::string &channelName);
		Channel(const std::string &channelName, const std::string &key);
		~Channel();
		void addUser(Client &client);
		void removeUser(Client &client);
		void addOperator(Client &client);
		void removeOperator(Client &client);

		//Topic Handling
		void setTopic(const std::string &topic);
		const std::string &getTopic() const;

		//Modes Handling
		void setMode(int mode, std::string value, bool set);
		bool isModeSet(int mode) const;
		std::string getModestring() const;
		std::string getModesvalues() const;

		//Invite Handling
		void inviteUser(Client &invitedClient, Client &inviter);
		bool isUserInvited(Client &client) const;

		//Getters
		const std::string &getName() const;
		const int getUserLimit() const;
		Client *getUser(std::string user);
		Client *getOperator(std::string operatr);
		//Messages
		void broadcastMessage(const std::string &message, Client &sender);
		void systemMessage(const std::string &message);

		void closeChannel(void);
};
