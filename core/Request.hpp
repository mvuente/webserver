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
#include "../parser/Config.hpp"
#include "to_size_t.hpp"
#include "Logger.hpp"
//#include "utils.cpp"

class Request {
public:
	Request();
	explicit Request(const std::string& msg);
	Request(const Request &cpRequest);
	~Request();
	Request &operator=(const Request& c);

	void 												lineParser(std::string line, bool& flag);
	void 												bodyRcrdr(std::istringstream& iss);
	std::string											typeParser();
	std::map<std::string, std::string>&					getParsedData();
	std::string 										getBody();
	std::string 										getMethod();
	std::string 										getSource();
	std::string											getContType();
	void												setSource(std::string& source);

	int 												getLengthOfChunk(std::string str);
	std::string 										mylow(std::string	str);
	std::string 										eraser(std::string token);
	std::string											headerOff(std::string buffer);
	std::string											lastOff(std::string& str);

	void 												msgPrint(); //временный метод
	//are checking

	//std::vector<std::string>	getStart();
	void												parse(const std::string &msg);
	void												parse();
	void 												addNewChunk(const std::string &chunk);
	bool 												completed();
	bool												ready();
	const std::string									getRaw() const;
	bool												ignoreUntilNext() const;
private:
	static const std::string LB;

	std::map<std::string, std::string>					_parsed_data; //хранит все строк запроса в виде пары: заголовок - значение
	std::string 										_body; //хранит тело запроса
	std::string											_startline; // хранит стартовую строку запроса (концевые символы сохранены)
	std::string 										_method; // хранит метод сапроса
	std::string 										_source; // хранит url запроса
	std::string 										_start_arg; // хранит HTTP/1.1
	
	/**
	 * Парсинг по частям
	 */
	std::string											_buffer;
	bool												_is_chuncked;
	bool												_has_content_length;
	size_t												_content_length;
	bool												_is_ready;

	bool _hasContentLength() const;
	bool _isEndOfBody() const;
	bool _isLastChunk();
	bool _isTEChunked();
	void _findContentLength();
};


#endif //_WEBSERV_REQUEST_HPP
