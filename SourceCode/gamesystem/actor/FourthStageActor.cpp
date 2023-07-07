#include "FourthStageActor.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include "BackObj.h"
#include "Menu.h"
#include "SelectScene.h"
#include "Helper.h"
#include <HungerGauge.h>
const XMVECTOR kSkyBlue{ 0.f,1.f,1.f,1.f };
const XMVECTOR kPink{ 0.9f,0.6f,0.8f,1.f };

//初期化
void FourthStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoopWave(AUDIO_BATTLE, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);
	//ポストエフェクト
	PlayPostEffect = true;

	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	backScreen_ = IKESprite::Create(ImageManager::PLAY, { 0,0 });
	backScreen_->SetSize({ 1280.0f,720.0f });

	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::Food);
	apple = make_unique<IKEObject3d>();
	apple->Initialize();
	apple->SetModel(m_Model);
	apple->SetPosition({0,0,-15});
	apple->SetScale({3, 3, 3});
	photo[Photo_Out_Top] = IKESprite::Create(ImageManager::PHOTO_OUT, { 0,-360 });
	photo[Photo_Out_Under] = IKESprite::Create(ImageManager::PHOTO_OUT, { 0,1080 });
	for (int i = Photo_Out_Top; i <= Photo_Out_Under; i++) {
		photo[i]->SetSize({ 1280,360 });
	}

	//各クラス
	//プレイヤー
	Player::GetInstance()->InitState({ 0.0f,5.0f,-70.0f });

	Player::GetInstance()->SetCanShot(false);
	Player::GetInstance()->MoveStop(true);
	//敵
	enemymanager = std::make_unique<EnemyManager>("FOURTHSTAGE");
	//カメラ
	camerawork->Update(camera);
	camerawork->SetBoss(enemymanager->GetBoss());
	camerawork->SetCameraState(CAMERA_BOSSAPPEAR);
	camerawork->SetSceneName("FOURTHSTAGE");
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
void FourthStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//関数ポインタで状態管理
	if (!Menu::GetIns()->GetMenuOpen()) {
		(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
		sceneChanger_->Update();
		camerawork->Update(camera);
	}
	if (isVisible) { apple->Update(); }
	Menu::GetIns()->Upda();
	ui->Update();
	postEffect->SetCloseRad(Menu::GetIns()->GetCloseIconRad());
	messagewindow_->Update(girl_color_, sutopon_color_);
	lightgroup->Update();
}
//描画
void FourthStageActor::Draw(DirectXCommon* dxCommon) {
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
void FourthStageActor::Finalize() {
}
//後ろの描画
void FourthStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	backScreen_->Draw();
	IKESprite::PostDraw();

	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);
	if ((m_SceneState == SceneState::MainState)&& !camerawork->GetFeedEnd()) {
		loadobj->Draw(dxCommon);
	}
	ParticleEmitter::GetInstance()->DeathDrawAll();
	//パーティクル描画
	if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSAPPEAR &&
		camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST) {
		ParticleEmitter::GetInstance()->BackDrawAll();
		ParticleEmitter::GetInstance()->FlontDrawAll();
	}

	////各クラスの描画
	if (camerawork->GetAppearEndF()) {
		Player::GetInstance()->Draw(dxCommon);
	}
	enemymanager->Draw(dxCommon);
	if (isVisible) {
		IKEObject3d::PreDraw();
		apple->Draw();
		IKEObject3d::PostDraw();
	}

	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void FourthStageActor::FrontDraw(DirectXCommon* dxCommon) {
	//パーティクル描画
	ParticleEmitter::GetInstance()->DeathDrawAll();
	//完全に前に書くスプライト
	if (m_SceneState == SceneState::MainState && !camerawork->GetFeedEnd()) {
		IKESprite::PreDraw();
		ui->Draw();
		IKESprite::PostDraw();
	}
	if (m_SceneState == SceneState::IntroState) {
			text_->SpriteDraw(dxCommon);
			IKESprite::PreDraw();
			for (int i = Photo_Out_Top; i <= Photo_Out_Under; i++) {
				photo[i]->Draw();
			}
			IKESprite::PostDraw();
	}
	sceneChanger_->Draw();
	Menu::GetIns()->Draw();
	camerawork->feedDraw();
}
//IMGuiの描画
void FourthStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//Player::GetInstance()->ImGuiDraw();
	//enemymanager->ImGuiDraw();
	//loadobj->ImGuiDraw();
	//ImGui::Begin("test");
	//ImGui::End();
	//loadobj->ImGuiDraw();
	//SceneSave::GetInstance()->ImGuiDraw();
}


