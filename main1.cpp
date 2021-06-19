#include "Dispatcher.hpp"
#include <sys/types.h> // for types i'm using here (like STRUCT SOCKADDR)
#include <sys/socket.h>// for sockets
#include <sys/select.h>
#include <netinet/in.h> // for INADDR_ANY and pre-defined structures
#include <fcntl.h> // для неблокирующих сокетов
#include <stdio.h>
#include <istream>
#include "Request.hpp"
#include "Response.hpp"
#include "ServerMachine.hpp"

#define PORT 8080

int 	main() {

	// take CONFIG class
	int server_fd, new_socket; long valread;
	struct sockaddr_in	address;
	int 				addrlen = sizeof(address);



	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // сокет для ДАННОГО адреса
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}
	fcntl(server_fd, F_SETFL, O_NONBLOCK); // сделали полученный сокет неблокирующим

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	memset(address.sin_zero, '\0', sizeof address.sin_zero);


	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	Dispatcher	disp(server_fd, address, addrlen);
	disp.run();

	return 0;
}

