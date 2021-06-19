//
// Created by Marceline Vuente on 6/11/21.
//

#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include <sstream>
#include <istream>
#include <map>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
//#include "utils.cpp"

class Request {
public:
	Request();
	explicit Request(int fd);
	~Request();

	void 												lineParser(std::string line, bool& flag);
	void 												bodyRcrdr(std::istringstream& iss);
	std::string											typeParser();
	std::map<std::string, std::string>&					getParsedData();
	std::string 										getBody();
	std::string 										getMethod();
	std::string 										getSource();
	std::string											getContType();
	void 												setCgiPath(std::string);
	void 												setCgiBody(std::string);
	bool 												methodValidator();


	std::string 										mylow(std::string	str);
	std::string 										eraser(std::string token);


	//are checking

	//std::vector<std::string>	getStart();

private:


	std::map<std::string, std::string>					_parsed_data; //хранит все строк запроса в виде пары: заголовок - значение
	std::string 										_body; //хранит тело запроса
	std::string											_startline; // хранит стартовую строку запроса (концевые символы сохранены)
	std::string 										_method; // хранит метод сапроса
	std::string 										_source; // хранит url запроса
	std::string 										_start_arg; // хранит HTTP/1.1
	std::string 										_cgiPath; // фрагмент CGI строки до ?
	std::string 										_cgiBody; // фрагмент CGI строки после ? (если есть)


};


#endif //_WEBSERV_REQUEST_HPP
