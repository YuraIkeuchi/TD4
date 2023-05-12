#include "FirstStageActor.h"
#include "Audio.h"
#include"Easing.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <algorithm>

//初期化
void FirstStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/Boss.wav");
	//ポストエフェクト
	PlayPostEffect = true;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	//各クラス
	Player::GetInstance()->InitState({ 0.0f,0.0f,0.0f });
	camerawork->Update(camera);
	ui = std::make_unique<UI>();
	ui->Initialize();

	conversationwindow = IKESprite::Create(ImageManager::WINDOW, window_pos);
	conversationwindow->SetAnchorPoint({ 0.5f,0.5f });
	conversationwindow->SetSize(window_size);

	blackwindow = IKESprite::Create(ImageManager::BLACKWINDOW, {});


	enemymanager = std::make_unique<EnemyManager>("FIRSTSTAGE");

	backobj = std::make_unique<BackObj>();
	backobj->Initialize();

	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad();
}
//更新
void FirstStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();

	if (Input::GetInstance()->TriggerButton(Input::A)) {
		SceneManager::GetInstance()->ChangeScene("SECONDSTAGE");
	}
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();

	if (input->Pushkey(DIK_A) &&
		nowstate != CONVERSATION) {
		nowstate = CONVERSATION;
		frame = {};
	}
	if (input->Pushkey(DIK_S) &&
		nowstate != FIGHT) {
		nowstate = FIGHT;
		frame = {};
	}
	if (nowstate == CONVERSATION) {
		frame++;
		nowframe = frame / maxframe;
		if (frame >= maxframe) {
			frame = maxframe;
		}
		window_pos.y = Ease(Out, Sine, nowframe, WinApp::window_height + 100, WinApp::window_height - 100);
		window_size.x = Ease(Out, Sine, nowframe, 0, 1300);
		window_size.y = Ease(Out, Sine, nowframe, 0, 223);
		black_color.w = Ease(Out, Sine, nowframe, 0, 1);
	} else if (nowstate == FIGHT) {
		frame++;
		nowframe = frame / maxframe;
		if (frame >= maxframe) {
			frame = maxframe;
		}
		window_pos.y = Ease(Out, Sine, nowframe, WinApp::window_height - 100, WinApp::window_height + 100);
		window_size.x = Ease(Out, Sine, nowframe, 1300, 0);
		window_size.y = Ease(Out, Sine, nowframe, 225, 0);
		black_color.w = Ease(Out, Sine, nowframe, 1, 0);
	}


	conversationwindow->SetPosition(window_pos);
	conversationwindow->SetSize(window_size);
	blackwindow->SetColor(black_color);

	//各クラス更新
	backobj->Update();
	if (nowstate != CONVERSATION) {
		Player::GetInstance()->Update();
		enemymanager->Update();
		loadobj->Update();
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
		postEffect->ImGuiDraw();
		dxCommon->PostDraw();
	} else {
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
void FirstStageActor::Finalize() {
}
//後ろの描画
void FirstStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	////各クラスの描画
	Player::GetInstance()->Draw(dxCommon);
	loadobj->Draw(dxCommon);
	backobj->Draw(dxCommon);
	enemymanager->Draw(dxCommon);
	
	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void FirstStageActor::FrontDraw(DirectXCommon* dxCommon) {
	//パーティクル描画
	ParticleEmitter::GetInstance()->FlontDrawAll();
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	blackwindow->Draw();
	conversationwindow->Draw();
	//ui->Draw();
	IKESprite::PostDraw();
}
//IMGuiの描画
void FirstStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	Player::GetInstance()->ImGuiDraw();
	loadobj->ImGuiDraw();
	//camerawork->ImGuiDraw();
}
