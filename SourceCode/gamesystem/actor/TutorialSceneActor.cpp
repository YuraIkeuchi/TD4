﻿#include "TutorialSceneActor.h"
#include "VariableCommon.h"
#include "HungerGauge.h"


const XMVECTOR kWhite{ 1.f,1.f,1.f,1.f };
const XMVECTOR kSkyBlue{ 0.f,1.f,1.f,1.f };
const XMFLOAT2 kFirstRowPos{ 5.f,0.f };
const XMFLOAT2 kSecondRowPos{ 5.f,-40.f };
const XMFLOAT2 kThirdRowPos{ 5.f, -80.f };
const XMFLOAT4 kHalfClear{ 0.5f,0.5f,0.5f,0.5f };

bool TutorialSceneActor::isDebug = true;
TextManager* instance = TextManager::GetInstance();

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


	if (DebugButton() ||
		input->TriggerButton(Input::B)) {
		text_->NoneText();
		nowstate_ = state::MOVE;
	}
}
void TutorialSceneActor::MoveState() {
	//メガホンobj更新
	sutepon->Update();

	shakeCount++;
	if (shakeCount > 30) {
		if (!isShake) {
			isShake = true;
		}
		else {
			isShake = false;
		}
		shakeCount = 0;
	}
	if (isShake) {
		shakeTimer += 0.7f;
		sutepon->SetPosition(RandomShake({ 0,0,15 }, shakeTimer));
	}

	XMFLOAT3 pos = Player::GetInstance()->GetPosition();
	XMFLOAT3 Spos = sutepon->GetPosition();
	if (Collision::CircleCollision(Spos.x, Spos.z, 5.f, pos.x, pos.z, 1.f)) {
		nowstate_ = state::TEXT_TALK;
	}
	if (input->TriggerKey(DIK_O)) {
		nowstate_ = state::TEXT_TALK;
	}
}
void TutorialSceneActor::TextTalkState() {

	messagewindow_->DisplayCharacter(sutopon_color_);
	if (input->TriggerButton(Input::B)) {
		conversation += 1;
	}

	if (conversation == 0) {
		text_->SetConversation(TextManager::TYUTORIAL_TALK2);
	}
	else if (conversation == 1) {
		text_->SetConversation(TextManager::TYUTORIAL_TALK3, { 0.f,0.5f,0.5f,1.f });
	}
	else if (conversation == 2) {
		text_->SetConversation(TextManager::TYUTORIAL_TALK4);
	}
	else if (conversation == 3) {
		text_->SetConversation(TextManager::TYUTORIAL_TALK5, { 0.f,0.5f,0.5f,1.f });
	}
	else if (conversation == 4) {
		text_->SetConversation(TextManager::TYUTORIAL_TALK6);
	}


	if (DebugButton() ||
		conversation == 5) {
		text_->SetConversation(TextManager::NONE);
		sutepon->SetPosition({ 0,0,15.0f });
		conversation = 0;
		nowstate_ = state::SPAWNENEMY;
	}
}
void TutorialSceneActor::SpawnEnemyState() {
	loadobj->TutorialUpdate();
	enemymanager->TutorialUpdate(0);
	XMFLOAT3 plaPos = Player::GetInstance()->GetPosition();
	XMFLOAT3 enePos = firstEnemy->GetPosition();
	if (DebugButton() ||
		Clear(Collision::CircleCollision(plaPos.x, plaPos.z, 20.0f, enePos.x, enePos.z, 1.0f), 0)) {
		waitTimer = 0;
		nowstate_ = state::TEXT_CATCHFOLLOW;
	}

}
void TutorialSceneActor::TextCatchFollowState() {

	if (input->TriggerButton(Input::B)) {
		conversation += 1;
	}
	if (conversation == 0) {
		text_->SetConversation(TextManager::TYUTORIAL_TALK7, { 0.f,0.5f,0.5f,1.f });
		loadobj->TutorialUpdate();
	}
	else if (conversation == 1) {
		text_->SetConversation(TextManager::TYUTORIAL_TALK8, { 0.f,0.5f,0.5f,1.f });
	}

	if (DebugButton() ||
		conversation == 5) {
		Player::GetInstance()->SetCanShot(true);
		nowstate_ = state::CATCHFOLLOW;
	}

}
void TutorialSceneActor::CatchFollowState() {
	loadobj->TutorialUpdate();
	firstEnemy->SetIsStop(true);
	enemymanager->TutorialUpdate(0);

	if (DebugButton() ||
		Clear(HungerGauge::GetInstance()->GetCatchCount() >= 1, 50)) {
		waitTimer = 0;
		nowstate_ = state::TEXT_SHOT;

	}

}
void TutorialSceneActor::TextShotState() {
	if (DebugButton() ||
		input->TriggerButton(Input::B)) {
		nowstate_ = state::SHOT;
	}
}
void TutorialSceneActor::ShotState() {
	loadobj->TutorialUpdate();
	enemymanager->TutorialUpdate(0);

	if (DebugButton() ||
		Clear(!firstEnemy->GetisAlive(), 45)) {
		waitTimer = 0;
		nowstate_ = state::TEXT_CATCHSEACH;
	}
}
void TutorialSceneActor::TextCatchSeachState() {
	if (DebugButton() ||
		input->TriggerButton(Input::B)) {
		HungerGauge::GetInstance()->ResetFirstCarry();
		nowstate_ = state::CATCHSEACH;
	}
}
void TutorialSceneActor::CatchSeachState() {
	loadobj->TutorialUpdate();
	enemymanager->TutorialUpdate(0);


	if (DebugButton() ||
		Clear(HungerGauge::GetInstance()->GetFirstCarry(), 30)) {
		waitTimer = 0;
		nowstate_ = state::TEXT_CLEAR;
	}
}
void TutorialSceneActor::TextClearState() {
	if (DebugButton() ||
		input->TriggerButton(Input::B)) {
		nowstate_ = state::SPAWNALLENEMY;
		s_eyepos = camerawork->GetEye();
		s_targetpos = camerawork->GetTarget();
	}
}
void TutorialSceneActor::SpawnAllEnemyState() {
	loadobj->TutorialUpdate();
	if (MovingCamera(s_eyepos, e_eyepos, s_targetpos, e_targetpos)) {
		enemymanager->TutorialUpdate(1);
	}
	if (Clear(cameraframe >= 1.0f, 50)) {
		s_eyepos = { Player::GetInstance()->GetPosition().x,
		Player::GetInstance()->GetPosition().y + 50.0f,
		Player::GetInstance()->GetPosition().z - 20.0f };
		s_targetpos.x = Player::GetInstance()->GetPosition().x;
		s_targetpos.z = Player::GetInstance()->GetPosition().z;
		nowstate_ = state::TEXT_LAST;
		cameraframe = 0.0f;
	}
}
void TutorialSceneActor::TextLastState() {
	loadobj->TutorialUpdate();
	enemymanager->TutorialUpdate(2);
	Player::GetInstance()->Update();
	Player::GetInstance()->SetCanShot(false);
	if (MovingCamera(e_eyepos, s_eyepos, e_targetpos, s_targetpos)) {
		if ((DebugButton() ||
			input->TriggerButton(Input::B))
			) {
			nowstate_ = state::MAINTUTORIAL;
			Player::GetInstance()->SetCanShot(true);
		}
	}

}
void TutorialSceneActor::MainTutorialState() {
	loadobj->TutorialUpdate();
	enemymanager->TutorialUpdate(1);
	if (DebugButton() ||
		Clear(enemymanager->AllDeadEnemy(), 60)) {
		nowstate_ = state::COMPLETE;
	}
}
void TutorialSceneActor::CompleteState() {
	if (DebugButton() ||
		input->TriggerButton(Input::B)) {
		sceneChanger_->ChangeStart();
	}
	sceneChanger_->ChangeScene("FIRSTSTAGE", SceneChanger::NonReverse);

}

