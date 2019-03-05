#pragma once

#include <Fogo.h>

class Camera : public Fogo::Component {
public:
	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 up = { 0, 1, 0 };
	Fogo::Degree fov;
	float nearZ, farZ;

	DirectX::XMMATRIX getView() const;
	DirectX::XMMATRIX getProjection() const;
};
