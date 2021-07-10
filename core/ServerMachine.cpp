
#include "ServerMachine.hpp"

const HTMLTemplate ServerMachine::_templates[4] =
{
	HTMLTemplate("html/404.html"),
	HTMLTemplate("html/405.html"),
	HTMLTemplate("html/500.html"),
	HTMLTemplate("html/index.html")
};

ServerMachine::ServerMachine(const Request& rqst, const Server& server_info) : 
	_server_info(server_info), _rqst(rqst), _rspns(rqst) // add , Server& srvr and , _srvr(srvr)
{
	this->_noLoc = true;
	this->_filepath = "/"; // знаечние по умолчанию. Это путь из Location
	this->_additionalPath = ""; // путь к запрошенному файлу, если запрошенный url не вполне соввпадает с location
	this->_filename = ""; // по умолчанию. Это нвазание (и, может быть, путь) к запрашиваемому файлу
	this->_cgipass = "";
	std::string tmpstr = _rqst.getSource();

	std::list<Location*>::const_iterator lit;

	for (lit = _getServerInfo().getLocation().begin(); lit != _getServerInfo().getLocation().end(); ++lit) //вероятно не имеет смысла
	{
		if ((*lit)->getPath() == ".bla" && pathProcessor(tmpstr).find(".bla") != std::string::npos
			&& (*lit)->getMethodAllowed()[this->_rqst.getMethod()] == 1)
		{
			this->_methodAllowed = (*lit)->getMethodAllowed();
			this->_cgipass = (*lit)->getCgiPass();
		}

		else if (pathProcessor(tmpstr).find((*lit)->getPath()) == 0)
		{
			this->setLocation(**lit); // выбрал указатель на текущий location
			this->_noLoc = false;
			this->_filepath = pathProcessor((*lit)->getRoot()); // установил root как путь к фалам ответа
			this->_uploadpath = pathProcessor((*lit)->getUploadStore()); // утановил путь к папке для загрузки
			this->_index = (*lit)->getIndex();
			this->_methodAllowed = (*lit)->getMethodAllowed();
			if (tmpstr != pathProcessor((*lit)->getPath()))
				this->_additionalPath = additionCutter(tmpstr, pathProcessor((*lit)->getPath()));
		}
	}

}

ServerMachine::ServerMachine() {}

ServerMachine::~ServerMachine() {}

void 		ServerMachine::setLocation(const Location &loc)
{
	this->_curLoc = loc;
}

void 		ServerMachine::cgiProcessor()
{
	CGI	c(_rqst, _curLoc.getRoot());

	if (_cgipass != "")
		c.pass(_cgipass);
	if (c.run() == -1)
	{
		_setResponse500();
	}	
	else
	{
		c.completeResponse(_rspns);
		_rspns.setRSP_MSG();
	}
}

void		ServerMachine::methodGet()
{
	std::string source  = this->_rqst.getSource();
    std::string dir;
    if (source == "/")
        dir = this->_curLoc.getRoot();
    else
        dir = "." + source;
    if (this->_curLoc.getAutoindex().compare("on") == 0 && opendir(dir.c_str()))
    {
        Autoindex in;
        std::string a = in.generate(_getServerInfo().getHost(), _getServerInfo().getPort(), dir);
		_setResponse200(a);
        return;
    }
	if (this->_curLoc.getRedirect().size() > 0)
	{
		_redirect();
		return ;
	}
    std::string     filename;
    
    //sourceChoice();
    if (this->_filename == "") 
		this->_filename = this->_index;
    filename = pathProcessor(this->_additionalPath) + this->_filename;
	if (this->_curLoc.getAutoindex() != "on")
		filename = pathProcessor(this->_filepath) + filename;

    std::ifstream   ifs(filename.c_str(), std::ifstream::in);

	if (!ifs) _setResponse404();
	else
	{
		std::stringstream ss;

		ss << ifs.rdbuf();
		_setResponse200(ss.str());
		ifs.close();
	}
}

std::string		ServerMachine::contentTypeProcessor()
{
	if (this->_rqst.getContType().find("application/x-www-form-urlencoded") != this->_rqst.getContType().npos)
		return bodyProcessorEncoded(this->_rqst.getBody());
	else if (this->_rqst.getContType().find("multipart/form-data") != this->_rqst.getContType().npos)
		return bodyProcessorMultipart(this->_rqst.getContType());
	else
		return "Unfinished";
}

std::string		ServerMachine::pathProcessor(std::string locpath)
{
	if (locpath.find_last_of("/") != locpath.size() - 1)
		return locpath + "/";
	return locpath;
}

std::string		ServerMachine::additionCutter(std::string url, std::string path)
{
	std::string add = url.substr(url.find(path) + path.size());
	if (add.find('.') == std::string::npos)
		return add;
	this->_filename = add;
	return "";
}

void			ServerMachine::methodPost()
{
	std::string		upload_store = "/";
	std::string		filename = "load.txt";
	std::ofstream	ofs((upload_store + filename).c_str());
	std::string result = contentTypeProcessor();
	size_t maxBody = this->_curLoc.getMaxBodySize();
	if (this->_rqst.getBody().size() > maxBody)
	{
		_setResponse413("");
	}

	ofs << result;
	ofs.close();
	_setResponse200("posted");
}

void			ServerMachine::methodPut()
{
	Logger::println(this->_uploadpath + this->_additionalPath);
	std::ofstream	ofs((this->_uploadpath + this->_additionalPath).c_str());
	ofs << this->_rqst.getBody();
	ofs.close();
	_setResponse200();
}

