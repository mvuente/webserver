//
// Created by Marceline Vuente on 6/1/21.
//

//

#include "Response.hpp"


Response::Response(Request &rqst) : _rqst(rqst)// OK
{
	this->_status_line = "HTTP/1.1";
	this->setStatus(); //сформировать набор кодов ответа
	this->_rsp_body = "";
	this->_fields["Date:"] += " " + setDate(); // установить дату и время твета сервера
	//this->_fields["Content-length"] = "";

	this->_fields["Connection:"] += " " + getConnection();
	//Location через сетер. см дальше
	//this->_fields["Retry-After:"]  если ответ 503. ДОЛЖЕН БЫТЬ! // не заню
	//this->_fields["Vary:"]  стандарт требует. Используется для Cach // не заню
	//this->_fields["ETag:"]  если есть Entyti // не заню
	//this->_fields["Last-Modified:"]  если есть Entyti // не заню
	//this->_fields["Proxy-Authenticate:"]  если ответ 407. ДОЛЖЕН БЫТЬ! // не заню
	//this->_fields["WWW-Authenticate:"]  если ответ 401 // не заню
	//this->_fields["Accept-Ranges:"]  // не заню
	//this->_fields["Allow:"]  // не заню
	this->_fields["Server:"] = this->_fields["Server:"] + " " + "KmichikoMvuenteFmoanyServer";//имя нашего сервера

	this->_rprsnt_data["Content-Type:"] += " " + this->_rqst.typeParser();




}

Response::Response() {}

Response::~Response() {}

std::string								Response::setDate(void)
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	return std::string(buffer);
}


//void									Response::setLocation(int code, std::string& redirect)
//{
//	if (code == 201 || code / 100 == 3)
//	{
//		this->_fields["Location:"].push_back(redirect);
//	}
//}

std::string								Response::getConnection()
{
	//std::cout << this->_rqst.getParsedData().size() << std::endl;
	//std::map<std::string, std::string>::iterator mit = this->_rqst.getParsedData().begin();
	//while (mit != this->_rqst.getParsedData().end())
	//{
		//std::cout << mit->first << std::endl;
	//	mit++;
	//}

//	if (this->_rqst.getParsedData().find("connection:") == this->_rqst.getParsedData().end())
//		std::cout << "first" << std::endl;
//	std::cout << "problem?" << std::endl;
//	std::cout << this->_rqst.getParsedData().at("connection:").size() << std::endl;
//	std::cout << this->_rqst.getParsedData()["connection:"].size() << std::endl;
//	std::cout << this->_rqst.getParsedData()["connection:"][0] << std::endl;
//	std::cout << this->_rqst.getParsedData()["connection:"][0].size() << std::endl;
	if (this->_rqst.getParsedData().find("connection:") == this->_rqst.getParsedData().end() ||
			this->_rqst.getParsedData()["connection:"].find(" keep-alive") != this->_rqst.getParsedData()["connection:"].npos)
	{
		//std::cout << "no problem" << std::endl;
		return "keep-alive";
	}

	else
	{
		return "close";
	}

}

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

void									Response::setStatus() // прлверить варианты ошибок
{
	this->_status[100] = "100 Continue";
	this->_status[200] = "200 OK";
	this->_status[201] = "201 Created";
	this->_status[204] = "204 No Content";
	this->_status[400] = "400 Bad Request";
	this->_status[403] = "403 Forbidden";
	this->_status[404] = "404 Not Found";
	this->_status[405] = "405 Method Not Allowed";
	this->_status[413] = "413 Payload Too Large";
	this->_status[500] = "500 Internal Server Error";
}

void 									Response::setCode(int code)
{
	this->_code = code;
	setRSP_STATUS_LINE();
}

void 									Response::setRSP_STATUS_LINE()
{
	this->_status_line += " " + this->_status[this->_code];
	//this->_status_line.push_back("\n");

}

void 									Response::setRSP_MSG() // make this!
{
	this->_rsp_msg = this->getRSP_STATUS_LINE() + "\r\n";
	std::map<std::string, std::string>::iterator	it = this->_fields.begin();
	//std::cout << "Fields map size is " << this->_fields.size() << std::endl;
	while (it != this->_fields.end())
	{
		this->_rsp_msg += it->first;
		this->_rsp_msg += it->second;
		this->_rsp_msg += "\r\n";
		it++;
	}
	it = this->_rprsnt_data.begin();
	while (it != this->_rprsnt_data.end())
	{
		this->_rsp_msg += it->first;
		this->_rsp_msg += it->second;
		this->_rsp_msg += "\r\n";
		it++;
	}
	if (this->_rsp_body.size() != 0)
	{
		this->_rsp_msg += "\r\n";
		this->_rsp_msg += this->_rsp_body;
	}
	//std::cout << this->_rsp_msg << std::endl;

}

void 									Response::setContent_Length()
{
//	this->_content_length = this->_rsp_body.size();
	if (this->_fields.find("Transfer-Encoding: ") != this->_fields.end() &&
			this->_fields["Transfer-Encoding: "].find("chunked") != this->_fields["Transfer-Encoding: "].npos)
		this->_fields["Content-length: "] += " " + myIntToStr(_rsp_body.length() - 2);
	//std::cout << " after setLength Fields map size is " << this->_fields.size() << std::endl;
}

void 									Response::setRSP_BODY(std::string body)
{
	this->_rsp_body = body;
	this->setContent_Length();
}

std::string 							Response::myIntToStr(int num)
{
	std::stringstream	ss;
	ss << num;
	return ss.str();
}

