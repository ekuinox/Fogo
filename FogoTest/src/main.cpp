#include <Fogo.h>
#include "./Scenes/MainScene.h"

auto main(int argc, char ** argv) -> int {
	
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;
	using namespace Microsoft::WRL;

	Initialize(
		Fogo::Properties()
		.setScenes({ std::make_shared<MainScene>() })
		.setWidth(800)
		.setHeight(640)
		.setTitle(L"FogoTest")
	);
	
	return 0;
}
