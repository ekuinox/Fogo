#include "./Camera.h"

using namespace DirectX;
using Fogo::Utility::Window;

XMMATRIX Camera::getView() const {
	return XMMatrixLookAtLH (
		XMVECTOR { position.x, position.y, position.z },
		XMVECTOR { target.x, target.y, target.z },
		XMVECTOR { up.x, up.y, up.z }
	);
}

XMMATRIX Camera::getProjection() const {
	return XMMatrixPerspectiveFovLH(
		fov.toRadian(),
		static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()),
		nearZ,
		farZ
	);
}
