#include "./System.h"
#include "../Utility.h"

using Fogo::Game::System;
using Fogo::Utility::Time;
using Fogo::Utility::Input;

System::System(const std::unordered_map<Key, Scene *> & scenes, Key key)
	: __current_key(key), __next_key(key) {
	for (const auto & [key, scene] : scenes) {
		bind(scene).makeIndex(key);
	}
	Input::Initialize();
	__thread = std::thread([&] { while (__is_thread_running) { exec(); }});
}

void System::exec() {
	const auto & currentScene = Store::Get<Scene>(__current_key);

	if (!currentScene) return;

	Time::Start();
	Input::Update();

	currentScene->execute<Updatable>([](Updatable & component) { component.update(); });
	currentScene->execute<Renderable>([](Renderable & component) { component.render(); });

	if (__go_next) {
		const auto & nextScene = Store::Get<Scene>(__next_key);
		
		if (nextScene) {
			__go_next = false;
			/*
			nextScene->start();
			__finalizer = { __current_key, true };
			// ƒL[‚ÌØ‚è‘Ö‚¦
			__current_key = __next_key;
			Time::RegisterTimer("FinalizingScene", 1.0f, [&] {
				currentScene->stop();
				currentScene->finalize();
				__finalizer.isFinaliing = false;
			});
			*/
		}
	}

	Time::Stop();
	Time::CheckTimers();
}

void System::setNextKey(Key key) {
}
