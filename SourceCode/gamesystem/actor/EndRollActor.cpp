#include "EndRollActor.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "Audio.h"
#include "BackObj.h"
#include "Helper.h"
//初期化
void EndRollActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);
	//このシーンだけセピアカラーつかう
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/SepiaPS.hlsl");
	camerawork->SetEye({ 0,0,-30 });
	camerawork->SetTarget({ 0,0,0 });
	camerawork->SetCameraState(CAMERA_NORMAL);
	camerawork->DefUpda(camera);
	//オーディオ
	//Audio::GetInstance()->LoadSound(3, "Resources/Sound/BGM/jto3s-8fzcz.wav");
	//Audio::GetInstance()->LoopWave(3, VolumManager::GetInstance()->GetBGMVolum());
	//シーンチェンジャー
	PlayPostEffect = false;
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();
	m_SceneState = SceneState::IntroState;
	
	endobj = make_unique<EndRollObj>();
	endobj->Initialize();
	BackObj::GetInstance()->Initialize();
}
//更新
void EndRollActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
	BackObj::GetInstance()->Update();
	endobj->Update();
	sceneChanger_->Update();

	postEffect->SetSepia(m_Sepia);
}
//描画
void EndRollActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw(dxCommon);
		FrontDraw();
		postEffect->ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		dxCommon->PreDraw();
		ImGuiDraw(dxCommon);
		BackDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}
//前面描画
void EndRollActor::FrontDraw() {
	sceneChanger_->Draw();
}
void EndRollActor::IntroUpdate(DebugCamera* camera) {
	camerawork->DefUpda(camera);
	m_EndTimer++;

	if (m_EndTimer == 100) {
		m_SceneState = SceneState::MainState;
		PlayPostEffect = true;
	}
}
void EndRollActor::MainUpdate(DebugCamera* camera) {
	float l_AddFrame = 0.005f;
	float l_AddColor = 0.01f;
	float l_EndSepia = 0.1f;
	camerawork->DefUpda(camera);
	//セピアカラーになる
	if (PlayPostEffect) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
		}
		m_Sepia = Ease(In, Cubic, m_Frame, m_Sepia, l_EndSepia);
	}
}
void EndRollActor::FinishUpdate(DebugCamera* camera) {
	//sceneChanger_->ChangeScene(str, SceneChanger::Reverse);
	//sceneChanger_->Update();
	camerawork->DefUpda(camera);
}

//背面
void EndRollActor::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	//ClearSprite->Draw();
	IKESprite::PostDraw();

	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);
	endobj->Draw(dxCommon);
	IKEObject3d::PostDraw();
	ParticleEmitter::GetInstance()->FlontDrawAll();
	IKESprite::PreDraw();
	IKESprite::PostDraw();

}
//ImGui描画
void EndRollActor::ImGuiDraw(DirectXCommon* dxCommon) {
	endobj->ImGuiDraw();
}
//解放
void EndRollActor::Finalize() {
}
