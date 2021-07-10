#ifndef TO_SIZE_T_HPP
#define TO_SIZE_T_HPP

#include <string>
#include <cstdio>
#include <cstddef>

namespace util
{
	static inline int to_size_t(const std::string &s, size_t *buf)
	{
		return sscanf(s.c_str(), "%zu", buf);
	}
}

#endif