#include "FirstStageActor.h"
#include "Audio.h"
#include"Easing.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <algorithm>
#include <HungerGauge.h>

//初期化
void FirstStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/BGM_boss.wav");
	Audio::GetInstance()->LoopWave(1, VolumManager::GetInstance()->GetBGMVolum());
	//ポストエフェクト
	PlayPostEffect = true;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();
	
	font_ = make_unique<Font>();
	font_->LoadFont(dxCommon);

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
	ui->SetBoss(enemymanager->GetBoss());

	backobj = std::make_unique<BackObj>();
	backobj->Initialize();

	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	loadobj->SetEnemyManager(enemymanager.get());
}
//更新
void FirstStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();

	if (enemymanager->BossDestroy() || input->TriggerKey(DIK_X)) {
		SceneManager::GetInstance()->ChangeScene("SECONDSTAGE");
	}

	if (PlayerDestroy()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();
	ui->Update();
	//各クラス更新
	backobj->Update();
	if (nowstate != CONVERSATION) {
		Player::GetInstance()->Update();
		enemymanager->Update();
		loadobj->FirstUpdate();
		ParticleEmitter::GetInstance()->Update();
	}
	camerawork->Update(camera);
	lightgroup->Update();
}
//描画
void FirstStageActor::Draw(DirectXCommon* dxCommon) {
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
		if (nowstate == CONVERSATION) {
			font_->Draw(dxCommon);
			Font::PostDraw(dxCommon);
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
		if (nowstate == CONVERSATION) {
			font_->Draw(dxCommon);
			Font::PostDraw(dxCommon);
		}
		dxCommon->PostDraw();
	}
}
//解放
void FirstStageActor::Finalize() {
}
//後ろの描画
void FirstStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	////各クラスの描画
	Player::GetInstance()->Draw(dxCommon);
	loadobj->Draw(dxCommon);
	backobj->Draw(dxCommon);
	//パーティクル描画
	ParticleEmitter::GetInstance()->FlontDrawAll();
	enemymanager->Draw(dxCommon);
	
	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void FirstStageActor::FrontDraw(DirectXCommon* dxCommon) {
	
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	//blackwindow->Draw();
	conversationwindow->Draw();
	girl->Draw();
	
	IKESprite::PostDraw();
	ui->Draw();
}
//IMGuiの描画
void FirstStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//Player::GetInstance()->ImGuiDraw();
	loadobj->ImGuiDraw();
	HungerGauge::GetInstance()->ImGuiDraw();
	//camerawork->ImGuiDraw();
}
