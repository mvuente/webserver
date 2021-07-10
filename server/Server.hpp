#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <algorithm>
#include <list>
#include <map>
#include "Location.hpp"

class Server
{
private:
	std::string host;
	int port;
	std::list<std::string> serverNames;
	std::list<Location *> locationList;
	std::map<std::string, std::string> error_pages;
	int isDefaultServer;
	int clientMaxBodySize;
	
	std::string accessLog;
	std::string errorLog;
	
public:
	Server(/* args */);
	Server(int port, std::string host);
	~Server();
	std::string getHost() const;
	int getPort() const;
	void setHost(std::string host);
	void setPort(int port);
	std::list<std::string> &getServerNames();
	void addServerName(std::string serverName);
	void addLocation(Location *loc);
	const std::list<Location *> &getLocation() const;
	int getIsDefaultServer();
	int getClientMaxBodySize();
	std::string getErrorPage();
	void addErrorPage(std::string code, std::string path);
	void setErrorLog(std::string path);
	void setAccessLog(std::string path);
	void setClientMaxBodySize(int value);
	void setIsDefaultServer(int value);
	std::string getErrorLog();
	std::string getAccessLog();
};

#endif