bool TutorialSceneActor::DebugButton() {
	if (!isDebug) { return false; }
	if (input->TriggerKey(DIK_SPACE)) {
		return true;
	}
	else {
		return false;
	}
}
XMFLOAT3 TutorialSceneActor::RandomShake(XMFLOAT3 pos, float shakeTimer) {
	//乱数指定
	//mt19937 mt{ std::random_device{}() };
	//std::uniform_real_distribution<float> dist(-0.5f, 0.5f);

	float angle = sinf(shakeTimer) * 0.3f;
	return XMFLOAT3(pos.x + angle, pos.y, pos.z);
}
bool TutorialSceneActor::MovingCamera(const XMFLOAT3& s_eye, const XMFLOAT3& e_eye, const XMFLOAT3& s_target, const XMFLOAT3& e_target) {
	XMFLOAT3 eye = camerawork->GetEye();
	XMFLOAT3 target = camerawork->GetTarget();
	cameraframe += 1.0f / kCameraFrameMax;
	cameraframe = min(1.0f, cameraframe);
	eye = {
	Ease(In,Linear,cameraframe, s_eye.x,e_eye.x),
	eye.y,
	Ease(In,Linear,cameraframe,  s_eye.z,e_eye.z),
	};
	target = {
	Ease(In,Linear,cameraframe, s_target.x,e_target.x),
	target.y,
	Ease(In,Linear,cameraframe, s_target.z,e_target.z),
	};

	camerawork->SetEye(eye);
	camerawork->SetTarget(target);
	Helper::GetInstance()->Clamp(cameraframe, 0.0f, 1.0f);

	if (cameraframe == 1.0f) {
		return true;
	}
	else {
		return false;
	}
}
void TutorialSceneActor::CameraUpdate(DebugCamera* camera) {
	if (!(nowstate_ == state::SPAWNALLENEMY || nowstate_ == state::TEXT_LAST)) {
		camerawork->SetCameraState(CAMERA_NORMAL);
	}
	else {
		camerawork->SetCameraState(CAMERA_LOAD);
	}

	camerawork->Update(camera);
}
bool TutorialSceneActor::Clear(bool mission, int waitTimerMax) {
	if (!mission) { return false; }
	waitTimer++;
	if (waitTimer >= waitTimerMax) {
		return true;
	}
	else {
		return false;
	}
}

