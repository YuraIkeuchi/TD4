#include "ClearSceneActor.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "VariableCommon.h"
#include "Audio.h"

//初期化
void ClearSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);

	//オーディオ
	Audio::GetInstance()->LoadSound(3, "Resources/Sound/BGM/jto3s-8fzcz.wav");
	Audio::GetInstance()->LoopWave(3, VolumManager::GetInstance()->GetBGMVolum());

	helper = make_unique< Helper>();

	//タイトル
	IKESprite* ClearSprite_;
	ClearSprite_ = IKESprite::Create(ImageManager::GAMECLEAR, { 0.0f,0.0f });
	ClearSprite.reset(ClearSprite_);
}
//更新
void ClearSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	if (input->TriggerButton(input->Button_X)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
		Audio::GetInstance()->StopWave(3);
	}
	lightgroup->Update();
	//丸影
	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ 0.0f,0.0f,0.0f }));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

}
//描画
void ClearSceneActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		GameDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw();
		ImGuiDraw(dxCommon);
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
void ClearSceneActor::ModelDraw(DirectXCommon* dxCommon) {

}
//前面描画
void ClearSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	ClearSprite->Draw();
	IKESprite::PostDraw();
}
//上の描画にスプライトなども混ぜた
void ClearSceneActor::GameDraw(DirectXCommon* dxCommon)
{
	//スプライトの描画
	ModelDraw(dxCommon);
}
//ImGui描画
void ClearSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void ClearSceneActor::Finalize() {
}
