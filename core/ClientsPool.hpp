#ifndef CLIENTS_POOL_HPP
#define CLIENTS_POOL_HPP

#include <list>
#include <sys/select.h>
#include <cstring>
#include "Client.hpp"
#include "Logger.hpp"

class ClientsPool
{
	typedef std::list<Client> t_clients;

	private:
		fd_set			_readset;
		fd_set 			_writeset;
		fd_set			_readset_copy;
		fd_set 			_writeset_copy;
		t_clients		_clients;
		const timeval	_timeout;
		timeval			_timeout_copy;

		t_clients::iterator	_removeClient(const t_clients::iterator &client);
		
		static void  _copyset(fd_set& dest, const fd_set& src)
		{
			std::memcpy(&dest, &src, sizeof(src));
		}
	
		void	_update()
		{
			_copyset(_readset_copy, _readset);
			_copyset(_writeset_copy, _writeset);
			std::memcpy(&_timeout_copy, &_timeout, sizeof(_timeout));
		}
	public:
		ClientsPool(const fd_set &main_set, const timeval &timeout);
		~ClientsPool();
		int		listen(int maxfd);
		int		addNewClient(int listener_fd, const Server& server_info);
		void	tryToSendResponses();
		void	lookUpNewRequests();

		bool	hasNewClient(int listener_fd) const
		{
			return FD_ISSET(listener_fd, &_readset_copy) != 0;
		}
};

#endif