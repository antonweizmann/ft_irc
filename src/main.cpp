
#include <string>
#include "ft_irc.hpp"
#include "Server.hpp"

int	main(int argc, char **argv)
{
	std::string	pass;
	int	port;

	if (argc != 3)
	{
		std::cout << "usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	try
	{
		port = std::stoi(argv[1]);
	}
	catch (std::exception &e)
	{
		std::cout << "Port out of range" << std::endl;
	}
	if (port <= PRIVILEGED_PORTS || port >= PRIVATE_PORTS)
	{
		if (port < 0)
			std::cout << "Port has to be positive" << std::endl;
		else if (port <= PRIVILEGED_PORTS)
			std::cout << "Ports below " << PRIVILEGED_PORTS + 1 << " are privileged" << std::endl;
		else if (port > PRIVATE_PORTS)
			std::cout << "Ports above " << PRIVATE_PORTS - 1 << " are privat" << std::endl;
		else if (port > MAX_PORTS)
			std::cout << "Port has to be lower than " << MAX_PORTS - 1 << std::endl;
		return (1);
	}
	std::cout << port << std::endl;
	pass = std::string(argv[2]);

	Server server(port, pass);
	try
	{
		server.init();
	}
	catch (const std::exception &e)
	{
		return 1;
	}
	try
	{
		server.poll();
	}
	catch (const std::exception &e)
	{
		return 1;
	}
}
