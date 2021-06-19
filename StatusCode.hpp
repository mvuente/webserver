#ifndef _STATUS_CODES_HPP
#define _STATUS_CODES_HPP
#include <map>
#include <iostream>

class StatusCode
{
private:
	StatusCode();
public:
	static std::string getDescStatusCode(std::string code);
	~StatusCode();
};

#endif