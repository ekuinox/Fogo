#include "./CameraController.h"
#include "./Camera.h"
#include "./Player.h"
#include "./Model.h"

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

	create<Updater>([&] {
		const auto camera = get<Camera>(currentType);

		if (!camera) return;

		static constexpr auto DISTANCE = 70.0f;
		static constexpr auto HEIGHT = 15.0f;

		const auto & targetMatrix = getMyScene()->get<Player>()->get<Model>()->getWorldMatrix();

		camera->target = { targetMatrix._41, targetMatrix._42 + HEIGHT, targetMatrix._43 };

		const auto & v = XMFLOAT3 { targetMatrix._31, targetMatrix._32, targetMatrix._33 };

		camera->position = { camera->target.x - DISTANCE * v.x, camera->target.y - DISTANCE * v.y, camera->target.z - DISTANCE * v.z };
	});
}

Camera * CameraController::getCurrentCamera() const {
	return *get<Camera>(currentType);
}
