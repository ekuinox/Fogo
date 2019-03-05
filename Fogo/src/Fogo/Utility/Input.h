#pragma once

#include <dinput.h>
#include "./KeyCode.h"
#include <memory>

namespace DirectX { struct XMFLOAT3; }

namespace Fogo {
	class Keyboard;
	class Mouse;

	enum class MouseButton {
		Left,
		Right,
		Center
	};

	class Input {
	private:
		LPDIRECTINPUT8 __device;
		std::unique_ptr<Keyboard> __keyboard;
		std::unique_ptr<Mouse> __mouse;

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
		static bool GetTrigger(MouseButton button);
		static bool GetPress(MouseButton button);
		static bool GetRepeat(MouseButton button);
		static bool GetRelease(MouseButton button);
		static bool GetAnyPress();
		static bool GetAnyTrigger();
		static DirectX::XMFLOAT3 GetMouseMove();
	};
}
