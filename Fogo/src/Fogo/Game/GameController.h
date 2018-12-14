#pragma once

#include  "./Scene.h"
#include "../Utility.h"
#include <vector>
#include <thread>

namespace Fogo::Game {
	template <typename Key>
	class GameController {
	public:
		enum class Event {
			Next,
			End, // 最後のシーンでNextSceneが呼ばれた際に発行する
		};

	private:
		std::unordered_map<Key, std::shared_ptr<Scene>> __scenes;
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
		

		auto exec() -> void {
			Utility::Time::Start();
			Utility::Input::Update();
			__scenes[__current_key]->update();
			__scenes[__current_key]->render();
			if (__go_next) {
				__go_next = false;
				__scenes[__next_key]->start();
				__finalizer = { __current_key, true };
				// キーの切り替え
				__current_key = __next_key;
				Utility::Time::RegisterTimer("FinalizingScene", 1.0f, [&] {
					__scenes[__finalizer.scene]->finalize();
					__finalizer.isFinaliing = false;
				});
			}
			Utility::Time::Stop();
			Utility::Time::CheckTimers();
		}

		auto onNext() -> void {
			if (__current_key == __next_key) { // 次のシーンキーが切り替えられていない
				Utility::PubSub<Event, void>::Publish(Event::End);
				return;
			}

			// 初期化されてなかったら初期化しましょう
			if (__scenes[__next_key]->getState() != Scene::State::Initialized) {
				__scenes[__next_key]->initialize();
			}

			__go_next = true;
		}

		auto onEnd() -> void {
			// Endをサブスクライブしているのが自分自身だけなら
			if (Utility::PubSub<Event, void>::GetSubscriberCount(Event::End) == 1) {
				Utility::Window::Stop();
			}
		}

		auto onDestroy() -> void {
			__is_thread_running = false;
			if (__thread.joinable()) __thread.join();
			Utility::Input::Finalize();
		}

		GameController(Key firstKey, std::unordered_map<Key, std::shared_ptr<Scene>> scenes)
		: __current_key(firstKey), __next_key(firstKey), __is_thread_running(true), __scenes(std::move(scenes)) {
			Utility::PubSub<Event, void>::RegisterSubscriber(Event::Next, [&] { onNext(); });
			Utility::PubSub<Event, void>::RegisterSubscriber(Event::End, [&] { onEnd(); });

			// 最初のシーンを初期化して開始
			__scenes[__current_key]->initialize();
			__scenes[__current_key]->start();

			Utility::Input::Initialize();
			__thread = std::thread([&] {
				while (__is_thread_running) { exec(); }
			});
		}

		~GameController() {
			onDestroy();
		}

		static GameController * __instance;
	public:
		static void Create(Key firstKey, const std::unordered_map<Key, std::shared_ptr<Scene>> & scenes) {
			if (!__instance) __instance = new GameController(firstKey, scenes);
		}

		static void Destroy() {
			delete __instance;
			__instance = nullptr;
		}

		static void SetNext(Key key) {
			__instance->__next_key = key;
		}

		static void LoadNext() {
			// 前のシーンの終了中にやるなバカ
			if (__instance->__finalizer.isFinaliing) return;

			__instance->__load_next_scene = std::thread([&] {
				__instance->__scenes[__instance->__next_key]->initialize();
				std::cout << "[GameController] Loaded next scene" << std::endl;
			});
		}

		static void LoadNextSync() {
			// 前のシーンの終了中にやるなバカ
			if (__instance->__finalizer.isFinaliing) return;

			__instance->__scenes[__instance->__next_key]->initialize();
			std::cout << "[GameController] Loaded next scene" << std::endl;
		}

		static bool IsNextSceneInitialized() {
			const auto result = __instance->__scenes[__instance->__next_key]->getState() == Scene::State::Initialized;
			if (result && __instance->__load_next_scene.joinable()) {
				__instance->__load_next_scene.join();
			}
			return result;
		}
	};

	template <typename Key>
	GameController<Key> * GameController<Key>::__instance = nullptr;

	using System = GameController<const char *>;
}