void			ServerMachine::methodDelete()
{
	const std::string filepath = path::path_join(_curLoc.getRoot(), _rqst.getSource());

	if (std::remove(filepath.c_str()))
	{
		_setResponse500();
	}
	else
	{
		_setResponse200("File deleted.");
	}
}

bool 			ServerMachine::checkCGI()
{
	return (_rqst.getSource().find("cgi-bin") != std::string::npos);
}

void 			ServerMachine::methodProcessor()
{
	if (this->_cgipass != "")
		this->cgiProcessor();
	else if (this->_rqst.getMethod() == "GET")
		this->methodGet();
	else if (this->_rqst.getMethod() == "POST")
		this->methodPost();
	else if (this->_rqst.getMethod() == "PUT")
		this->methodPut();
	else
		this->methodDelete();
}

bool 			ServerMachine::methodValidator()
{
	if (this->_methodAllowed[this->_rqst.getMethod()] == 1)
		return true;
	_setResponse405();
	return false;
}

const Response&	ServerMachine::ResponseCrtr()
{
	if (this->_noLoc)
		_setResponse404();
	else if (this->methodValidator())
		methodProcessor();
	return _rspns;
}

std::string		ServerMachine::bodyProcessorEncoded(std::string	body)
{
	std::istringstream				iss(body);
	std::string 					str, res("");

	while (getline(iss, str, '&'))
	{
		while (str.find("%") != str.npos) //замена всех hex на измененные перед передачей символы
		{
			std::string::size_type	found = str.find("%");
			std::string hexnum = str.substr(found + 1, 2);
			std::stringstream		sstemp;
			int 					tmpnum;
			sstemp << std::hex << hexnum;
			sstemp >> tmpnum;
			std::string				hexstr(1, static_cast<char>(tmpnum));
			str = str.substr(0, found) + hexstr + str.substr(found + 3, str.npos);
		}
		if (res.size() != 0)
		{
			res += "\n";
		}
		res += str.substr(0, str.find('='));
		res += " ";
		res += str.substr(str.find('=') + 1, str.npos);
	}
	return res;
}

std::string		ServerMachine::bodyProcessorMultipart(std::string type) //убрал переменную body
{
	std::string	boundary = type.substr(type.find('=') + 1);
	return "";
}

int 			ServerMachine::getLength()
{
	return this->_length;
}

Response&		ServerMachine::getRspObj()
{
	return this->_rspns;
}

const Server&	ServerMachine::_getServerInfo()
{
	return this->_server_info;
}

ssize_t ServerMachine::_send(int fd)
{
	return _send(fd, _rspns.getRSP_MSG());
}

ssize_t ServerMachine::_send(int fd, const std::string &resp)
{
	static const int 	N_ATTEMPTS_TO_SEND	= 10;
	static const size_t	TIME_TO_SLEEP		= 1000;
	const size_t		len			= resp.length();
	const char* 		bytes		= resp.c_str();
	size_t	 			sum 		= 0;
	ssize_t				n			= -1;
	int					attempts;

	while (sum < len)
	{
		attempts = 0;
		do
		{
			n = _send(fd, bytes + sum, len - sum, 0);
			if (n == -1)
				usleep(TIME_TO_SLEEP);
		} 
		while (n == -1 && ++attempts < N_ATTEMPTS_TO_SEND);

		if (n == -1) return -1;
		sum += n;
	}
	return sum;
}

ssize_t ServerMachine::_send(int fd, const void *buffer, size_t length, int flags)
{
	#ifdef DEBUG2
		static const 		TempFile in("webserv.log");
		static const int	log_fd	= in.getFd();
		static int			last_fd = fd;

		if (last_fd != fd)
		{
			last_fd = fd;
			write(log_fd, "\n\n", 2);
		}
		write(log_fd, buffer, length);
	#endif
	return send(fd, buffer, length, flags);
}

const HTMLTemplate& ServerMachine::_getTemplate(int statusCode)
{
	switch(statusCode)
	{
		case 404:
			return _templates[0];
		case 405:
			return _templates[1];
		case 500:
			return _templates[2];
		case 0:
			return _templates[3];
		default:
			return _templates[3];
	}
}

void	ServerMachine::_setResponseXXX(int statusCode, const std::string &body)
{
	_rspns.setCode(statusCode);
	_rspns.setRSP_BODY(body);
	_rspns.setRSP_MSG();
}

void	ServerMachine::_setResponseWithTemplate(int statusCode, const std::string &msg)
{
	_setResponseXXX(statusCode, _getTemplate(statusCode).getContentOrMessage(msg));
}

void	ServerMachine::_setResponse404()
{
	_setResponseWithTemplate(404, "404 Not Found");
}

void	ServerMachine::_setResponse405()
{
	_setResponseWithTemplate(405, "405 Method Not Allowed");
}

void	ServerMachine::_setResponse500()
{
	_setResponseWithTemplate(500, "500 Iteranal Server Error");
}

void	ServerMachine::_setResponse200(const std::string &body)
{
	_setResponseXXX(200, body);
}


void	ServerMachine::_setResponse413(const std::string &body)
{
	_setResponseXXX(413, body);
}

void	ServerMachine::_redirect()
{
	std::map<int, std::string>::value_type redirInfo = *this->_curLoc.getRedirect().begin();

	_redirect(redirInfo.second, redirInfo.first);
}


void	ServerMachine::_redirect(const std::string &location, int statusCode)
{
	_rspns.setRSP_HEADER("Location:", " " + location);
	_setResponseXXX(statusCode, "");
}