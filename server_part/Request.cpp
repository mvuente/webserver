//
// Created by Marceline Vuente on 6/3/21.
//

#include "Request.hpp"

Request::Request(const std::string& msg) // проверять ли пустые/непустые строки в запросе?
{
	bool is_startline(true);
	std::istringstream	_iss(msg);
	std::string	whole_msg = _iss.str();
	std::string line;
	//std::cout << basic_string << std::endl;
	while (whole_msg.length() != 0)
	{
		size_t lbl = whole_msg.find(10);
		if (lbl != std::string::npos)
		{
			line = whole_msg.substr(0, lbl - 1);
			if (line.length() == 0)
				break;
			if (is_startline)
			{
				_method = line.substr(0, line.find(" "));
				_startline.push_back(_method);
				_startline.push_back(line.substr(line.find(" ") + 1, line.length() - 1));
				is_startline = false;
			}
			else
				_parsed_data[line.substr(0, line.find(" "))] = line.substr(line.find(" ") + 1, line.length() - 1);
			whole_msg = whole_msg.substr(lbl + 1);
		}
		else // NO NEED this, since there is an empty line between headers and body!
		{
			line = whole_msg;
			whole_msg = "";
		}
	}

	//std::cout << "METHOD: " << _startline[0] << std::endl;
	//std::cout << "START ARG: " << _startline[1] << std::endl;
	std::map<std::string, std::string>::iterator it = _parsed_data.begin();
	while (it != _parsed_data.end())
	{
		//std::cout << "HEADER: " << it->first << "  " << "ARG: " << it->second << std::endl;
		it++;
	}
	std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
};
Request::~Request() {};

std::vector<std::string>			Request::getStart()
{
	return this->_startline;
}

std::map<std::string, std::string>	Request::getData()
{
	return this->_parsed_data;
}
