#include "SecondStageActor.h"
#include "Audio.h"
#include"Easing.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <algorithm>
#include "BackObj.h"
//初期化
void SecondStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/Boss.wav");
	//ポストエフェクト
	PlayPostEffect = false;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	//各クラス
	Player::GetInstance()->InitState({ 0.0f,0.0f,-5.0f });
	
	//シーンチェンジャー
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	enemymanager = std::make_unique<EnemyManager>("SECONDSTAGE");
	//enemymanager->Initialize(dxCommon);
	text_ = make_unique<BossText>();
	text_->Initialize(dxCommon);
	camerawork->SetBoss(enemymanager->GetBoss());
	camerawork->SetCameraState(CAMERA_BOSSAPPEAR);
	camerawork->SetSceneName("SECONDSTAGE");
	camerawork->Update(camera);
	ui = std::make_unique<UI>();
	ui->Initialize();

	ui->SetBoss(enemymanager->GetBoss());
	BackObj::GetInstance()->Initialize();

	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("SECONDSTAGE");
	loadobj->SetEnemyManager(enemymanager.get());

	m_SceneState = SceneState::IntroState;
}
//更新
void SecondStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	
	//関数ポインタで状態管理
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);

	lightgroup->Update();
}
//描画
void SecondStageActor::Draw(DirectXCommon* dxCommon) {
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
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//解放
void SecondStageActor::Finalize() {
}
//後ろの描画
void SecondStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	////各クラスの描画
	Player::GetInstance()->Draw(dxCommon);
	loadobj->Draw(dxCommon);
	BackObj::GetInstance()->Draw(dxCommon);
	//パーティクル描画
	ParticleEmitter::GetInstance()->FlontDrawAll();
	enemymanager->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void SecondStageActor::FrontDraw(DirectXCommon* dxCommon) {
	
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	if (m_SceneState == SceneState::MainState) {
		ui->Draw();
	}
	text_->SpriteDraw(dxCommon);
	IKESprite::PostDraw();
	sceneChanger_->Draw();
}
//IMGuiの描画
void SecondStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	Player::GetInstance()->ImGuiDraw();
	loadobj->ImGuiDraw();
	camerawork->ImGuiDraw();
	//enemymanager->ImGuiDraw();
}
//登場シーン
void SecondStageActor::IntroUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
	if (enemymanager->GetEnemyFinishAppear()) {
		m_SceneState = SceneState::MainState;
		camerawork->SetCameraState(CAMERA_NORMAL);
	}
	//各クラス更新
	BackObj::GetInstance()->Update();

	Player::GetInstance()->AppearUpdate();
	enemymanager->AppearUpdate();

	camerawork->Update(camera);
}
//バトルシーン
void SecondStageActor::MainUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
	ui->Update();
	if (enemymanager->BossDestroy()) {
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMECLEAR", SceneChanger::NonReverse);
	}

	if (PlayerDestroy()) {
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMEOVER", SceneChanger::Reverse);
	}

	if (input->TriggerKey(DIK_X)) {
		m_SceneState = SceneState::FinishState;
	}
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();

	//各クラス更新
	BackObj::GetInstance()->Update();

	Player::GetInstance()->Update();
	enemymanager->BattleUpdate();
	loadobj->SecondUpdate();
	ParticleEmitter::GetInstance()->Update();

	camerawork->Update(camera);

}
//撃破シーン
void SecondStageActor::FinishUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
}