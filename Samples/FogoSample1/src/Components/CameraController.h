#pragma once

#include <Fogo.h>
#include "./Camera.h"

class CameraController : public Fogo::Component {
	const char * currentType;

	Camera * currentCamera = nullptr;

public:
	CameraController();
	Camera * getCurrentCamera() const;
};
