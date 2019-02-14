#pragma once

#include "./Scene.h"
#include "./Store.h"
#include "./Component.h"
#include "../Utility.h"
#include <vector>
#include <thread>

namespace Fogo::Game {
	class System : public Component {
	public:
		using Key = const char *;
		enum class Event {
			Next,
			End, // ç≈å„ÇÃÉVÅ[ÉìÇ≈NextSceneÇ™åƒÇŒÇÍÇΩç€Ç…î≠çsÇ∑ÇÈ
		};

	private:
		std::vector<Key> keys;
		Key __current_key;
		Key __next_key;

		std::thread __thread;
		bool __is_thread_running;
		std::thread __load_next_scene;

		struct Finalizer {
			Key scene;
			bool isFinaliing = false;
		} __finalizer;

		bool __go_next = false;
		
		void exec();
		void onNext();
		void onEnd();
		void onDestroy();
		System(Key firstKey, std::unordered_map<Key, Scene*> scenes);
		~System();

		static System * __instance;

	public:
		static void Create(Key firstKey, const std::unordered_map<Key, Scene*> & scenes);
		static void Destroy();
		static void SetNext(Key key);
		static void LoadNext();
		static void LoadNextSync();
		static bool IsNextSceneInitialized();

	};
}
