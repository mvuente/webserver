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

class Request {
public:
	explicit Request(std::string& msg);
	~Request();

	void 												lineParser(std::string line, bool& flag);
	std::string											sourceParser();
	std::map<std::string, std::vector<std::string> >	getParsedData();
	std::string 										getMethod();
	std::string 										getSource();
	bool 												methodValidator();


	//are checking

	//std::vector<std::string>	getStart();

private:
	Request();
	//std::istringstream					_iss;
	std::map<std::string, std::vector<std::string> >	_parsed_data; //хранит все строк запроса в виде пары: заголовок - значение
	//std::vector<std::string>			_startline; // хранит стартовую строку запроса ?
	std::string 						_method;
	std::string 						_source;
	std::string 						_start_arg;
	//std::string 						_fileType;

};


#endif //_WEBSERV_REQUEST_HPP
