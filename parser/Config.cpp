#include "Config.hpp"

Config::Config()
{
	this->serverParams = initServerParams();
	initMap();
}

Config::~Config()
{

}

std::list<std::string> Config::initServerParams()
{
	std::list<std::string> params;
	params.push_back("listen");
	params.push_back("server_name");
	params.push_back("error_page");
	params.push_back("error_log");
	params.push_back("access_log");
	params.push_back("client_max_body_size");
	return (params);
}

void Config::initMap()
{
	this->mapFun["listen"] = Config::parseHostPort;
	this->mapFun["server_name"] = Config::parseServerName;
	this->mapFun["error_page"] = Config::parseErrorPage;
	this->mapFun["error_log"] = Config::parseErrorLog;
	this->mapFun["access_log"] = Config::parseAccessLog;
	this->mapFun["client_max_body_size"] = Config::parseClientMaxBodySize;
}

//------parse Location functions
int Config::parseRoot(Location *loc, 
std::vector<std::string>::iterator &begin,
std::vector<std::string>::iterator end)
{
	loc->setRoot(*begin);
	begin++;
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (0);
}

int Config::parseIndex(Location *loc, 
std::vector<std::string>::iterator &begin,
std::vector<std::string>::iterator end)
{
	loc->setIndex(*begin);
	begin++;
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (0);
}

int Config::parseLimitExcept(Location *loc, 
std::vector<std::string>::iterator &begin,
std::vector<std::string>::iterator end)
{
	std::string m;
	loc->denyAllMethods();

	while (begin != end && (*begin).compare(";"))
	{
		m = *begin;
		if (m[m.size() - 1] == ',')
		{
			m = m.substr(0, m.size() - 1);
		}
		if (m.compare("GET") == 0 || m.compare("POST") == 0 || m.compare("DELETE") == 0)
		{
			loc->allowMethod(m);
		}
		begin++;
	}
	if (begin != end && (*begin).compare(";") == 0)
	{
		if ((*begin).compare("GET") == 0 || (*begin).compare("POST") == 0 || (*begin).compare("DELETE") == 0)
			loc->allowMethod(*begin);
		begin++;
		return(0);
	}

	return (0);
}

int Config::parseAutoindex(Location *loc, 
std::vector<std::string>::iterator &begin,
std::vector<std::string>::iterator end)
{
	loc->setAutoindex(*begin);
	begin++;
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (0);
}

int Config::parseCgiPass(Location *loc, 
std::vector<std::string>::iterator &begin,
std::vector<std::string>::iterator end)
{
	loc->setCgiPass(*begin);
	begin++;
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (0);
}

int Config::parseCgiExtension(Location *loc, 
std::vector<std::string>::iterator &begin,
std::vector<std::string>::iterator end)
{
	loc->setCgiExtension(*begin);
	begin++;
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (0);
}

int Config::parseUploadStore(Location *loc, 
std::vector<std::string>::iterator &begin,
std::vector<std::string>::iterator end)
{
	loc->setUploadStore(*begin);
	begin++;
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (0);
}

int Config::parseRedirect(Location *loc, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end)
{
	std::vector<std::string>::iterator start = begin;
	begin++;
	if (!isNumber(*start))
		return (1);
	loc->setRedirect(atoi((*start).c_str()), *begin);
	begin++;
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (1);
}
//--------------------------

int Config::find(std::list<std::string> list, std::string word)
{
	if(std::find(list.begin(), list.end(), word)!= list.end())
		return (1);
	return (0);
}

int Config::validate(Server *server)
{
	(void)server;
	return (1);
}

int Config::validate(Location *loc)
{
	(void)loc;
	return (1);
}

