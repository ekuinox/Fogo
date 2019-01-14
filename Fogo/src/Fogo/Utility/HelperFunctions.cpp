#include "HelperFunctions.h"
#include <iostream>
#include <random>
#include <string>

void Fogo::Utility::Log(const char * message) {
	if constexpr (!DEBUG) return;
	std::cout << message << std::endl;
}

void Fogo::Utility::Log(const std::string & message) {
	Log(message.c_str());
}

std::string Fogo::Utility::GenerateGUID() {
	static std::random_device randomDevice;
	return std::to_string(randomDevice());
}
