//
// Created by Marceline Vuente on 6/1/21.
//

//

#include "Response.hpp"


Response::Response(Request &rqst) : _rqst(rqst)// OK
{
	this->_status_line = "HTTP/1.1";
	this->_rsp_body = "Hello, scoundrels!";
	//this->_fields["Date:"].pushback(); //СДЕЛАТЬ!!!!
	//this->_fields["Location:"]  если ответ не 200. Смотреть // не заню
	//this->_fields["Retry-After:"]  если ответ 503. ДОЛЖЕН БЫТЬ! // не заню
	//this->_fields["Vary:"]  стандарт требует. Используется для Cach // не заню
	//this->_fields["ETag:"]  если есть Entyti // не заню
	//this->_fields["Last-Modified:"]  если есть Entyti // не заню
	//this->_fields["Proxy-Authenticate:"]  если ответ 407. ДОЛЖЕН БЫТЬ! // не заню
	//this->_fields["WWW-Authenticate:"]  если ответ 401 // не заню
	//this->_fields["Accept-Ranges:"]  // не заню
	//this->_fields["Allow:"]  // не заню
	this->_fields["Server:"].push_back("KmichikoMvuenteFmoanyServer");//имя нашего сервера

	this->_rprsnt_data["Content-Type:"].push_back(this->_rqst.sourceParser());

}

Response::~Response() {}

std::string								Response::getRSP_STATUS_LINE()
{
	return this->_status_line;
}

std::string								Response::getRSP_MSG()
{
	return this->_rsp_msg;
}

std::string								Response::getRSP_BODY()
{
	return this->_rsp_body;
}

void 									Response::setRSP_STATUS_LINE(std::string rsp_code)
{
	this->_status_line += " " + rsp_code;
	//this->_status_line.push_back("\n");

}

void 									Response::setRSP_MSG() // make this!
{
	this->_rsp_msg = this->getRSP_STATUS_LINE() + "\r\n";
	std::map<std::string, std::vector<std::string> >::iterator	it = this->_fields.begin();
	while (it != this->_fields.end())
	{
		this->_rsp_msg += it->first;
		std::vector<std::string>::iterator	vit = it->second.begin();
		while (vit != it->second.end())
		{
			this->_rsp_msg += " " + *vit;
			vit++;
		}
		this->_rsp_msg += "\r\n";
		it++;
	}
	it = this->_rprsnt_data.begin();
	while (it != this->_rprsnt_data.end())
	{
		this->_rsp_msg += it->first;
		std::vector<std::string>::iterator	vit = it->second.begin();
		while (vit != it->second.end())
		{
			this->_rsp_msg += " " + *vit;
			vit++;
		}
		this->_rsp_msg += "\r\n";
		it++;
	}
	if (this->_rsp_body.size() != 0)
	{
		this->_rsp_msg += "\r\n";
		this->_rsp_msg += this->_rsp_body;
	}
	std::cout << this->_rsp_msg << std::endl;

}

void 									Response::setContent_Length()
{
	this->_content_length = this->_rsp_body.size();
}

void 									Response::setRSP_BODY(std::string body)
{
	this->_rsp_body = body;
	this->setContent_Length();
}

