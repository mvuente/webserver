#include "Autoindex.hpp"

Autoindex::Autoindex()
{

}

Autoindex::~Autoindex()
{

}

std::string Autoindex::generate(std::string host, int port, std::string path)
{
	DIR *d;
	struct dirent *dir;
	std::string index;
	d = opendir(path.c_str());
	index = "<!DOCTYPE html>\n\
<html>\n\
<body>\n\
<h1 style=\"text-align:center\">Index</h1>\n";
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			std::stringstream link;
			link << "<p>\
<a href=\"http://" << host << ":" << port << "/" << \
path << "/" << dir->d_name << "\">" << dir->d_name << "</a></p>\n";
			index += link.str();
		}
		closedir(d);
	}
	index += "</body>\n</html>";
	return (index);
}