#pragma once

#include <vector>
#include <functional>
#include <thread>
#include "./Store.h"
#include "./Component.h"
#include "./Components/Scene.h"
#include "../Utility.h"

namespace Fogo {
	class System : public Component {
	/*

	������Root Component
	���: Scene�̊Ǘ�

	*/
	public:
		using Key = const char *;
		enum class Event {
			Next,
			End, // �Ō�̃V�[����NextScene���Ă΂ꂽ�ۂɔ��s����
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
		System(const Key & firstKey, const std::function<void(System &)> & createScenes);
		~System();

		static System * __instance;

	public:
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
