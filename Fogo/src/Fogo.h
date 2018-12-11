#pragma once

#include "Fogo/Utility.h"
#include "Fogo/Graphics.h"
#include "Fogo/Game.h"

namespace Fogo {
	struct Properties {
		std::vector<std::shared_ptr<Game::Scene>> scenes;
		Utility::Window::Properties window;
		Properties & setScenes(const std::vector<std::shared_ptr<Game::Scene>> & newScenes) { scenes = newScenes; return * this; }
		Properties & setWidth(const UINT & newWidth) { window.width = newWidth; return *this; }
		Properties & setHeight(const UINT & newHeight) { window.height = newHeight; return *this; }
		Properties & setProcedure(const WNDPROC & newProcedure) { window.procedure = newProcedure; return *this; }
		Properties & setTitle(const LPCWSTR & newTitle) { window.title = newTitle; return *this; }
		Properties & setClassName(const LPCWSTR & newClassName) { window.className = newClassName; return *this; }
	};
	void Initialize(const Properties & properties);
	void Finalize();
}
