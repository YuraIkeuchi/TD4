#include "TutorialSceneActor.h"
#include "Audio.h"
#include"Easing.h"
#include"VariableCommon.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <algorithm>

const XMVECTOR kWhite{ 1.f,1.f,1.f,1.f };
const XMVECTOR kSkyBlue{ 0.f,1.f,1.f,1.f };
const XMFLOAT2 kFirstRowPos{ 5.f,0.f };
const XMFLOAT2 kSecondRowPos{ 5.f,-40.f };
const XMFLOAT2 kThardRowPos{ 5.f, -80.f };
const XMFLOAT4 kHalfClear{ 0.5f,0.5f,0.5f,0.5f };

//状態遷移
/*stateの並び順に合わせる*/
void (TutorialSceneActor::* TutorialSceneActor::stateTable[])() = {
	&TutorialSceneActor::IntroState,//
	&TutorialSceneActor::MoveState,//
	&TutorialSceneActor::TextTalkState,//
	&TutorialSceneActor::SpawnEnemyState,//
	&TutorialSceneActor::TextCatchFollowState,//
	&TutorialSceneActor::CatchFollowState,//
	&TutorialSceneActor::TextShotState,//
	&TutorialSceneActor::ShotState,//
	&TutorialSceneActor::TextCatchSeachState,//
	&TutorialSceneActor::CatchSeachState,//
	&TutorialSceneActor::TextClearState,//
	&TutorialSceneActor::SpawnAllEnemyState,//
	&TutorialSceneActor::TextLastState,//
	&TutorialSceneActor::MainTutorialState,//
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
	ward = L"ここはどこだろう?";
	firstrow_->SetString(ward);
	ward = L"あたりを見回してみよう";
	secondrow_->SetString(ward);
	ward = L"'Lスティックで移動してみよう'";
	thardrow_->SetString(ward);
	if (input->TriggerKey(DIK_SPACE)) {
		nowframe = 0;
		frame = 0;
		ward = L" ";
		firstrow_->SetString(ward);
		secondrow_->SetString(ward);
		thardrow_->SetString(ward);
		girl->SetColor(girl_color);
		nowstate_ = state::MOVE;
	}
}

void TutorialSceneActor::MoveState() {
	XMFLOAT3 pos=Player::GetInstance()->GetPosition();

	//メガホンobjが到着次第変更
	if (!Collision::CircleCollision(0,0,45.f,pos.x,pos.z,1.f)) {
		nowstate_ = state::TEXT_TALK;
	}
}

void TutorialSceneActor::TextTalkState() {
	
	frame+=0.1f;
	nowframe = frame / maxframe;
	if (frame >= maxframe) {
		frame = maxframe;
	}
	sutopon_color.w = Ease(Out, Sine, nowframe, 0, 1);
	megahon->SetColor(sutopon_color);

	if (conversation == 0) {
		ward = L"これは.....メガホン?";
		firstrow_->SetString(ward);
		ward = L"でも,動いてる?";
		secondrow_->SetString(ward);
		if (input->TriggerKey(DIK_RIGHT)) {
			conversation = 1;
		}

		loadobj->TutorialUpdate();

	}
	else if (conversation == 1) {
		girl_color = kHalfClear;
		ward = L"う..う........はっ!?";
		firstrow_->SetString(ward);
		firstrow_->SetColor(kSkyBlue);
		ward = L"敵!?......じゃないみたいだな";
		secondrow_->SetString(ward);
		secondrow_->SetColor(kSkyBlue);
		if (input->TriggerKey(DIK_RIGHT)) {
			conversation = 2;
		}
	}
	else if (conversation == 2) {
		sutopon_color = kHalfClear;
		girl_color = { 1.5f,1.5f,1.5f,1.f };
		ward = L"え!?メガホンが喋った!";
		firstrow_->SetString(ward);
		firstrow_->SetColor(kWhite);
		if (input->TriggerKey(DIK_RIGHT)) {
			conversation = 3;
		}
	}
	else if (conversation == 3) {
		sutopon_color = { 1.f,1.f,1.f,1.f };
		girl_color = kHalfClear;
		firstrow_->SetColor(kSkyBlue);
		secondrow_->SetColor(kSkyBlue);
		thardrow_->SetColor(kSkyBlue);
		ward = L"メガホンじゃない,オレはストポンだ";
		firstrow_->SetString(ward);
		ward = L"メガホンに取り憑いた亡霊だ!";
		secondrow_->SetString(ward);
		ward = L"こうしちゃいられない....追手が来ちまう!";
		thardrow_->SetString(ward);

		if (input->TriggerKey(DIK_RIGHT)) {
			conversation = 4;
		}
		//loadobj->FirstUpdate();
	}
	else if (conversation == 4) {
		sutopon_color = kHalfClear;
		girl_color = {1.5f,1.5f,1.5f,1.f};
		firstrow_->SetColor(kWhite);
		ward = L"追手ってあれのこと?";
		firstrow_->SetString(ward);
		if (input->TriggerKey(DIK_RIGHT)) {
			conversation = 4;
		}
		//loadobj->FirstUpdate();
	}
	girl->SetColor(girl_color);
	megahon->SetColor(sutopon_color);
	if (old_conversation < conversation) {
		firstrow_->StringReset();
		secondrow_->StringReset();
		thardrow_->StringReset();
		old_conversation = conversation;
	}

	if (input->TriggerKey(DIK_SPACE)) {
		firstrow_->StringReset();
		secondrow_->StringReset();
		thardrow_->StringReset();
		conversation = old_conversation = 0;
		nowstate_ = state::SPAWNENEMY;
	}
}