//------------parse params functions-------------
int Config::parseServerName(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end)
{
	std::string name;
	while (begin != end && (*begin).compare(";"))
	{
		name = *begin;
		if (name[name.size() - 1] == ',')
		{
			name = name.substr(0, name.size() - 1);
		}
		server->addServerName(name);
		begin++;
	}
	if (server->getServerNames().size() > 0  && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (1);
}

int Config::parseHost(Server *server, std::string host)
{
	server->setHost(host);
	return (0);
}

int Config::parsePort(Server *server, std::string port)
{
	if (isNumber(port))
		server->setPort(atoi(port.c_str()));
	return (0);
}

int Config::parseHostPort(Server *server, std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end)
{
	std::vector<std::string> parser;
	if (begin != end)
	{
		std::istringstream is(*begin);
		std::string s;
		while (std::getline(is, s, ':'))
			parser.push_back(s);
		if (parser.size() != 2 && parser.size() != 1)
			return (1);
		if (parser.size() == 2)
		{
			parseHost(server, parser[0]);
			parsePort(server, parser[1]);
		}
		else if (parser.size() == 1)
		{
			parsePort(server, parser[0]);
		}
		begin++;
	}
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (-1);
}

int Config::parseLocation(Server *server, std::vector<std::string>::iterator &begin,
std::vector<std::string>::iterator end)
{

	std::string names[8] = {
		"root",
		"index",
		"limit_except",
		"autoindex",
		"cgi_pass",
		"cgi_extension",
		"return",
		"upload_store"
	};

	typedef int (Config::*funptr)(Location *loc,
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end);
	
	funptr funs[8] =
	{
		&Config::parseRoot,
		&Config::parseIndex,
		&Config::parseLimitExcept,
		&Config::parseAutoindex,
		&Config::parseCgiPass,
		&Config::parseCgiExtension,
		&Config::parseRedirect,
		&Config::parseUploadStore
	};

	Location *loc;
	loc = NULL;
	int ret;
	int flag;
	ret = 0;
	if (begin == end)
		return (1);
	loc = new Location();
	if ((*begin)[0] != '/')
		return (1);
	loc->setPath(*begin);
	begin++;
	if (begin != end && (*begin).compare("{") == 0)
	{
		begin++;
		while (begin != end)
		{
			flag = 0;
			if ((*begin).compare("}") == 0)
			{
				begin++;
				server->addLocation(loc);
				return (0);
			}
			for (int i = 0; i < 8; i++)
			if ((*begin).compare(names[i]) == 0)
			{
				flag = 1;
				begin++;
				(this->*funs[i])(loc, begin, end);
				break;
			}
			if (!flag)
				return (1); // FIX
		}
	}
	return (0);
}

int Config::parseErrorPage(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end)
{
	std::vector<std::string>::iterator start = begin;
	begin++;
	if (!isNumber(*start))
		return (1);
	server->addErrorPage(*start, *begin);
	begin++;
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (1);
}
int Config::parseErrorLog(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end)
{
	if (begin == end)
		return (1);
	server->setErrorLog(*begin);
	begin++;
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (1);
}
int Config::parseAccessLog(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end)
{
	if (begin == end)
		return (1);
	server->setAccessLog(*begin);
	begin++;
	if (begin != end && (*begin).compare(";") == 0)
	{
		begin++;
		return(0);
	}
	return (1);
}

int Config::isNumber(std::string value)
{
	size_t len = value.length();
	if (len > 0 && value.substr(0).find_first_not_of("0123456789") == std::string::npos)
		return (1);
	return (0);
}

int Config::parseClientMaxBodySize(Server *server, 
	std::vector<std::string>::iterator &begin,
	std::vector<std::string>::iterator end)
	{
		if (begin == end)
			return (1);
		if (isNumber(*begin))
			server->setClientMaxBodySize(atoi((*begin).c_str()));
		begin++;
		if (begin != end && (*begin).compare(";") == 0)
		{
			begin++;
			return(0);
		}
		return (0);
	}

//===========================
Server *Config::free_all(Server *server)
{
	if (server)
		free(server);
	return (NULL);
}

Server* Config::parseServer(std::vector<std::string>::iterator &begin,
std::vector<std::string>::iterator end)
{
	Server *server;
	server = NULL;
	std::string key;
	int ret;

	ret = 0;
	if (begin != end && (*begin).compare("{") == 0)
	{
		server = new Server();
		begin++;
		while (begin != end)
		{
			if ((*begin).compare("}") == 0 && validate(server))
			{
				begin++;
				return (server);
			}
			if ((*begin).compare("location") == 0)
			{
				begin++;
				parseLocation(server, begin, end);
				continue;
			}
			if (mapFun.find(*begin) != mapFun.end())
			{
				key = *begin;
				begin++;
				if (mapFun[key](server, begin, end))
						return (free_all(server));
				continue;
			}
			return (free_all(server));
		}
	}
	return (server);
}

bool Config::isSpace(unsigned char c) {
	return (c == ' ' || c == '\n' || c == '\r' ||
		c == '\t' || c == '\v' || c == '\f');
}

int Config::readConfig(std::string file)
{
	std::ifstream config;
	config.open (file, std::ios::in);
	if (config.fail())
	{
		std::cout << "Error: can't open config file." << std::endl;
		return (-1);
	}
	std::string line;
	std::string word;
	size_t i;
	while (getline(config, line))
	{
		i = 0;
		while (i < line.size() && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i] == '#' || line.empty())
			continue;
		std::istringstream strstream (line);
		while (std::getline(strstream, line, ' '))
		{
			line.erase(std::remove_if(line.begin(), line.end(), isSpace), line.end());
			if (line[0] == '#' || line.empty())
				continue;
			if (line[line.size() - 1] == ';')
			{
				line = line.substr(0, line.size() - 1);
				this->words.push_back(line);
				this->words.push_back(";");
			}
			else	
				this->words.push_back(line);
		}
	}
	config.close();
	return (0);
}

