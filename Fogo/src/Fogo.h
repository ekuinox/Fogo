#pragma once

#include "Fogo/Utility.h"
#include "Fogo/Graphics.h"
#include "Fogo/Game.h"

namespace Fogo {
	struct Properties {
		std::vector<std::shared_ptr<Game::Scene>> scenes;
		Utility::Window::Properties window;
		Properties & setScenes(const std::vector<std::shared_ptr<Game::Scene>> & newScenes) { scenes = newScenes; return * this; }
		Properties & setWidth(const UINT & newWidth) { window.setWidth(newWidth); return *this; }
		Properties & setHeight(const UINT & newHeight) { window.setHeight(newHeight); return *this; }
		Properties & setProcedure(const WNDPROC & newProcedure) { window.setProcedure(newProcedure); return *this; }
		Properties & setTitle(const LPCWSTR & newTitle) { window.setTitle(newTitle); return *this; }
		Properties & setClassName(const LPCWSTR & newClassName) { window.setClassName(newClassName); return *this; }
		Properties & makeFullScreen() { window.makeFullScreen(); return *this; }
	};
	void Initialize(const Properties & properties);
	void Finalize();
}
