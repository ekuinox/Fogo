#pragma once

#include <Fogo.h>
#include <iostream>
#include "../Components/Camera.h"

class MainScene : public Fogo::Game::Scene {
public:
	MainScene();
	virtual void update();
protected:
	Camera * camera;
};
