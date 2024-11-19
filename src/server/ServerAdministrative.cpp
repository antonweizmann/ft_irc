#include "Server.hpp"

void	Server::signalHandler(int signal)
{
	std::cout << std::endl << "Signal " << signal << " received!" << std::endl;
	_signal = true;
}

/**
 * @brief sets up signal handlers and the server socket.
 * on error the server gets shut down cleanly
 */
void	Server::init()
{
	try {
		signal(SIGINT, signalHandler);
		signal(SIGQUIT, signalHandler);
		// if (signal(SIGINT, signalHandler) == SIG_ERR) // "ctrl + c"
		// 	throw(std::runtime_error("setting signal handler failed"));
		// if (signal(SIGQUIT, signalHandler) == SIG_ERR); // "ctrl + \"
		// 	throw(std::runtime_error("setting signal handler failed"));

		setupServerSocket();
		std::cout << GREEN << "Server " << _server_socket_fd << " started" << RESET << std::endl;
		std::cout << "Waiting for connections" << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << RED << "Error while initializing server:" << std::endl;
		std::cout << e.what() << RESET << std::endl;
		shutdown();
		throw(e);
	}
}

void	Server::setupServerSocket()
{
	_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0); // create IPv4 TCP socket
	if (_server_socket_fd == -1)
		throw(std::runtime_error("server socket creation failed"));

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port); // convert from .host to network byte order
	addr.sin_addr.s_addr = INADDR_ANY;
	int value = 1;
	if (fcntl(_server_socket_fd, F_SETFL, O_NONBLOCK) == -1) // make fd nonblocking for MacOS
		throw(std::runtime_error("setting server socket to `NONBLOCK` failed"));
	if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
		throw(std::runtime_error("setting server socket option `REUSEADDR` failed"));
	if (bind(_server_socket_fd, (struct sockaddr *)&addr, sizeof(sockaddr_in)) == -1)
		throw(std::runtime_error("server socket binding failed"));
	if (listen(_server_socket_fd, SOMAXCONN) == -1) // wait for connections
		throw(std::runtime_error("server socket listening failed"));

	struct pollfd new_poll;
	new_poll.fd = _server_socket_fd;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	_sockets.push_back(new_poll);
}

void	Server::acceptClient()
{
	Client		client;
	sockaddr_in	addr;
	socklen_t	len = sizeof(addr);

	int	client_fd = accept(_server_socket_fd, (sockaddr *)&addr, &len);
	if (client_fd == -1)
		throw (std::runtime_error("accepting client failed"));

	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("setting client socket to `NONBLOCK` failed"));

	pollfd	client_poll;
	client_poll.fd = client_fd;
	client_poll.events = POLLIN | POLLOUT | POLLHUP;
	client_poll.revents = 0;

	client.setFd(client_fd);
	client.setIpAddr(inet_ntoa(addr.sin_addr));
	_clients[client_fd] = client;
	_sockets.push_back(client_poll);

	std::cout << GREEN << "Client " << client_fd << " connected" << RESET << std::endl;
}

void	Server::finishRegistration(int fd)
{
	std::string	msg;
	Client		client = _clients[fd];
	std::string version = "0.69";

	sendResponse(RPL_WELCOME(client.getNickname(), _name, client.getNickname()), fd);
	sendResponse(RPL_YOURHOST(client.getNickname(), _name, version), fd);
	sendResponse(RPL_CREATED(client.getNickname(), "today"), fd);
	sendResponse(RPL_MYINFO(client.getNickname(), _name, version, "-", "itkol", "kl"), fd);
	sendResponse(RPL_ISUPPORT(), fd);
	// sendResponse(ERR_NOMOTD(client.getNickname()), fd);
}

void	Server::shutdown()
{
	std::cout << RED;
	for (auto it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << "Disconnecting client " << it->first << std::endl;
		sendResponse("ERROR :server shutdown", it->first);
		close(it->first);
	}
	if (_server_socket_fd != -1)
	{
		std::cout << "Shutting down server " << _server_socket_fd << std::endl;
		close(_server_socket_fd);
	}
	std::cout << RESET;
}

void	Server::clearClient(int fd)
{
	std::cout << RED << "Disconnecting client " << fd << RESET << std::endl;
	for (auto it = _sockets.begin(); it != _sockets.end();)
		if (it->fd == fd)
			it = _sockets.erase(it);
		else
			it++;
	Client &me = *getClient(fd);
	for (auto it = _channels.begin(); it != _channels.end(); it++)
	{
		std::cout << it->first << std::endl;
		Channel &curChannel = it->second;
		if (curChannel.getUser(me.getNickname()))
			partClient(me, curChannel);
		if (curChannel.getUserCount() == 0)
		{
			it =_channels.erase(it);
			if (it == _channels.end())
				break;
		}
	}
	close(fd);
	_clients.erase(fd);
}

void Server::partClient(Client &sender, Channel &curChannel)
{
	std::string reason = "Gotta gooo bye bye";
	
	std::stringstream ss;
	ss << ":" << sender.getNickname() << "!" << sender.getUsername() << "@" << "localhost" << " PART #" << curChannel.getName() << " :" << reason;
	curChannel.systemMessage(ss.str());
	curChannel.removeUser(sender);
	if (curChannel.getOperator(sender.getNickname()))
		curChannel.removeOperator(sender);
	if (curChannel.isUserInvited(sender.getNickname()))
		curChannel.removeInvited(sender);
}
