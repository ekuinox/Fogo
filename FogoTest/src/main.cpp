#include <Fogo.h>
#include <iostream>

auto main(int argc, char ** argv) -> int
{
	const auto & window = Fogo::Window(800, 640);

	Fogo::Window::HideConsole();

	window.run();

	return 0;
}