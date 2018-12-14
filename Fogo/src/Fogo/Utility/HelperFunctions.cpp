#include "HelperFunctions.h"
#include <iostream>

void Fogo::Utility::Log(const char * message) {
	if constexpr (!DEBUG) return;
	std::cout << message << std::endl;
}

void Fogo::Utility::Log(const std::string & message) {
	Log(message.c_str());
}
