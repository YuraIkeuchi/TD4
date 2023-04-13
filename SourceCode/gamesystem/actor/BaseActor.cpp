#include "BaseActor.h"
#include "VariableCommon.h"
bool BaseActor::s_GameLoop = false;
//共通初期化
void BaseActor::BaseInitialize(DirectXCommon* dxCommon) {
	//ポストエフェクトの初期化
	//(普通)
	postEffect = make_unique<PostEffect>();
	postEffect->Initialize();
	//ポストエフェクトのファイル指定
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/NewToneMapPS.hlsl");

	camerawork = make_unique<CameraWork>();
}
