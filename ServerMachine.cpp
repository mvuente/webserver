//
// Created by Marceline Vuente on 6/5/21.
//

#include "ServerMachine.hpp"

ServerMachine::ServerMachine(Request& rqst, Response& rspns) : _rqst(rqst), _rspns(rspns) // add , Server& srvr and , _srvr(srvr)
{

	this->_filepath = "/Users/mvuente/Documents/web"; // take it from config
}

ServerMachine::ServerMachine() {}

ServerMachine::~ServerMachine() {}

void		ServerMachine::methodGet()
{
	std::string 		tmpval = this->_rqst.getSource();
	if (tmpval == "/") // открыть запрошенный файл
	{
		this->_rspns.setCode(200);
		this->_rspns.setRSP_BODY("Hello, scoundrels!\r\n"); // //тело ответа по умолчанию
	}

	else
	{
		std::string 	filename = _filepath + tmpval;
		std::ifstream 	ifs(filename, std::ifstream::in); //открываю искомый файл
		if (!ifs)
		{
			this->_rspns.setCode(404);
			this->_rspns.setRSP_BODY("ERROR 404. Resource you've requested is unavailable\r\n");
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
			this->_rspns.setCode(200);
		}
	}
	this->_rspns.setRSP_MSG();
}

std::string		ServerMachine::contentTypeProcesser()
{
	//std::cout << "Conetnt-type is " << this->_rqst.getContType() << std::endl;
	if (this->_rqst.getContType().find(" application/x-www-form-urlencoded") != this->_rqst.getContType().npos)
		return bodyProcessorEncoded(this->_rqst.getBody());
	else if (this->_rqst.getContType().find(" multipart/form-data") != this->_rqst.getContType().npos)
		return bodyProcessorMultipart(this->_rqst.getBody(), this->_rqst.getContType());
	else
		return "Unfinished"; //закончить
}

void			ServerMachine::methodPost()
{
	std::string		upload_store = "/Users/mvuente/Documents/pers/"; // Папка ял загрузки на сервер. ьерем из конфига
	std::string		filename = "load.txt"; // файл загрузки. Надо брать из тела запроса
	this->_rspns.setCode(200);
	std::ofstream	ofs(upload_store + filename);
	//char[stoi(_rqst["content-length:"])]	buf;
	//std::cout << _rqst.getBody() << std::endl;
	std::string result = contentTypeProcesser(); // пишу
	ofs << result;
	ofs.close();

	this->_rspns.setRSP_BODY("posted");
	this->_rspns.setRSP_MSG();
}

bool 			ServerMachine::checkCGI()
{
	std::cout << "checked" << std::endl;
	if (this->_rqst.getSource().find("cgi-bin") == this->_rqst.getSource().npos)
		return false;
	if (this->_rqst.getSource().find('?') == this->_rqst.getSource().npos)
		this->_rqst.setCgiPath(this->_rqst.getSource());
	else
	{
		this->_rqst.setCgiPath(this->_rqst.getSource().substr(0, this->_rqst.getSource().find('?')));
		this->_rqst.setCgiBody(this->_rqst.getSource().substr(this->_rqst.getSource().find('?') + 1));
	}
	return true;
}

void 			ServerMachine::methodProcesser()
{
	if (this->_rqst.getMethod() == "GET")
		this->methodGet();
	else if (this->_rqst.getMethod() == "POST")
		this->methodPost();
//		else
//			return this->methodDelete();
}



long		ServerMachine::ResponseCrtr(int fd) // здесь CGI
{
//	if (checkCGI()) // закомментировал пока неподключен клас CGI
//	{
//		CGI	cgi(this->_rqst);
//		cgi.run();
//		return cgi.getOutput();
//	}

	if (!_rqst.methodValidator())
		this->_rspns.setCode(400);  //CHECK ERROR CODES
	else
	{
		methodProcesser();
	}
	//this->_rspns.setLocation(); // получиь отдельно код ответа и использовать location
	std::string rsp = this->_rspns.getRSP_MSG();
	char* hello = new char [rsp.length()+1];
	std::strcpy (hello, rsp.c_str());

	return send(fd, hello, strlen(hello), 0); // отправка в сокет ответа севрера
}


std::string		ServerMachine::bodyProcessorEncoded(std::string	body)
{
	std::istringstream				iss(body);
	std::string 					str, res("");

	while (getline(iss, str, '&'))
	{
		while (str.find("%") != str.npos) //замена всех hex на измененные перед передачей символы
		{
			std::string::size_type	found = str.find("%");
			std::string hexnum = str.substr(found + 1, 2);
			std::stringstream		sstemp;
			int 					tmpnum;
			sstemp << std::hex << hexnum;
			sstemp >> tmpnum;
			std::string				hexstr(1, static_cast<char>(tmpnum));
			str = str.substr(0, found) + hexstr + str.substr(found + 3, str.npos);
		}
		if (res.size() != 0)
		{
			res += "\n";
		}
		res += str.substr(0, str.find('='));
		res += " ";
		res += str.substr(str.find('=') + 1, str.npos);
	}
	return res;
}

std::string		ServerMachine::bodyProcessorMultipart(std::string body, std::string type)
{
	std::string	boundary = type.substr(type.find('=') + 1);
	return "";
}

int 			ServerMachine::getLength()
{
	return this->_length;
}

Response&		ServerMachine::getRspObj()
{
	return this->_rspns;
}



