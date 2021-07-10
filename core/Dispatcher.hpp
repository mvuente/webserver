//
// Created by Marceline Vuente on 6/17/21.
//

#ifndef JOINT_DISPATCHER_HPP
#define JOINT_DISPATCHER_HPP

#include <sys/select.h> // for select function
#include <sys/types.h> // for types i'm using here (like STRUCT SOCKADDR)
#include <sys/time.h>
#include <sys/socket.h>// for sockets
#include <netinet/in.h> // for INADDR_ANY and pre-defined structures
#include <fcntl.h> // для неблокирующих сокетов
#include <set> // для массива fd клиентов
#include <algorithm>
#include <istream>
#include <unistd.h>
#include <errno.h>
#include <csignal>
#include <exception>
#include "ClientsPool.hpp"
#include "Config.hpp"
#include "Logger.hpp"
//#include <stdlib.h>

class Dispatcher {
public:
	Dispatcher(std::string path);
	~Dispatcher();
	void 		prep();
	void 		run();

	class ConfigException : public std::exception
	{};

	class InvalidConfigException : public ConfigException
	{
	public:
		virtual const char* what() const throw();
	};

	class EmptyConfigException : public ConfigException
	{
	public:
		virtual const char* what() const throw();
	};

	class InterruptException : public std::exception
	{};
private:
	Dispatcher();

	Config					_config; //экземпляр класса с распарсенным конфигом
	std::map<int, Server*>	_listeners; // словарь слушающих сокетов и соответствующих им серверов
	fd_set					_fd_set;
	int 					_maxfd; // максимальный fd  в наборе сокетов
	static bool				__isInterruptSignal;

	void					_lookUpNewClients(ClientsPool &clientPool);
	void					_throwIterruptException() const;
	static bool				_isInterruptSignal();
	static void				_setIsInterruptSignal();
	static void				_handleInterrupt(int signal);
};


#endif //JOINT_DISPATCHER_HPP
