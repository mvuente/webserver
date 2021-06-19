//
// Created by Marceline Vuente on 6/5/21.
//

#ifndef WEBSERV_SERVERMACHINE_HPP
#define WEBSERV_SERVERMACHINE_HPP
#include "Request.hpp"
#include "Response.hpp"
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
//#include "CGI.hpp"

#include <sstream>

class ServerMachine {
public:
	ServerMachine();
	ServerMachine(Request& rqst, Response& rspns); // add , Server& srvr
	~ServerMachine();
	//std::string getResponse();
	Response&			getRspObj(); //временный метод

	bool				checkCGI();
	long 				ResponseCrtr(int fd); // метод отправляет ответ сервера по заданному fd
	void 				methodProcesser();
	std::string			contentTypeProcesser();
	std::string			bodyProcessorMultipart(std::string body, std::string type);
	std::string			bodyProcessorEncoded(std::string	body);
	void				methodGet();
	void				methodPost();

	//long 				rspns_send(int fd);

	// under construction
	//void 		respHeaderCreater();


	int 		getLength();

private:
	Request		_rqst;
	Response	_rspns;
	std::string _coderspns;
	std::string _filepath;// temporary value. Replace by data ROOT from CONFIG
	std::string		_body;
	char*			_bytestream;
	int 			_length;
	//std::ifstream _ifs;
	//Server		_srvr;

};


#endif //_WEBSERV_SERVERMACHINE_HPP
