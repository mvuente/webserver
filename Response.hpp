//
// Created by Marceline Vuente on 6/1/21.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <time.h> // for strftime
#include <sys/time.h> //for gettimeofday
#include "Request.hpp"
#include <sstream>
#include <algorithm>

class Response {
public:
	Response(Request& rqst);
	Response();
	~Response();

	std::string								getRSP_MSG();
	std::string								getRSP_BODY();
	std::string								getRSP_STATUS_LINE();
	std::string								getConnection();



	std::string								setDate();
	//void									setLocation(int code, std::string& redirect);
	void 									setRSP_MSG();
	void 									setRSP_BODY(std::string body);
	void									setStatus();
	void 									setCode(int code);
	void 									setRSP_STATUS_LINE();
	void 									setContent_Length();
	std::string 							myIntToStr(int num);

private:
	Request												_rqst;
	std::string 										_status_line;
	int 												_code;
	std::map<int, std::string>							_status;
	std::map<std::string, std::string>					_fields;
	std::map<std::string, std::string>					_entity_fields; //не знаю
	std::map<std::string, std::string>					_rprsnt_data;
	std::string											_rsp_msg;
	std::string											_rsp_body;
	size_t 												_content_length;


};


#endif //WEBSERV_RESPONSE_HPP
