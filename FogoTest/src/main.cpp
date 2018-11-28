#include <Fogo.h>

auto main(int argc, char ** argv) -> int {
	const auto & window = Fogo::Window(800, 640);

	Fogo::Window::HideConsole();

	return window.run();
}
