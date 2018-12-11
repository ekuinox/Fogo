#pragma once

#include <dinput.h>
namespace DirectX { struct XMFLOAT3; }
namespace Fogo::Utility {
	class Mouse {
	private:
		static constexpr unsigned int LIMIT_COUNT_REPEAT = 20;
		static constexpr unsigned char NUM_BUTTONS_COUNT = 4;
		LPDIRECTINPUTDEVICE8 __device;
		DIMOUSESTATE __buffer;
		struct {
			BYTE trigger;
			BYTE repeat;
			BYTE release;
			unsigned int repeat_count;
		} __states[NUM_BUTTONS_COUNT];

	public:
		Mouse(LPDIRECTINPUT8 input);
		~Mouse();
		void update();
		bool getPress(unsigned int button) const;
		bool getTrigger(unsigned int button) const;
		bool getRepeat(unsigned int button) const;
		bool getRelease(unsigned int button) const;
		DirectX::XMFLOAT3 getMouseMove() const;
	};
}