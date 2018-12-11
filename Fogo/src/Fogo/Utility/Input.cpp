#include "Input.h"
#include <dinput.h>
#include "./Window.h"
#include "Exception.h"
#include "./Keyboard.h"
#include "./Mouse.h"
#include <DirectXMath.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

using namespace Fogo::Utility;

Input * Input::__instance = nullptr;

Input::Input() {
	ExecOrFail(DirectInput8Create(Window::GetInstanceHandle(), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&__device), nullptr));
	__keyboard = std::make_unique<Keyboard>(__device);
	__mouse = std::make_unique<Mouse>(__device);
}

Input::~Input() {
	if (__device) __device->Release();
	__keyboard.release();
	__mouse.release();
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
	__instance->__mouse->update();
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

bool Input::GetTrigger(MouseButton button) {
	return __instance->__mouse->getTrigger(static_cast<unsigned int>(button));
}

bool Input::GetPress(MouseButton button) {
	return __instance->__mouse->getPress(static_cast<unsigned int>(button));
}

bool Input::GetRepeat(MouseButton button) {
	return __instance->__mouse->getRepeat(static_cast<unsigned int>(button));
}

bool Input::GetRelease(MouseButton button) {
	return __instance->__mouse->getRelease(static_cast<unsigned int>(button));
}

bool Input::GetAnyPress() {
	return __instance->__keyboard->getAnyPress();
}

bool Input::GetAnyTrigger() {
	return __instance->__keyboard->getAnyTrigger();
}

DirectX::XMFLOAT3 Input::GetMouseMove() {
	return __instance->__mouse->getMouseMove();
}
