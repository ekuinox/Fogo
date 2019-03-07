#pragma once

#include <Fogo.h>
#include "./Camera.h"

class CameraController : public Fogo::Component {
	const char * currentType;
	float distanceToTarget;

public:
	CameraController();
	Camera * getCurrentCamera() const;
};
