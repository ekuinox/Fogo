#include "Input.h"
#include <dinput.h>
#include "./Window.h"
#include "Exception.h"
#include "./Keyboard.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

using namespace Fogo::Utility;

Input * Input::__instance = nullptr;

Input::Input() {
	ExecOrFail(DirectInput8Create(Window::GetInstanceHandle(), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&__device), nullptr));
	__keyboard = std::make_unique<Keyboard>(__device);
}

Input::~Input() {
	if (__device) __device->Release();
	__keyboard.release();
}

void Input::Initialize() {
	if (!__instance) __instance = new Input();
}

void Input::Finalize() {
	delete __instance;
	__instance = nullptr;
}

void Input::Update() {
	__instance->__keyboard->update();
}

bool Input::GetTrigger(KeyCode key) {
	return __instance->__keyboard->getTrigger(static_cast<unsigned int>(key));
}

bool Input::GetPress(KeyCode key) {
	return __instance->__keyboard->getPress(static_cast<unsigned int>(key));
}

bool Input::GetRepeat(KeyCode key) {
	return __instance->__keyboard->getRepeat(static_cast<unsigned int>(key));
}

bool Input::GetRelease(KeyCode key) {
	return __instance->__keyboard->getRelease(static_cast<unsigned int>(key));
}

bool Input::GetAnyPress() {
	return __instance->__keyboard->getAnyPress();
}

bool Input::GetAnyTrigger() {
	return __instance->__keyboard->getAnyTrigger();
}

