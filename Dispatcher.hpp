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
#include <set> // для массива fd клиентов
#include <algorithm>
#include <istream>
#include "Request.hpp"
#include "Response.hpp"
#include "ServerMachine.hpp"
#include <unistd.h>
#include <errno.h>

class Dispatcher {
public:
	Dispatcher(int serv_fd, struct sockaddr_in address, int addrlen);
	~Dispatcher();
	void 	run();
	int 	mymax(std::set<int> myset);
private:
	Dispatcher();

	fd_set				_fd_set;
	int 				_addrlen; // размер структуры при создании сокета
	struct sockaddr_in 	_address;
	int 				_serv_fd; // слушающий fd данного сервера
	int 				_maxfd; // максимальный fd для данного селекта
	int 				_debag;
	ServerMachine		_srvm;
};


#endif //JOINT_DISPATCHER_HPP
