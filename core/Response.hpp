//
// Created by Marceline Vuente on 6/1/21.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <iostream>
#include <vector>
#include <map>

#include "Request.hpp"
#include "date.hpp"
#include <sstream>
#include <algorithm>
#include <ostream>

class Response {
public:
	Response(const Request& rqst);
	Response();
	Response(const Response &cpResponse);
	~Response();

	const std::string&						getRSP_MSG() const;
	const std::string&						getRSP_BODY() const;
	const std::string&						getRSP_STATUS_LINE() const;
	std::string								getConnection();
	int 									getCode() const;
	



	const std::string&						setDate();
	void 									setRSP_MSG();
	void									setRSP_HEADER(const std::string &key, const std::string &val);
	void 									setRSP_BODY(std::string body);
	void									setStatus();
	void 									setCode(int code);
	void 									setRSP_STATUS_LINE();
	void 									setRSP_STATUS_LINE(const std::string &statusLine);
	void 									setContent_Length();
	std::string 							myIntToStr(int num);

	static const std::string 				LB;
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
	//size_t 												_content_length;


};


inline std::ostream &operator<<(std::ostream &os, Response &r)
{
	const std::string body = r.getRSP_BODY();

	os << "status line: " << r.getRSP_STATUS_LINE();
	os << " connection: " << r.getConnection();
	
	os << " body: '";
	if (body.length() > 150)
	{
		os << body.substr(0, 25);
		os << "...";
		os << body.substr(body.length() - 25, 25);
	}
	else
		os << body;
	os << "'";
	return os;
}

#endif //WEBSERV_RESPONSE_HPP
