#include "SevenStageActor.h"
#include"Easing.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include "BackObj.h"
#include "Menu.h"
#include "SelectScene.h"
#include "Helper.h"
//初期化
void SevenStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoopWave(AUDIO_BATTLE, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);

	//ポストエフェクト
	PlayPostEffect = false;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	//各クラス
	Player::GetInstance()->InitState({ 0.0f,5.0f,-5.0f });

	backScreen_ = IKESprite::Create(ImageManager::PLAY, { 0,0 });
	backScreen_->SetSize({ 1280.0f,720.0f });
	//シーンチェンジャー
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	enemymanager = std::make_unique<EnemyManager>("SEVENSTAGE");
	//enemymanager->Initialize(dxCommon);
	text_ = make_unique<BossText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::LAST_TALK_FIRST);
	camerawork->SetBoss(enemymanager->GetBoss());
	camerawork->SetCameraState(CAMERA_BOSSAPPEAR);
	camerawork->SetSceneName("SEVENSTAGE");
	camerawork->SplineSet();
	camerawork->Update(camera);
	ui = std::make_unique<UI>();
	ui->Initialize();

	SelectScene::GetIns()->Init();
	Menu::GetIns()->Init();
	ui->SetBoss(enemymanager->GetBoss());
	BackObj::GetInstance()->Initialize();

	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	loadobj->LightSet(lightgroup);
	LoadStageObj::SetEnemyManager(enemymanager.get());

	m_SceneState = SceneState::IntroState;

	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);
}
//更新
void SevenStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//関数ポインタで状態管理
	if (!Menu::GetIns()->GetMenuOpen()) {
		(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
		sceneChanger_->Update();
	}

	//プレイヤー
	if (enemymanager->BossDestroy() && camerawork->GetFeedEnd()) {
		SceneSave::GetInstance()->SetClearFlag(kSevenStage, true);
	}

	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, -2.0f, Player::GetInstance()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	//ボス
	lightgroup->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(1, XMFLOAT3({ enemymanager->GetBoss()->GetPosition().x, 	0.0f, 	enemymanager->GetBoss()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));

	lightgroup->Update();
	/*if (SelectScene::GetIns()->GetCloseScl() < 10000.f)
		SelectScene::GetIns()->Upda();

	if (Input::GetInstance()->TriggerButton(Input::Y)) {
		SelectScene::GetIns()->ResetParama();
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}*/	
	ui->Update();
	Menu::GetIns()->Upda();
	postEffect->SetCloseRad(SelectScene::GetIns()->GetCloseIconRad());
}
//描画
void SevenStageActor::Draw(DirectXCommon* dxCommon) {
	//描画方法
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		//ImGuiDraw(dxCommon);
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
void SevenStageActor::Finalize() {
}
//後ろの描画
void SevenStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	backScreen_->Draw();
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);
	//パーティクル描画
	if (!camerawork->GetFeedEnd() && m_SceneState == SceneState::MainState) {
		if (!enemymanager->BossDestroy()) {
			if (camerawork->GetCameraState() != CAMERA_BOSS_STRONG) {
				ParticleEmitter::GetInstance()->BackDrawAll();
			}
		}
	}

	if (!camerawork->GetChangeStrong()) {
		Player::GetInstance()->Draw(dxCommon);
	}
	////各クラスの描画
	if (!camerawork->GetFeedEnd()) {
		if (camerawork->GetCameraState() != CAMERA_BOSS_STRONG) {
			loadobj->Draw(dxCommon);
		}
	}
	enemymanager->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void SevenStageActor::FrontDraw(DirectXCommon* dxCommon) {
	//パーティクル描画
	if (!camerawork->GetFeedEnd() && m_SceneState == SceneState::MainState) {
		if (camerawork->GetCameraState() != CAMERA_BOSS_STRONG) {
			ParticleEmitter::GetInstance()->FlontDrawAll();
		}
	}

	ParticleEmitter::GetInstance()->DeathDrawAll();
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	if (m_SceneState == SceneState::MainState && !camerawork->GetFeedEnd() && camerawork->GetCameraState() == CAMERA_NORMAL) {
		ui->Draw();
	}
	if ((m_SceneState == SceneState::IntroState) ||
		(m_SceneState == SceneState::MainState && (camerawork->GetChangeStrong() || camerawork->GetCameraState() == CAMERA_BOSSDEAD_AFTER_SEVEN))) {
		text_->SpriteDraw(dxCommon);
	}sceneChanger_->Draw();
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	//blackwindow->Draw();
	Menu::GetIns()->Draw();
	camerawork->feedDraw();
	//SelectScene::GetIns()->Draw_Sprite();
	IKESprite::PostDraw();
}
//IMGuiの描画
void SevenStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//enemymanager->ImGuiDraw();
	//camerawork->ImGuiDraw();
	////loadobj->ImGuiDraw();
	///*
	//
	//*/
	//ImGui::Begin("Seven");
	//ImGui::Text("Timer:%d", m_EndTimer);
	//ImGui::End();

	//Player::GetInstance()->ImGuiDraw();
}
//登場シーン
void SevenStageActor::IntroUpdate(DebugCamera* camera) {

	//演出スキップ
	if (Input::GetInstance()->TriggerButton(Input::A)) {
		camerawork->SetCameraSkip(true);
	}

	if (camerawork->GetAppearEndF()) {
		_AppState = APP_END;
		m_SceneState = SceneState::MainState;
		camerawork->SetCameraState(CAMERA_NORMAL);
		enemymanager->SkipInitialize();
	}
	//テキスト読み
	TextRead();
	//各クラス更新
	Player::GetInstance()->LastAppearUpdate(m_AppTimer);
	BackObj::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	enemymanager->AppearUpdate();
	camerawork->SetLastTimer(m_AppTimer);
	camerawork->Update(camera);
}
//バトルシーン
void SevenStageActor::MainUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
	//覚醒シーンに入る
	if (enemymanager->GetEnemyStrong() && !camerawork->GetCameraStrong()) {
		if (!camerawork->GetFeedF()) {
			camerawork->SetFeedF(true);
			camerawork->SetCameraState(CAMERA_BOSS_STRONG);
			camerawork->SetCameraStrong(true);
		}
	}

	if (camerawork->GetCameraState() == CAMERA_BOSS_STRONG) {
		//一回目のフェードの後に位置を初期化する
		if (camerawork->GetChangeStrong()) {
			Player::GetInstance()->AwakeInit();
			enemymanager->DeleteObj();
			loadobj->AwakeInit();
			text_->Display();
			m_AwakeTimer++;
			AwakeText();
		}
		//二回目のフェードが終わるとバトルモードに戻る
		if (camerawork->GetEndStrong()) {
			Player::GetInstance()->SetPosition({ 0.0f,0.0f,-5.0f });
			camerawork->SetCameraState(CAMERA_NORMAL);
		}
	}
	//カメラワークのセット
	if (enemymanager->BossDestroy())
	{
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		//フェード前
		if (!camerawork->GetFeedEnd()) {
			enemymanager->SetDeadThrow(true);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_BEFORE);
		}
		//フェード後
		else
		{
			text_->Display();
			m_EndTimer++;
			PlayPostEffect = false;
			enemymanager->SetDeadThrow(false);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_AFTER_SEVEN);
			camerawork->SetEndTimer(m_EndTimer);
			Player::GetInstance()->LastDeadUpdate(m_EndTimer);
			DeathText();

			if (Input::GetInstance()->TriggerButton(Input::A)) {
				camerawork->SetEndDeath(true);
			}
		}

		if (camerawork->GetEndDeath()) {
			sceneChanger_->ChangeStart();
			sceneChanger_->ChangeScene("ENDROLL", SceneChanger::NonReverse);

		}

		Player::GetInstance()->DeathUpdate();
	}
	else
	{
		if(camerawork->GetCameraState() == CAMERA_NORMAL)
		Player::GetInstance()->Update();
		else 
		Player::GetInstance()->DeathUpdate();
	}

	if (PlayerDestroy()) {
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		SceneSave::GetInstance()->SetLoseFlag(SeceneCategory::kSevenStage, true);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeSceneLose("GAMEOVER");
	}

	//音楽の音量が変わる
	VolumManager::GetInstance()->Update();

	//各クラス更新
	BackObj::GetInstance()->Update();
	if (camerawork->GetCameraState() == CAMERA_NORMAL) {
		enemymanager->BattleUpdate();
	}
	else {
		enemymanager->Awake();
	}

	loadobj->SevenUpdate();
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
void SevenStageActor::FinishUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
}
//テキスト関係
void SevenStageActor::TextRead() {
	text_->Display();
	m_AppTimer++;
	if (m_AppTimer == 240) {
		text_->SelectText(TextManager::LAST_TALK_SECOND);
	}
	else if (m_AppTimer == 350) {
		text_->SelectText(TextManager::LAST_TALK_THIRD);
	}
	else if (m_AppTimer == 500) {
		text_->SelectText(TextManager::LAST_TALK_FOURTH);
	}
	else if (m_AppTimer == 650) {
		text_->SelectText(TextManager::LAST_TALK_FIVE);
	}
	else if (m_AppTimer == 800) {
		text_->SelectText(TextManager::LAST_TALK_SIX);
	}
	else if (m_AppTimer == 950) {
		text_->SelectText(TextManager::LAST_TALK_SEVEN);
	}
	else if (m_AppTimer == 1100) {
		text_->SelectText(TextManager::LAST_TALK_EIGHT);
	}
	else if (m_AppTimer == 1250) {
		text_->SelectText(TextManager::LAST_TALK_NINE);
	}
	else if (m_AppTimer == 1400) {
		text_->SelectText(TextManager::LAST_TALK_TEN);
	}
	else if (m_AppTimer == 1550) {
		text_->SelectText(TextManager::LAST_TALK_ELEVEN);
	}
	else if (m_AppTimer == 1700) {
		text_->SelectText(TextManager::LAST_TALK_TWELVE);
	}
	else if (m_AppTimer == 1850) {
		text_->SelectText(TextManager::LAST_TALK_THIRTEEN);
	}
	else if (m_AppTimer == 2000) {
		text_->SelectText(TextManager::LAST_TALK_FOURTEEN);
	}
	else if (m_AppTimer == 2150) {
		text_->SelectText(TextManager::LAST_TALK_FIFETEEN);
	}
	else if (m_AppTimer == 2300) {
		text_->SelectText(TextManager::LAST_TALK_SIXTEEN);
	}
	else if (m_AppTimer == 2450) {
		text_->SelectText(TextManager::LAST_TALK_SEVENTEEN);
	}
	else if (m_AppTimer == 2600) {
		text_->SelectText(TextManager::LAST_TALK_EIGHTTEEN);
	}
	else if (m_AppTimer == 2750) {
		text_->SelectText(TextManager::LAST_TALK_NINETEEN);
	}
	else if (m_AppTimer == 2950) {
		camerawork->SetCameraSkip(true);
	}
}
//覚醒時のテキスト
void SevenStageActor::AwakeText() {
	if (m_AwakeTimer == 1) {
		text_->SelectText(TextManager::AWAKE_FIRST);
	}
	else if (m_AwakeTimer == 200) {
		text_->SelectText(TextManager::AWAKE_SECOND);
	}
	else if (m_AwakeTimer == 400) {
		text_->SelectText(TextManager::AWAKE_THIRD);
	}
}
//ボス撃破時のエフェクト
void SevenStageActor::DeathText() {
	if (m_EndTimer == 1) {
		text_->SelectText(TextManager::DEATH_FIRST);
	}
	else if (m_EndTimer == 200) {
		text_->SelectText(TextManager::DEATH_SECOND);
	}
	else if (m_EndTimer == 350) {
		text_->SelectText(TextManager::DEATH_THIRD);
	}
	else if (m_EndTimer == 500) {
		text_->SelectText(TextManager::DEATH_FOURTH);
	}
	else if (m_EndTimer == 650) {
		text_->SelectText(TextManager::DEATH_FIVE);
	}
	else if (m_EndTimer == 800) {
		text_->SelectText(TextManager::DEATH_SIX);//ボス
	}
	else if (m_EndTimer == 900) {
		text_->SelectText(TextManager::DEATH_SEVEN);//ボス
	}
	else if (m_EndTimer == 1000) {
		text_->SelectText(TextManager::DEATH_EIGHT);//コトコ
	}
	else if (m_EndTimer == 1100) {
		text_->SelectText(TextManager::DEATH_NINE);//ボス
	}
	else if (m_EndTimer == 1200) {
		text_->SelectText(TextManager::DEATH_TEN);//コトコ
	}
	else if (m_EndTimer == 1300) {
		text_->SelectText(TextManager::DEATH_ELEVEN);//コトコ
	}
	else if (m_EndTimer == 1400) {
		text_->SelectText(TextManager::DEATH_TWELVE);//ボス
	}
	else if (m_EndTimer == 1500) {
		text_->SelectText(TextManager::DEATH_THIRTEEN);//コトコ
	}
	else if (m_EndTimer == 1600) {
		text_->SelectText(TextManager::DEATH_FOURTEEN);//すと
	}
	else if (m_EndTimer == 1700) {
		text_->SelectText(TextManager::DEATH_FIFTEEN);//ボス
	}
}