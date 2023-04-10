#include "PlaySceneActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "ParticleEmitter.h"
#include "ModelManager.h"
#include <algorithm>

//初期化
void PlaySceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/Boss.wav");
	//ポストエフェクトのファイル指定
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/NewToneMapPS.hlsl");

	PlayPostEffect = true;

	//タイトル
	IKESprite* PlaySprite_;
	PlaySprite_ = IKESprite::Create(ImageManager::PLAY, { 0.0f,0.0f });
	PlaySprite.reset(PlaySprite_);

	modelGround = ModelManager::GetInstance()->GetModel(ModelManager::Ground);
	modelCube = ModelManager::GetInstance()->GetModel(ModelManager::Cube);

	objCube = make_unique<IKEObject3d>();
	objCube->Initialize();
	objCube->SetModel(modelCube);
	objCube->SetPosition({ 0.0f,5.0f,0.0f });

	objGround = make_unique<IKEObject3d>();
	objGround->Initialize();
	objGround->SetModel(modelGround);
	objGround->SetPosition({ 0.0f,0.0f,0.0f });


	player.reset(new Player({0.f,0.f,0.f}));
	ui.reset(new UI());
	ui->Initialize();
}
//更新
void PlaySceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
	Input* input = Input::GetInstance();
	if (input->TriggerButton(input->A)) {
		//SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
		//Audio::GetInstance()->StopWave(1);
	}
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	camerawork->Update(camera);

	lightgroup->Update();

	objCube->Update();
	objGround->Update();

	player->Update();
}
//普通の更新
void PlaySceneActor::NormalUpdate() {
	VolumManager::GetInstance()->Update();
}
//ボス登場の更新
void PlaySceneActor::BossAppUpdate() {
}
//ボス終了の更新
void PlaySceneActor::BossEndUpdate() {
}
//描画
void PlaySceneActor::Draw(DirectXCommon* dxCommon)
{
	//描画方法
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		postEffect->ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//解放
void PlaySceneActor::Finalize()
{
}
//モデルの描画
void PlaySceneActor::ModelDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	objCube->Draw();
	objGround->Draw();
	player->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//後ろの描画
void PlaySceneActor::BackDraw(DirectXCommon* dxCommon)
{
#pragma endregion
	ModelDraw(dxCommon);
}
//ポストエフェクトがかからない
void PlaySceneActor::FrontDraw(DirectXCommon* dxCommon) {
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	ui->Draw();
	IKESprite::PostDraw();
}
//IMGuiの描画
void PlaySceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("Pos");
	ImGui::Text("POSX:%f", objCube->GetPosition().x);
	ImGui::Text("POSY:%f", objCube->GetPosition().y);
	ImGui::Text("POSZ:%f", objCube->GetPosition().z);
	ImGui::End();
	camerawork->ImGuiDraw();
}
//普通の描画
void PlaySceneActor::NormalDraw(DirectXCommon* dxCommon) {
}
//ボス登場シーンの描画
void PlaySceneActor::BossAppDraw(DirectXCommon* dxCommon) {
}
//ボス終了シーンの描画
void PlaySceneActor::BossEndDraw(DirectXCommon* dxCommon) {
	ParticleEmitter::GetInstance()->FlontDrawAll();
}
