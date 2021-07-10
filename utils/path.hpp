#ifndef PATH_HPP
#define PATH_HPP

#include <string>

namespace path
{ 
	const static char SEP = '/';

	static inline bool is_sep(char c)
	{
		return c == SEP;
	}

	static inline std::string path_join(const std::string &p1, const std::string &p2)
	{
		return p1 + SEP + p2;
	}

	static inline std::string pardir(const std::string &p)
	{
		std::string::size_type pos = p.rfind(SEP);

		if (pos == std::string::npos)
			return "";
		else
			return p.substr(0, pos);
	}

	static inline std::string filename(const std::string &p)
	{
		std::string::size_type pos = p.rfind(SEP);

		if (pos == std::string::npos)
			return "";
		else
			return p.substr(pos + 1);
	}
}

#endif
