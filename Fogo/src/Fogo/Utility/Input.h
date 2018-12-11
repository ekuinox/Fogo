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
		static bool GetAnyPress();
		static bool GetAnyTrigger();
	};
}
