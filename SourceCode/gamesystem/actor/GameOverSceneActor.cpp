#include "GameOverSceneActor.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "VariableCommon.h"
#include "Audio.h"

//初期化
void GameOverSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);
	camerawork->SetEye({ 0,10,0 });
	camerawork->SetTarget({ 0,0,0 });
	camerawork->SetCameraState(CAMERA_NORMAL);
	camerawork->Update(camera);
	//オーディオ
	//Audio::GetInstance()->LoadSound(3, "Resources/Sound/BGM/jto3s-8fzcz.wav");
	//Audio::GetInstance()->LoopWave(3, VolumManager::GetInstance()->GetBGMVolum());
	//シーンチェンジャー
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	SutoponObj = make_unique<IKEObject3d>();
	SutoponObj->Initialize();
	SutoponObj->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Sutopon));
	//タイトル
	ClearSprite = IKESprite::Create(ImageManager::GAMEOVER, { 0.0f,0.0f });
	ClearSprite->SetSize({1280.0f,720.0f});
	ClearSprite->SetColor({0,0,0,1});
}
//更新
void GameOverSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	if (input->TriggerButton(input->A)|| input->TriggerButton(input->B)) {
		if (sceneChanger_->GetEasingStart()) { return; }
		sceneChanger_->ChangeStart();
		if (input->TriggerButton(input->A)) {
			str = "TITLE";
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button_Over.wav", VolumManager::GetInstance()->GetSEVolum());
		} else {
			str = NextStageName();
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/Voice_Retry.wav", VolumManager::GetInstance()->GetSEVolum());
		}
		m_SceneState = SceneState::FinishState;
 	}
	camerawork->Update(camera);
	SutoponObj->Update();
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
}
//描画
void GameOverSceneActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
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
		BackDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}
//前面描画
void GameOverSceneActor::FrontDraw() {
	sceneChanger_->Draw();
}
void GameOverSceneActor::IntroUpdate(DebugCamera* camera) {


}
void GameOverSceneActor::MainUpdate(DebugCamera* camera) {



}
void GameOverSceneActor::FinishUpdate(DebugCamera* camera) {
	sceneChanger_->ChangeScene(str, SceneChanger::Reverse);
	sceneChanger_->Update();
}
string GameOverSceneActor::NextStageName() {
	string str = "TUTORIAL";
	for (int i = 0; i < kMaxStage; i++) {
		if (!SceneSave::GetInstance()->GetLoseFlag((SeceneCategory)i)) { continue; }
		switch (i) {
		case 1:
			str = "FIRSTSTAGE";
			break;
		case 2:
			str = "SECONDSTAGE";
			break;
		case 3:
			str = "THIRDSTAGE";
			break;
		case 4:
			str = "FOURTHSTAGE";
			break;
		case 5:
			str = "FIVESTAGE";
			break;
		case 6:
			str = "SIXSTAGE";
			break;
		case 7:
			str = "SEVENSTAGE";
			break;
		default:
			break;
		}
		SceneSave::GetInstance()->SetLoseFlag((SeceneCategory)i, false);
		break;
	}
	return str;
}
//背面
void GameOverSceneActor::BackDraw(DirectXCommon* dxCommon)
{
	IKESprite::PreDraw();
	ClearSprite->Draw();
	IKESprite::PostDraw();

	IKEObject3d::PreDraw();
	SutoponObj->Draw();
	IKEObject3d::PostDraw();
}
//ImGui描画
void GameOverSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//SceneSave::GetInstance()->ImGuiDraw();
}
//解放
void GameOverSceneActor::Finalize() {
}
