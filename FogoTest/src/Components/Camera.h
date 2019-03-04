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

	DirectX::XMVECTOR target;
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR up = { 0, 1, 0 };
	Degree fov;
	float nearZ, farZ;

	DirectX::XMMATRIX getView() const;
	DirectX::XMMATRIX getProjection() const;
};
