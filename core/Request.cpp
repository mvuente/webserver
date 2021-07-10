
#include "Request.hpp"

const std::string Request::LB = "\r\n";

Request::Request(const std::string& msg) :
	_parsed_data(),
	_body(),
	_startline(),
	_method(),
	_source(),
	_start_arg(),
	_buffer(),
	_is_chuncked(false),
	_has_content_length(false),
	_content_length(0),
	_is_ready(false)
{
	parse(msg);
}

Request::~Request() {}

Request::Request() :
	_parsed_data(),
	_body(),
	_startline(),
	_method(),
	_source(),
	_start_arg(),
	_buffer(),
	_is_chuncked(false),
	_has_content_length(false),
	_content_length(0),
	_is_ready(false)
{}

Request::Request(const Request &cpRequest)
{
	*this = cpRequest;
}

Request &Request::operator=(const Request& c)
{
	if (this == &c) return *this;
	_parsed_data = c._parsed_data;
	_body = c._body;
	_startline = c._startline;
	_method = c._method;
	_source = c._source;
	_start_arg = c._start_arg;
	_buffer = c._buffer;
	_is_chuncked = c._is_chuncked;
	_has_content_length = c._has_content_length;
	_content_length = c._content_length;
	_is_ready = c._is_ready;
	return *this;
}

void											Request::parse(const std::string &msg)
{
	std::istringstream	iss(msg);
	std::string 		line;
	bool 				flag_start(1); //флаг первой строки запроса

	while (getline(iss, line, '\n')) // дроблю весь запрос на отдельные строки
	{
		if (line.size() > 1)
		{
			this->lineParser(line, flag_start);
		}
		else
		{
			bodyRcrdr(iss);
			break;
		}
	}
	_is_ready = true;
}

void											Request::parse()
{
	parse(_buffer);
}

std::string										Request::typeParser()
{

	if (this->_source == "/")
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
		return "text/html";

}

void											Request::lineParser(std::string line, bool& flag_start)
{
	std::istringstream 	isl(line);
	
	if (flag_start)
	{
		this->_startline = line; // необрезанная стартовая строка
		getline(isl, this->_method, ' ');
		getline(isl, this->_source, ' ');
		getline(isl, this->_start_arg, ' ');
		flag_start = 0;
		return ;
	}

		std::string header_key;
		std::string header_val;

		getline(isl, header_key, ':');
		isl >> header_val;
		this->_parsed_data[mylow(header_key)] = header_val;
		return ;
}

void 												Request::bodyRcrdr(std::istringstream& iss) // не учитываю content-length
{
	std::string str;
	if (this->_parsed_data["transfer-encoding"] == "chunked")
	{
		while (getline(iss, str))
		{
			int length;
			if ((length = getLengthOfChunk(str)) > 0)
			{
				getline(iss, str);
				if (str.find("\r") == str.size() - 1)
				{
					str = lastOff(str);
				}

				else
					str += "\n";
				this->_body += str;
			}
			else
			break;
		}
	}
	else
	{
		while (getline(iss, str))
		{
			this->_body += str;
			if (!iss.eof())
				this->_body += "\n";
		}
	}
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
	return this->_parsed_data["content-type"];
}

void												Request::setSource(std::string& source)
{
	this->_source = source;
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

std::string											Request::headerOff(std::string buffer)
{
	return buffer;
}

int 												Request::getLengthOfChunk(std::string str) //преобразвоание строки HEX в число int
{
	str = lastOff(str);
	std::stringstream		sstemp;
	int 					tmpnum;
	sstemp << std::hex << str;
	sstemp >> tmpnum;
	return tmpnum;
}

std::string											Request::lastOff(std::string& str)
{
	return str.substr(0, str.find_last_of("\r"));
}


/**
 *	Работа с buffer'ом 
 */

const std::string Request::getRaw() const
{
	return _buffer;
}

bool Request::ignoreUntilNext() const
{
	return _buffer.length() == 5 && _buffer == ("0" + LB + LB);
}

void Request::addNewChunk(const std::string &chunk)
{
	_buffer += chunk;

	if (!_hasContentLength() && !_isTEChunked())
		_findContentLength();
}

bool Request::ready()
{
	return _is_ready;
}

bool Request::completed()
{
	if (_isTEChunked())
		return _isLastChunk();
	else
		return _isEndOfBody();
}

bool Request::_hasContentLength() const
{
	return _has_content_length;
}

bool Request::_isLastChunk()
{
	return _buffer.find("0" + LB + LB) != std::string::npos;
}

bool Request::_isEndOfBody() const
{
	static const std::string		DOUBLE_LB	= LB + LB;
	const std::string::size_type	body_start	= _buffer.find(DOUBLE_LB);
	
	if (body_start != std::string::npos)
	{
		if (_hasContentLength())
		{
			const std::string body = _buffer.substr(body_start + DOUBLE_LB.length());
		
			return body.length() == _content_length;
		}
		return true;
	}
	return false;
}

bool Request::_isTEChunked()
{
	if (!_hasContentLength() && !_is_chuncked)
		_is_chuncked = _buffer.find(LB + "Transfer-Encoding: chunked" + LB) != std::string::npos;
	return _is_chuncked;
}

void Request::_findContentLength()
{
	static const std::string h = LB + "Content-Length:";
	const std::string::size_type idx = _buffer.find(h);

	if (idx != std::string::npos)
	{
		const std::string::size_type end_idx = _buffer.find(LB, idx + h.length());
		
		if (end_idx != std::string::npos)
		{
			const std::string len = _buffer.substr(idx + h.length(), end_idx);

			if (util::to_size_t(len, &_content_length) == 1)
				_has_content_length = true;
		}
	}
}

//------------временно---------------------------------------
void 												Request::msgPrint()
{
	std::map<std::string, std::string>::iterator mit;
	for (mit = _parsed_data.begin(); mit != _parsed_data.end(); ++mit)
	{
		std::cout << mit->first << " " << mit->second << std::endl;
	}
}