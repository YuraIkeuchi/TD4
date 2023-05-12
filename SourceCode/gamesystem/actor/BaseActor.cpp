#include "BaseActor.h"
#include "VariableCommon.h"
#include "Player.h"
bool BaseActor::s_GameLoop = false;

void (BaseActor::* BaseActor::stateTable[])() = {
	&BaseActor::IntroUpdate,//要素0
	&BaseActor::MainUpdate, //要素1
	&BaseActor::FinishUpdate,
};
//共通初期化
void BaseActor::BaseInitialize(DirectXCommon* dxCommon, XMFLOAT3 eye, XMFLOAT3 target) {
	//ポストエフェクトの初期化
	//(普通)
	postEffect = make_unique<PostEffect>();
	postEffect->Initialize();
	//ポストエフェクトのファイル指定
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/NewToneMapPS.hlsl");

	//カメラ
	camerawork = make_unique<CameraWork>(eye, target);

	//プレイヤーの弾を全削除する(シーン跨ぎで描画させないため)
	Player::GetInstance()->DeleteBullet();
}

//プレイヤーが死んだか
bool BaseActor::PlayerDestroy() {
	if (Player::GetInstance()->GetHP() <= 0.0f) {
		return true;
	}
	else {
		return false;
	}

	return false;
}