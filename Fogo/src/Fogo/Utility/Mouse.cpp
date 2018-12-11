#include "Mouse.h"
#include "Exception.h"
#include "Window.h"
#include <dinput.h>
#include <DirectXMath.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

using namespace Fogo::Utility;

Mouse::Mouse(LPDIRECTINPUT8 input) {
	ExecOrFail(input->CreateDevice(GUID_SysMouse, &__device, nullptr));
	ExecOrFail(__device->SetDataFormat(&c_dfDIMouse));
	// 協調モードを設定（フォアグラウンド＆非排他モード）
	ExecOrFail(__device->SetCooperativeLevel(Window::GetWindowHandle(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)));

	// デバイスの設定
	DIPROPDWORD prop;
	prop.diph.dwSize = sizeof prop;
	prop.diph.dwHeaderSize = sizeof prop.diph;
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）
	ExecOrFail(__device->SetProperty(DIPROP_AXISMODE, &prop.diph));

	__device->Acquire();
}

Mouse::~Mouse() {
	if (!__device) return;
	__device->Unacquire();
	__device->Release();
}


void Mouse::update() {
	if (FAILED(__device->Acquire())) return;
	DIMOUSESTATE current;
	if (FAILED(__device->GetDeviceState(sizeof(DIMOUSESTATE), &current))) return;

	for (auto button = 0; button < NUM_BUTTONS_COUNT; ++button) {
		__states[button].trigger = (__buffer.rgbButtons[button] ^ current.rgbButtons[button]) & current.rgbButtons[button];
		__states[button].release = (__buffer.rgbButtons[button] ^ current.rgbButtons[button]) & ~current.rgbButtons[button];

		// キーピート情報を生成
		if (current.rgbButtons[button]) {
			if (__states[button].repeat_count < LIMIT_COUNT_REPEAT) {
				++__states[button].repeat_count;
				__states[button].repeat = __states[button].repeat_count == 1 || __states[button].repeat_count >= LIMIT_COUNT_REPEAT ? current.rgbButtons[button] : 0;
			}
		}
		else {
			__states[button].repeat_count = 0;
			__states[button].repeat = 0;
		}
	}
	__buffer = current;
}

bool Mouse::getTrigger(const unsigned button) const {
	return (__states[button].trigger & 0x80) != 0;
}

bool Mouse::getPress(const unsigned button) const {
	return (__buffer.rgbButtons[button] & 0x80) != 0;
}

bool Mouse::getRepeat(const unsigned button) const {
	return (__states[button].repeat & 0x80) != 0;
}

bool Mouse::getRelease(const unsigned button) const {
	return (__states[button].release & 0x80) != 0;
}

DirectX::XMFLOAT3 Mouse::getMouseMove() const {
	return {
		static_cast<float>(__buffer.lX),
		static_cast<float>(__buffer.lY),
		static_cast<float>(__buffer.lZ)
	};
}
