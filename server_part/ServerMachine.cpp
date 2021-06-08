//
// Created by Marceline Vuente on 6/5/21.
//

#include "ServerMachine.hpp"

ServerMachine::ServerMachine(Request& rqst, Response& rspns) : _rqst(rqst), _rspns(rspns) // add , Server& srvr and , _srvr(srvr)
{
	this->_rspns.setRSP_BODY("Hello, scoundrels!");
	this->_filepath = "/Users/mvuente/Documents/web"; // take it from config
}

void		ServerMachine::methodGet()
{
	std::string 		tmpval = this->_rqst.getData()["source"];
	if (tmpval == "/") // открыть запрошенный файл
		this->_rspns.setRSP_START_LINE("200 OK");
	else
	{
		std::string 	filename = _filepath + tmpval;
		std::ifstream 	ifs(filename, std::ifstream::in); //открываю искомый файл
		if (!ifs)
		{
			this->_rspns.setRSP_START_LINE("404 FILE IS NOT FOUND");
			this->_rspns.setRSP_BODY("ERROR 404. Resource you've requested is unavailable");
		}
		else
		{
			bool 		flag(true);
			std::string tmpbody = this->_rspns.getRSP_BODY();
			while (ifs)
			{
				std::string	tmpstr;
				if (flag)
				{
					tmpbody.clear();
				}
				if (!tmpbody.empty())
					tmpbody += "\n";
				std::getline(ifs, tmpstr);
				tmpbody += tmpstr;
				flag = false;
			}
			ifs.close();
			this->_rspns.setRSP_BODY(tmpbody);
			this->_rspns.setRSP_START_LINE("200 OK");
		}
	}
}


void	ServerMachine::ResponseCrtr()
{
	if (!_rqst.methodValidator())
		this->_rspns.setRSP_START_LINE("400 METHOD IS UNKNOWN");  //CHECK ERROR CODES
	else
	{
		if (this->_rqst.getMethod() == "GET")
			this->methodGet();
//		else if (this->_rqst.getMethod() == "POST")
//			return this->methodPost();
//		else
//			return this->methodDelete();
	}
}



int 		ServerMachine::getLength()
{
	return this->_length;
}

Response&	ServerMachine::getRspObj()
{
	return this->_rspns;
}

std::string 	ServerMachine::getResponce()
{
	this->_rspns.setRSP_MSG();
	return (this->_rspns.getRSP_START_LINE() + this->_rspns.getRSP_MSG());// + this->_rspns.getRSP_BODY());
}