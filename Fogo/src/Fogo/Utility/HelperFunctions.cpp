#include "HelperFunctions.h"
#include <iostream>
#include <random>
#include <string>

void Fogo::Log(const char * message) {
	if constexpr (!DEBUG) return;
	std::cout << message << std::endl;
}

void Fogo::Log(const std::string & message) {
	Log(message.c_str());
}

std::string GenerateGUID() {
	static std::random_device randomDevice;
	return std::to_string(randomDevice());
}
