﻿#include "SecondStageActor.h"
#include "ParticleEmitter.h"
#include "BackObj.h"
#include "Menu.h"
#include "SelectScene.h"
#include "ImageManager.h"
#include "Helper.h"
const XMVECTOR kSkyBlue{ 0.f,1.f,1.f,1.f };
const XMVECTOR kPink{ 0.9f,0.6f,0.8f,1.f };

//初期化
void SecondStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoopWave(AUDIO_BATTLE, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);
	//ポストエフェクト
	PlayPostEffect = false;

	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	backScreen_ = IKESprite::Create(ImageManager::PLAY, { 0,0 });
	backScreen_->SetSize({ 1280.0f,720.0f });

	//各クラス
	//プレイヤー
	Player::GetInstance()->InitState({ 0.0f,5.0f,-70.0f });

	Player::GetInstance()->SetCanShot(false);
	Player::GetInstance()->MoveStop(true);
	//敵
	enemymanager = std::make_unique<EnemyManager>("SECONDSTAGE");
	//カメラ
	camerawork->Update(camera);
	camerawork->SetBoss(enemymanager->GetBoss());
	camerawork->SetCameraState(CAMERA_BOSSAPPEAR);
	camerawork->SetSceneName("SECONDSTAGE");
	camerawork->SplineSet();
	//UI
	ui = std::make_unique<UI>();
	ui->Initialize();
	ui->SetBoss(enemymanager->GetBoss());
	//背景
	BackObj::GetInstance()->Initialize();
	//ステージOBJ
	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	loadobj->LightSet(lightgroup);
	LoadStageObj::SetEnemyManager(enemymanager.get());
	_Tscne = TextScene::NON;
	text_ = make_unique<BossText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::Name_First::VIEWBOSS);

	messagewindow_ = make_unique<MessageWindow>();
	messagewindow_->Initialize();
	messagewindow_->Display();

	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);

	Menu::GetIns()->Init();
}
//更新
void SecondStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	if (!Menu::GetIns()->GetMenuOpen()) {

		constexpr int IntervalTextC = 200;


		messagewindow_->DisplayCharacter(sutopon_color_);
		if (_Tscne == TextScene::NON)
		{
			sutopon_color_ = { 1.0f,1.0f,1.0f,1.0f };
			girl_color_ = { 0.5f,0.5f,0.5f,0.5f };
			textT++;
			text_->SelectText(TextManager::Name_First::VIEWBOSS, kSkyBlue);
			if (Input::GetInstance()->TriggerButton(Input::B) || textT > 1 * IntervalTextC)
			{
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button_Text.wav", VolumManager::GetInstance()->GetSEVolum());
				_Tscne = TextScene::TIEYOSHI_EXP;
			}
		} else if (_Tscne == TextScene::TIEYOSHI_EXP)
		{
			sutopon_color_ = { 0.50f,0.50f,0.50f,0.50f };
			girl_color_ = { 1.2f,1.2f,1.2f,1.f };

			textT++;
			text_->SelectText(TextManager::Name_First::SPEAKPLAYER1, kPink);
			if (Input::GetInstance()->TriggerButton(Input::B) || textT > 2 * IntervalTextC)
			{
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button_Text.wav", VolumManager::GetInstance()->GetSEVolum());
				_Tscne = TextScene::KILL_TIEYOSHI;
			}
		} else if (_Tscne == TextScene::KILL_TIEYOSHI)
		{
			girl_color_ = { 0.50f,0.50f,0.50f,0.50f };
			sutopon_color_ = { 1.f,1.f,1.f,1.f };

			textT++;
			text_->SelectText(TextManager::Name_First::SPEALPLAYER2, kSkyBlue);
			if (Input::GetInstance()->TriggerButton(Input::B) || textT > 3 * IntervalTextC)
			{
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button_Text.wav", VolumManager::GetInstance()->GetSEVolum());
				_Tscne = TextScene::LET_GO;
			}
		}

		else if (_Tscne == TextScene::LET_GO)
		{
			sutopon_color_ = { 0.50f,0.50f,0.50f,0.50f };
			girl_color_ = { 1.2f,1.2f,1.2f,1.f };
			textT++;
			text_->SelectText(TextManager::Name_First::SPEALPLAYER3, kPink);
			if (Input::GetInstance()->TriggerButton(Input::B) || textT > 4 * IntervalTextC)
			{
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button_Text.wav", VolumManager::GetInstance()->GetSEVolum());
				_Tscne = TextScene::ENDTEXT;
			}
		}


		if (_Tscne == TextScene::TIEYOSHI_EXP ||
			_Tscne == TextScene::NON)
		{
			if (Input::GetInstance()->TriggerButton(Input::A))
			{
				_Tscne = TextScene::ENDTEXT;
			}
		}
		//関数ポインタで状態管理
	//	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
	
	Input* input = Input::GetInstance();
	//プレイヤー
	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, 0.0f, Player::GetInstance()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	//ボス
	lightgroup->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(1, XMFLOAT3({ enemymanager->GetBoss()->GetPosition().x, 0.0f, 	enemymanager->GetBoss()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));


	if (enemymanager->BossDestroy()) {
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		SceneSave::GetInstance()->SetClearFlag(kSecondStage, true);
		if (camerawork->GetCameraState() == CameraState::CAMERA_BOSSDEAD_AFTER_FIRST)
		{
			PlayPostEffect = false;
		}
	}
	enemymanager->AppearUpdate();
enemymanager->BattleUpdate();
	if (camerawork->GetAppearEndF()) {
		Player::GetInstance()->MoveStop(false);
		Player::GetInstance()->SetCanShot(true);
		camerawork->SetCameraState(CAMERA_NORMAL);
		//enemymanager->SkipInitialize();
		//各クラス更新
		
	}

	if (PlayerDestroy()) {
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		SceneSave::GetInstance()->SetLoseFlag(SeceneCategory::kSecondStage, true);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMEOVER", SceneChanger::NonReverse);
	}
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();
	ui->Update();


	if (enemymanager->BossDestroy())
	{
		Player::GetInstance()->DeathUpdate();
	} else
	{
		Player::GetInstance()->Update();
	}
	//enemymanager->BattleUpdate();
	ColEnemy(enemymanager->GetBulEnemy());
	loadobj->SecondUpdate();
	ParticleEmitter::GetInstance()->Update();
	if (input->TriggerKey(DIK_X)) {
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		//SceneManager::GetInstance()->ChangeScene("SECONDSTAGE");

	}
	//カメラワークのセット
	if (enemymanager->BossDestroy())
	{
		//フェード前
		if (!camerawork->GetFeedEnd()) {
			enemymanager->SetDeadThrow(true);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_BEFORE);
		}
		//フェード後
		else
		{
			Player::GetInstance()->InitState({ 0.0f,0.0f,-5.0f });

			enemymanager->SetDeadThrow(false);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_AFTER_FIRST);
		}


		if (camerawork->GetEndDeath()) {
			sceneChanger_->ChangeStart();
			sceneChanger_->ChangeScene("GAMECLEAR", SceneChanger::ReverseType::NonReverse);
		}
	} else
	{
		//if (camerawork->FinishAppear()) {
			//m_SceneState = SceneState::MainState;
		//	camerawork->SetCameraState(CAMERA_NORMAL);
		//}
	}
	XMFLOAT3 Position = enemymanager->GetBoss()->GetPosition();
	XMVECTOR tex2DPos = { Position.x, Position.y, Position.z };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewMatrix(), false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetProjectionMatrix(), true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewPort(), false);

	postEffect->SetRadCenter(XMFLOAT2(tex2DPos.m128_f32[0], tex2DPos.m128_f32[1]));
	postEffect->SetRadPower(camerawork->GetEffectPower());
	sceneChanger_->Update();

	if(_Tscne!=TextScene::ENDTEXT)
	text_->Display();

	messagewindow_->Update(girl_color_, sutopon_color_);

	camerawork->Update(camera);
	lightgroup->Update();

	if (SelectScene::GetIns()->GetCloseScl() < 10000.f)
		SelectScene::GetIns()->Upda();

	if (Input::GetInstance()->TriggerButton(Input::Y)) {
		SelectScene::GetIns()->ResetParama();
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}

}Menu::GetIns()->Upda();
	BackObj::GetInstance()->Update();
	postEffect->SetCloseRad(SelectScene::GetIns()->GetCloseIconRad());
}
//描画
void SecondStageActor::Draw(DirectXCommon* dxCommon) {
	//描画方法
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());

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

	if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSAPPEAR &&
		camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST) {
		if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_BEFORE && camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST) {
			ParticleEmitter::GetInstance()->BackDrawAll();
		}
	}
	loadobj->Draw(dxCommon);

	ParticleEmitter::GetInstance()->DeathDrawAll();
	//パーティクル描画
	if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSAPPEAR &&
		camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST) {
		ParticleEmitter::GetInstance()->FlontDrawAll();
	}

	IKEObject3d::PostDraw();


	////各クラスの描画
	if (camerawork->GetAppearEndF()) {
		Player::GetInstance()->Draw(dxCommon);
	}
	enemymanager->Draw(dxCommon);
}
//ポストエフェクトがかからない
void SecondStageActor::FrontDraw(DirectXCommon* dxCommon) {
	//ParticleEmitter::GetInstance()->DeathDrawAll();
	
	//完全に前に書くスプライト
	if (_Tscne != TextScene::ENDTEXT) {
		IKESprite::PreDraw();
	messagewindow_->Draw();
	IKESprite::PostDraw();
		if (messagewindow_->DisplayCheck()) {
			text_->SpriteDraw(dxCommon);
		}
	}
	else {
		IKESprite::PreDraw();
		if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_BEFORE && camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST)
			ui->Draw();
		sceneChanger_->Draw();
		IKESprite::PostDraw();

		//完全に前に書くスプライト
		IKESprite::PreDraw();
		Menu::GetIns()->Draw();
		camerawork->feedDraw();

		SelectScene::GetIns()->Draw_Sprite();
		IKESprite::PostDraw();
	}
}
//IMGuiの描画
void SecondStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//Player::GetInstance()->ImGuiDraw();
	//enemymanager->ImGuiDraw();
	//loadobj->ImGuiDraw();
	//SceneSave::GetInstance()->ImGuiDraw();
}


void SecondStageActor::ColEnemy(std::vector<InterEnemy*> enelist)
{
	for (auto i = 0; i < enelist.size(); ++i) {
		for (auto j = 0; j < enelist.size(); ++j) {
			XMFLOAT3 ghostpos = enelist[i]->GetPosition();
			XMFLOAT3 ghostpos2 = enelist[j]->GetPosition();
			if ((i == j)) { continue; }
			if (Collision::SphereCollision(ghostpos, 1.5f, ghostpos2, 1.5f)) {
				enelist[i]->EnemyColNormal(ghostpos2);
				enelist[j]->EnemyColNormal(ghostpos);
			}
		}
	}
}