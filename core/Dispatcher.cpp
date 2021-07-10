
#include "Dispatcher.hpp"

Dispatcher::Dispatcher(std::string path) : _maxfd(0)
{
	if (this->_config.parse(path))
		throw Dispatcher::InvalidConfigException();
	if (this->_config.getServers().size() == 0)
		throw Dispatcher::EmptyConfigException();
	__isInterruptSignal = false;
	FD_ZERO(&_fd_set);
	signal(SIGINT, &Dispatcher::_handleInterrupt);
}

Dispatcher::~Dispatcher()
{
	for (std::map<int, Server*>::iterator it = _listeners.begin(); it != _listeners.end() ; ++it)
		close(it->first);
}

void 		Dispatcher::prep()
{
	int 							server_fd;
	long 							valread;
	struct sockaddr_in				address;
	int 							addrlen = sizeof(address);

	(void)addrlen;
	(void)valread;

	// Creating socket file descriptor
	std::list<Server*>::iterator 	it;
	Logger::println("Servers are", _config.getServers().size());
	for (it = _config.getServers().begin(); it != _config.getServers().end(); ++it) // making "listeners" for every port, mentioned in config
	{
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		{
			perror("In socket");
			exit(EXIT_FAILURE);
		}
		fcntl(server_fd, F_SETFL, O_NONBLOCK); // make the socket non-blocking
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons((*it)->getPort());

		memset(address.sin_zero, '\0', sizeof address.sin_zero);
		Logger::println("Port is", (*it)->getPort());
		Logger::println("Host is", (*it)->getHost());
		int 						opt = 1;
		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // to avoid stopping port
		if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
		{
			perror("In bind");
			exit(EXIT_FAILURE);
		}
		if (listen(server_fd, 1000) < 0)
		{
			perror("In listen");
			exit(EXIT_FAILURE);
		}
		_listeners[server_fd] = *it; //listeners array
		FD_SET(server_fd, &_fd_set);
		_maxfd = server_fd;
	}
}

void 		Dispatcher::run()
{
	static const timeval 			timeout = {1, 0};
	ClientsPool 					cliPool(_fd_set, timeout);

	while(!_isInterruptSignal())
	{
		cliPool.listen(_maxfd);
		cliPool.lookUpNewRequests();
		cliPool.tryToSendResponses();
		_lookUpNewClients(cliPool);
	}
	_throwIterruptException();
}

void		Dispatcher::_lookUpNewClients(ClientsPool &clientsPool)
{
	for (std::map<int, Server*>::iterator mit = _listeners.begin(); mit != _listeners.end(); ++mit)
	{
		if (clientsPool.hasNewClient(mit->first))
		{
			int client_fd = clientsPool.addNewClient(mit->first, *mit->second); // -1 on error
			_maxfd = std::max(client_fd, _maxfd);
		}
	}
}

const char* Dispatcher::InvalidConfigException::what() const throw()
{
		return "Error:: Invalid config";
}

const char* Dispatcher::EmptyConfigException::what() const throw()
{
		return "Error:: Config is empty";
}

bool 		Dispatcher::__isInterruptSignal = false;

void		Dispatcher::_setIsInterruptSignal()
{
	__isInterruptSignal = true;
}

bool		Dispatcher::_isInterruptSignal()
{
	return __isInterruptSignal;
}

void		Dispatcher::_throwIterruptException() const
{
	throw InterruptException();
}

void		Dispatcher::_handleInterrupt(int signal)
{
	Logger::println("<Interrupt handler> signal is ", signal);
	_setIsInterruptSignal();
}
