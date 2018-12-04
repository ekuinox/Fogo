#pragma once

#include  "./Scene.h"
#include <vector>
#include <thread>

namespace Fogo::Game {
	class GameController {
	private:
		std::vector<std::shared_ptr<Scene>> __scenes;
		unsigned char __current_scene_index;
		std::thread __thread;
		bool __is_thread_running;

		auto exec() const -> void;
	public:
		GameController(std::vector<std::shared_ptr<Scene>> scenes);
		~GameController();
	};
}
