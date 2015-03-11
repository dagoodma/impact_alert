#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <cmath>
#include <cctype>
#include <stdint.h>
#include <iostream>
#include <sstream>

// Return codes
#define ERROR		(int8_t)(-1)
#define FAILURE		(int8_t)(-1)
#define SUCCESS		(int8_t)(0)

namespace utility {

	bool isNumber(const std::string& s);
	std::string dblToString(double number);
}

#endif