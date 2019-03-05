#pragma once

#include <Fogo.h>

class Camera : public Fogo::Game::Component {
public:
	class Degree {
	private:
		float raw;
	public:
		Degree(float initValue);
		Degree();
		float toRadian() const;
		operator float() const;
		Degree & operator=(const float newValue);
	};

	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 up = { 0, 1, 0 };
	Degree fov;
	float nearZ, farZ;

	DirectX::XMMATRIX getView() const;
	DirectX::XMMATRIX getProjection() const;
};
