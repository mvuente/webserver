#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "../server/Server.hpp"

class Config
{
private:
	//map<params, pointer to function for parsing this param>
	std::map<std::string, int (*)(Server *,
	std::vector<std::string>::iterator &,
	std::vector<std::string>::iterator)> mapFun;

	//server params: listen, server_name and etc.
	std::list<std::string> serverParams;
	std::list<std::string> initServerParams();
	
	//fill map with params and fnctions
	void initMap();

	//word in config
	std::vector<std::string> words;
	
	//read config from file
	int readConfig(std::string file);

	//list of servers in config
	std::list<Server*> servers;

	//parse one server in config and return it
	Server *parseServer(std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);

	//utils 
	static int isNumber(std::string value);
	static bool isSpace(unsigned char c);
	int find(std::list<std::string> vector, std::string word);
	
	//parse location params functions
	int parseRoot(Location *loc, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	int parseIndex(Location *loc, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	int parseLimitExcept(Location *loc, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	int parseAutoindex(Location *loc, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	int parseCgiPass(Location *loc, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	int parseCgiExtension(Location *loc, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	int parseRedirect(Location *loc, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	int parseUploadStore(Location *loc, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	
	//parse server params functions
	static int parseHostPort(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	static int parseServerName(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	int parseLocation(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	static int parseErrorPage(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	static int parseErrorLog(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	static int parseAccessLog(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	static int parseClientMaxBodySize(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);

	//check that all required params is presented in config
	int validate(Server *server);
	int validate(Location *loc);

	Server *free_all(Server *server);
	static int parseHost(Server *server, std::string host);
	static int parsePort(Server *server, std::string port);

public:
	Config();
	~Config();
	int 				parse(std::string file);
	std::list<Server*>	&getServers();
};

//print server for debug
std::ostream &operator<<(std::ostream &out, Server const &server);

#endif