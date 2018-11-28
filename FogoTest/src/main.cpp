#include <Fogo.h>
#include <iostream>

auto main(int argc, char ** argv) -> int
{
	const auto & window = Fogo::Window::GetInstance(800, 640);

	window.hideConsole();

	window.run();

	return 0;
}