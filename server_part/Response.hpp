//
// Created by Marceline Vuente on 6/1/21.
//

#ifndef _WEBSERV_RESPONSE_HPP
#define _WEBSERV_RESPONSE_HPP

#include <iostream>
#include <vector>
#include <map>

class Response {
public:
	Response();
	std::string getMethod(std::vector<std::string>& startline);
	std::string								getRSP_MSG();
	std::string								getRSP_BODY();
	std::string								getRSP_START_LINE();
	void 									setRSP_MSG();
	void 									setRSP_BODY(std::string body);
	void 									setRSP_START_LINE(std::string rsp_code);
	void 									setContent_Length();
private:
	std::string 							_rsp_start_line;
	std::string								_rsp_msg;
	std::string								_rsp_body;
	std::string 							_server;
	std::string 							_date;
	std::string 							_content_type;
	size_t 									_content_length;
	std::string 							_last_modified;

};


#endif //_WEBSERV_RESPONSE_HPP
