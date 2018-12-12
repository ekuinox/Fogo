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
			End, // �Ō�̃V�[����NextScene���Ă΂ꂽ�ۂɔ��s����
		};

	private:
		std::unordered_map<Key, std::shared_ptr<Scene>> __scenes;
		Key __current_key;
		Key __next_key;

		std::thread __thread;
		bool __is_thread_running;

		auto exec() -> void {
			Utility::Time::Start();
			Utility::Input::Update();
			__scenes[__current_key]->update();
			__scenes[__current_key]->render();
			Utility::Time::Stop();
		}

		auto onNext() -> void {
			if (__current_key == __next_key) { // ���̃V�[���L�[���؂�ւ����Ă��Ȃ�
				Utility::PubSub<Event, void>::Publish(Event::End);
				return;
			}

			// ���̃V�[�����I�������āA���̃V�[�����J�n����
			__scenes[__current_key]->finalize();

			__current_key = __next_key;

			// ����������ĂȂ������珉�������܂��傤
			if (__scenes[__current_key]->getState() != Scene::State::Initialized) {
				__scenes[__current_key]->initialize();
			}

			__scenes[__current_key]->start();
		}

		auto onEnd() -> void {
			// End���T�u�X�N���C�u���Ă���̂��������g�����Ȃ�
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

			// �ŏ��̃V�[�������������ĊJ�n
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
	};

	template <typename Key>
	GameController<Key> * GameController<Key>::__instance = nullptr;

	using System = GameController<const char *>;
}
