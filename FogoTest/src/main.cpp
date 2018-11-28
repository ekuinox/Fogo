#include <Fogo.h>
#include <iostream>

auto main(int argc, char ** argv) -> int
{

	for (auto i = 0; i < 100; ++i)
	{
		if (const auto & s = Fogo::fizzbuzz(i)) std::cout << s << std::endl;
		else std::cout << i << std::endl;
	}

	return 0;
}