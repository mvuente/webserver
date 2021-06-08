//
// Created by Marceline Vuente on 6/5/21.
//

#ifndef _WEBSERV_SERVERMACHINE_HPP
#define _WEBSERV_SERVERMACHINE_HPP
#include "Request.hpp"
#include "Response.hpp"
#include <fstream>

class ServerMachine {
public:
	ServerMachine(Request& rqst, Response& rspns); // add , Server& srvr

	std::string getResponce(); // ПОКА складывает стартовую строку и остальные хелеры. Тело делаю отдельно. Потом совмещу
	Response&	getRspObj(); //временный метод

	void ResponseCrtr();
	void	methodGet();

	// under construction
	void 		respHeaderCreater();


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
