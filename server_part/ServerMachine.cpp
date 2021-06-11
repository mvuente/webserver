//
// Created by Marceline Vuente on 6/5/21.
//

#include "ServerMachine.hpp"

ServerMachine::ServerMachine(Request& rqst, Response& rspns) : _rqst(rqst), _rspns(rspns) // add , Server& srvr and , _srvr(srvr)
{

	this->_filepath = "/Users/mvuente/Documents/web"; // take it from config
}

ServerMachine::~ServerMachine() {}

void		ServerMachine::methodGet()
{
	std::string 		tmpval = this->_rqst.getSource();
	if (tmpval == "/") // открыть запрошенный файл
		this->_rspns.setRSP_STATUS_LINE("200 OK");
	else
	{
		std::string 	filename = _filepath + tmpval;
		std::ifstream 	ifs(filename, std::ifstream::in); //открываю искомый файл
		if (!ifs)
		{
			this->_rspns.setRSP_STATUS_LINE("404 FILE IS NOT FOUND");
			this->_rspns.setRSP_BODY("ERROR 404. Resource you've requested is unavailable");
		}
		else
		{
			std::string tmpbody = "";
			while (ifs)
			{
				std::string	tmpstr;
				if (!tmpbody.empty())
					tmpbody += "\n";
				std::getline(ifs, tmpstr);
				tmpbody += tmpstr;
			}
			ifs.close();

			this->_rspns.setRSP_BODY(tmpbody);
			this->_rspns.setRSP_STATUS_LINE("200 OK");
		}
	}
	std::cout << "nothing" << std::endl;
	this->_rspns.setRSP_MSG();
}

//void	ServerMachine::methodPost()
//{
//
//}

void	ServerMachine::ResponseCrtr()
{
	if (!_rqst.methodValidator())
		this->_rspns.setRSP_STATUS_LINE("400 METHOD IS UNKNOWN");  //CHECK ERROR CODES
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

std::string 	ServerMachine::getResponse()
{
	//std::cout << this->_rspns.getRSP_MSG() << std::endl;
	return this->_rspns.getRSP_MSG();
}