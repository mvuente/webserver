//
// Created by Marceline Vuente on 6/11/21.
//

#include "Request.hpp"

Request::Request(int fd)
{
	char buffer[30000] = {0};
	size_t	valret = recv(fd, buffer, 30000, 0); // START TO READ CLIENT QUIERY HERE
	std::string msg = std::string(buffer, static_cast<size_t>(valret)); //transform buffer to a std::string
	std::cout << msg << std::endl;
	std::istringstream	iss(msg);
	std::string 		line;
	bool 				flag_start(1); //флаг первой строки запроса
	while (getline(iss, line, '\n')) // дроблю весь запрос на отдельные строки
	{
		//std::cout << "NEW LINE SIZE IS  " << line.size() << " AND LINE IS "<< line << std::endl;
		if (line.size() > 1)
			this->lineParser(line, flag_start);
		else
		{
			bodyRcrdr(iss);
			break;
		}
	}
}

Request::~Request() {}

Request::Request() {}

std::string										Request::typeParser()
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
	{    // СЮДА добавить exception
		this->_startline = line; // необрезанная стартовая строка
		getline(isl, this->_method, ' ');
		getline(isl, this->_source, ' ');
		getline(isl, this->_start_arg, ' ');
		flag_start = 0;
		return ;
	}

		getline(isl, word, ' ');
		std::string 				token;
		//std::cout << "new word is " << mylow(word) << std::endl;
		while (getline(isl, token, ' '))
		{
			this->_parsed_data[mylow(word)] += " " + eraser(token); //записываю все токены строки запроса в словарь с ключевым словом - header
		}
		return ;
}

void 												Request::bodyRcrdr(std::istringstream& iss) // не учитываю content-length
{
	std::string str;
	while (getline(iss, str))
	{
		this->_body += str;
		if (!iss.eof())
			this->_body += "\n";
	}
	std::cout << "BODY size is " << _body.length() << std::endl;
}

std::map<std::string, std::string>&					Request::getParsedData()
{
	return this->_parsed_data;
}

std::string 										Request::getBody()
{
	return this->_body;
}

std::string 										Request::getMethod()
{
	return this->_method;
}

std::string 										Request::getSource()
{
	return this->_source;
}

std::string											Request::getContType()
{
	return this->_parsed_data["content-type:"];
}

void 												Request::setCgiPath(std::string path)
{
	this->_cgiPath = path;
}

void 												Request::setCgiBody(std::string body)
{
	this->_cgiBody = body;
}

bool 												Request::methodValidator()
{
	if (this->_method == "GET" || this->_method == "POST" || this->_method == "DELETE")
		return true;
	return false;
}

std::string 										Request::mylow(std::string	str)
{
	if (!str.length())
		return str;
	for (size_t i = 0; i < str.length(); ++i)
	{
		str[i] = tolower(str[i]);
	}
	return str;
}

std::string 										Request::eraser(std::string token)
{
	for (size_t i = 0; i < token.length(); ++i)
	{
		if (token[i] == ',' || !isprint(token[i]))
			token.erase(token.begin() + i);
	}
	return token;
}