void FourthStageActor::ColEnemy(std::vector<InterEnemy*> enelist)
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

void FourthStageActor::IntroUpdate(DebugCamera* camera) {
	//演出スキップ
	if (Input::GetInstance()->TriggerButton(Input::A)) {
		camerawork->SetCameraSkip(true);
	}

	if (camerawork->GetAppearEndF()) {
		isVisible = false;
		m_SceneState = SceneState::MainState;
		Player::GetInstance()->SetCanShot(true);
		Player::GetInstance()->MoveStop(false);
		camerawork->SetCameraState(CAMERA_NORMAL);
		enemymanager->SkipInitialize();
	}

	//各クラス更新
	BackObj::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	Player::GetInstance()->AppearUpdate();
	enemymanager->AppearUpdate();
	camerawork->Update(camera);
	if (!isShutter) {
		m_AppTimer++;
	} else {
		if (ShutterEffect()) {
			if (m_AppTimer < 1000) {
				isVisible = true;
			}
			stopTime++;
			if (stopTime >= stopTimerMax) {
				if (ShutterFeed()) {
					ShutterReset();
				}
			}
		}
	}
	//テキスト関係
	text_->Display();
	if (m_AppTimer == 1) {
		text_->SelectText(TextManager::TALK_FIRST_T);
		//text_->ChangeColor(0, { 1.0f,1.0f,1.0f,1.0f });
	} else if (m_AppTimer == 200) {
		text_->SelectText(TextManager::TALK_SECOND_T);
	} else if (m_AppTimer == 400) {
		text_->SelectText(TextManager::TALK_THIRD_T);
		//text_->ChangeColor(0, { 1.0f,0.0f,0.0f,1.0f });
	} else if (m_AppTimer == 600) {
		text_->SelectText(TextManager::TALK_FOURTH_T);
		//for (int i = 0; i < 3; i++) {
		//	text_->ChangeColor(i, { 1.0f,1.0f,0.0f,1.0f });
		//}
	} else if (m_AppTimer == 800) {
		isShutter = true;
		text_->SelectText(TextManager::TALK_NULL);
		m_AppTimer++;
	}
	else if (m_AppTimer == 850) {
		text_->SelectText(TextManager::TALK_FIVE_T);
	}
	else if (m_AppTimer == 1050) {
		text_->SelectText(TextManager::TALK_SIX_T);
	} else if (m_AppTimer == 1250) {
		text_->SelectText(TextManager::TALK_SEVEN_T);
	} else if (m_AppTimer == 1450) {
		text_->SelectText(TextManager::TALK_EIGHT_T);
	} else if (m_AppTimer == 1650) {
		text_->SelectText(TextManager::TALK_NINE_T);
	} else if (m_AppTimer == 1850) {
		text_->SelectText(TextManager::TALK_TEN_T);
	} else if (m_AppTimer == 2050) {
		isVisible = false;
		shutterTimeMax = 60.0f;
		stopTimerMax = 180.0f;
		feedTimeMax = 10.0f;
		text_->SelectText(TextManager::TALK_ELEVEN_T);
	} else if (m_AppTimer == 2250) {
		isShutter = true;
		text_->SelectText(TextManager::TALK_NULL);
		m_AppTimer++;
	} else if (m_AppTimer == 2450) {
		text_->SelectText(TextManager::TALK_XII_T);
	} else if (m_AppTimer == 2650) {
		text_->SelectText(TextManager::TALK_XIII_T);
	} else if (m_AppTimer == 2850) {
		text_->SelectText(TextManager::TALK_XIV_T);
	} else if (m_AppTimer == 3050) {
		text_->SelectText(TextManager::TALK_XV_T);
	} else if (m_AppTimer == 3250) {
		text_->SelectText(TextManager::TALK_XVI_T);
	} else if (m_AppTimer == 3450) {
		text_->SelectText(TextManager::TALK_XVII_T);
	} else if (m_AppTimer == 3650) {
		text_->SelectText(TextManager::TALK_XVII_T);
	} else if (m_AppTimer == 3850) {
		text_->SelectText(TextManager::TALK_XVIII_T);
	} else if (m_AppTimer == 4050) {
		text_->SelectText(TextManager::TALK_XVIV_T);
	} else if(m_AppTimer == 4250) {
		camerawork->SetCameraSkip(true);
	}
}

