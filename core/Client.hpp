#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/select.h>
#include <string>
#include "ServerMachine.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Logger.hpp"
#include "IO.hpp"

class Client
{
	private:
		int			_fd;
		std::string _rest_of_resp;
		Request		_request;
		Server		_server_info;
		bool		_keep_alive;

		bool 		_keepConnectionAlive() const
		{
			return  _keep_alive;
		}
		void		_clearOldRequest()
		{
			_request = Request();
		}

		bool		_responseIsFullySent() const
		{
			return !_rest_of_resp.length();
		}

		void _setNewResponse();


		bool _sendResponse();

	public:
		Client(int fd, const Server& server_info);
		~Client() {}

		int getFd() const
		{
			return _fd;
		}

		bool hasNewRequest(fd_set *readset) const
		{
			return FD_ISSET(getFd(), readset) != 0;
		}
		
		bool readyToRecvResponse(fd_set *writeset) const
		{
			return FD_ISSET(getFd(), writeset) != 0;
		}
		
		void disconnect() const
		{
			close(getFd());
		}

		bool tryToRecvRequest();
		bool tryToSendResponse();
};

#endif