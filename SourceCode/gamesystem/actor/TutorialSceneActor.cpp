#include "TutorialSceneActor.h"
#include "Audio.h"
#include"Easing.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <algorithm>

//状態遷移
/*stateの並び順に合わせる*/
void (TutorialSceneActor::* TutorialSceneActor::stateTable[])() = {
	&TutorialSceneActor::IntroState,//
	&TutorialSceneActor::MoveState,//
	&TutorialSceneActor::ConversationCatchState,//
	&TutorialSceneActor::CatchGhorstState,//
	&TutorialSceneActor::CompleteState,//
};

void TutorialSceneActor::IntroState() {
	frame++;
	nowframe = frame / maxframe;
	if (frame >= maxframe) {
		frame = maxframe;
	}
	window_pos.y = Ease(Out, Sine, nowframe, WinApp::window_height + 100, WinApp::window_height - 100);
	window_size.x = Ease(Out, Sine, nowframe, 0, 1300);
	window_size.y = Ease(Out, Sine, nowframe, 0, 223);
	black_color.w = Ease(Out, Sine, nowframe, 0, 1);
	girl_color.w = Ease(Out, Sine, nowframe, 0, 1);

	conversationwindow->SetPosition(window_pos);
	conversationwindow->SetSize(window_size);
	blackwindow->SetColor(black_color);
	girl->SetColor(girl_color);
	if (input->TriggerKey(DIK_SPACE)) {
		nowstate_ = state::MOVE;
	}
}

void TutorialSceneActor::MoveState() {

	if (input->TriggerKey(DIK_SPACE)) {
		nowstate_ = state::CONVERSATION_CATCH;
	}
}

void TutorialSceneActor::ConversationCatchState() {


	if (input->TriggerKey(DIK_SPACE)) {
		nowstate_ = state::CATCHGHORST;
	}
}

void TutorialSceneActor::CatchGhorstState() {


	if (input->TriggerKey(DIK_SPACE)) {
		nowstate_ = state::COMPLETE;
	}

}

void TutorialSceneActor::CompleteState() {


}

//初期化
void TutorialSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/BGM_boss.wav");
	//ポストエフェクト
	PlayPostEffect = true;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	font_ = make_unique<Font>();
	font_->Initialize(dxCommon, { 1.f,1.f,0.f,1.f }, { 300.f,360.f });

	//各クラス
	Player::GetInstance()->InitState({ 0.0f,0.0f,0.0f });
	camerawork->Update(camera);
	ui = std::make_unique<UI>();
	ui->Initialize();

	conversationwindow = IKESprite::Create(ImageManager::WINDOW, window_pos);
	conversationwindow->SetAnchorPoint({ 0.5f,0.5f });
	conversationwindow->SetSize(window_size);

	blackwindow = IKESprite::Create(ImageManager::BLACKWINDOW, {});

	girl = IKESprite::Create(ImageManager::GIRL, { -100.f,300.f });
	girl->SetColor(girl_color);

	enemymanager = std::make_unique<EnemyManager>("FIRSTSTAGE");

	backobj = std::make_unique<BackObj>();
	backobj->Initialize();

	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	loadobj->SetEnemyManager(enemymanager.get());
}
//更新
void TutorialSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	if (PlayerDestroy()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();

	//状態移行(stateに合わせる)
	(this->*stateTable[static_cast<size_t>(nowstate_)])();

	//各クラス更新
	backobj->Update();
	if (static_cast<int>(nowstate_) % 2 == 1) {
		ui->Update();
		Player::GetInstance()->Update();
		enemymanager->Update();
		loadobj->FirstUpdate();
		ParticleEmitter::GetInstance()->Update();
	}
	camerawork->Update(camera);
	lightgroup->Update();
}
//描画
void TutorialSceneActor::Draw(DirectXCommon* dxCommon) {
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
		if (static_cast<int>(nowstate_) % 2 == 1) {
			font_->Draw(dxCommon);
		}
		postEffect->ImGuiDraw();
		dxCommon->PostDraw();
	} else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		if (static_cast<int>(nowstate_) % 2 == 1) {
			font_->Draw(dxCommon);
		}
		dxCommon->PostDraw();
	}
}
//解放
void TutorialSceneActor::Finalize() {
}
//後ろの描画
void TutorialSceneActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	////各クラスの描画
	Player::GetInstance()->Draw(dxCommon);
	loadobj->Draw(dxCommon);
	backobj->Draw(dxCommon);
	enemymanager->Draw(dxCommon);

	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void TutorialSceneActor::FrontDraw(DirectXCommon* dxCommon) {
	//パーティクル描画
	ParticleEmitter::GetInstance()->FlontDrawAll();
	//完全に前に書くスプライト
	if (static_cast<int>(nowstate_) % 2 == 0) {
		IKESprite::PreDraw();
		blackwindow->Draw();
		conversationwindow->Draw();
		girl->Draw();
		IKESprite::PostDraw();
	}
	if (static_cast<int>(nowstate_) % 2 == 1) {
		ui->Draw();
	}
}
//IMGuiの描画
void TutorialSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//Player::GetInstance()->ImGuiDraw();
	//loadobj->ImGuiDraw();
	//enemymanager->ImGuiDraw();
	//camerawork->ImGuiDraw();
}
