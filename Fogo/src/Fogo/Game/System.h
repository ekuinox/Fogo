#pragma once

#include <vector>
#include <functional>
#include <thread>
#include "./Store.h"
#include "./Component.h"
#include "./Components/Scene.h"
#include "../Utility.h"

namespace Fogo::Game {
	class System : public Component {
	/*

	À¿‚ÌRoot Component
	–ğ–Ú: Scene‚ÌŠÇ—

	*/
	public:
		using Key = const char *;
		enum class Event {
			Next,
			End, // ÅŒã‚ÌƒV[ƒ“‚ÅNextScene‚ªŒÄ‚Î‚ê‚½Û‚É”­s‚·‚é
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
		System(const Key & firstKey, const std::function<void(System &)> & createScenes);
		~System();

		static System * __instance;

	public:
		static void Create(Key firstKey, const std::unordered_map<Key, Scene*> & scenes);
		static void Create(const Key & firstKey, const std::function<void(System &)> & createScenes);
		static void Destroy();
		static void SetNext(Key key);
		static void LoadNext();
		static void LoadNextSync();
		static bool IsNextSceneInitialized();
		static System & GetInstance();
		static Scene & GetCurrentScene();
		static Scene & GetScene(const Key & key);

	};
}
