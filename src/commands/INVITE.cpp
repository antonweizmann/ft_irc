# include "Server.hpp"

void Server::INVITE(std::vector<std::string> cmd, int fd)
{
	Client		&sender = *(getClient(fd));
	if (cmd.size() < 2)
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNickname(), (cmd.empty() ? "INVITE" : "INVITE " + cmd[0])), fd); return;}
	std::string target = cmd[0];
	std::string channelnick = cmd[1].substr(1);
	if (cmd[1][0]!= '#' || _channels.find(channelnick) == _channels.end())
		{sendResponse(ERR_NOSUCHCHANNEL(sender.getNickname(), cmd[1]), fd); return;}
	Channel		&channel = _channels[channelnick];
	if (!channel.getUser(sender.getNickname())) // check if sender is in channel
		{sendResponse(ERR_NOTONCHANNEL(sender.getNickname(), channelnick), fd); return ;}
	if (!getClient(target))
		{sendResponse(ERR_NOSUCHNICK(sender.getNickname(), target), fd); return ;}
	if (channel.isModeSet(INVITE_ONLY) && !channel.getOperator(sender.getNickname())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender.getNickname(), channelnick), fd); return ;}
	if (channel.getUser(target))
		{sendResponse(ERR_USERONCHANNEL(sender.getNickname(), target, channelnick), fd); return ;}
	Client		&toInv = *(getClient(target));
	if (!channel.isUserInvited(toInv))
	{
		channel.inviteUser(toInv, sender);
		sendResponse(RPL_INVITING(sender.getNickname(), target, channelnick), fd);
		toInv.receiveMsg(":" + sender.getNickname() + "!" + sender.getUsername() + "@localhost INVITE " + target + " #" + channelnick);
	}
}
