#pragma once

#include  "./Scene.h"
#include <vector>
#include <thread>

namespace Fogo::Game {
	class GameController {
	public:
		enum class Event {
			NextScene
		};
	private:
		std::vector<std::shared_ptr<Scene>> __scenes;
		unsigned char __current_scene_index;
		std::thread __thread;
		bool __is_thread_running;

		auto exec() const -> void;
		auto onDestroy() -> void;
		GameController(std::vector<std::shared_ptr<Scene>> scenes);
		~GameController();

		static GameController * __instance;
	public:
		static void Create(const std::vector<std::shared_ptr<Scene>> & scenes);
		static void Destroy();
	};
}
