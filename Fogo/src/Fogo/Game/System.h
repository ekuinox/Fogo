#pragma once

#include "./Scene.h"
#include <thread>

namespace Fogo::Game {
	class System final : public Component, public Renderable, public Updatable {
	public:
		using Key = const char *;

	private:
		Key __current_key, __next_key;

		std::thread __thread;
		bool __is_thread_running;
		std::thread __load_next_scene;

		struct Finalizer {
			Key scene;
			bool isFinaliing = false;
		} __finalizer;

		bool __go_next = false;

		void exec(); // フレーム更新
	public:
		System(const std::unordered_map<Key, Scene *> & scenes, Key key);
		void setNextKey(Key key);
	};
}