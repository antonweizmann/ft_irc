#include "Server.hpp"

void	Server::CAP(int fd)
{
	sendResponse("CAP * LS :", fd);
	// if (!cmd[0].compare("CAP LS 302"))
	// 		{sendResponse("CAP * LS :\r\n", fd); return ;}
	// if (!cmd[0].compare("CAP REQ :"))
	// 	sendResponse("CAP * ACK :\r\n", fd);
}
