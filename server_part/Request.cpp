//
// Created by Marceline Vuente on 6/11/21.
//

#include "Request.hpp"

Request::Request(std::string& msg)
{
	std::istringstream	iss(msg);
	std::string 		line;
	bool 				flag_start(1); //флаг первой строки запроса
	while (getline(iss, line, '\n')) // дроблю весь запрос на отдельные строки
	{
		this->lineParser(line, flag_start);
	}
}

Request::~Request() {}

Request::Request() {}

std::string										Request::sourceParser()
{
	if (this->_source == "/") // а бывает вообще пусто?????
		return "text/html";
	std::string	type = this->_source.substr(this->_source.find('.') + 1, this->_source.find(' '));
	if (type == "html")
		return "text/html";
	else if (type == "css")
		return "text/css";
	else if (type == "js")
		return "text/javascript";
	else if (type == "jpeg" || type == "jpg")
		return "image/jpeg";
	else if (type == "png")
		return "image/png";
	else if (type == "bmp")
		return "image/bmp";
	else
		return "application/" + type;
}

void											Request::lineParser(std::string line, bool& flag_start)
{
	std::istringstream 	isl(line);
	std::string 		word;
	if (flag_start)
	{    // СЮДА добавить methodvalidator и exception
		getline(isl, this->_method, ' ');
		getline(isl, this->_source, ' ');
		getline(isl, this->_start_arg, ' ');
		flag_start = 0;
		//sourceParser();
		return ;
	}
	std::vector<std::string>	v;
	getline(isl, word, ' ');
	//this->_parsed_data[word] = v(0);
	std::string 				token;
	while (getline(isl, token, ' '))
	{
		this->_parsed_data[word].push_back(token); //записываю все токены строки запроса в словарь с ключевым словом - header
	}
	return ;
}

std::map<std::string, std::vector<std::string> >	Request::getParsedData()
{
	return this->_parsed_data;
}

std::string 										Request::getMethod()
{
	return this->_method;
}

std::string 										Request::getSource()
{
	return this->_source;
}

bool 												Request::methodValidator()
{
	if (this->_method == "GET" || this->_method == "POST" || this->_method == "DELETE")
		return true;
	return false;
}