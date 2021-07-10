#ifndef CONTAINS_HPP
#define CONTAINS_HPP

#include <vector>
#include <algorithm>

template <typename ValueType, typename Container>
static inline bool contains(const ValueType &value, const Container &array)
{
	return std::find(array.begin(), array.end(), value) != array.end();
}

#endif
