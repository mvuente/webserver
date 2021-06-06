//
// Created by Marceline Vuente on 6/5/21.
//

#include "ServerMachine.hpp"

ServerMachine::ServerMachine(Request& rqst, Response& rspns) : _rqst(rqst), _rspns(rspns) // add , Server& srvr and , _srvr(srvr)
{

}

std::string ServerMachine::CodeOfRespns()
{
	return "200 OK";
}

void 		ServerMachine::respCreater()
{
	this->_rspns.setRSP_START_LINE(this->CodeOfRespns());
	this->_rspns.setRSP_BODY("Hello, World!");
	this->_rspns.setRSP_MSG();

}

std::string ServerMachine::getResponce()
{
	return (this->_rspns.getRSP_START_LINE() + this->_rspns.getRSP_MSG() + this->_rspns.getRSP_BODY());
}