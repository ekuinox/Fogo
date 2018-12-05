#include "FBXModel.h"

#pragma comment(lib, "libfbxsdk-md.lib")

using namespace Fogo::Graphics::DX12;

FBXModel::FBXModel() {
	const auto & manager = FbxManager::Create();
}
