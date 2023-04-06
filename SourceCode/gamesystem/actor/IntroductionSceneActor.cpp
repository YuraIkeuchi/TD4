#include "IntroductionSceneActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "VariableCommon.h"
#include "imgui.h"
//初期化
void IntroductionSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);

	//ライト
	lightgroup->SetDirLightActive(0, true);
	lightgroup->SetDirLightActive(1, true);
	lightgroup->SetDirLightActive(2, true);
	lightgroup->SetPointLightActive(0, false);
	lightgroup->SetPointLightActive(1, false);

	//ポストエフェクトのファイル指定
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/SepiaPS.hlsl");

	//オーディオ
	Audio::GetInstance()->LoadSound(2, "Resources/Sound/BGM/8bo8k-1eq6w.wav");
	Audio::GetInstance()->LoopWave(2, VolumManager::GetInstance()->GetBGMVolum());

	helper = make_unique< Helper>();
}
//更新
void IntroductionSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	lightgroup->Update();
	//丸影
	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ 0.0f,0.0f,0.0f }));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
}
//描画
void IntroductionSceneActor::Draw(DirectXCommon* dxCommon) {

	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		GameDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		ImGuiDraw(dxCommon);
		GameDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}
//背景
void IntroductionSceneActor::ModelDraw(DirectXCommon* dxCommon) {
}
//前面描画
void IntroductionSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	IKESprite::PostDraw();
}
//上の描画にスプライトなども混ぜた
void IntroductionSceneActor::GameDraw(DirectXCommon* dxCommon)
{
	//スプライトの描画
	ModelDraw(dxCommon);
}
//ImGui描画
void IntroductionSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void IntroductionSceneActor::Finalize() {
}
