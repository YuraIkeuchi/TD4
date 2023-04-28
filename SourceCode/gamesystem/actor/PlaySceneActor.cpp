#include "PlaySceneActor.h"
#include "Audio.h"
#include"Easing.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "ParticleEmitter.h"
#include "ModelManager.h"
#include <algorithm>
#include "Block.h"

//初期化
void PlaySceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/Boss.wav");

	PlayPostEffect = true;
	ParticleEmitter::GetInstance()->AllDelete();

	player = new Player({ 0.0f, 0.0f, 0.0f });
	//player.reset(new Player({0.f,0.f,0.f}));
	camerawork->SetPlayer(player);
	ui.reset(new UI());
	ui->Initialize();

	conversationwindow = IKESprite::Create(ImageManager::WINDOW, window_pos);
	conversationwindow->SetAnchorPoint({ 0.5f,0.5f });
	conversationwindow->SetSize(window_size);

	blackwindow = IKESprite::Create(ImageManager::BLACKWINDOW,{});


	boss.reset(new FirstBoss());
	boss->Initialize();
	enemymanager.reset(new EnemyManager(player));

	backobj.reset(new BackObj());
	backobj->Initialize();

	loadfood.reset(new LoadFood());
	loadfood->Load(player);

	loadghost.reset(new LoadGhost());
	loadghost->Load(player);
}
//更新
void PlaySceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
	Input* input = Input::GetInstance();
	if (input->TriggerButton(input->A)) {
		//SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
		//Audio::GetInstance()->StopWave(1);
	}
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();
	camerawork->Update(camera);

	lightgroup->Update();

	if (input->Pushkey(DIK_A)&&
		nowstate!=CONVERSATION) {
		nowstate = CONVERSATION;
		frame = {};
	}
	if (input->Pushkey(DIK_S)&&
		nowstate!=FIGHT) {
		nowstate = FIGHT;
		frame = {};
	}
	if (nowstate==CONVERSATION) {
		frame++;
		nowframe = frame / maxframe;
		if (frame >= maxframe) {
			frame = maxframe;
		}
		window_pos.y = Ease(Out, Sine, nowframe, WinApp::window_height + 100, WinApp::window_height - 100);
		window_size.x = Ease(Out, Sine, nowframe, 0, 1300);
		window_size.y = Ease(Out, Sine, nowframe, 0, 223);
		black_color.w = Ease(Out, Sine, nowframe,0, 1);
	}
	else if(nowstate ==FIGHT) {
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
	backobj->SetColor({ 0.5f,0.5f,0.5f,1.f});
	backobj->Update();
	if (nowstate != CONVERSATION) {
		player->Update();
		//enemymanager->Update();
		boss->Update();
		loadfood->Update();
		loadghost->Update();
		ParticleEmitter::GetInstance()->Update();
	}
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
	loadfood->Draw(dxCommon);
	loadghost->Draw(dxCommon);
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
	blackwindow->Draw();
	conversationwindow->Draw();
	//ui->Draw();
	IKESprite::PostDraw();
}
//IMGuiの描画
void PlaySceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//player->ImGuiDraw();
	//loadfood->ImGuiDraw();
	loadghost->ImGuiDraw();
}
