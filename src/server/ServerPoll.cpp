#include "Server.hpp"

void	Server::poll()
{
	while (_signal == false)
	{
		if (::poll(&_sockets[0], _sockets.size(), -1) == -1 && !_signal)
			throw(std::runtime_error("polling failed"));
		for (size_t i = 0; i < _sockets.size(); ++i)
		{
			if (_sockets[i].revents & POLLIN)
			{
				if (_sockets[i].fd == _server_socket_fd)
					acceptClient();
				else
					receiveData(_sockets[i].fd);
			}
		}
	}
	shutdown();
}

void splitData(std::string data, std::vector<std::string> &cmd)
{
	std::istringstream	strm(data);
	std::string			line;
	while (std::getline(strm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		cmd.push_back(line);
	}
}

void	Server::receiveData(int fd)
{
	char	buff[1024];
	std::vector<std::string> cmd;
	// memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);
	buff[bytes] = 0;
	if (bytes <= 0)
	{
		std::cout << RED << "Client " << fd << " has disconnected" << std::endl;
		clearClient(fd);
		close(fd);
	} else {
		std::cout << MAGENTA << "Client " << fd << " data: " << RESET << buff << std::endl;
		splitData(buff, cmd);
		if (!cmd[0].compare("CAP LS 302"))
			{sendResponse("CAP * LS :\r\n", fd); return ;}
		// if (!cmd[0].compare("CAP REQ :"))
		// 	sendResponse("CAP * ACK :\r\n", fd);
		for (auto it = cmd.begin(); it != cmd.end(); it++)
			parseCommand(*it, fd);
	}
}

void	Server::parseCommand(const std::string command, int fd)
{
	std::string	cmd;
	std::vector<std::string> args;
	size_t	cmd_end = command.find_first_of(' ');
	size_t	colon = command.find_first_of(':');

	if (cmd_end == std::string::npos)
		cmd_end = command.size();
	cmd_end = std::min(colon, cmd_end);
	cmd = command.substr(0, cmd_end);
	args = parseArgs(command.substr(cmd_end), fd);

	std::cout << std::endl << "PARSED COMMAND:" << std::endl;
	std::cout << '"' << cmd << '"' << std::endl;
	std::cout << std::endl << "ARGS:" << std::endl;
	for (auto it = args.begin(); it != args.end(); it++)
		std::cout << '"' << *it << '"' << std::endl;
	std::cout << std::endl;
	cmdDecide(cmd, args, fd);
}

std::vector<std::string>	Server::parseArgs(const std::string command_args, int fd)
{
	std::vector<std::string>	args;
	size_t	colon;
	std::stringstream obj(command_args);
	std::string	temp;

	while (obj >> temp)
	{
		colon = temp.find_first_of(':');
		if (colon != std::string::npos)
		{
			if (colon > 0)
				args.emplace_back(temp.substr(0, colon));
			break;
		}
		args.emplace_back(temp);
	}
	colon = command_args.find_first_of(':');
	if (colon != std::string::npos)
		args.emplace_back(command_args.substr(colon + 1));
	return args;
}

void	Server::cmdDecide(const std::string cmd, const std::vector<std::string> args, int fd)
{
	std::vector<std::pair<std::string, void (Server::*)(std::vector<std::string>, int)>>	commands;
	Client	&client = _clients[fd];
	std::string cmd_upper = cmd;

	transform(cmd.begin(), cmd.end(), cmd_upper.begin(), toupper);
	if (client.getAuth() == 2)
	{
		if ((cmd_upper != "PASS"))
			sendResponse(ERR_NOTREGISTERED(_clients[fd].getNickname()), fd);
		else
			PASS(args, fd);
		return;
	}
	else if (client.getAuth() == 1)
	{
		if ((cmd_upper == "USER"))
			USER(args, fd);
		else if ((cmd_upper == "NICK"))
			NICK(args, fd);
		else
			sendResponse(ERR_NOTREGISTERED(_clients[fd].getNickname()), fd);
		if (!client.getNickname().empty() && !client.getUsername().empty())
		{
			client.setAuth(0);
			finishRegistration(fd);
		}
		return;
	}

	CMD_PAIR(MODE);
	CMD_PAIR(JOIN);
	CMD_PAIR(PART);
	CMD_PAIR(TOPIC);
	CMD_PAIR(KICK);
	CMD_PAIR(INVITE);
	CMD_PAIR(USER);
	CMD_PAIR(NICK);
	CMD_PAIR(PRIVMSG);
	for (auto it = commands.begin(); it != commands.end(); it++)
	{
		if (!it->first.compare(cmd_upper))
		{
			(this->*(it->second))(args, fd);
			break;
		}
	}
}