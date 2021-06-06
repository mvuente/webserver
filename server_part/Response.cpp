//
// Created by Marceline Vuente on 6/1/21.
//

#include "Response.hpp"

Response::Response()// make this!!!
{
	this->_rsp_start_line = "HTTP/1.1 ";
	this->_rsp_msg = std::string("");
	this->_server = std::string("KmichikoMvuenteServer"); //имя нашего сервера
	this->_date = std::string(""); //наверное, не надо. делать апдейт даты при гаждом формировании ответа
	this->_content_type = std::string("text/plain"); //зависит от вида нашей страницы по умолчанию
	this->_content_length = 0;
	this->_last_modified = std::string(""); //наверное, не надо. делать апдейт даты при гаждом формировании ответа
}

std::string 							Response::getMethod(std::vector<std::string>& startline)
{
	return startline[0];
}

std::string								Response::getRSP_START_LINE()
{
	return this->_rsp_start_line;
}

std::string								Response::getRSP_MSG()
{
	return this->_rsp_msg;
}

std::string								Response::getRSP_BODY()
{
	return this->_rsp_body;
}

void 									Response::setRSP_START_LINE(std::string rsp_code)
{
	this->_rsp_start_line.append(rsp_code);
	this->_rsp_start_line.append("\n");
}

void 									Response::setRSP_MSG() // make this!
{
	if (this->_server != "")
		this->_rsp_msg += "server: " + this->_server + "\n";
	if (this->_date != "")
		this->_rsp_msg += "date: " + this->_date + "\n";
	if (this->_content_type != "")
		this->_rsp_msg += "content-type: " + this->_content_type + "\n";
	if (this->_content_length != 0)
		this->_rsp_msg += "content-length: " + std::to_string(this->_content_length) + "\n";
	if (this->_last_modified != "")
		this->_rsp_msg += "last_modified: " + this->_last_modified + "\n";
	this->_rsp_msg += "\n\n";
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

