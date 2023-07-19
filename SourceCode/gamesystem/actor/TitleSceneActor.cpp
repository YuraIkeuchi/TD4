#include "TitleSceneActor.h"
#include "input.h"
#include <Easing.h>
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
	PlayPostEffect =true;
	Menu::GetIns()->Init();
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

	Menu::GetIns()->Upda();
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
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}

//前面描画
void TitleSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	TitleSprite->Draw();
	TitleWordSprite->Draw();
	Menu::GetIns()->Draw();
	SelectScene::GetIns()->Draw_Sprite();
	IKESprite::PostDraw();
	sceneChanger_->Draw();
}
//背面描画
void TitleSceneActor::BackDraw(DirectXCommon* dxCommon)
{
}
//ImGui描画
void TitleSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void TitleSceneActor::Finalize() {

}
