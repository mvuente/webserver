//
// Created by Marceline Vuente on 6/5/21.
//

#ifndef _WEBSERV_SERVERMACHINE_HPP
#define _WEBSERV_SERVERMACHINE_HPP
#include "Request.hpp"
#include "Response.hpp"

class ServerMachine {
public:
	ServerMachine(Request& rqst, Response& rspns); // add , Server& srvr
	std::string CodeOfRespns();
	void 		respCreater();
	std::string getResponce();
private:
	Request		_rqst;
	Response	_rspns;
	//Server		_srvr;

};


#endif //_WEBSERV_SERVERMACHINE_HPP
