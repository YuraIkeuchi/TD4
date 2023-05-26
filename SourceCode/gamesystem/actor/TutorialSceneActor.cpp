#include "TutorialSceneActor.h"
#include "VariableCommon.h"
#include "HungerGauge.h"

const XMVECTOR kWhite{ 1.f,1.f,1.f,1.f };
const XMVECTOR kSkyBlue{ 0.f,1.f,1.f,1.f };
const XMFLOAT2 kFirstRowPos{ 5.f,0.f };
const XMFLOAT2 kSecondRowPos{ 5.f,-40.f };
const XMFLOAT2 kThirdRowPos{ 5.f, -80.f };
const XMFLOAT4 kHalfClear{ 0.5f,0.5f,0.5f,0.5f };

bool TutorialSceneActor::isDebug = true;


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


	if (DebugButton() ||
		input->TriggerButton(Input::B)) {
		conversation_->WardNone();

		nowstate_ = state::MOVE;
	}
}
void TutorialSceneActor::MoveState() {
	//メガホンobj更新
	sutepon->Update();
	shakeTimer++;
	if (shakeTimer%10==0) {
		sutepon->SetPosition(RandomShake({ 0,0,15 }));
		shakeTimer = 0;
	}
	XMFLOAT3 pos = Player::GetInstance()->GetPosition();
	XMFLOAT3 Spos = sutepon->GetPosition();
	if (Collision::CircleCollision(Spos.x, Spos.z, 5.f, pos.x, pos.z, 1.f)) {
		nowstate_ = state::TEXT_TALK;
	}
}
void TutorialSceneActor::TextTalkState() {
	if (input->TriggerKey(DIK_RIGHT)) {
		conversation_->WardNone();
	}

	if (DebugButton() ||
		input->TriggerButton(Input::B)) {
		firstrow_->StringReset();
		secondrow_->StringReset();
		thirdrow_->StringReset();
		sutepon->SetPosition({ 0,0,15.0f });
		conversation = old_conversation = 0;
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
	if (conversation == 0) {
		ward = L"てきがせっきんしました。";
		firstrow_->SetString(ward);
		secondrow_->SetString(L"なかまにしましょう。");
		if (input->TriggerKey(DIK_RIGHT)) {
			conversation = 1;
		}
		loadobj->TutorialUpdate();
	}

	if (DebugButton() ||
		input->TriggerButton(Input::B)) {
		Player::GetInstance()->SetCanShot(true);
		nowstate_ = state::CATCHFOLLOW;
	}

}
void TutorialSceneActor::CatchFollowState() {
	loadobj->TutorialUpdate();
	firstEnemy->SetIsStop(true);
	enemymanager->TutorialUpdate(0);

	if (DebugButton() ||
		Clear(HungerGauge::GetInstance()->GetCatchCount() >= 1.0f, 50)) {
		waitTimer = 0;
		nowstate_ = state::TEXT_SHOT;

	}

}
void TutorialSceneActor::TextShotState() {
	ward = L"なかまができました。";
	firstrow_->SetString(ward);
	secondrow_->SetString(L"たまをうってたおしてください");

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
	ward = L"たおすことにせいこうしました。";
	firstrow_->SetString(ward);
	secondrow_->SetString(L"たんさくがたのかくとくしてください。");

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
	ward = L"ながれをかんぜんにりかいしました";
	firstrow_->SetString(ward);
	secondrow_->SetString(L"たくさんてきがでてきました。");

	if (DebugButton() ||
		input->TriggerButton(Input::B)) {
		nowstate_ = state::SPAWNALLENEMY;
	}
}
void TutorialSceneActor::SpawnAllEnemyState() {
	XMFLOAT3 eye = camerawork->GetEye();
	XMFLOAT3 target = camerawork->GetTarget();
	cameraframe += 1.0f / kCameraFrameMax;
	cameraframe = min(1.0f, cameraframe);
	eye = {
	Ease(In,Linear,cameraframe, Player::GetInstance()->GetPosition().x,0),
	eye.y,
	Ease(In,Linear,cameraframe,  Player::GetInstance()->GetPosition().z - 20.0f,30.0f),
	};
	target = {
	Ease(In,Linear,cameraframe, Player::GetInstance()->GetPosition().x,0),
	target.y,
	Ease(In,Linear,cameraframe,  Player::GetInstance()->GetPosition().z, 50.0f),
	};

	camerawork->SetEye(eye);
	camerawork->SetTarget(target);
	loadobj->TutorialUpdate();
	if (cameraframe >= 1.0f) {
		enemymanager->TutorialUpdate(1);
	}
	if (Clear(cameraframe >= 1.0f, 50)) {
		nowstate_ = state::TEXT_LAST;
	}
}
void TutorialSceneActor::TextLastState() {
	ward = L"これがおわったらケッコンするんだ";
	firstrow_->SetString(ward);
	secondrow_->SetString(L"ヤムチャしやがって・・・");


	if (DebugButton() ||
		input->TriggerButton(Input::B)) {
		nowstate_ = state::MAINTUTORIAL;
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

	ward = L"あんたはオレのアイボウだぜ";
	firstrow_->SetString(ward);
	secondrow_->SetString(L"おれがついてるぜ");

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
	} else {
		return false;
	}
}
XMFLOAT3 TutorialSceneActor::RandomShake(XMFLOAT3 pos) {
	//乱数指定
	mt19937 mt{ std::random_device{}() };
	std::uniform_real_distribution<float> dist(-0.5f, 0.5f);
	//	uniform_int_distribution<int> l_distZ(-55, 55);
	return XMFLOAT3(pos.x + dist(mt), pos.y, pos.z);
}
void TutorialSceneActor::CameraUpdate(DebugCamera* camera) {
	if (nowstate_ != state::SPAWNALLENEMY) {
		camerawork->DefaultCam();
		camerawork->Update(camera);
	} else {
		camerawork->SpecialUpdate(camera);
	}
}
bool TutorialSceneActor::Clear(bool mission, int waitTimerMax) {
	if (!mission) { return false; }
	waitTimer++;
	if (waitTimer >= waitTimerMax) {
		return true;
	} else {
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
	PlayPostEffect = true;
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

	conversation_ = make_unique<Conversation>();
	conversation_->Initialize(dxCommon);
	//生成
	firstrow_ = make_unique<Font>();
	secondrow_ = make_unique<Font>();
	thirdrow_ = make_unique<Font>();
	//読み込み

	//背景objの生成
	BackObj::GetInstance()->Initialize();


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
		ParticleEmitter::GetInstance()->Update();
	}
	BackObj::GetInstance()->Update();
	CameraUpdate(camera);
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
	if (nowstate_ >= state::SPAWNENEMY) {
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
		conversation_->SproteDraw();
		IKESprite::PostDraw();
		if (static_cast<int>(nowstate_) % 2 == 0) {
			firstrow_->Draw(dxCommon);
			secondrow_->Draw(dxCommon);
			conversation_->FontDraw(dxCommon);
			Font::PostDraw(dxCommon);

		}
	} else {
		ui->Draw();
	}
	sceneChanger_->Draw();
}
//IMGuiの描画
void TutorialSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//Player::GetInstance()->ImGuiDraw();
	//loadobj->ImGuiDraw();
	//enemymanager->ImGuiDraw();
	//camerawork->ImGuiDraw();
}
