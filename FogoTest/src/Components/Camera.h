#pragma once

#include <Fogo.h>

class Camera : public Fogo::Game::Component {
public:
	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 up = { 0, 1, 0 };
	Fogo::Utility::Degree fov;
	float nearZ, farZ;

	DirectX::XMMATRIX getView() const;
	DirectX::XMMATRIX getProjection() const;
};
