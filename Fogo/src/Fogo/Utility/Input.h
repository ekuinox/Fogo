#pragma once

#include <dinput.h>
#include "./KeyCode.h"
#include <memory>

namespace Fogo::Utility {
	class Keyboard;

	class Input {
	private:
		LPDIRECTINPUT8 __device;
		std::unique_ptr<Keyboard> __keyboard;

		static Input * __instance;
		
		Input();
		~Input();
		void update() const;
		bool getTrigger(KeyCode key) const;
		bool getPress(KeyCode key) const;
		bool getRepeat(KeyCode key) const;
		bool getRelease(KeyCode key) const;
	public:
		Input(const Input &) = delete;
		Input & operator=(const Input &) = delete;

		static void Initialize();
		static void Finalize();
		static void Update();
		static bool GetTrigger(KeyCode key);
		static bool GetPress(KeyCode key);
		static bool GetRepeat(KeyCode key);
		static bool GetRelease(KeyCode key);
	};
}
