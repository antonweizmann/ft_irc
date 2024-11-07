#pragma once
// Registration
# define RPL_WELCOME(client, networkname, nick) (":irc.andiarbeit.com 001 " + client + " :Welcome to the " + networkname + ", " + nick) //todo  !user@host
# define RPL_YOURHOST(client, servername, version) (":irc.andiarbeit.com 002 " + client + " :Your host is " + servername + ", running version " + version)
# define RPL_CREATED(client, datetime) (":irc.andiarbeit.com 003 " + client + " :This server was created " + datetime)
# define RPL_MYINFO(client, servername, version, usermodes, channelmodes, channelmodesArgs) (":irc.andiarbeit.com 004 " + client + " " + servername + " " + version + " " + usermodes + " " + channelmodes + " " + channelmodesArgs) //todo more stuff
# define RPL_ISUPPORT() (":irc.andiarbeit.com 005 ")

// Replies
# define RPL_CHANGEDMODE(nick, host, channel, modes, args) (":" + nick + "!" + host + "@localhost MODE  " + (channel[0] == '#' ? "" : "#") + channel + " " + modes + " " + args)
# define RPL_CHANNELMODEIS(nick, channel, modes, mode_values) (":irc.andiarbeit.com 324 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " " + modes + " " + mode_values)
# define RPL_NOTOPIC(nick, channel) (":irc.andiarbeit.com 331 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :No topic is set")
# define RPL_TOPIC(nick, channel, topic) (":irc.andiarbeit.com 332 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :" + topic)
# define RPL_TOPICWHOTIME(nick, channel, senderTime) (":irc.andiarbeit.com 333 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " " + senderTime)
# define RPL_INVITING(nick, user_nick, channel) (":irc.andiarbeit.com 341 "+ nick + " " + user_nick + " " + (channel[0] == '#' ? "" : "#") + channel)
# define RPL_NAMREPLY(nick, channel, allUsers) (":irc.andiarbeit.com 353 " + nick + " = " + (channel[0] == '#' ? "" : "#") + channel + " :" + allUsers)
# define RPL_ENDOFNAMES(nick, channel) (":irc.andiarbeit.com 366 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " " + ":irc.andiarbeit.com End of /NAMES list")

// Errors
# define ERR_NOSUCHNICK(nick, user_nick) (":irc.andiarbeit.com 401 " + nick + " " + user_nick + " :No such nick/channel")
# define ERR_NOSUCHCHANNEL(nick, channel) (":irc.andiarbeit.com 403 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :No such channel")
# define ERR_NORECIPIENT(nick) (":irc.andiarbeit.com 411 " + nick + " :No recipient given")
# define ERR_NOTEXTTOSEND(nick) (":irc.andiarbeit.com 412 " + nick + " :No text to send")
# define ERR_NOMOTD(nick) (":irc.andiarbeit.com 422 " + nick + " :MOTD File is missing")
# define ERR_NONICKNAMEGIVEN(nick) (":irc.andiarbeit.com 431 " + nick + " :No nickname given")
# define ERR_ERRONEUSNICKNAME(nick, falseNick) (":irc.andiarbeit.com 432 " + nick + " " + falseNick + " :Erroneus nickname")
# define ERR_NICKNAMEINUSE(nick, falseNick) (":irc.andiarbeit.com 433 " + nick + " " + falseNick + " :Nickname already in use")
# define ERR_USERNOTINCHANNEL(nick, user_nick, channel) (":irc.andiarbeit.com 441 " + nick + " " + user_nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :They aren't on that channel")
# define ERR_NOTONCHANNEL(nick, channel) (":irc.andiarbeit.com 442 " + nick + "  #" + channel + " :You're not on that channel")
# define ERR_USERONCHANNEL(nick, user_nick, channel) (":irc.andiarbeit.com 443 " + nick + " " + user_nick + "  " + (channel[0] == '#' ? "" : "#") + channel + " :is already on channel")
# define ERR_NOTREGISTERED(client) (":irc.andiarbeit.com 451 " + client + " :You have not registered")
# define ERR_NEEDMOREPARAMS(nick, cmd) (":irc.andiarbeit.com 461 " + nick + " " + cmd + " :Not enough parameters")
# define ERR_ALREADYREGISTERED(client) (":irc.andiarbeit.com 462 " + client + " :You may not reregister")
# define ERR_PASSWDMISMATCH(client) (":irc.andiarbeit.com 464 " + client + " :Password incorrect")
# define ERR_CHANNELISFULL(nick, channel) (":irc.andiarbeit.com 471 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :Cannot join channel (+l)")
# define ERR_INVITEONLYCHAN(nick, channel) (":irc.andiarbeit.com 473 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :Cannot join channel (+i)")
# define ERR_BADCHANNELKEY(nick, channel) (":irc.andiarbeit.com 475 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :Cannot join channel (+k)")
# define ERR_CHANOPRIVSNEEDED(nick, channel) (":irc.andiarbeit.com 482 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :You're not channel operator")
# define ERR_UMODEUNKOWNFLAG(nick) (":irc.andiarbeit.com 501 " + nick + " :Unknown MODE flag")
# define ERR_INVALIDMODEPARAM(nick, target, mode, param, description) (":irc.andiarbeit.com 696 " + nick + " " + (target[0] == '#' ? "" : "#") + target + " " + mode + " " + param + " :" + description)