//初期化
void TutorialSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/BGM_boss.wav");
	//ポストエフェクト
	PlayPostEffect = false;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();
	//各クラス
	Player::GetInstance()->InitState({ 0.0f,0.0f,0.0f });
	Player::GetInstance()->SetCanShot(false);
	//カメラ更新
	camerawork->Update(camera);
	//UIの初期化
	ui = std::make_unique<UI>();
	ui->Initialize();
	//ストポンの初期化
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Sutopon);
	sutepon = make_unique<IKEObject3d>();
	sutepon->Initialize();
	sutepon->SetModel(m_Model);
	sutepon->SetPosition({ 0,0,15.0f });
	sutepon->SetRotation({ 60.0f,0,0 });
	//エネミーの生成
	enemymanager = std::make_unique<EnemyManager>("TUTORIAL");
	//最初のエネミーの参照
	firstEnemy = enemymanager->GetEnemy(0);
	//メッセージウィンドウ生成
	messagewindow_ = make_unique<MessageWindow>();
	messagewindow_->Initialize();
	messagewindow_->Display();
	//背景objの生成
	BackObj::GetInstance()->Initialize();


	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::TYUTORIAL_TALK1)
		;
	BackObj::GetInstance()->Initialize();
	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	LoadStageObj::SetEnemyManager(enemymanager.get());
	//シーンチェンジャー
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();
}
//更新
void TutorialSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	if (PlayerDestroy()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	XMFLOAT2 pos[3] = { kFirstRowPos,kSecondRowPos,kThirdRowPos };
	XMFLOAT3 color[3] = { {1,1,1},{1,1,1},{1,1,1} };

	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();
	//状態移行(stateに合わせる)
	(this->*stateTable[static_cast<size_t>(nowstate_)])();
	//各クラス更新
	if (static_cast<int>(nowstate_) % 2 == 1) {
		ui->Update();
		Player::GetInstance()->Update();
	}
	ParticleEmitter::GetInstance()->Update();
	BackObj::GetInstance()->Update();
	CameraUpdate(camera);
	lightgroup->Update();
	messagewindow_->Update(girl_color_, sutopon_color_);
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
void TutorialSceneActor::Finalize() {
}
//後ろの描画
void TutorialSceneActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	////各クラスの描画
	Player::GetInstance()->Draw(dxCommon);
	if (nowstate_ <= state::TEXT_TALK) {
		sutepon->Draw();
	}
	else {
		loadobj->Draw(dxCommon);
	}
	BackObj::GetInstance()->Draw(dxCommon);
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
		messagewindow_->Draw();
		IKESprite::PostDraw();
		if (messagewindow_->DisplayCheck()) {
			text_->Draw(dxCommon);
		}
	}
	else {
		ui->Draw();
	}
	sceneChanger_->Draw();
}
//IMGuiの描画
void TutorialSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//Player::GetInstance()->ImGuiDraw();
	//loadobj->ImGuiDraw();
	enemymanager->ImGuiDraw();
	//camerawork->ImGuiDraw();
}
