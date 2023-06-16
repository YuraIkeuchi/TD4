#include "SecondStageActor.h"
#include "Audio.h"
#include"Easing.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <algorithm>
#include "BackObj.h"
#include "Menu.h"
//初期化
void SecondStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/BGM_boss.wav");
	Audio::GetInstance()->LoopWave(1, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);

	//ポストエフェクト
	PlayPostEffect = true;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	//各クラス
	Player::GetInstance()->InitState({ 0.0f,5.0f,-5.0f });
	
	backScreen_ = IKESprite::Create(ImageManager::PLAY, {0,0});
	backScreen_->SetSize({1280.0f,720.0f});
	//シーンチェンジャー
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	enemymanager = std::make_unique<EnemyManager>("SECONDSTAGE");
	//enemymanager->Initialize(dxCommon);
	text_ = make_unique<BossText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::ANGER_TALK);
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
	loadobj->LightSet(lightgroup);
	LoadStageObj::SetEnemyManager(enemymanager.get());

	m_SceneState = SceneState::IntroState;

	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);

	Menu::GetIns()->Init();
}
//更新
void SecondStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	
	//関数ポインタで状態管理
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
	sceneChanger_->Update();

	//プレイヤー
	if (enemymanager->BossDestroy() && camerawork->GetFeedEnd()) {
		SceneSave::GetInstance()->SetClearFlag(kSecondStage, true);
		lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, 0.0f, Player::GetInstance()->GetPosition().z }));
		lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
	}
	else {//ボス撃破ムービーの後は丸影消す
		lightgroup->SetCircleShadowActive(0, false);
	}

	//ボス
	lightgroup->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(1, XMFLOAT3({ enemymanager->GetBoss()->GetPosition().x, 	0.0f, 	enemymanager->GetBoss()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));
	lightgroup->Update();

	Menu::GetIns()->Upda();
	postEffect->SetCloseRad(Menu::GetIns()->GetCloseIconRad());
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
	IKESprite::PreDraw();
	backScreen_->Draw();
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);
	//パーティクル描画
	if (!camerawork->GetFeedEnd() && m_SceneState == SceneState::MainState) {
		if (!enemymanager->BossDestroy()) {
			ParticleEmitter::GetInstance()->BackDrawAll();
		}
	}
	////各クラスの描画
	if (!camerawork->GetFeedEnd()) {
		Player::GetInstance()->Draw(dxCommon);
		loadobj->Draw(dxCommon);
	}
	enemymanager->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void SecondStageActor::FrontDraw(DirectXCommon* dxCommon) {
	//パーティクル描画
	if (!camerawork->GetFeedEnd() && m_SceneState == SceneState::MainState) {
		ParticleEmitter::GetInstance()->FlontDrawAll();
	}

	ParticleEmitter::GetInstance()->DeathDrawAll();
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	if (m_SceneState == SceneState::MainState && !camerawork->GetFeedEnd()) {
		ui->Draw();
	}
	if (m_SceneState == SceneState::IntroState) {
		if ((camerawork->GetAppearType() == APPEAR_SEVEN) || (camerawork->GetAppearType() == APPEAR_EIGHT)) {
			text_->SpriteDraw(dxCommon);
		}
	}
	IKESprite::PostDraw();
	sceneChanger_->Draw();
	Menu::GetIns()->Draw();
	camerawork->feedDraw();
}
//IMGuiの描画
void SecondStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	Player::GetInstance()->ImGuiDraw();
	//loadobj->ImGuiDraw();
	//camerawork->ImGuiDraw();
	enemymanager->ImGuiDraw();
	//loadobj->ImGuiDraw();
	//SceneSave::GetInstance()->ImGuiDraw();
}
//登場シーン
void SecondStageActor::IntroUpdate(DebugCamera* camera) {
	if (camerawork->GetAppearType() == APPEAR_SEVEN || camerawork->GetAppearType() == APPEAR_EIGHT) {
		text_->Display();
		//最初の言葉(怒り)
		if (m_AppState == AppState::ANGER_START) {
			text_->SelectText(TextManager::ANGER_TALK);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::ANGER_SECOND;
			}
		}
		//2つ目の言葉(怒り)
		else if (m_AppState == AppState::ANGER_SECOND) {
			text_->SelectText(TextManager::ANGER_TALK2);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::JOY_START;
				enemymanager->DirSet(DIR_JOY);
			}
		}
		//最初の言葉(喜び)
		else if (m_AppState == AppState::JOY_START) {
			text_->SelectText(TextManager::JOY_TALK);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::JOY_SECOND;
			}
		}
		//2個目の言葉(喜び)
		else if (m_AppState == AppState::JOY_SECOND) {
			text_->SelectText(TextManager::JOY_TALK2);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::JOY_THIRD;
			}
		}
		//3個めの言葉(喜び)
		else if (m_AppState == AppState::JOY_THIRD) {
			text_->SelectText(TextManager::JOY_TALK3);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::SELECT_EMO;
			}
		}
		//選択肢
		else if (m_AppState == AppState::SELECT_EMO) {
			text_->ChangeColor(1, { 1.0f,0.0f,0.0f,1.0f });
			text_->ChangeColor(2, { 0.5f,1.0f,0.1f,1.0f });
			text_->SelectText(TextManager::SELECT_TALK);
			if (Input::GetInstance()->TriggerButton(Input::Y)) {
				enemymanager->DirSet(DIR_ANGER);
				m_AppState = AppState::EMO_ANGER;
			}
			else if (Input::GetInstance()->TriggerButton(Input::X)) {
				enemymanager->DirSet(DIR_JOY);
				m_AppState = AppState::EMO_JOY;
			}
		}
		//イカリを選んだ場合
		else if (m_AppState == AppState::EMO_ANGER) {
			for (int i = 0; i < 3; i++) {
				text_->ChangeColor(i, { 1.0f,1.0f,1.0f,1.0f });
			}
			text_->SelectText(TextManager::SELECT_ANGER);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::EMO_ANGER2;
				camerawork->SetApproach(true);
			}
		}
		else if (m_AppState == AppState::EMO_ANGER2) {
			for (int i = 0; i < 3; i++) {
				text_->ChangeColor(i, { 1.0f,0.0f,0.0f,1.0f });
			}
			text_->SelectText(TextManager::SELECT_ANGER2);
		}
		//よろこびを選んだ場合
		else if (m_AppState == AppState::EMO_JOY) {
			for (int i = 0; i < 3; i++) {
				text_->ChangeColor(i, { 1.0f,1.0f,1.0f,1.0f });
			}
			text_->SelectText(TextManager::SELECT_JOY);
			if (Input::GetInstance()->TriggerButton(Input::B)) {
				m_AppState = AppState::EMO_JOY2;
				camerawork->SetApproach(true);
			}
		}
		else if (m_AppState == AppState::EMO_JOY2) {
			for (int i = 0; i < 3; i++) {
				text_->ChangeColor(i, { 1.0f,0.0f,0.0f,1.0f });
			}
			text_->SelectText(TextManager::SELECT_JOY2);
		}
	}

	//最後までテキストを見た
	if (enemymanager->GetEnemyFinishAppear()) {
		m_SceneState = SceneState::MainState;
		camerawork->SetCameraState(CAMERA_NORMAL);
	}

	//演出スキップ
	if (Input::GetInstance()->TriggerButton(Input::A)) {
		camerawork->SetCameraSkip(true);
	}

	if (camerawork->GetAppearEndF()) {
		m_SceneState = SceneState::MainState;
		camerawork->SetCameraState(CAMERA_NORMAL);
		enemymanager->SkipInitialize();
	}

	//各クラス更新
	BackObj::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	Player::GetInstance()->AppearUpdate();
	enemymanager->AppearUpdate();

	camerawork->Update(camera);
}
//バトルシーン
void SecondStageActor::MainUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
	ui->Update();
	//カメラワークのセット
	if (enemymanager->BossDestroy())
	{
		Audio::GetInstance()->StopWave(1);
		//フェード前
		if (!camerawork->GetFeedEnd()) {
			enemymanager->SetDeadThrow(true);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_BEFORE);
		}
		//フェード後
		else
		{
			PlayPostEffect = false;
			Player::GetInstance()->InitState({ 0.0f,0.0f,-5.0f });
			enemymanager->SetDeadThrow(false);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_AFTER_SECOND);
		}

		if (camerawork->GetEndDeath()) {
			sceneChanger_->ChangeStart();
			sceneChanger_->ChangeScene("GAMECLEAR", SceneChanger::NonReverse);
		
		}

		Player::GetInstance()->DeathUpdate();
	}
	else
	{
		Player::GetInstance()->Update();
	}
	
	if (PlayerDestroy()) {
		Audio::GetInstance()->StopWave(1);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMEOVER", SceneChanger::Reverse);
	}

	//音楽の音量が変わる
	VolumManager::GetInstance()->Update();

	//各クラス更新
	BackObj::GetInstance()->Update();

	
	enemymanager->BattleUpdate();
	loadobj->SecondUpdate();
	ParticleEmitter::GetInstance()->Update();

	camerawork->Update(camera);

	XMFLOAT3 Position = enemymanager->GetBoss()->GetPosition();
	XMVECTOR tex2DPos = { Position.x, Position.y, Position.z };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewMatrix(), false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetProjectionMatrix(), true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewPort(), false);

	postEffect->SetRadCenter(XMFLOAT2(tex2DPos.m128_f32[0], tex2DPos.m128_f32[1]));
	postEffect->SetRadPower(camerawork->GetEffectPower());
}
//撃破シーン
void SecondStageActor::FinishUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
}