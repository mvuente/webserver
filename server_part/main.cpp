
#include <sys/types.h> // for types i'm using here (like STRUCT SOCKADDR)
#include <sys/socket.h>// for sockets
#include <netinet/in.h> // for INADDR_ANY and pre-defined structures
#include <unistd.h>
#include <stdio.h>
#include <istream>
#include "Request.hpp"
#include "Response.hpp"
#include "ServerMachine.hpp"
#define PORT 8080

int 	main() {

	// take CONFIG class
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);



	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}


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
	while(1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}
// вопрос с границами цикла. В цикле находится только accept. !
		char buffer[30000] = {0};
		valread = read( new_socket , buffer, 30000); // START TO READ CLIENT QUIERY HERE
		std::string client_msg = std::string(buffer, static_cast<size_t>(valread)); //transform buffer to a std::string
		Request		rqst = Request(client_msg); // Создаю объект класса, работающего со строкой запроса клиента

		//printf("%s\n", buffer );
		//std::cout << "Before respomse constructor" << std::endl;
		Response	rspns = Response();
		//Server		srv = Server(); //написать, из чего его делать
		//std::cout << "Before servermachine constructor" << std::endl;
		ServerMachine		srvm = ServerMachine(rqst, rspns); // добавить srv
		srvm.respHeaderCreater();
		std::string 		responce = srvm.getResponce();
		//std::cout << responce << std::endl;
		char* hello = new char [responce.length()+1];
		std::strcpy (hello, responce.c_str());
		responce = srvm.getBody();
		char* body = new char [responce.length()+1];

		std::strcpy (body, responce.c_str());


		//char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		write(new_socket , hello , strlen(hello));
		write(new_socket , body , strlen(body));
		printf("------------------Hello message sent-------------------\n");
		close(new_socket);
	}
	return 0;
}
