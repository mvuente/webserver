#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <dirent.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

class Autoindex
{
private:
public:
	Autoindex();
	~Autoindex();
	std::string generate(std::string host, int port, std::string path); //returns page autoindex
};

#endif