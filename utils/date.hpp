#ifndef DATE_HPP
#define DATE_HPP

#include <time.h> // for strftime
#include <sys/time.h> //for gettimeofday
#include <string>

namespace date
{
	static inline const std::string &getDate()
	{
		static std::string	res;
		char				buffer[100];
		struct timeval		tv;
		struct tm*			tm;

		gettimeofday(&tv, NULL);
		tm = gmtime(&tv.tv_sec);
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
		res = buffer;

		return res;
	}
}


#endif