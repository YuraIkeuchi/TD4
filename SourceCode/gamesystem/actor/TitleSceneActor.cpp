﻿#include "TitleSceneActor.h"
#include "input.h"
#include "ImageManager.h"
#include "ParticleEmitter.h"
#include "Menu.h"
#include "SelectScene.h"
//初期化
void TitleSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	//オーディオ
	Audio::GetInstance()->LoopWave(AUDIO_TITLE, VolumManager::GetInstance()->GetBGMVolum()+2.0f);

	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();
	sceneChanger_->ChangeFeedStart();
	//タイトル
	TitleSprite = IKESprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	TitleWordSprite= IKESprite::Create(ImageManager::TITLEWORD, pos);
	TitleWordSprite->SetSize(size);
	PlayPostEffect = true;
	menu = make_unique<Menu>();
	menu->Initialize();

	camerawork->SetCameraState(CAMERA_TITLE);
	camerawork->Update(camera);

	//各クラス
	Player::GetInstance()->LoadResource();
	Player::GetInstance()->InitState({ 0.0f,-2.0f,-30.0f });
}
//更新
void TitleSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B) || input->Pushkey(DIK_SPACE)) &&
		!sceneChanger_->GetEasingStart()) {
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button_Decide.wav", VolumManager::GetInstance()->GetSEVolum());
		sceneChanger_->ChangeStart();
		Audio::GetInstance()->StopWave(AUDIO_TITLE);
	}
	frame += 0.01f;
	TitleWordSprite->SetPosition({pos.x+(sinf(frame*5.0f) * 25.0f), pos.y + (sinf(frame) *50.0f)});

	if (sceneChanger_->GetEasingStart()) {
		string str = "LOAD";
		sceneChanger_->ChangeSceneExtra(str, SceneChanger::NonReverse);
	}
	sceneChanger_->Update();
	lightgroup->Update();
	ParticleEmitter::GetInstance()->FireEffect(100, { 0.0f,23.0f,0.0f }, 5.0f, 0.0f, { 1.0f,0.5f,0.0f,0.5f }, { 1.0f,0.5f,0.0f,0.5f });
	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	postEffect->SetCloseRad(SelectScene::GetIns()->GetCloseIconRad());
	if (SelectScene::GetIns()->GetPedestal() != nullptr) {
		if (SelectScene::GetIns()->GetCloseScl() < 10000.f)
			SelectScene::GetIns()->Upda();
		else
			SelectScene::GetIns()->ResetParama();
	}

	Player::GetInstance()->TitleUpdate();
	menu->Update();
	camerawork->Update(camera);
}
//描画
void TitleSceneActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw();
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw();
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
}

//前面描画
void TitleSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	//TitleWordSprite->Draw();
	SelectScene::GetIns()->Draw_Sprite();
	IKESprite::PostDraw();
	menu->Draw();
	sceneChanger_->Draw();
}
//背面描画
void TitleSceneActor::BackDraw(DirectXCommon* dxCommon)
{
	IKESprite::PreDraw();
	TitleSprite->Draw();
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	Player::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//ImGui描画
void TitleSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	Player::GetInstance()->ImGuiDraw();
}
//解放
void TitleSceneActor::Finalize() {

}
