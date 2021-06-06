//
// Created by Marceline Vuente on 6/3/21.
//

#ifndef _WEBSERV_REQUEST_HPP
#define _WEBSERV_REQUEST_HPP

#include <sstream>
#include <istream>
#include <map>
#include <vector>
#include <iostream>

class Request {
	public:
		Request(const std::string& msg);
		~Request();
		std::vector<std::string>	getStart();
		std::map<std::string, std::string>	getData();
	private:
		Request();
		//std::istringstream					_iss;
		std::map<std::string, std::string>	_parsed_data;
		std::vector<std::string>			_startline;
		std::string 						_method;

};


#endif //_WEBSERV_REQUEST_HPP
