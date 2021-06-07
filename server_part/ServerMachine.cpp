//
// Created by Marceline Vuente on 6/5/21.
//

#include "ServerMachine.hpp"

ServerMachine::ServerMachine(Request& rqst, Response& rspns) : _rqst(rqst), _rspns(rspns) // add , Server& srvr and , _srvr(srvr)
{
	this->_body = "Hello, scoundrels!";
	this->_filepath = "/Users/mvuente/Documents/web";
}

std::string 	ServerMachine::CodeOfRespns()
{
	std::string tmpval = this->_rqst.getData()["source"];
	if (tmpval == "/") // открыть запрошенный файл
		this->setCodeRespns("200 OK");
	else
	{
		std::string filename = _filepath + tmpval;
		//std::ifstream ifs(filename, std::ifstream::binary);
		std::cout << "What's this?" << std::endl;
		std::ifstream ifs(filename, std::ifstream::in);
		if (!ifs)
		{
			this->setCodeRespns("404 FILE IS NOT FOUND");
			this->_body = "ERROR 404. Resource you've requested is unavailable";
		}

		else
		{
			bool flag(true);
			while (ifs)
			{
				std::string	tmpstr;
				if (flag)
				{

					this->_body.clear();

				}

				if (!this->_body.empty())
					this->_body += "\n";

				std::getline(ifs, tmpstr);

				this->_body += tmpstr;
				flag = false;
			}

//				// get length of file:
//				ifs.seekg (0, ifs.end);
//				int length = ifs.tellg();
//				ifs.seekg (0, ifs.beg);

				//char * buffer = new char [length];


				// read data as a block:
				//ifs.read (buffer,length);
			ifs.close();
//			this->_bytestream = buffer;
//			this->_length = length;
			this->setCodeRespns("200 OK");
		}
		//std::cout << filename << std::endl;
	}
	return this->getCodeRespns();
}

std::string		ServerMachine::getBody()
{
	return this->_body;
}

int 		ServerMachine::getLength()
{
	return this->_length;
}

std::string ServerMachine::getCodeRespns()
{
	return this->_coderspns;
}

void 		ServerMachine::setCodeRespns(std::string str)
{
	this->_coderspns = str;
}

void 		ServerMachine::respHeaderCreater()
{
	this->_rspns.setRSP_START_LINE(this->CodeOfRespns());
	//СЮДА ПОСТАВИТЬ ВЫЗОВ СОСТАВТИТЕЛЯ BODY
	this->_rspns.setRSP_BODY(this->_body);
	this->_rspns.setRSP_MSG();

}

std::string ServerMachine::getResponce()
{
	return (this->_rspns.getRSP_START_LINE() + this->_rspns.getRSP_MSG());// + this->_rspns.getRSP_BODY());
}