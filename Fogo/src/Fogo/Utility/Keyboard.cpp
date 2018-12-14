#include "Keyboard.h"
#include "HelperFunctions.h"
#include "Window.h"
#include <dinput.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

using namespace Fogo::Utility;

Keyboard::Keyboard(LPDIRECTINPUT8 input) {
	ExecOrFail(input->CreateDevice(GUID_SysKeyboard, &__device, nullptr));
	ExecOrFail(__device->SetDataFormat(&c_dfDIKeyboard));
	// 協調モードを設定（フォアグラウンド＆非排他モード）
	ExecOrFail(__device->SetCooperativeLevel(Window::GetWindowHandle(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)));
	__device->Acquire();
}

Keyboard::~Keyboard() {
	if (!__device) return;
	
	__device->Unacquire();
	__device->Release();
}

void Keyboard::update() {
	
	BYTE current[NUM_KEY_MAX];

	if (FAILED(__device->GetDeviceState(sizeof current, current))) {
		// キーボードへのアクセス権を取得
		__device->Acquire();
		return;
	}

	for (auto key = 0; key < NUM_KEY_MAX; ++key) {
		// キートリガー・リリース情報を生成．
		__states[key].trigger = (__buffer[key] ^ current[key]) & current[key];
		__states[key].release = (__buffer[key] ^ current[key]) & ~current[key];

		// キーピート情報を生成
		if (current[key]) {
			if (__states[key].repeat_count < LIMIT_COUNT_REPEAT) {
				++__states[key].repeat_count;
				__states[key].repeat = __states[key].repeat_count == 1 || __states[key].repeat_count >= LIMIT_COUNT_REPEAT? current[key] : 0;
			}
		}
		else {
			__states[key].repeat_count = 0;
			__states[key].repeat = 0;
		}

		// キープレス情報を保存
		__buffer[key] = current[key];
	}
}

bool Keyboard::getTrigger(const unsigned int key) const {
	return (__states[key].trigger & 0x80) != 0;
}

bool Keyboard::getPress(const unsigned int key) const {
	return (__buffer[key] & 0x80) != 0;
}

bool Keyboard::getRepeat(const unsigned int key) const {
	return (__states[key].repeat & 0x80) != 0;

}

bool Keyboard::getRelease(const unsigned int key) const {
	return (__states[key].release & 0x80) != 0;

}

bool Keyboard::getAnyPress() const {
	for (auto i = 0u; i < NUM_KEY_MAX; ++i) {
		if (getPress(i)) return true;
	}
	return false;
}

bool Keyboard::getAnyTrigger() const {
	for (auto i = 0u; i < NUM_KEY_MAX; ++i) {
		if (getTrigger(i)) return true;
	}
	return false;
}
