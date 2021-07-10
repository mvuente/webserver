#include "Server.hpp"

std::list<std::string> &Server::getServerNames() 
{
	return (this->serverNames);
}

void Server::addServerName(std::string serverName)
{
	this->serverNames.push_back(serverName);
}

void Server::addLocation(Location *loc)
{
	this->locationList.push_back(loc);
}

int Server::getIsDefaultServer()
{
	return (this->isDefaultServer);
}

void Server::setIsDefaultServer(int value)
{
	this->isDefaultServer = value;
}

void Server::setHost(std::string host)
{
	this->host = host;
}

void Server::setPort(int port)
{
	this->port = port;
}

std::string Server::getHost() const
{
	return (this->host);
}

int Server::getPort() const
{
	return (this->port);
}

Server::Server()
{
	this->host = "localhost";
	this->port = 80;
}

Server::~Server()
{

}

const std::list<Location *> &Server::getLocation() const
{
	return (this->locationList);
}

void Server::addErrorPage(std::string code, std::string path)
{
	this->error_pages[code] = path;
}

void Server::setAccessLog(std::string path)
{
	this->accessLog = path;
}

void Server::setErrorLog(std::string path)
{
	this->errorLog = path;
}

void Server::setClientMaxBodySize(int value)
{
	this->clientMaxBodySize = value;
}

std::string Server::getErrorLog()
{
	return (this->errorLog);
}

std::string Server::getAccessLog()
{
	return (this->accessLog);
}

int Server::getClientMaxBodySize()
{
	return (this->clientMaxBodySize);
}