void TutorialSceneActor::SpawnEnemyState() {
	loadobj->TutorialUpdate();


	if (input->TriggerKey(DIK_SPACE)) {
		nowstate_ = state::TEXT_CATCHFOLLOW;
	}

}
void TutorialSceneActor::TextCatchFollowState() {
	if (conversation == 0) {
		firstrow_->SetColor(kSkyBlue);
		secondrow_->SetColor(kSkyBlue);
		ward = L"もう追って来たのか..!";
		firstrow_->SetString(ward);

		if (input->TriggerKey(DIK_RIGHT)) {
			conversation = 1;
		}
	}

	if (old_conversation < conversation) {
		firstrow_->StringReset();
		secondrow_->StringReset();
		thardrow_->StringReset();
		old_conversation = conversation;
	}
}
void TutorialSceneActor::CatchFollowState() {}
void TutorialSceneActor::TextShotState() {}
void TutorialSceneActor::ShotState() {}
void TutorialSceneActor::TextCatchSeachState() {}
void TutorialSceneActor::CatchSeachState() {}
void TutorialSceneActor::TextClearState() {}
void TutorialSceneActor::SpawnAllEnemyState() {}
void TutorialSceneActor::TextLastState() {}
void TutorialSceneActor::MainTutorialState() {}
void TutorialSceneActor::CompleteState() {}

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

	firstrow_ = make_unique<Font>();
	secondrow_= make_unique<Font>();
	thardrow_ = make_unique<Font>();
	firstrow_->LoadFont(dxCommon);
	secondrow_->LoadFont(dxCommon);
	thardrow_->LoadFont(dxCommon);
	firstrow_->SetColor(kWhite);
	secondrow_->SetColor(kWhite);
	thardrow_->SetColor(kWhite);
	firstrow_->SetPos(kFirstRowPos);
	secondrow_->SetPos(kSecondRowPos);
	thardrow_->SetPos(kThardRowPos);

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

	megahon = IKESprite::Create(ImageManager::SUTOPON, { 1100.f,560.f });
	megahon->SetColor(sutopon_color);
	megahon->SetAnchorPoint({ 0.5f,0.5f });
	megahon->SetSize({ 250.f,250.f });

	//enemymanager = std::make_unique<EnemyManager>("FIRSTSTAGE");

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
	if (static_cast<int>(nowstate_) % 2 == 1) {
		ui->Update();
		Player::GetInstance()->Update();
		//enemymanager->Update();
		ParticleEmitter::GetInstance()->Update();
	}
	backobj->Update();
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
		if (static_cast<int>(nowstate_) % 2 == 0) {
			firstrow_->Draw(dxCommon);
			secondrow_->Draw(dxCommon);
			thardrow_->Draw(dxCommon);
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
		if (static_cast<int>(nowstate_) % 2 == 0) {
			firstrow_->Draw(dxCommon);
			secondrow_->Draw(dxCommon);
			thardrow_->Draw(dxCommon);
			Font::PostDraw(dxCommon);
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
	if (nowstate_>=state::SPAWNENEMY){
		loadobj->Draw(dxCommon);
	}
	backobj->Draw(dxCommon);
	//enemymanager->Draw(dxCommon);

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
		megahon->Draw();
		IKESprite::PostDraw();
	}else{
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