std::ostream &operator<<(std::ostream &out, Server *server)
{
		out << "===server params===" << std::endl;
		out << "host: " << server->getHost() << std::endl;
		out << "port: " << server->getPort() << std::endl;
		out << "access log: " << server->getAccessLog() << std::endl;
		out << "error log: " << server->getErrorLog() << std::endl;
		out << "client body size:" << server->getClientMaxBodySize() << std::endl;
		out << "Is default: " << server->getIsDefaultServer() << std::endl;
		out << "server names: ";
		for (std::list<std::string>::iterator it = server->getServerNames().begin(); 
				it != server->getServerNames().end(); it++)
					out << *it << " ";
		out << std::endl;
		out << "locations:";
		for (std::list<Location *>::iterator it = server->getLocation().begin(); 
				it != server->getLocation().end(); it++)
					{
						out << (*it)->getRoot() << " ";
						out << (*it)->getIndex() << " ";
						out << (*it)->getAutoindex() << " ";
						out << (*it)->getCgiPass() << " ";
						out << (*it)->getCgiExtension() << " ";
						out << (*it)->getUploadStore() << " ";
						out << (*it)->getRedirect()[301] << " ";
						out << std::endl;
						out << "GET:" << (*it)->getMethodAllowed()["GET"] << " ";
						out << "POST:" << (*it)->getMethodAllowed()["POST"] << " ";
						out << "DELETE:" << (*it)->getMethodAllowed()["DELETE"] << " ";
						out << std::endl;

					}
		out << std::endl;
		return (out);

}

int Config::parse(std::string file)
{
	Server *server;
	std::vector<std::string>::iterator begin;
	std::vector<std::string>::iterator end;
	readConfig(file);
	begin = this->words.begin();
	end = this->words.end();
	while (begin != end)
	{
		if ((*begin).compare("server") == 0)
		{
			begin++;
			server = parseServer(begin, end);
			if (!server)
				return (-1);
			{
				servers.push_back(server);
				if (servers.size() == 1)
					server->setIsDefaultServer(1);
				std::cout << server;
			}
		}
		else 
			return (-1);
	}
	return (0);
}
