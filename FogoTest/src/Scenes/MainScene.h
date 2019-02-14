#pragma once

#include <Fogo.h>
#include "../Square.h"
#include "../FBXSample.h"
#include <iostream>

class MainScene : public Fogo::Game::Scene {
public:
	void initialize() override;
	void update() override;
	void finalize() override;
};
