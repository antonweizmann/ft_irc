
#pragma once

#include <vector>
#include <poll.h>
#include <csignal>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sstream>
#include <regex>
#include "ft_irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "respond.hpp"

#define CMD_PAIR(cmd) commands.push_back(std::make_pair(#cmd, &Server::cmd));

class Server
{
	private:
		static bool _signal;
		int _port;
		int _server_socket_fd;
		std::vector<struct pollfd> _sockets;
		std::unordered_map<int, Client> _clients;
		std::unordered_map<std::string, Channel> _channels;
		std::string			_name;
		std::string			_pass;
	public:
		Server(int port);
		Server(int port, std::string pass);
		~Server();

		// administrative
		static void	signalHandler(int signal);
		void		init();
		void		setupServerSocket();
		void		acceptClient();
		void		finishRegistration(int fd);
		void		shutdown();
		void		clearClient(int fd);

		// getter
		Client*		getClient(int fd);
		Client*		getClient(std::string nick);

		// polling
		void						poll();
		void						receiveData(int fd);
		void						parseCommand(const std::string command, int fd);
		std::vector<std::string>	parseArgs(const std::string command_args, int fd);
		void						cmdDecide(const std::string cmd, const std::vector<std::string> args, int fd);
		void						sendData(int fd);

		void		sendResponse(std::string message, int fd);
		bool		userLimit(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator &argsIt, std::string &argsReport, int fd);
		bool		channelKey(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator &argsIt, std::string &argsReport, int fd);
		bool		operatorPriv(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator &args_it, std::string &argsReport, int fd);
		void		systemMessage(const std::string &message);

		// Commands
		void	KICK(std::vector<std::string> cmd, int fd);
		void	MODE(std::vector<std::string> cmd, int fd);
		void	INVITE(std::vector<std::string> cmd, int fd);
		void	TOPIC(std::vector<std::string> cmd, int fd);
		void	PART(std::vector<std::string> cmd, int fd);
		void	JOIN(std::vector<std::string> cmd, int fd);
		void	leaveAllChannels(Client &sender);
		void	NICK(std::vector<std::string> cmd, int fd);
		bool	checkDuplicate(std::string nick);
		void	USER(std::vector<std::string> cmd, int fd);
		void	PASS(std::vector<std::string> cmd, int fd);
		void	PRIVMSG(std::vector<std::string> cmd, int fd);
		void	CAP(int fd);
		void	QUIT(std::vector<std::string> cmd, int fd);
};

void splitComma(std::string &cmd, std::vector<std::string> &split);
