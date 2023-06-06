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

	//オーディオ
	//Audio::GetInstance()->LoadSound(3, "Resources/Sound/BGM/jto3s-8fzcz.wav");
	//Audio::GetInstance()->LoopWave(3, VolumManager::GetInstance()->GetBGMVolum());
	//シーンチェンジャー
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	//タイトル
	ClearSprite = IKESprite::Create(ImageManager::GAMEOVER, { 0.0f,0.0f });
	ClearSprite->SetSize({1280.0f,720.0f});
}
//更新
void GameOverSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	if (input->TriggerButton(input->B)) {
		sceneChanger_->ChangeStart();
		if (SceneSave::GetInstance()->GetClearFlag(kFirstStage)) {
			if (SceneSave::GetInstance()->GetClearFlag(kSecondStage)) {
		
			} else {
				str = "SECONDSTAGE";
			}
		} else {
			str = "FIRSTSTAGE";
		}
		//Audio::GetInstance()->StopWave(3);
	}
	if (input->TriggerButton(input->A)) {
		sceneChanger_->ChangeStart();
		str = "TITLE";
		//Audio::GetInstance()->StopWave(3);
	}

	sceneChanger_->ChangeScene(str, SceneChanger::Reverse);
	sceneChanger_->Update();

	lightgroup->Update();
	//丸影
	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ 0.0f,0.0f,0.0f }));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

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
	IKESprite::PreDraw();
	ClearSprite->Draw();
	IKESprite::PostDraw();
	sceneChanger_->Draw();
}
//背面
void GameOverSceneActor::BackDraw(DirectXCommon* dxCommon)
{
}
//ImGui描画
void GameOverSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void GameOverSceneActor::Finalize() {
}
