#pragma once

#include "Fogo/Utility.h"
#include "Fogo/Graphics.h"
#include "Fogo/Game.h"

namespace Fogo {
	struct Properties {
		/*

		Systemにわたすパラメタをメソッドチェーンで指定できるようにするのが目的

		*/
		std::unordered_map<const char *, Game::Scene*> scenes;
		std::function<void(Game::System &)> createScenes;
		Utility::Window::Properties window;
		Game::System::Key first_key;

		Properties & setFirstSceneKey(Game::System::Key newFirstKey) { first_key = newFirstKey; return * this; }
		Properties & setScenes(const std::unordered_map<const char *, Game::Scene*> & newScenes) { scenes = newScenes; return *this; }
		Properties & setCreateScenes(const std::function<void(Game::System &)> newCreateScenes) { createScenes = newCreateScenes; return * this; }
		Properties & setWidth(const UINT & newWidth) { window.setWidth(newWidth); return *this; }
		Properties & setHeight(const UINT & newHeight) { window.setHeight(newHeight); return *this; }
		Properties & setProcedure(const WNDPROC & newProcedure) { window.setProcedure(newProcedure); return *this; }
		Properties & setTitle(const LPCWSTR & newTitle) { window.setTitle(newTitle); return *this; }
		Properties & setClassName(const LPCWSTR & newClassName) { window.setClassName(newClassName); return *this; }
		Properties & makeFullScreen() { window.makeFullScreen(); return *this; }
	};
	
	// Propertiesを受け取り、WindowとSystemを初期化する（開始する）
	void Initialize(const Properties & properties);
	void Finalize();
}
