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
	camerawork->SetEye({ 0,5,-30 });
	camerawork->SetTarget({ 0,5,0 });
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

	photo[Photo_Out_Top] = IKESprite::Create(ImageManager::PHOTO_OUT, { 0,-360 });
	photo[Photo_Out_Under] = IKESprite::Create(ImageManager::PHOTO_OUT, { 0,1080 });
	for (int i = Photo_Out_Top; i <= Photo_Out_Under; i++) {
		photo[i]->SetSize({ 1280,360 });
	}
}
//更新
void EndRollActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
	BackObj::GetInstance()->Update();
	endobj->Update();
	//各クラス更新
	Player::GetInstance()->EndRollUpdate(m_EndTimer);
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
	float l_AddFrame = 0.003f;
	float l_AddColor = 0.01f;
	float l_EndSepia = 0.1f;
	camerawork->DefUpda(camera);
	m_EndTimer++;

	if (m_EndTimer == 1620) {
		isShutter = true;
	}

	if (isShutter) {
		if (ShutterEffect()) {
			stopTime++;
			PlayPostEffect = true;
			if (stopTime >= stopTimerMax) {
				if (ShutterFeed()) {
					ShutterReset();
				}
			}
		}
	}

	//セピアカラーになる
	if (PlayPostEffect) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
		}
		m_Sepia = Ease(In, Cubic, m_Frame, m_Sepia, l_EndSepia);
	}

	if (m_EndTimer == 1900) {
		m_Change = true;
	}
	if (m_Change) {
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("TITLE", SceneChanger::NonReverse);
	}
}
void EndRollActor::MainUpdate(DebugCamera* camera) {
	
	camerawork->DefUpda(camera);
	
}
void EndRollActor::FinishUpdate(DebugCamera* camera) {
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
	Player::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();
	ParticleEmitter::GetInstance()->FlontDrawAll();
	IKESprite::PreDraw();
	for (int i = Photo_Out_Top; i <= Photo_Out_Under; i++) {
		photo[i]->Draw();
	}
	IKESprite::PostDraw();

}
//ImGui描画
void EndRollActor::ImGuiDraw(DirectXCommon* dxCommon) {
	///endobj->ImGuiDraw();
	ImGui::Begin("End");
	ImGui::Text("Timer:%d", m_EndTimer);
	ImGui::End();
}
//解放
void EndRollActor::Finalize() {
}
//シャッターのエフェクト
bool EndRollActor::ShutterEffect() {
	shutterTime += 1.0f / shutterTimeMax;
	shutterTime = clamp(shutterTime, 0.0f, 1.0f);

	shutterHight[0] = Ease(Out, Quad, shutterTime, -360, 0);
	shutterHight[1] = Ease(Out, Quad, shutterTime, 1080, 360);

	photo[Photo_Out_Top]->SetPosition({ 0,shutterHight[0] });
	photo[Photo_Out_Under]->SetPosition({ 0,shutterHight[1] });
	if (shutterTime == 1.0f) {
		return true;
	}
	else {
		return false;
	}
}
//フェーフォ
bool EndRollActor::ShutterFeed() {
	feedTimer += 1.0f / feedTimeMax;
	float color = Ease(Out, Linear, feedTimer, 1.0f, 0.0f);
	photo[Photo_Out_Top]->SetColor({ 1,1,1, color });
	photo[Photo_Out_Under]->SetColor({ 1,1,1,color });
	feedTimer = clamp(feedTimer, 0.0f, 1.0f);
	if (feedTimer == 1.0f) {
		return true;
	}
	else {
		return false;
	}
}
//リセット
void EndRollActor::ShutterReset() {
	isShutter = false;
	stopTime = 0;
	shutterHight[Photo_Out_Top] = -360.0f;
	shutterHight[Photo_Out_Under] = 1080.0f;
	photo[Photo_Out_Top]->SetPosition({ 0,shutterHight[Photo_Out_Top] });
	photo[Photo_Out_Under]->SetPosition({ 0,shutterHight[Photo_Out_Under] });
	photo[Photo_Out_Top]->SetColor({ 1,1,1,1 });
	photo[Photo_Out_Under]->SetColor({ 1,1,1,1 });
	shutterTime = 0.0f;
	feedTimer = 0.0f;
}
