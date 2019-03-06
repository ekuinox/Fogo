#include "./CameraController.h"
#include "./Camera.h"

using namespace Fogo;
using namespace DirectX;

struct CameraType {
	static constexpr const char * Main = "main";
	static constexpr const char * Sub1 = "sub1";
};

CameraController::CameraController() : currentType(CameraType::Main) {
	const auto & mainCamera = create<Camera>().makeIndex(CameraType::Main);

	mainCamera->target = XMFLOAT3 { 0, 0, 0 };
	mainCamera->position = XMFLOAT3 { 0, 0, -50 };
	mainCamera->fov = 60;
	mainCamera->nearZ = 1;
	mainCamera->farZ = 1000;

	const auto & sub1Camera = create<Camera>().makeIndex(CameraType::Sub1);

	currentCamera = &mainCamera;

	create<Updater>([&] {
		const auto camera = get<Camera>(currentType);

		if (!camera) return;

		static constexpr auto CAMERA_SPEED = 10.0f;

		if (Input::GetPress(KeyCode::UpArrow)) camera->position.z += CAMERA_SPEED * Time::GetElapsedTime();
		if (Input::GetPress(KeyCode::DownArrow)) camera->position.z -= CAMERA_SPEED * Time::GetElapsedTime();
		if (Input::GetPress(KeyCode::LeftArrow)) camera->position.x += CAMERA_SPEED * Time::GetElapsedTime();
		if (Input::GetPress(KeyCode::RightArrow)) camera->position.x -= CAMERA_SPEED * Time::GetElapsedTime();

	});
}

Camera * CameraController::getCurrentCamera() const {
	return *get<Camera>(currentType);
}
