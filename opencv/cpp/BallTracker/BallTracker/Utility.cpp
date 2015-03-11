
#include "Utility.hpp"


using namespace std;

namespace utility {

	bool isNumber(const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && std::isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	}

	std::string dblToString(double number){


	std::stringstream ss;
	ss << number;
	return ss.str();
	}
}