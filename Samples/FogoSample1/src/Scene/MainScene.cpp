#include "./MainScene.h"
#include "../Store.h"
#include "../Components/CameraController.h"
#include "../Components/Player.h"
#include "../Components/Model.h"

using namespace Fogo;

MainScene::MainScene() {
	static auto once = true;
	if (once) {
		ResourceStore::Insert(VertexShader::BOX, Graphics::CompileVertexShader(L"./resources/shader/VertexShader.hlsl", 0, "main", "vs_5_1"));
		ResourceStore::Insert(PixelShader::BOX, Graphics::CompilePixelShader(L"./resources/shader/PixelShader.hlsl", 0, "main", "ps_5_1"));
		ResourceStore::Insert(TextureType::BOX, std::make_shared<Texture>(L"./resources/Textures/KUTIJE/M_1.jpg"));
		once = false;
	}

	create<CameraController>();

	create<Player>();

	
	const auto & boxes = create<Component>().makeIndex("boxes");
	for (auto i = 0; i < 10; ++i) {
		const auto & box = boxes->create<Model>(
			"./resources/2.fbx",
			ResourceStore::Get<Microsoft::WRL::ComPtr<ID3DBlob>>(VertexShader::BOX),
			ResourceStore::Get<Microsoft::WRL::ComPtr<ID3DBlob>>(PixelShader::BOX)
		);
	}

	create<Updater>([this] {
		static auto isBoxesMoved = false;
		if (!isBoxesMoved) {
			if (const auto & boxes = get<Component>("boxes")) {
				auto n = 0;
				boxes->execute<Model>([&](Model & model) {
					model.setPosition({ -100.0f + n++  * 20.0f, 0.0f, 0.0f });
				});
			}
			isBoxesMoved = true;
		}

		if (Input::GetTrigger(KeyCode::Escape)) {
			PubSub<System::Event, void>::Publish(System::Event::End);
		}

		if (Input::GetTrigger(KeyCode::P)) {
			Console::Log(System::GetInstance());
		}
	});

	create<Finalizer>([] {
		FBXModel::FlushMemos();
	});
}
