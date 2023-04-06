#include "BaseActor.h"
#include "VariableCommon.h"

//共通初期化
void BaseActor::BaseInitialize(DirectXCommon* dxCommon) {
	//ポストエフェクトの初期化
	//(普通)
	postEffect = make_unique<PostEffect>();
	postEffect->Initialize();
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/SepiaPS.hlsl");

	camerawork = make_unique<CameraWork>();
}
