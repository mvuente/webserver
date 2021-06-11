//
// Created by Marceline Vuente on 6/1/21.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <iostream>
#include <vector>
#include <map>
#include "Request.hpp"

class Response {
public:
	Response(Request& rqst);
	~Response();

	std::string								getRSP_MSG();
	std::string								getRSP_BODY();
	std::string								getRSP_STATUS_LINE();
	void 									setRSP_MSG();
	void 									setRSP_BODY(std::string body);
	void 									setRSP_STATUS_LINE(std::string rsp_code);
	void 									setContent_Length();
private:
	Request												_rqst;
	std::string 										_status_line;
	std::map<std::string, std::vector<std::string> >	_fields;
	std::map<std::string, std::vector<std::string> >	_entity_fields; //не знаю
	std::map<std::string, std::vector<std::string> >	_rprsnt_data;
	std::string											_rsp_msg;
	std::string											_rsp_body;
	size_t 												_content_length;


};


#endif //WEBSERV_RESPONSE_HPP
