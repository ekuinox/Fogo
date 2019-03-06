#include <Fogo.h>
#include "./Scene/MainScene.h"

using namespace Fogo;

int main(int argc, char* argv[]) {
	Initialize(
		Properties()
		.setCreateScenes([](System & system) {
			system.createAs<MainScene, Scene>().makeIndex("main");
		})
		.setFirstSceneKey("main")
		.setWidth(1280)
		.setHeight(1080)
		.setTitle(L"FogoSample1")
	);

	return 0;
}
