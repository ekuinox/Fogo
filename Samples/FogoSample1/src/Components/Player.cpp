#include "./Player.h"
#include "./Model.h"
#include "../Store.h"

using namespace Fogo;

Player::Player() {
	const auto & model = create<Model>(
		"./resources/model/unitychan.fbx",
		ResourceStore::Get<Microsoft::WRL::ComPtr<ID3DBlob>>(VertexShader::BOX),
		ResourceStore::Get<Microsoft::WRL::ComPtr<ID3DBlob>>(PixelShader::BOX)
	)->create<Initializer>([&] {
		get<Model>()->setWorldMatrix(DirectX::XMMatrixIdentity() * DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f));
	});
	create<Updater>([&] { update(); });
}

void Player::update() {
	static constexpr auto SPEED = 40.0f;

	const auto & model = get<Model>();
	if (!model) return;

	model->translation = { 0, 0, 0 };
	model->rotation = { 0, 0, 0 };

	if (Input::GetPress(KeyCode::A)) model->rotation.y -= Time::GetElapsedTime() * SPEED;
	if (Input::GetPress(KeyCode::D)) model->rotation.y += Time::GetElapsedTime() * SPEED;
	if (Input::GetPress(KeyCode::W)) model->translation.z += Time::GetElapsedTime() * SPEED;
	if (Input::GetPress(KeyCode::S)) model->translation.z -= Time::GetElapsedTime() * SPEED;
	if (Input::GetPress(KeyCode::LShift)) model->translation.y -= Time::GetElapsedTime() * SPEED;
	if (Input::GetPress(KeyCode::Space)) model->translation.y += Time::GetElapsedTime() * SPEED;
}
