#include "ClientsPool.hpp"

ClientsPool::ClientsPool(const fd_set &main_set, const timeval &timeout) :
	_readset(),
	_writeset(),
	_clients(),
	_timeout(timeout)
{
	FD_ZERO(&_writeset);
	_copyset(_readset, main_set);
	_update();
}

ClientsPool::~ClientsPool()
{
	for (t_clients::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		it->disconnect();
}

int		ClientsPool::listen(int maxfd)
{
	static int		active;

	do {
		_update();
		active = select(maxfd + 1, &_readset_copy, &_writeset_copy, NULL, &_timeout_copy);
		if (active == -1) Logger::println("[Select error] errno is", errno);
	} while (active == 0);
	return active;
}

int		ClientsPool::addNewClient(int listener_fd, const Server& server_info)
{
	int	client_fd = accept(listener_fd, NULL, NULL);
	if (client_fd != -1)
	{
		Logger::println("New client with fd =", client_fd);
		FD_SET(client_fd, &_readset);
		FD_SET(client_fd, &_writeset);
		_clients.push_back(Client(client_fd, server_info));
	}
	return client_fd;
}

void	ClientsPool::tryToSendResponses()
{
	t_clients::iterator it = _clients.begin();
	
	while (it != _clients.end())
	{
		if (it->readyToRecvResponse(&_writeset_copy))
		{
			if (it->tryToSendResponse())
			{
				it = _removeClient(it);
				continue ;
			}
		}
		++it;
	}
}

void	ClientsPool::lookUpNewRequests()
{
	t_clients::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		if (it->hasNewRequest(&_readset_copy))
		{
			if (it->tryToRecvRequest())
			{
				it = _removeClient(it);
				continue ;
			}

		}
		++it;
	}
}

ClientsPool::t_clients::iterator	ClientsPool::_removeClient(const t_clients::iterator &client)
{
	Logger::println("[ FD:", client->getFd(), "] disconnect");
	FD_CLR(client->getFd(), &_readset);
	FD_CLR(client->getFd(), &_writeset);
	client->disconnect();
	return _clients.erase(client);
}
