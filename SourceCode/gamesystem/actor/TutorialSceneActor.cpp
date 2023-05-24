#include "TutorialSceneActor.h"
#include "Audio.h"
#include"Easing.h"
#include"VariableCommon.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <algorithm>
#include "HungerGauge.h"

const XMVECTOR kWhite{ 1.f,1.f,1.f,1.f };
const XMVECTOR kSkyBlue{ 0.f,1.f,1.f,1.f };
const XMFLOAT2 kFirstRowPos{ 5.f,0.f };
const XMFLOAT2 kSecondRowPos{ 5.f,-40.f };
const XMFLOAT2 kThirdRowPos{ 5.f, -80.f };
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

	if (input->TriggerKey(DIK_SPACE)) {
		conversation_->WardNone();
		nowstate_ = state::MOVE;
	}
}

void TutorialSceneActor::MoveState() {
	XMFLOAT3 pos=Player::GetInstance()->GetPosition();

	//メガホンobjが到着次第変更
	if (!Collision::CircleCollision(0,0,45.f,pos.x,pos.z,1.f)||input->TriggerKey(DIK_SPACE)) {
		nowstate_ = state::TEXT_TALK;
	}
}

void TutorialSceneActor::TextTalkState() {
	

	if (input->TriggerKey(DIK_RIGHT)) {
		conversation_->WardNone();
	}
	
	if (input->TriggerKey(DIK_SPACE)) {
		firstrow_->StringReset();
		secondrow_->StringReset();
		thirdrow_->StringReset();
		conversation = old_conversation = 0;
		nowstate_ = state::SPAWNENEMY;
	}
}

void TutorialSceneActor::SpawnEnemyState() {
	loadobj->TutorialUpdate();
	enemymanager->TutorialUpdate(0);
	XMFLOAT3 plaPos = Player::GetInstance()->GetPosition();
	XMFLOAT3 enePos = firstEnemy->GetPosition();
	if (input->TriggerKey(DIK_SPACE)||
		Clear(Collision::CircleCollision(plaPos.x, plaPos.z,20.0f, enePos.x, enePos.z,1.0f),0))
	{
		waitTimer = 0;
		nowstate_ = state::TEXT_CATCHFOLLOW;
	}

}
void TutorialSceneActor::TextCatchFollowState() {
	if (conversation == 0) {
		ward = L"てきがせっきんしました。";
		firstrow_->SetString(ward);
		secondrow_->SetString(L"なかまにしましょう。");
		if (input->TriggerKey(DIK_RIGHT)) {
			conversation = 1;
		}
		loadobj->TutorialUpdate();
	}

	if (input->TriggerKey(DIK_SPACE)) {
		nowstate_ = state::CATCHFOLLOW;
	}

}
void TutorialSceneActor::CatchFollowState() {
	loadobj->TutorialUpdate();
	enemymanager->TutorialUpdate(0);

	if (Clear(HungerGauge::GetInstance()->GetCatchCount()>=1.0f,50)) {
		waitTimer = 0;
		nowstate_ = state::TEXT_SHOT;

	}

}
void TutorialSceneActor::TextShotState() {
	ward = L"なかまができました。";
	firstrow_->SetString(ward);
	secondrow_->SetString(L"たまをうってたおしてください");

	if (input->TriggerKey(DIK_SPACE)) {
		nowstate_ = state::SHOT;
	}
}
void TutorialSceneActor::ShotState() {
	loadobj->TutorialUpdate();
	enemymanager->TutorialUpdate(0);

	if (input->TriggerKey(DIK_SPACE)||
		Clear(!firstEnemy->GetisAlive(),45)) {
		waitTimer = 0;
		nowstate_ = state::TEXT_CATCHSEACH;
	}
}
void TutorialSceneActor::TextCatchSeachState() {
	ward = L"たおすことにせいこうしました。";
	firstrow_->SetString(ward);
	secondrow_->SetString(L"たんさくがたのかくとくしてください。");

	if (input->TriggerKey(DIK_SPACE)) {
		HungerGauge::GetInstance()->ResetFirstCarry();
		nowstate_ = state::CATCHSEACH;
	}
}
void TutorialSceneActor::CatchSeachState() {
	loadobj->TutorialUpdate();
	enemymanager->TutorialUpdate(0);


	if (input->TriggerKey(DIK_SPACE) || 
		Clear(HungerGauge::GetInstance()->GetFirstCarry(),30)) {
		waitTimer = 0;
		nowstate_ = state::TEXT_CLEAR;
	}
}
void TutorialSceneActor::TextClearState() {
	ward = L"ながれをかんぜんにりかいしました";
	firstrow_->SetString(ward);
	secondrow_->SetString(L"たくさんてきがでてきました。");

	if (input->TriggerKey(DIK_SPACE)) {
		nowstate_ = state::SPAWNALLENEMY;
	}
}
void TutorialSceneActor::SpawnAllEnemyState() {
	loadobj->TutorialUpdate();
	enemymanager->TutorialUpdate(1);

}
void TutorialSceneActor::TextLastState() {}

void TutorialSceneActor::MainTutorialState() {}


bool TutorialSceneActor::Clear(bool mission, int waitTimerMax) {
	if (!mission) { return false; }
	waitTimer++;
	if (waitTimer >= waitTimerMax) {
		return true;
	} else {
		return false;
	}
}
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
	//各クラス
	Player::GetInstance()->InitState({ 0.0f,0.0f,0.0f });
	camerawork->Update(camera);
	ui = std::make_unique<UI>();
	ui->Initialize();
	enemymanager = std::make_unique<EnemyManager>("TUTORIAL");
	firstEnemy = enemymanager->GetEnemy(0);
	conversation_ = make_unique<Conversation>();
	conversation_->Initialize(dxCommon);
	//生成
	firstrow_ = make_unique<Font>();
	secondrow_ = make_unique<Font>();
	thirdrow_ = make_unique<Font>();
	//読み込み
	firstrow_->LoadFont(dxCommon);
	secondrow_->LoadFont(dxCommon);
	thirdrow_->LoadFont(dxCommon);
	//座標セット
	firstrow_->SetPos(kFirstRowPos);
	secondrow_->SetPos(kSecondRowPos);
	thirdrow_->SetPos(kThirdRowPos);
	//色
	firstrow_->SetColor(kWhite);
	secondrow_->SetColor(kWhite);
	thirdrow_->SetColor(kWhite);

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
	conversation_->Tyutorial();
	conversation_->Update();
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
			conversation_->FontDraw(dxCommon);
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
			conversation_->FontDraw(dxCommon);
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
	enemymanager->TutorialDraw(dxCommon);

	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void TutorialSceneActor::FrontDraw(DirectXCommon* dxCommon) {
	//パーティクル描画
	ParticleEmitter::GetInstance()->FlontDrawAll();
	//完全に前に書くスプライト
	if (static_cast<int>(nowstate_) % 2 == 0) {
		IKESprite::PreDraw();
		conversation_->SproteDraw();
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
