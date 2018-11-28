#include <Fogo.h>
#include <iostream>

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo;

	Time::Start();

	getchar();

	Time::Stop();

	printf("%f", Time::GetSeconds());

	return 0;
}
