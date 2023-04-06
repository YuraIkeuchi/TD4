#include "TitleSceneActor.h"
#include "Audio.h"
#include "input.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "ParticleEmitter.h"
//初期化
void TitleSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	//オーディオ
	Audio::GetInstance()->LoadSound(0, "Resources/Sound/BGM/ruinsBGM.wav");
	Audio::GetInstance()->LoopWave(0, VolumManager::GetInstance()->GetBGMVolum());

	helper = make_unique< Helper>();

	//タイトル
	IKESprite* TitleSprite_;
	TitleSprite_ = IKESprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	TitleSprite.reset(TitleSprite_);
}
//更新
void TitleSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	if (input->TriggerButton(input->Button_B)) {
		SceneManager::GetInstance()->ChangeScene("FIRSTSTAGE");
		Audio::GetInstance()->StopWave(0);
	}
	lightgroup->Update();
	ParticleEmitter::GetInstance()->FireEffect(100, { 0.0f,23.0f,0.0f }, 5.0f, 0.0f, { 1.0f,0.5f,0.0f,0.5f }, { 1.0f,0.5f,0.0f,0.5f });
	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();
}
//描画
void TitleSceneActor::Draw(DirectXCommon* dxCommon) {
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
		GameDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}
//背景
void TitleSceneActor::ModelDraw(DirectXCommon* dxCommon) {
}
//前面描画
void TitleSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	TitleSprite->Draw();
	IKESprite::PostDraw();
}
//上の描画にスプライトなども混ぜた
void TitleSceneActor::GameDraw(DirectXCommon* dxCommon)
{
	//背景の描画
	ModelDraw(dxCommon);
}
//ImGui描画
void TitleSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void TitleSceneActor::Finalize() {
}
