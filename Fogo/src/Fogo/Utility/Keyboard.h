#pragma once

#include "Input.h"

namespace Fogo::Utility {
	class Keyboard {
	private:
		static constexpr unsigned int NUM_KEY_MAX = 256;
		static constexpr unsigned int LIMIT_COUNT_REPEAT = 20;
		LPDIRECTINPUTDEVICE8 __device;
		BYTE __buffer[NUM_KEY_MAX];
		struct {
			BYTE trigger;
			BYTE repeat;
			BYTE release;
			unsigned int repeat_count;
		} __states[NUM_KEY_MAX];
	public:
		Keyboard(LPDIRECTINPUT8 input);
		~Keyboard();
		void update();
		bool getPress(unsigned int key) const;
		bool getTrigger(unsigned int key) const;
		bool getRepeat(unsigned int key) const;
		bool getRelease(unsigned int key) const;
		bool getAnyPress() const;
		bool getAnyTrigger() const;
	};
}