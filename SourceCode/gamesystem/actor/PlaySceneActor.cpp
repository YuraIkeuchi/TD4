#include "PlaySceneActor.h"
#include "Audio.h"
#include"Easing.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include <algorithm>

//初期化
void PlaySceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
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
	player = new Player({ 0.0f, 0.0f, 0.0f });
	camerawork->SetPlayer(player);
	ui.reset(new UI());
	ui->Initialize();

	conversationwindow = IKESprite::Create(ImageManager::WINDOW, window_pos);
	conversationwindow->SetAnchorPoint({ 0.5f,0.5f });
	conversationwindow->SetSize(window_size);

	boss.reset(new FirstBoss());
	boss->Initialize();
	enemymanager.reset(new EnemyManager(player));

	backobj.reset(new BackObj());
	backobj->Initialize();

	loadobj.reset(new LoadStageObj());
	loadobj->AllLoad(player);
}
//更新
void PlaySceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
	Input* input = Input::GetInstance();
	
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();

	if (input->Pushkey(DIK_A)) {
		test = true;
	}
	if (test) {
		frame++;
		nowframe = frame / maxframe;
		if (frame >= maxframe) {
			frame = maxframe;
		}
	}
	window_pos.y = Ease(Out, Sine, nowframe, WinApp::window_height+100, WinApp::window_height-100);
	window_size.x= Ease(Out, Sine, nowframe,0, 700);
	window_size.y = Ease(Out, Sine, nowframe, 0, 150);
	conversationwindow->SetPosition(window_pos);
	conversationwindow->SetSize(window_size);

	//各クラス更新
	camerawork->Update(camera);
	lightgroup->Update();
	player->Update();
	//enemymanager->Update();
	boss->Update();
	backobj->Update();
	loadobj->Update();
	ParticleEmitter::GetInstance()->Update();
}

//描画
void PlaySceneActor::Draw(DirectXCommon* dxCommon)
{
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
		dxCommon->PostDraw();
	}
}
//解放
void PlaySceneActor::Finalize()
{
}

//後ろの描画
void PlaySceneActor::BackDraw(DirectXCommon* dxCommon)
{
	IKEObject3d::PreDraw();
	////各クラスの描画
	player->Draw(dxCommon);
	boss->Draw(dxCommon);
	loadobj->Draw(dxCommon);
	//enemymanager->Draw(dxCommon);
	backobj->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void PlaySceneActor::FrontDraw(DirectXCommon* dxCommon) {
	//パーティクル描画
	ParticleEmitter::GetInstance()->FlontDrawAll();
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	conversationwindow->Draw();
	//ui->Draw();
	IKESprite::PostDraw();
}
//IMGuiの描画
void PlaySceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	player->ImGuiDraw();
	loadobj->ImGuiDraw();
}