void FourthStageActor::MainUpdate(DebugCamera* camera) {

	if (enemymanager->BossDestroy()) {
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		//フェード前
		if (!camerawork->GetFeedEnd()) {
			enemymanager->SetDeadThrow(true);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_BEFORE);
			apple->SetPosition({ Player::GetInstance()->GetPosition().x,10.0f,Player::GetInstance()->GetPosition().z+5.0f });
		}
		//フェード後
		else {
			PlayPostEffect = false;
			isVisible = true;
			XMFLOAT3 pos = apple->GetPosition();
			pos.y -= 0.3f;
			pos.y =clamp(pos.y,0.0f,100.0f);
			apple->SetPosition(pos);
			HungerGauge::GetInstance()->SetNowHunger(0);
			HungerGauge::GetInstance()->Update();
			Player::GetInstance()->InitState({ 0.0f,0.0f,-5.0f });
			enemymanager->SetDeadThrow(false);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_AFTER_FOURTH);
		}

		if (camerawork->GetEndDeath()) {
			sceneChanger_->ChangeStart();
			sceneChanger_->ChangeScene("GAMECLEAR", SceneChanger::NonReverse);

		}

		Player::GetInstance()->DeathUpdate();
	} else {
		Player::GetInstance()->Update();
	}

	if (PlayerDestroy()) {
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMEOVER", SceneChanger::Reverse);
	}
	//音楽の音量が変わる
	VolumManager::GetInstance()->Update();

	//各クラス更新
	BackObj::GetInstance()->Update();
	enemymanager->BattleUpdate();
	ColEnemy(enemymanager->GetBulEnemy());
	loadobj->FourthUpdate();
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

void FourthStageActor::FinishUpdate(DebugCamera* camera) {
}
bool FourthStageActor::ShutterEffect() {
	shutterTime += 1.0f / shutterTimeMax;
	shutterTime = clamp(shutterTime, 0.0f, 1.0f);

	shutterHight[0] = Ease(Out, Quad, shutterTime, -360, 0);
	shutterHight[1] = Ease(Out, Quad, shutterTime, 1080, 360);

	photo[Photo_Out_Top]->SetPosition({ 0,shutterHight[0] });
	photo[Photo_Out_Under]->SetPosition({ 0,shutterHight[1] });
	if (shutterTime == 1.0f) {
		return true;
	} else {
		return false;
	}
}

bool FourthStageActor::ShutterFeed() {
	feedTimer += 1.0f / feedTimeMax;
	float color = Ease(Out, Linear, feedTimer, 1.0f, 0.0f);
	photo[Photo_Out_Top]->SetColor({ 1,1,1, color });
	photo[Photo_Out_Under]->SetColor({ 1,1,1,color });
	feedTimer = clamp(feedTimer, 0.0f, 1.0f);
	if (feedTimer == 1.0f) {
		return true;
	} else {
		return false;
	}
}

void FourthStageActor::ShutterReset() {
	isShutter = false;
	stopTime = 0;
	shutterHight[0] = -360.0f;
	shutterHight[1] = 1080.0f;
	photo[Photo_Out_Top]->SetPosition({ 0,shutterHight[0] });
	photo[Photo_Out_Under]->SetPosition({ 0,shutterHight[1] });
	photo[Photo_Out_Top]->SetColor({ 1,1,1,1 });
	photo[Photo_Out_Under]->SetColor({ 1,1,1,1 });
	shutterTime = 0.0f;
	feedTimer = 0.0f;
}
