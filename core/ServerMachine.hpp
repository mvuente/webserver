//
// Created by Marceline Vuente on 6/5/21.
//

#ifndef WEBSERV_SERVERMACHINE_HPP
#define WEBSERV_SERVERMACHINE_HPP
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio> // std::remove
#include <cstring>

#include "Request.hpp"
#include "Response.hpp"
#include "HTMLTemplate.hpp"
#include "path.hpp" // path_join() from utils/path.hpp
#include "Logger.hpp"
#include "Server.hpp"
#include "CGI.hpp"
#include "Autoindex.hpp"

#include <sstream>

class ServerMachine {
public:
	ServerMachine();
	ServerMachine(const Request& rqst, const Server& config); // add , Server& srvr
	~ServerMachine();
	//std::string getResponse();
	Response&			getRspObj(); //временный метод

	bool				checkCGI();
	const Response&		ResponseCrtr();
	void 				methodProcessor();
	std::string			contentTypeProcessor();
	std::string			pathProcessor(std::string locpath);
	std::string			additionCutter(std::string url, std::string path);
	std::string			bodyProcessorMultipart(std::string type);
	std::string			bodyProcessorEncoded(std::string	body);
	bool 				methodValidator();
	void 				cgiProcessor();
	void				methodGet();
	void				methodPost();
	void				methodDelete();
	void				methodPut();
	std::string 		sourceChoice();

	void 				setLocation(const Location &loc);

	//long 				rspns_send(int fd);

	// under construction
	//void 		respHeaderCreater();


	int 		getLength();

private:
	Server		_server_info;
	Request		_rqst;
	Response	_rspns;
	bool 		_noLoc;
	std::map<std::string, int>	_methodAllowed;
	std::vector<std::string>	_methodSet;
	Location					_curLoc; // BEDA!Location, на основе которого формируется ответ сервера
	std::string 				_coderspns;
	std::string					_uploadpath; // путь для загрузки
	std::string 				_filepath; // путб для выгрузки
	std::string 				_cgipass;
	std::string					_additionalPath;
	std::string 				_filename;
	std::string 				_index;
	std::string					_body;
	char*						_bytestream;
	int 						_length;

	ssize_t _send(int fd);
	ssize_t _send(int fd, const std::string &resp);
	ssize_t _send(int fd, const void *buffer, size_t length, int flags = 0); // внутри есть debug

	static const HTMLTemplate &_getTemplate(int statusCode); // 404, 405, 500 or 0 - index
	static const HTMLTemplate _templates[4]; // 0 - 404.html, 1 - 405.html, 2 - 500.html, 3 - index.html

	void	_setResponseXXX(int statusCode, const std::string &body);
	void	_setResponseWithTemplate(int statusCode, const std::string &msg);
	void	_setResponse404();
	void	_setResponse405();
	void	_setResponse500();
	void	_setResponseCode(int code);
	void	_setResponse200(const std::string &body = "");
	void	_setResponse413(const std::string &body);
	void	_redirect(const std::string &location, int statusCode);
	void	_redirect();

	const Server& _getServerInfo();
};


#endif //_WEBSERV_SERVERMACHINE_HPP
